#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include <pthread.h>
#include "utility.h"
#include "concertSeats.h"
#include "lowSeller.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main (int argc, char **argv){

    if (argc < 2){
        fprintf(stderr, "Error: no customers supplied. \nTo correctly use program type ./main N where N is the number of customers\n");
        exit(1); // Exit with a non-zero exit code to indicate an error
    }

    //Initialize the concert seats
    ConcertSeats concertSeats;
    int i, j; // Declare the loop counters here

    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            strcpy(concertSeats.seatChart[i][j], "----");
        }
    }
    concertSeats.availableSeats = 100;
    printConcertSeats(&concertSeats);

    //Generate customer qeues
    linked_list *customerList1 = generateCustomers(atoi(argv[1]));
    linked_list *customerList2 = generateCustomers(atoi(argv[1]));
    //Generate more per your needs


    //Two Low Seller Threads
    ThreadArgs threadArgs1;
    threadArgs1.customerList = customerList1;
    threadArgs1.sellerName = "L1";
    threadArgs1.concertSeats = &concertSeats;

    ThreadArgs threadArgs2;
    threadArgs2.customerList = customerList2;
    threadArgs2.sellerName = "L2";
    threadArgs2.concertSeats = &concertSeats;

    pthread_t thread1, thread2; // Define two threads

    int threadCreationResult1 = pthread_create(&thread1, NULL, lowSeller, (void *) &threadArgs1);
    int threadCreationResult2 = pthread_create(&thread2, NULL, lowSeller, (void *) &threadArgs2);



    // Wait for the thread to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printConcertSeats(&concertSeats);

    pthread_mutex_destroy(&mutex);
    

}