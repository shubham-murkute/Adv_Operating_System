#include <stdio.h>
#include "concertSeats.h"

void printConcertSeats(ConcertSeats *concertSeats) {
    printf("===Printing Concert Seats Chart===\n");
    int i;
    int j; // Declare the loop counters here
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            printf("%s ", concertSeats->seatChart[i][j]);
        }
        printf("\n");
    }
}