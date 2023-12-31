#include <stdio.h>
#include "utility.h"
#include "process.h"
#include "stat.h"

void dispQueue(queue *q) {
	node * ptr = (node *)q->head;
				printf("<");
				while(ptr!=NULL) {
					printf("%c ",((process *) ptr->data)->pid);
					ptr = ptr->next;
				}
				printf(">\n");
}
int main(int argc,char **argv) {
	int ctr = 0;
	//Run 5 times
	average_stats fcfs[6],sjf[6],srf[6],rr[6],hpfp[6],hpfnp[6],final[6];

	while (ctr<5){
		//Generate Processes
		printf("*****************************************************************************************************************************\n");
		printf("Round %d\n",ctr+1);
		printf("*****************************************************************************************************************************\n");

		linked_list * processList = generateProcesses(52);
		node * ptr = processList->head;
		printf("-----------------------------------------------------------------------------------------------------------------------------\n");
		printf("Process ID | Arrival Time | Run Time | Priority |\n");
		printf("-----------------------------------------------------------------------------------------------------------------------------\n");
		while(ptr != NULL) {
			process *proc = (process *)ptr->data;
			printf("%10c | %12.1f | %8.1f | %8d |\n",proc->pid,proc->arrivalTime,proc->runTime,proc->priority);
			ptr = ptr->next;
		}
		printf("-----------------------------------------------------------------------------------------------------------------------------\n");
		printf("Total Number of Processes : %d\n",processList->size);
		printf("-----------------------------------------------------------------------------------------------------------------------------\n");

		//First Come First Serve
		fcfs[ctr] = firstComeFirstServeNP(processList);

		//Shortest job first
		sjf[ctr] = shortestJobFirstNP(processList);
		
	        //shortest remaining time first
		srf[ctr] = shortestRemainingTimeP(processList);
		
		//Highest Priority First Preemptive
		hpfp[ctr] = highestPriorityFirstP(processList);
		
		//Highest Priority First Non Preemptive
		hpfnp[ctr] = highestPriorityFirstNP(processList);

		//Round Robin
		rr[ctr] = roundRobinP(processList,1);
		
		ctr++;
	}
	
	for (int i = 0; i<6 ; i++)
	{
			final[i].avg_response_time = 0;
			final[i].avg_wait_time = 0;
			final[i].avg_turnaround = 0;
			final[i].avg_throughput = 0;
	}

	// Total of all average values
	for (int i = 0; i<5 ; i++)
	{
                        //FCFS
		       final[0].avg_response_time += fcfs[i].avg_response_time;
                       final[0].avg_wait_time += fcfs[i].avg_wait_time;
                       final[0].avg_turnaround += fcfs[i].avg_turnaround;
                       final[0].avg_throughput += fcfs[i].avg_throughput;

			//SJF
			final[1].avg_response_time += sjf[i].avg_response_time;
			final[1].avg_wait_time += sjf[i].avg_wait_time;
			final[1].avg_turnaround += sjf[i].avg_turnaround;
			final[1].avg_throughput += sjf[i].avg_throughput;
			
                       //SRTF
			final[2].avg_response_time += srf[i].avg_response_time;
			final[2].avg_wait_time += srf[i].avg_wait_time;
			final[2].avg_turnaround += srf[i].avg_turnaround;
			final[2].avg_throughput += srf[i].avg_throughput;
			
			//HPFP
			final[4].avg_response_time += hpfp[i].avg_response_time;
			final[4].avg_wait_time += hpfp[i].avg_wait_time;
			final[4].avg_turnaround += hpfp[i].avg_turnaround;
			final[4].avg_throughput += hpfp[i].avg_throughput;	
			
			//HPFnP
			final[5].avg_response_time += hpfnp[i].avg_response_time;
			final[5].avg_wait_time += hpfnp[i].avg_wait_time;
			final[5].avg_turnaround += hpfnp[i].avg_turnaround;
			final[5].avg_throughput += hpfnp[i].avg_throughput;	

			//Round Robin
			final[3].avg_response_time += rr[i].avg_response_time;
        		final[3].avg_wait_time += rr[i].avg_wait_time;
        		final[3].avg_turnaround += rr[i].avg_turnaround;
        		final[3].avg_throughput += rr[i].avg_throughput;
			
	}

	// Average values for all the algorithms 
	for (int i = 0; i<6 ; i++)
	{
			final[i].avg_response_time /= 5;
			final[i].avg_wait_time /= 5;
			final[i].avg_turnaround /= 5;
			final[i].avg_throughput /= 5;
	}

	printf("\n\n\n");
	//printf("*****************************************************************************************************************************\n");
	//printf("*****************************************************************************************************************************\n");
	printf("\nThe average of the 5 runs of every algorithms is as follows:\n");
        printf("\n");
        printf("ALGORITHM: First-come First-served (FCFS) [non-preemptive]:\n");
	printf("Average Response Time: %.1f\n",final[0].avg_response_time);
	printf("Average Wait Time: %.1f\n",final[0].avg_wait_time);
	printf("Average Turn Around Time: %.1f\n",final[0].avg_turnaround);
	printf("Average Throughput: %.1f\n",final[0].avg_throughput);
	printf("\n");

	printf("ALGORITHM:Shortest Job First [non-reemptive]:\n");
	printf("Average Response Time : %.1f\n",final[1].avg_response_time);
	printf("Average Wait Time : %.1f\n",final[1].avg_wait_time);
	printf("Average Turn Around Time :%.1f\n",final[1].avg_turnaround);
	printf("Average throughput :%.1f\n",final[1].avg_throughput);
        printf("\n");
	
	printf("ALGORITHM:Shortest Remaining Time First(SRTF) [Preemptive]:\n");
	printf("Average Response Time : %.1f\n",final[2].avg_response_time);
	printf("Average Wait Time : %.1f\n",final[2].avg_wait_time);
	printf("Average Turn Around Time :%.1f\n",final[2].avg_turnaround);
	printf("Average throughput :%.1f\n",final[2].avg_throughput);
        printf("\n");
    
    	printf("ALGORITHM:Highest Priority First(HPFP) [Preemptive]:\n");
	printf("Average Response Time : %.1f\n",final[4].avg_response_time);
	printf("Average Wait Time : %.1f\n",final[4].avg_wait_time);
	printf("Average Turn Around Time :%.1f\n",final[4].avg_turnaround);
	printf("Average throughput :%.1f\n",final[4].avg_throughput);
        printf("\n");
    
    	printf("ALGORITHM:Highest Priority First(HPFNP) [Non Preemptive]:\n");
	printf("Average Response Time : %.1f\n",final[5].avg_response_time);
	printf("Average Wait Time : %.1f\n",final[5].avg_wait_time);
	printf("Average Turn Around Time :%.1f\n",final[5].avg_turnaround);
	printf("Average throughput :%.1f\n",final[5].avg_throughput);
        printf("\n");

	printf("ALGORITHM: Round Robin (RR):\n");
    	printf("Average Response Time : %.1f\n", final[3].avg_response_time);
    	printf("Average Wait Time : %.1f\n", final[3].avg_wait_time);
    	printf("Average Turn Around Time :%.1f\n", final[3].avg_turnaround);
    	printf("Average throughput :%.1f\n", final[3].avg_throughput);
	printf("\n");
	
}
