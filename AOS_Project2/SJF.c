#include <stdio.h>
#include <stdlib.h>
#include "stat.h"
#include "utility.h"

process_stat *createProcessStat2(process *proc) {
	process_stat *ps = (process_stat *) malloc(sizeof(process_stat));
	ps->proc = proc;
	ps->waitingTime = 0;
	ps->turnaroundTime = 0;
	ps->responseTime = 0;

	ps->runTime = 0;
	ps->startTime = -1;
	ps->endTime = -1;
	return ps;
}

int compareRemainingTimes(void * data1, void * data2)
{
  process_stat * ps1 = (process_stat *) data1;
	process_stat * ps2 = (process_stat *) data2;
	if(((((process *)ps1->proc)->runTime) - (ps1->runTime)) < ((((process *)ps2->proc)->runTime) - (ps2->runTime))) {
		return -1;
	} else {
		return 1;
	}
}



int compareRunTimes(void *data1, void *data2) {
	process *p1 = (process *)data1;
	process *p2 = (process *)data2;
	if(p1->runTime < p2->runTime) {
		return -1;
	} else if(p1->runTime == p2->runTime){
		return 0;
	} else {
		return 1;
	}
}



average_stats shortestJobFirstNP(linked_list *processes) {
	int t = 0;

	//Creation of Process Queue
	queue *processQueue = (queue *)createQueue();
	node * procPtr = processes->head;
	if(processes->head == NULL) {
		fprintf(stderr," There is no Process to schedule\n\n");
	}
	
	//keep checking while time quanta is less than 100 or the process queue is empty...
	process_stat * scheduledProcess = NULL;

	linked_list *ll = createLinkedList();
	printf("\nShortest Job First Algorithm:\n");
	while(t<100 || scheduledProcess!=NULL) {
		//check for incoming new process and do enqueue.
		if(procPtr != NULL) {
			process * newProcess = (process *)(procPtr->data);
			if(newProcess->arrivalTime <= t) {
				enqueue(processQueue,createProcessStat2(newProcess));
				procPtr = procPtr->next;
			}
			// Sort the processQueue by runTime (shortest runTime first)
			sort(processQueue, compareRemainingTimes);
		}


		//check process queue and schedule it if there is no scheduled process now.
		if(scheduledProcess == NULL && processQueue->size > 0) {
			scheduledProcess = (process_stat *) dequeue(processQueue);
		}

        
		if(scheduledProcess != NULL) {
			process * proc = scheduledProcess->proc;

			//add  the currently running process to the time chart
			printf("%c",proc->pid);

			//update current processes stat
			if(scheduledProcess->startTime == -1) {
				scheduledProcess->startTime = t;
			}
			scheduledProcess->runTime++;
    //check if scheduled process run time is greater than the currently running process runtime..
			if(scheduledProcess->runTime >= proc->runTime) {
				scheduledProcess->endTime = t;
				//add the scheduled process.
				addNode(ll,scheduledProcess);
				scheduledProcess = NULL;
			}
		} else {
			printf("_");
		}
		//keep increasing the time.
		t++;
	}

	//Print the Stat of the process.
	return printPolicyStat(ll);
	
}