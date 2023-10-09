#include <stdio.h>
#include <stdlib.h>
#include "stat.h"
#include "utility.h"

process_stat * createProcessStat(process* proc);

// function to compare time to completion processes while sorting 
int compareRemainingTime(void * data1, void * data2)
{
  process_stat * ps1 = (process_stat *) data1;
	process_stat * ps2 = (process_stat *) data2;
	if(((((process *)ps1->proc)->runTime) - (ps1->runTime)) < ((((process *)ps2->proc)->runTime) - (ps2->runTime))) {
		return -1;
	} else {
		return 1;
	}
}

// function to print contents of queue for testing purpose during shortest remaining time to completion 
void printQueueSrtc(queue * q)
{
  if (q->head != NULL)
  {
    node * n = q->head;
    printf("Queue Contains:\n");
    while(n != NULL)
    {
      process_stat* ps = n->data;
      process * p = ps->proc;
      printf("Process Id %c Remaining Time %f\n",p->pid,(p->runTime - ps->runTime));
      n = n->next;
    }
  }
  return;
}

// Implementation of shortest remaining time to completion preemptive 

average_stats shortestRemainingTimeP(linked_list * processes)
{
  int t = 0; 

  // creation of a queue of processes
  queue * processQueue = createQueue();
  // creation of linked list for managing process order in preemption

  linked_list * ll = createLinkedList();

  node * procPtr = processes->head;
  if(processes->head == NULL) {
		fprintf(stderr,"There is no Process to schedule\n\n");
	}

  //keep checking while time quanta is less than 100 or the process queue is empty
  process_stat * scheduledProcess = NULL;

  printf("\nShortest Remaining Time To Completion Preemptive:\n");
  while(t<100 || scheduledProcess!=NULL )
  {
    
    if(scheduledProcess!=NULL)
    {
      enqueue(processQueue,scheduledProcess);
      scheduledProcess = NULL;
    }
    //check for incoming new process and enqueue it in the queue
		if(procPtr != NULL) {
			process * new_process = (process *)(procPtr->data);
			while(procPtr !=NULL && new_process->arrivalTime <= t) {
				enqueue(processQueue,createProcessStat(new_process));
				
				procPtr = procPtr->next;
				if(procPtr!=NULL)
        {
					new_process = (process *)(procPtr->data);
        }
			}
      
     // sort all the processes that have arrived based on their remaining running time to completion 
      sort(processQueue,compareRemainingTime);
    }
    

    //check process queue and schedule it if there is no scheduled process now
		if(scheduledProcess == NULL && processQueue->size > 0) {
			scheduledProcess = (process_stat *) dequeue(processQueue);

      // If the process has not started before quanta 100, remove the process from the queue and take the next process in queue for execution //
      while(t>=100 && scheduledProcess->startTime == -1)
      {
        scheduledProcess = (process_stat *) dequeue(processQueue);
      }
		}
    if(scheduledProcess != NULL) {
  			process * proc = scheduledProcess->proc;

  			//add current running process to the time chart
  			printf("%c",proc->pid);

  			//update current processes stat
  			if(scheduledProcess->startTime == -1) {
  				scheduledProcess->startTime = t;
  			}

  			scheduledProcess->runTime++;



        if(scheduledProcess->runTime >= proc->runTime) {
          scheduledProcess->endTime = t;
          //add the scheduled process
          addNode(ll,scheduledProcess);
          scheduledProcess = NULL;
        }
      }else {
    			printf("_");
    	}
  		//keep increasing the time
  		t++;
  }
  //Print the stat of the process
  return printPolicyStat(ll);
 
}
