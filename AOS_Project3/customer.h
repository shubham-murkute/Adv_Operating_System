#ifndef _CUSTOMER_H_
#define _CUSTOMER_H_

#include "utility.h"

typedef struct {
    unsigned char id;
    float arrivalTime;
    float responseTime;
    float waitingTime;
} Customer;


Customer *createCustomer(char id, float arrivalTime, float responseTime, float waitingTime);

Customer *getCopyofCustomer(Customer * customer);

linked_list *generateCustomers(int n);

void printCustomers(linked_list *customer_list);


#endif