#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include "utility.h"

typedef struct {
    /*
    the concert seating chart as a 10-by-10 matrix that 
    shows which customer was assigned to each seat. Identify ticket seller H’s customers as H001, H002, H003, ...; the customers of ticket sellers M1, M2, …, as 
    M101, M102, …, M201, M202, …; and the customers of ticket sellers L1, L2, … 
    as L101, L102, …, L201, L202, ... You can indicate still-unsold seats with dashes
    */
    char seatChart[10][10][5];
    int availableSeats;
} Seats;

void printConcertSeats(Seats *concertSeats) {
    int i, j; // Declare the loop counters here
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            printf("%s ", concertSeats->seatChart[i][j]);
        }
        printf("\n");
    }
}



int main (int argc, char **argv){

    //initialize the concert seats
    Seats concertSeats;
    int i, j; // Declare the loop counters here

    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            strcpy(concertSeats.seatChart[i][j], "----");
        }
    }
    concertSeats.availableSeats = 100;
    printConcertSeats(&concertSeats);

    //generate customer qeues
    linked_list *customerList = generateCustomers(10);
    printCustomers(customerList);



    

}