#include "lowSeller.h"


void* lowSeller(void *args){
    srand(time(NULL));
    ThreadArgs *threadArgs = (ThreadArgs*)args;
    linked_list *customers = threadArgs->customerList;
    const char *sellerName = threadArgs->sellerName;
    ConcertSeats *concertSeats = threadArgs->concertSeats;
    node *customerPtr = customers->head;


    linked_list *processedCustomers = create_linked_list();

    Customer *firstCustomer = (Customer *)(customerPtr->data);
    int time = firstCustomer->arrivalTime;

    while (customerPtr != NULL){
        Customer *currentCustomer  = (Customer *)(customerPtr->data);
        if (time < currentCustomer->arrivalTime){
            time = currentCustomer->arrivalTime;
        }
        pthread_mutex_lock(&mutex);
        if (time < 60 && concertSeats->availableSeats>0){
            //Time still less than one hour and seats available
            int i, j;
            //Traverse concert seats from the bottom corner
            for (i = 9; i>= 0; i--){
                for (j = 9; j>=0; j--){
                    if (strcmp(concertSeats->seatChart[i][j], "----")==0){
                        //Empty availabe seat found, reserve seat

                        //Set the Concert Seats
                        char seatName[5];
                        seatName[0] = '\0';
                        char customerId[2];
                        sprintf(customerId, "%c", currentCustomer->id);
                        strcat(seatName,sellerName);
                        strcat(seatName,customerId);
                        strcpy(concertSeats->seatChart[i][j],seatName);
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
            //Time ran out or concert is full
            pthread_mutex_unlock(&mutex);
            break;
        }
        int responseTime = time - currentCustomer->arrivalTime;
        int saleTime = (rand() % 4) + 4;
        time += saleTime;
        int turnAroundTime = time - currentCustomer->arrivalTime;

        Customer *processedCustomer = createCustomer(currentCustomer->id, currentCustomer->arrivalTime, responseTime, turnAroundTime);
        add_node(processedCustomers,processedCustomer);
        customerPtr = customerPtr->next;        

    }
    printCustomers(processedCustomers);


}
