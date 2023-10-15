#include "lowSeller.h"


void* lowSeller(void *args){
    srand(time(NULL));
    ThreadArgs *threadArgs = (ThreadArgs*)args;
    linked_list *customers = threadArgs->customerList;
    const char *sellerName = threadArgs->sellerName;
    ConcertSeats *concertSeats = threadArgs->concertSeats;
    //Creation of Customers Queue
    queue *customersQueue = (queue *)create_queue();
    node *customerPtr = customers->head;
    if(customers->head == NULL) {
		fprintf(stderr," There is no customer to process\n\n");
	}

    linked_list *ll = create_linked_list();

    int time = 0;

    while (customerPtr != NULL){
        pthread_mutex_lock(&mutex);
        if (time < 60 && concertSeats->availableSeats>0){
            int i;
            int j;
            //Traverse concert seats from the bottom corner
            for (i = 9; i>= 0; i--){
                for (j = 9; j>=0; j--){
                    if (strcmp(concertSeats->seatChart[i][j], "----")==0){
                        //Empty availabe seat found, reserve seat
                        //pop the customer (TODO)
                        strcpy(concertSeats->seatChart[i][j],sellerName);
                        concertSeats->availableSeats -= 1;
                        break;
                    }
                }
                if (j>=0){
                    break;
                }
            }
            pthread_mutex_unlock(&mutex);
        }else{
            pthread_mutex_unlock(&mutex);
            break;
        }
        time += 10;
        int random_seconds = (rand() % 3) + 1;
        sleep(random_seconds);
        customerPtr = customerPtr->next;        

    }


}
