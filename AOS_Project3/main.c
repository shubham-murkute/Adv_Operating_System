#include<stdio.h>

#include<stdlib.h>

#include<unistd.h>

#include<time.h>

#include<string.h>

#include<malloc.h>

#include<pthread.h>

#include"utility.h"

#define hp_Seller_count 1
#define mp_Seller_count 3
#define lp_Seller_count 6
#define total_Seller (hp_Seller_count + mp_Seller_count + lp_Seller_count)
#define concert_row 10
#define concert_col 10
#define simulation_duration 60

// Seller Argument Structure
typedef struct Sell_arg_struct {
  char Seller_no;
  char Seller_type;
  queue * Seller_queue;
}
Sell_arg;

typedef struct customer_struct {
  char cust_no;
  int arrivalTime;
}
customer;

//Global Variable
int sTime;
int N = 5;
int at1[15] = {
  0
}, st1[15] = {
  0
}, tat1[15] = {
  0
}, bt1[15] = {
  0
}, rt1[15] = {
  0
};
float throughput[3] = {
  0
};

float avg_ResTime = 0, avg_Turnaround_Time = 0, No_customer_Serv = 0;
char SeatMatrix[concert_row][concert_col][5]; //4 to hold L002\0

//Thread Variable
pthread_t Seller_t[total_Seller];
pthread_mutex_t ThreadCount_Mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t Thread_Wait_Clock_Tick_Mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t ReservationMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t Thread_Completion_Mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t Condition_Mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t Condition_Cond = PTHREAD_COND_INITIALIZER;

//Function Defination
void DispQueue(queue * q);
void CreateSellerThreads(pthread_t * thread, char Seller_type, int no_of_Sellers);
void WaitForThreadServingCurrentSlice();
void WakeAllSellerThreads();
void * Sell(void * );
queue * generatecustomerQueue(int);
int Compare_By_arrivalTime(void * data1, void * data2);
int AvailableSeatFinder(char Seller_type);

