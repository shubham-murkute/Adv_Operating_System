#include "lowSeller.h"


void* lowSeller(void *args){
    srand(time(NULL));

    //Parse thread function arguments
    ThreadArgs *threadArgs = (ThreadArgs*)args;
    linked_list *customers = threadArgs->customerList;
    const char *sellerName = threadArgs->sellerName;
    ConcertSeats *concertSeats = threadArgs->concertSeats;
    node *customerPtr = customers->head;


    //Store processed customers here
    linked_list *processedCustomers = create_linked_list();

    //Set the first customer in queue
    Customer *firstCustomer = (Customer *)(customerPtr->data);
    int time = firstCustomer->arrivalTime;

    //Attempt to process all of the customers in queue
    while (customerPtr != NULL){

        //Process current customer in queue
        Customer *currentCustomer  = (Customer *)(customerPtr->data);
        
        //No customer has arrived, move forward to time where a customer has arrived
        if (time < currentCustomer->arrivalTime){
            time = currentCustomer->arrivalTime;
        }

        //print out event
        double formattedMinute = (double)time / 100;
        printf("%.2f: Customer %c has arrived to tail queue of seller %s\n", formattedMinute, currentCustomer->id, sellerName);

        //Entering crtical region, accessing shared variable concerSeats
        pthread_mutex_lock(&mutex);

        //If time less than the hour and concert still has available seats
        if (time < 60 && concertSeats->availableSeats>0){
            printf("%.2f: Customer %c served and assigned by seller %s\n", formattedMinute, currentCustomer->id, sellerName);
            int i, j;
            //Traverse concert seats from the bottom corner to find an available seat
            for (i = 9; i>= 0; i--){
                for (j = 9; j>=0; j--){
                    if (strcmp(concertSeats->seatChart[i][j], "----")==0){
                        //Empty availabe seat found, reserve seat
                        char seatName[5];
                        seatName[0] = '\0';
                        char customerId[2];
                        sprintf(customerId, "%c", currentCustomer->id);
                        strcat(seatName,sellerName);
                        strcat(seatName,customerId);
                        strcpy(concertSeats->seatChart[i][j],seatName);

                        //Update available seats in concert
                        concertSeats->availableSeats -= 1;

                        //break out of for loop
                        break;
                    }
                }
                if (j>=0){
                    //break out of for loop
                    break;
                }
            }
            //Exit critical region
            pthread_mutex_unlock(&mutex);
        }else{
            //Time ran out or no available seats left
            printf("%.2f: Customer %c is told that concert is sold out by seller %s\n", formattedMinute, currentCustomer->id, sellerName);
            pthread_mutex_unlock(&mutex);
            break;
        }

        //Process time statistics and increment time
        int responseTime = time - currentCustomer->arrivalTime;
        //Low seller takes 4,5,6, or 7 minutes to complete a sale
        int saleTime = (rand() % 4) + 4;
        time += saleTime;
        int turnAroundTime = time - currentCustomer->arrivalTime;

        //Add processed customer to prcessed customer list
        Customer *processedCustomer = createCustomer(currentCustomer->id, currentCustomer->arrivalTime, responseTime, turnAroundTime);
        add_node(processedCustomers,processedCustomer);
        printf("%.2f: Customer %c completes ticket purchase by seller %s\n", formattedMinute, currentCustomer->id, sellerName);

        //set customePtr to the next customer in list
        customerPtr = customerPtr->next;        

    }
    //printCustomers(processedCustomers);


}
