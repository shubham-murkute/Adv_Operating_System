#ifndef _CUSTOMER_H_
#define _CUSTOMER_H_
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utility.h"


typedef struct {
    unsigned char id;
    float arrivalTime;
    float responseTime;
    float turnAroundTime;
} Customer;

typedef struct {
    float averageResponseTime;
    float averageTurnAroundTime;
    float throughput;
    char sellerType[5];
} CustomersStats;



Customer *createCustomer(char id, float arrivalTime, float responseTime, float turnAroundTime);

Customer *getCopyofCustomer(Customer * customer);

linked_list *generateCustomers(int n);

void printCustomers(linked_list *customer_list);
void printCustomer(Customer *customer);
CustomersStats *createCustomersStats(linked_list *customer_list, const char *sellerType);
void printCustomersStats(CustomersStats *customersStats);


#endif