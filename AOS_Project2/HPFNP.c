#include <stdio.h>
#include <stdlib.h>
#include "stat.h"
#include "utility.h"

process_stat * createProcessStat(process* proc);
//comparing priority of processes//
int ComparePriority(void * data1, void * data2);
void printqueue(queue * q);

// Implementation of highest priority first non preemptive //

average_stats highestPriorityFirstNP(linked_list * processes)
{
  int t = 0; // quanta //
  // creation of a queue of processes
  queue * PQueue1 =(queue *) createQueue();
  queue * PQueue2 =(queue *) createQueue();
  queue * PQueue3 =(queue *) createQueue();
  queue * PQueue4 =(queue *) createQueue();
  // creation of linked list for managaing process order in preemeption
  linked_list * ll_1 = createLinkedList();
  linked_list * ll_2 = createLinkedList();
  linked_list * ll_3 = createLinkedList();
  linked_list * ll_4 = createLinkedList();
  node * PrsPointer = processes->head;
  if(processes->head == NULL) {
		fprintf(stderr,"No Process to schedule\n");
	}
  printf("\nHighest Priority First Non Preemptive:\n");
  //while process queue is not empty or time quanta is less than 100
  process_stat * Prs_Scheduled = NULL;
  while(t<100 || Prs_Scheduled!=NULL )
  {
    //printf("Time %d\n",t);
    /** // Use this while preemeption //
    if(Prs_Scheduled!=NULL)
    {
      enqueue(process_queue,Prs_Scheduled);
      Prs_Scheduled = NULL;
    }
    **/
    //check for incoming new process and enqueue it in the queue
		if(PrsPointer != NULL) {
			process * new_process = (process *)(PrsPointer->data);
			while(PrsPointer !=NULL && new_process->arrivalTime <= t) {
        if(new_process->priority == 1) enqueue(PQueue1,createProcessStat(new_process));
        if(new_process->priority == 2) enqueue(PQueue2,createProcessStat(new_process));
        if(new_process->priority == 3) enqueue(PQueue3,createProcessStat(new_process));
        if(new_process->priority == 4) enqueue(PQueue3,createProcessStat(new_process));
				//sort(process_queue,ComparePriority);
				PrsPointer = PrsPointer->next;
				if(PrsPointer!=NULL)
        {
					new_process = (process *)(PrsPointer->data);
        }
			}
      // sort all the processes that have arrived based on their priority //
      //sort(process_queue,ComparePriority);
    }
    //printqueue(process_queue);
    //if there is no scheduled process, then check process queue and schedule it //
		if(Prs_Scheduled == NULL) {
      if (PQueue1->size > 0) Prs_Scheduled = (process_stat *) dequeue(PQueue1);
      else if (PQueue2->size > 0) Prs_Scheduled = (process_stat *) dequeue(PQueue2);
      else if (PQueue3->size > 0) Prs_Scheduled = (process_stat *) dequeue(PQueue3);
      else if (PQueue4->size > 0) Prs_Scheduled = (process_stat *) dequeue(PQueue4);
      // If the process has not started before quanta 100, remove the process from the queue and take the next process in queue for execution //
      /*
      while(t>=100 && Prs_Scheduled->startTime == -1)
      {
        Prs_Scheduled = (process_stat *) dequeue(process_queue);
      }*/
      if (t>=100 && Prs_Scheduled->startTime == -1){
        //free(Prs_Scheduled);
        Prs_Scheduled = NULL;
        continue;
      }
    }


    if(Prs_Scheduled != NULL) {
  			process * proc = Prs_Scheduled->proc;

  			//add current running process to the time chart
  			printf("%c",proc->pid);

  			//update current processes stat
  			if(Prs_Scheduled->startTime == -1) {
  				Prs_Scheduled->startTime = t;
  			}

  			Prs_Scheduled->runTime++;

        //printf("Process name %c", proc->pid);

        if(Prs_Scheduled->runTime >= proc->runTime) {
          Prs_Scheduled->endTime = t;
          if(Prs_Scheduled->proc->priority == 1) addNode(ll_1,Prs_Scheduled);
          else if(Prs_Scheduled->proc->priority == 2) addNode(ll_2,Prs_Scheduled);
          else if(Prs_Scheduled->proc->priority == 3) addNode(ll_3,Prs_Scheduled);
          else if(Prs_Scheduled->proc->priority == 4) addNode(ll_4,Prs_Scheduled);
          //addNode(ll,Prs_Scheduled);
          Prs_Scheduled = NULL;
          //free(Prs_Scheduled);
        }
      }else {
    			printf("_");
  		}
  		//increase the time
  		t++;
    }
    //Print Process Stat
    average_stats avg1,avg2,avg3,avg4,avg;
    printf("\nFor Priority Queue 1");
    avg1 = printPolicyStat(ll_1);
    printf("\nFor Priority Queue 2");
    avg2 = printPolicyStat(ll_2);
    printf("\nFor Priority Queue 3");
    avg3 = printPolicyStat(ll_3);
    printf("\nFor Priority Queue 4");
    avg4 = printPolicyStat(ll_4);

    avg.avg_response_time = (avg1.avg_response_time + avg2.avg_response_time + avg3.avg_response_time + avg4.avg_response_time)/4 ;
    avg.avg_wait_time = (avg1.avg_wait_time + avg2.avg_wait_time + avg3.avg_wait_time + avg4.avg_wait_time)/4 ;
    avg.avg_turnaround = (avg1.avg_turnaround + avg2.avg_turnaround + avg3.avg_turnaround + avg4.avg_turnaround)/4 ;
    avg.avg_throughput = (avg1.avg_throughput + avg2.avg_throughput + avg3.avg_throughput + avg4.avg_throughput) ;

    printf("The average statistics High Priority First Non Preemptive considering all priority queues is:\n");
    printf("Average Response Time : %.1f\n",avg.avg_response_time);
    printf("Average Wait Time : %.1f\n",avg.avg_wait_time);
    printf("Average Turn Around Time :%.1f\n",avg.avg_turnaround);
    printf("Average Throughput(TP) :%.1f\n",avg.avg_throughput);

    return avg;

}
