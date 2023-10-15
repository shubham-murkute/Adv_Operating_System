#ifndef _LOWSELLER_H_
#define _LOWSELLERH_

#include "utility.h"
#include "concertSeats.h"
#include "customer.h"
#include <pthread.h>
#include <string.h>
#include <stdio.h>

// Declare the mutex lock
extern pthread_mutex_t mutex;

typedef struct{
    linked_list *customerList;
    const char* sellerName;     
    ConcertSeats* concertSeats;

}ThreadArgs;

void *lowSeller(void *args);



#endif