int ThreadCount = 0;
int Threads_Waiting_For_Clock_Tick = 0;
int ActiveThread = 0;
int verbose = 0;
int main(int argc, char ** argv) {
  srand(4388);

  if (argc == 2) {
    N = atoi(argv[1]);
  }
  int r, c;
  //Initialize Global Variables
  for (r = 0; r < concert_row; r++) {
    for (c = 0; c < concert_col; c++) {
      strncpy(SeatMatrix[r][c], "-", 1);
    }
  }

  //Create all threads
  CreateSellerThreads(Seller_t, 'H', hp_Seller_count);
  CreateSellerThreads(Seller_t + hp_Seller_count, 'M', mp_Seller_count);
  CreateSellerThreads(Seller_t + hp_Seller_count + mp_Seller_count, 'L', lp_Seller_count);

  //Wait for threads to finish initialization and wait for synchronized clock tick
  while (1) {
    pthread_mutex_lock( & ThreadCount_Mutex);
    if (ThreadCount == 0) {
      pthread_mutex_unlock( & ThreadCount_Mutex);
      break;
    }
    pthread_mutex_unlock( & ThreadCount_Mutex);
  }

  //Simulate each time quanta/slice as one iteration
  printf("Starting Simulation\n");
  Threads_Waiting_For_Clock_Tick = 0;
  WakeAllSellerThreads(); //For first tick

  do {

    //Wake up all thread
    WaitForThreadServingCurrentSlice();
    sTime = sTime + 1;
    WakeAllSellerThreads();
    //Wait for thread completion
  } while (sTime < simulation_duration);

  //Wakeup all thread so that no more thread keep waiting for clock Tick in limbo
  WakeAllSellerThreads();

  while (ActiveThread);

  //Display concert chart
  printf("\n\n");
  printf("Final Concert Seat Chart\n");
  printf("========================\n");
  int h_customers = 0, m_customers = 0, l_customers = 0;
  for (r = 0; r < concert_row; r++) {
    for (c = 0; c < concert_col; c++) {
      if (c != 0)
        printf("\t");
      printf("%5s", SeatMatrix[r][c]);
      if (SeatMatrix[r][c][0] == 'H') h_customers++;
      if (SeatMatrix[r][c][0] == 'M') m_customers++;
      if (SeatMatrix[r][c][0] == 'L') l_customers++;
    }
    printf("\n");
  }

  printf("\n\nStat for N = %02d\n", N);
  printf("===============\n");
  printf(" ============================================\n");
  printf("|%3c | No of customers | Got Seat | Returned |\n", ' ');
  printf(" ============================================\n");
  printf("|%3c | %15d | %8d | %8d |\n", 'H', hp_Seller_count * N, h_customers, (hp_Seller_count * N) - h_customers);
  printf("|%3c | %15d | %8d | %8d |\n", 'M', mp_Seller_count * N, m_customers, (mp_Seller_count * N) - m_customers);
  printf("|%3c | %15d | %8d | %8d |\n", 'L', lp_Seller_count * N, l_customers, (lp_Seller_count * N) - l_customers);
  printf(" ============================================\n");
  int z1;
  for (z1 = 0; z1 < N; z1++) {
    int ct = 0;
    ct = st1[z1] + bt1[z1];
    rt1[z1] = abs(st1[z1] - at1[z1]);
    tat1[z1] = abs(ct - at1[z1]);
  }
  int j1;
  for (j1 = 0; j1 < N; j1++) {
    avg_Turnaround_Time += tat1[j1];
    avg_ResTime += rt1[j1];
  }

  printf("\nAverage Turnaround Time = %.2f\n", avg_Turnaround_Time / N);
  printf("Average Response Time = %.2f\n", avg_ResTime / N);
  printf("Throughput of Seller H = %.2f\n", throughput[0] / 60.0);
  printf("Throughput of Seller M = %.2f\n", throughput[1] / 60.0);
  printf("Throughput of Seller L = %.2f\n", throughput[2] / 60.0);

  return 0;
}

void CreateSellerThreads(pthread_t * thread, char Seller_type, int no_of_Sellers) {
  //Create all threads
  int t_no;
  for (t_no = 0; t_no < no_of_Sellers; t_no++) {
    Sell_arg * Seller_arg = (Sell_arg * ) malloc(sizeof(Sell_arg));
    Seller_arg -> Seller_no = t_no;
    Seller_arg -> Seller_type = Seller_type;
    Seller_arg -> Seller_queue = generatecustomerQueue(N);

    pthread_mutex_lock( & ThreadCount_Mutex);
    ThreadCount++;
    pthread_mutex_unlock( & ThreadCount_Mutex);
    if (verbose)
      printf("Creating thread %c%02d\n", Seller_type, t_no);
    pthread_create(thread + t_no, NULL, & Sell, Seller_arg);
  }
}

void DispQueue(queue * q) {
  node * ptr;
  for (ptr = q -> head; ptr != NULL; ptr = ptr -> next) {
    customer * cust = (customer *)  ptr -> data;
    printf("[%d,%d]", cust -> cust_no, cust -> arrivalTime);
  }
}

void WaitForThreadServingCurrentSlice() {
  //Check if all threads has finished their jobs for this time slice
  while (1) {
    pthread_mutex_lock( & Thread_Wait_Clock_Tick_Mutex);
    if (Threads_Waiting_For_Clock_Tick == ActiveThread) {
      Threads_Waiting_For_Clock_Tick = 0;
      pthread_mutex_unlock( & Thread_Wait_Clock_Tick_Mutex);
      break;
    }
    pthread_mutex_unlock( & Thread_Wait_Clock_Tick_Mutex);
  }
}
void WakeAllSellerThreads() {

  pthread_mutex_lock( & Condition_Mutex);
  if (verbose)
    printf("00:%02d Main Thread Broadcasting Clock Tick\n", sTime);
  pthread_cond_broadcast( & Condition_Cond);
  pthread_mutex_unlock( & Condition_Mutex);
}

