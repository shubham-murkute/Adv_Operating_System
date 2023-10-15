#ifndef _CONCERTSEATS_H_
#define _CONCERTSEATS_H_

typedef struct {
    /*
    the concert seating chart as a 10-by-10 matrix that 
    shows which customer was assigned to each seat. Identify ticket seller H’s customers as H001, H002, H003, ...; the customers of ticket sellers M1, M2, …, as 
    M101, M102, …, M201, M202, …; and the customers of ticket sellers L1, L2, … 
    as L101, L102, …, L201, L202, ... You can indicate still-unsold seats with dashes
    */
    char seatChart[10][10][5];
    int availableSeats;
} ConcertSeats;

void printConcertSeats(ConcertSeats *concertSeats);

#endif