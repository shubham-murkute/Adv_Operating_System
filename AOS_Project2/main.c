#include <stdio.h>
#include "utility.h"
#include "process.h"
#include "stat.h"

void dispQueue(queue *q) {
	node *ptr = (node *)q->head;
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
		printf("\n*********************************************************************************\n");
		printf("\t\t\t\tIteration %d\n",ctr+1);
		printf("*********************************************************************************\n");

		linked_list * processList = generateProcesses(52);
		node * ptr = processList->head;
		//printf("-----------------------------------------------------------------------------------------------------------------------------\n");
		printf("-------------------------------------------------\n");
		printf("Process ID | Arrival Time | Run Time | Priority |\n");
		printf("-------------------------------------------------\n");
		while(ptr != NULL) {
			process *proc = (process *)ptr->data;
			printf("%10c | %12.1f | %8.1f | %8d |\n",proc->pid,proc->arrivalTime,proc->runTime,proc->priority);
			ptr = ptr->next;
		}
		printf("--------------------------------------------------\n");
		printf("Total No. of Processes : %d\n",processList->size);
		printf("--------------------------------------------------\n");

		//First Come First Serve
		fcfs[ctr] = firstComeFirstServeNP(processList);
		
		//Call other functions here in the respective arrays
		
		
		
		ctr++;
	}

	for (int i = 0; i<6 ; i++)
	{
			final[i].avg_response_time = 0;
			final[i].avg_wait_time = 0;
			final[i].avg_turnaround = 0;
			final[i].avg_throughput = 0;
	}

	// Total of all avaerage values
	for (int i = 0; i<5 ; i++)
	{
			//FCFS
			final[0].avg_response_time += fcfs[i].avg_response_time;
			
			final[0].avg_wait_time += fcfs[i].avg_wait_time;

			final[0].avg_turnaround += fcfs[i].avg_turnaround;
			
			final[0].avg_throughput += fcfs[i].avg_throughput;
			
			//Other algos
			
			
			
	}

	//Average for all the algorithms
	for (int i = 0; i<6 ; i++)
	{
			final[i].avg_response_time /= 5;
			final[i].avg_wait_time /= 5;
			final[i].avg_turnaround /= 5;
			final[i].avg_throughput /= 5;
	}
	printf("\n\n\n");
	//printf("*****************************************************************************************************************************\n");
	//printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("_____________________________________________________________________________________________________________________________\n\n");
	printf("The average of the 5 runs of every algorithm is as follows:\n");
	printf("\n");
	printf("ALGORITHM: First-come First-served (FCFS) [non-preemptive]:\n");
	printf("Average Response Time: %.1f\n",final[0].avg_response_time);
	printf("Average Wait Time: %.1f\n",final[0].avg_wait_time);
	printf("Average Turn Around Time: %.1f\n",final[0].avg_turnaround);
	printf("Average Throughput: %.1f\n",final[0].avg_throughput);
	printf("\n");
	
	//Print other stats here
}