void * Sell(void * t_args) {
  //Initializing thread
  Sell_arg * args = (Sell_arg * ) t_args;
  queue * customer_queue = args -> Seller_queue;
  queue * Seller_queue = CreateQueue();
  char Seller_type = args -> Seller_type;
  int Seller_no = args -> Seller_no + 1;

  pthread_mutex_lock( & ThreadCount_Mutex);
  ThreadCount--;
  ActiveThread++;
  pthread_mutex_unlock( & ThreadCount_Mutex);

  customer * cust = NULL;
  int random_wait_time = 0;
  int temp1 = 0;

  while (sTime < simulation_duration) {
    //Waiting for clock tick
    pthread_mutex_lock( & Condition_Mutex);
    if (verbose)
      printf("00:%02d %c%02d Waiting for next clock tick\n", sTime, Seller_type, Seller_no);

    pthread_mutex_lock( & Thread_Wait_Clock_Tick_Mutex);
    Threads_Waiting_For_Clock_Tick++;
    pthread_mutex_unlock( & Thread_Wait_Clock_Tick_Mutex);

    pthread_cond_wait( & Condition_Cond, & Condition_Mutex);
    if (verbose)
      printf("00:%02d %c%02d Received Clock Tick\n", sTime, Seller_type, Seller_no);
    pthread_mutex_unlock( & Condition_Mutex);

    // Sell
    if (sTime == simulation_duration) break;
    //All New customer Came
    while (customer_queue -> size > 0 && ((customer *)  customer_queue -> head -> data) -> arrivalTime <= sTime) {
      customer * temp = (customer *)  dequeue(customer_queue);
      enqueue(Seller_queue, temp);
      printf("00:%02d %c%d customer No %c%d%02d arrived\n", sTime, Seller_type, Seller_no, Seller_type, Seller_no, temp -> cust_no);
    }
    //Serve next customer
    if (cust == NULL && Seller_queue -> size > 0) {
      cust = (customer *)  dequeue(Seller_queue);
      printf("00:%02d %c%d Serving customer No %c%d%02d\n", sTime, Seller_type, Seller_no, Seller_type, Seller_no, cust -> cust_no);
      switch (Seller_type) {
      case 'H':
        random_wait_time = (rand() % 2) + 1;
        bt1[temp1] = random_wait_time;
        temp1++;
        break;
      case 'M':
        random_wait_time = (rand() % 3) + 2;
        bt1[temp1] = random_wait_time;
        temp1++;
        break;
      case 'L':
        random_wait_time = (rand() % 4) + 4;
        bt1[temp1] = random_wait_time;
        temp1++;
      }
    }
    if (cust != NULL) {
      //printf("Wait time %d\n",random_wait_time);
      if (random_wait_time == 0) {
        //Selling Seat
        pthread_mutex_lock( & ReservationMutex);

        // Find seat
        int seatIndex = AvailableSeatFinder(Seller_type);
        if (seatIndex == -1) {
          printf("00:%02d %c%d customer No %c%d%02d has been told Concert Sold Out.\n", sTime, Seller_type, Seller_no, Seller_type, Seller_no, cust -> cust_no);
        } else {
          int row_no = seatIndex / concert_col;
          int col_no = seatIndex % concert_col;
          sprintf(SeatMatrix[row_no][col_no], "%c%d%02d", Seller_type, Seller_no, cust -> cust_no);
          printf("00:%02d %c%d customer No %c%d%02d assigned seat %d,%d \n", sTime, Seller_type, Seller_no, Seller_type, Seller_no, cust -> cust_no, row_no, col_no);
          No_customer_Serv++;
          if (Seller_type == 'L')
            throughput[0]++;
          else if (Seller_type == 'M')
            throughput[1]++;
          else if (Seller_type == 'H')
            throughput[2]++;
        }
        pthread_mutex_unlock( & ReservationMutex);
        cust = NULL;
      } else {
        random_wait_time--;
      }
    } else {
      //printf("00:%02d %c%02d Waiting for customer\n",sTime,Seller_type,Seller_no);
    }
  }

  while (cust != NULL || Seller_queue -> size > 0) {
    if (cust == NULL)
      cust = (customer *)  dequeue(Seller_queue);
    printf("00:%02d %c%d Ticket Sale Closed. customer No %c%d%02d Leaves\n", sTime, Seller_type, Seller_no, Seller_type, Seller_no, cust -> cust_no);
    cust = NULL;
  }

  pthread_mutex_lock( & ThreadCount_Mutex);
  ActiveThread--;
  pthread_mutex_unlock( & ThreadCount_Mutex);
}

