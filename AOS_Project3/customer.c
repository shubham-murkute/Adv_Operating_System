#include <stdio.h>
#include <stdlib.h>
#include "customer.h"
#include <time.h>

int compare(void *data1, void *data2) {
	Customer *p1 = (Customer *)data1;
	Customer *p2 = (Customer *)data2;
	if(p1->arrivalTime < p2->arrivalTime) {
		return -1;
	} else if(p1->arrivalTime == p2->arrivalTime){
		return 0;
	} else {
		return 1;
	}
}

Customer *createCustomer(char id, float arrivalTime, float responseTime, float waitingTime) {
	Customer *customer = (Customer *) malloc(sizeof(Customer));
	customer->id = id;
    customer->arrivalTime = arrivalTime;
    customer-> responseTime = responseTime;
    customer->waitingTime = waitingTime;
	return customer;
}


linked_list *generateCustomers(int n) {
	linked_list *customer_list = create_linked_list();
	char id = 'A';
	float arrivalTime, responseTime, waitingTime;
	time_t t;


   /* Intializes random number generator */
   srand((unsigned) time(&t));

	if(customer_list == NULL) fprintf(stderr,"Unable to create Linked List\n");

	while(n--) {

        arrivalTime = (float)((rand() % 590) + 1) / 10;
        responseTime = 0;
        waitingTime = 0;
		Customer *c = createCustomer(id, arrivalTime, responseTime, waitingTime);

		add_node(customer_list,c);
		id++;
	}

	sort(customer_list,compare);

	node *ptr = customer_list->head;
	id = 'A';
	while(ptr!=NULL) {
		((Customer *)ptr->data)->id = id;
		if(id == 'Z') id = 'a' - 1;
		id++;
		ptr=ptr->next;
	}

	return customer_list;
}

void printCustomers(linked_list *customer_list) {
    if (customer_list == NULL || customer_list->head == NULL) {
        printf("Customer list is empty.\n");
        return;
    }

    node *ptr = customer_list->head;
    while (ptr != NULL) {
        Customer *customer = (Customer *)ptr->data;
        printf("Customer ID: %c\n", customer->id);
        printf("Arrival Time: %.1f\n", customer->arrivalTime);
        printf("Response Time: %.1f\n", customer->responseTime);
        printf("Waiting Time: %.1f\n", customer->waitingTime);
        printf("\n");
        ptr = ptr->next;
    }
}

void printCustomer(Customer *customer){
	printf("Customer ID: %c\n", customer->id);
	printf("Arrival Time: %.1f\n", customer->arrivalTime);
	printf("Response Time: %.1f\n", customer->responseTime);
    printf("Waiting Time: %.1f\n", customer->waitingTime);
	printf("\n");
}