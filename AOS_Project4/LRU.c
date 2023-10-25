#include "PAGE.h"

// LRU algorithm
/*
void LRU_FUNCTION(LISTOFPAGES* plLRU) {
    page* current = plLRU->HDL;
    page* leastRecent = NULL;

    while (current) {
        if (leastRecent == NULL || current->CNTER < leastRecent->CNTER) {
            leastRecent = current;
        }
        current = current->next;
    }

    if (leastRecent != NULL) {
        if (DEBUG == 1) {
            printf("EVICTED ones :: p[%03d] c:%02d l:%02f\n", leastRecent->pid, leastRecent->CNTER, leastRecent->LONE);
        }
        leastRecent->pid = -1;
        leastRecent->PGENUMBER = -1;
    }
}
*/


//LRU algorithm
void LRU_FUNCTION(LISTOFPAGES* plLRU) {
    page* counter = plLRU->HDL;
    page* evicted = plLRU->HDL;
    int lru = counter->LONE;
    while(counter) {
        if(counter->LONE < lru){
            evicted = counter;
            lru = counter->LONE;
        }
        counter = counter->next;
    }
    if (DEBUG==1) printf("EVICTED ones :: p[%03d] c:%02d l:%02f\n", evicted->pid, evicted->CNTER, evicted->LONE);
    evicted->pid = -1;
    evicted->PGENUMBER = -1;
}