int AvailableSeatFinder(char Seller_type) {
  int seatIndex = -1;
  int row_no, col_no;
  if (Seller_type == 'H') {
    for (row_no = 0; row_no < concert_row; row_no++) {
      for (col_no = 0; col_no < concert_col; col_no++) {
        if (strcmp(SeatMatrix[row_no][col_no], "-") == 0) {
          seatIndex = row_no * concert_col + col_no;
          return seatIndex;
        }
      }
    }
  } else if (Seller_type == 'M') {
    int mid = concert_row / 2;
    int row_jump = 0;
    int next_row_no = mid;
    for (row_jump = 0;; row_jump++) {
      row_no = mid + row_jump;
      if (mid + row_jump < concert_row) {
        for (col_no = 0; col_no < concert_col; col_no++) {
          if (strcmp(SeatMatrix[row_no][col_no], "-") == 0) {
            seatIndex = row_no * concert_col + col_no;
            return seatIndex;
          }
        }
      }
      row_no = mid - row_jump;
      if (mid - row_jump >= 0) {
        for (col_no = 0; col_no < concert_col; col_no++) {
          if (strcmp(SeatMatrix[row_no][col_no], "-") == 0) {
            seatIndex = row_no * concert_col + col_no;
            return seatIndex;
          }
        }
      }
      if (mid + row_jump >= concert_row && mid - row_jump < 0) {
        break;
      }
    }
  } else if (Seller_type == 'L') {
    for (row_no = concert_row - 1; row_no >= 0; row_no--) {
      for (col_no = concert_col - 1; col_no >= 0; col_no--) {
        if (strcmp(SeatMatrix[row_no][col_no], "-") == 0) {
          seatIndex = row_no * concert_col + col_no;
          return seatIndex;
        }
      }
    }
  }

  return -1;
}

queue * generatecustomerQueue(int N) {
  queue * customer_queue = CreateQueue();
  char cust_no = 0;
  while (N--) {
    customer * cust = (customer *)  malloc(sizeof(customer));
    cust -> cust_no = cust_no;
    cust -> arrivalTime = rand() % simulation_duration;
    at1[cust_no] = cust -> arrivalTime;
    enqueue(customer_queue, cust);
    cust_no++;
  }
  Sort(customer_queue, Compare_By_arrivalTime);
  node * ptr = customer_queue -> head;
  cust_no = 0;
  while (ptr != NULL) {
    cust_no++;
    customer * cust = (customer *)  ptr -> data;
    cust -> cust_no = cust_no;
    ptr = ptr -> next;
  }
  return customer_queue;
}

int Compare_By_arrivalTime(void * data1, void * data2) {
  customer * c1 = (customer * ) data1;
  customer * c2 = (customer * ) data2;
  if (c1 -> arrivalTime < c2 -> arrivalTime) {
    return -1;
  } else if (c1 -> arrivalTime == c2 -> arrivalTime) {
    return 0;
  } else {
    return 1;
  }
}
