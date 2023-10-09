#include <stdio.h>
#include <stdlib.h>
#include "stat.h"
#include "utility.h"

process_stat * createProcessStat(process* proc);

average_stats roundRobinP(linked_list * processes, int time_slice) {
	int t = 0;

	//Create Process Queue
	queue *prcQueue = (queue *)createQueue();
	node * prcPtr = processes->head;
	if(processes->head == NULL) {
		fprintf(stderr,"No Process to schedule\n");
	}
	//while process queue is not empty or time quanta is less than 100
	process_stat * scheduled_process = NULL;

	linked_list *ll = createLinkedList();
	printf("\nRound Robbin:\n");
	node * cur_node = NULL;
	int cur_run = 0;
	while(t<100 || prcQueue->size > 0) {
		//check for incoming new process and enqueue it in the queue only when its arrival time is before 100
		if(prcPtr != NULL && t<100) {
			process * new_process = (process *)(prcPtr->data);
			while(prcPtr!=NULL && new_process->arrivalTime <= t) {
				enqueue(prcQueue,createProcessStat(new_process));
				prcPtr = prcPtr->next;
				if(prcPtr!=NULL)
					new_process = (process *)(prcPtr->data);
			}
		}

		//if there is no scheduled process, then check process queue and schedule it
		if(cur_node == NULL) {
			cur_run = 0;
			cur_node = prcQueue->head;
		} else if(cur_run == time_slice) {
			cur_run = 0;
			cur_node = cur_node->next;
			if(cur_node == NULL) {
				cur_node = prcQueue->head;
			}
		}

		if(cur_node != NULL) {
			scheduled_process = (process_stat *) cur_node->data;
			process * proc = scheduled_process->proc;

			if(t>=100) {
				if(scheduled_process->startTime == -1) {
					//Don't start any new process, remove it from prcQueue
					free(scheduled_process);
					//dequeue(scheduled_process);
					node * next = cur_node->next;
					removeNode(prcQueue,cur_node->data);
					cur_node = next;
					cur_run = 0;
					continue;
				}
			}
			//add current running process to the time chart
			printf("%c",proc->pid);
			cur_run++;
			//update current processes stat
			if(scheduled_process->startTime == -1) {
				scheduled_process->startTime = t;
			}
			scheduled_process->runTime++;

			if(scheduled_process->runTime >= proc->runTime) {
				scheduled_process->endTime = t;
				addNode(ll,scheduled_process);
				node * next = cur_node -> next;
				removeNode(prcQueue, cur_node->data);
				cur_node = next;
				cur_run = 0;
			}
		} else {
			printf("_");
		}
		//increase the time
		t++;
	}

	return printPolicyStat(ll);

}
