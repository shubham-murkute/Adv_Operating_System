#include <stdio.h>
#include <stdlib.h>

#define NUM 20

typedef struct process {
    int name;
    int burstTime;
    int arrivalTime;
    int waitTime;
} process;

typedef struct processQueue {
    process list[NUM];
    int size;
    int front;
    int rear;
} processQueue;

typedef struct average_stats {
    float avg_wait_time;
    float avg_turnaround;
    float avg_response_time;
    float throughput;
} average_stats;

processQueue* initQueue() {
    processQueue *pq = (processQueue*) malloc(sizeof(processQueue));
    pq->size = 0;
    pq->front = 0;
    pq->rear = -1;
    return pq;
}

void enQueue(processQueue *pq, process *p) {
    pq->size++;
    pq->rear = (pq->rear + 1) % NUM;
    pq->list[pq->rear] = *p;		
}

void deQueue(processQueue *pq, int time, int ct[]) {
    ct[pq->list[pq->front].name] = time;
    pq->size--;
    pq->front = (pq->front + 1) % NUM;
}

void sortQueue(processQueue *pq) {
    int i;
    process temp = pq->list[pq->front];
    for(i = pq->front; i < pq->rear; i++) {
        pq->list[i] = pq->list[i+1];
    }
    pq->list[pq->rear] = temp;
}

void sortListByAT(process list[]) {
    int i, j;
    for(i = 0; i < NUM; i++) {
        for(j = i+1; j < NUM; j++) {
            if(list[i].arrivalTime > list[j].arrivalTime) {
                process temp = list[i];
                list[i] = list[j];
                list[j] = temp;
            }
        }
    }
}

void checkProcessQueue(processQueue *pq, process list[], int time) {
    int i;
    for(i = 0; i < NUM; i++) {
        if(time == list[i].arrivalTime) {
            enQueue(pq, &list[i]);
        }
    } 
}

void run(process *p) {
    printf("%d ", p->name);
    p->burstTime--;	
}

void wait(process *p) {
    p->waitTime++;
}

average_stats roundRobinPreemptive(process list[]) {
    srand(200);
    int i, time, timeProcessed = 0;
    int totalWaitTime = 0;	
    int totalBurstTime = 0;
    int bt[NUM], tat = 0;
    int completionTime[NUM];
    float max = -1e9, min = 1e9;

    for(i = 0; i < NUM; i++) {	
        list[i].burstTime = (rand() % 10) + 1;
        bt[i] = list[i].burstTime;
        list[i].arrivalTime = rand() % 100;
        list[i].name = i;
        list[i].waitTime = 0;
        totalBurstTime += list[i].burstTime;
    }
  
    sortListByAT(list);
    processQueue *queue = initQueue();

    for(time = 0; time < totalBurstTime; time++) {
        timeProcessed++;
        checkProcessQueue(queue, list, time);
        if(queue->size != 0) {
            run(&queue->list[queue->front]);
        } else {
            printf("CPU Idle\n");
        }

        for(i = queue->front+1; i <= queue->rear; i++) {
            wait(&queue->list[i]);
        }

        if(queue->list[queue->front].burstTime == 0) {
            totalWaitTime += queue->list[queue->front].waitTime;
            deQueue(queue, timeProcessed, completionTime);
            timeProcessed = 0;
        }
    }

    for(i = 0; i < NUM; i++) {
        tat += list[i].waitTime + bt[i];
    }

    for(int l = 0; l < NUM; l++) {
        if(min > list[l].arrivalTime)
            min = list[l].arrivalTime;
    }

    for(int ll = 0; ll < NUM; ll++) {
        if(max < completionTime[ll])
            max = completionTime[ll];
    }

    average_stats avg;
    avg.avg_wait_time = (float)totalWaitTime / NUM;
    avg.avg_turnaround = (float)tat / NUM;
    avg.avg_response_time = (float)totalWaitTime / NUM;
    avg.throughput = NUM / (max - min);

    return avg;
}

int main() {
    process list[NUM];
    average_stats avg = roundRobinPreemptive(list);

    printf("\nAverage wait time - %f\n", avg.avg_wait_time);
    printf("\nAverage turn around time - %f\n", avg.avg_turnaround);
    printf("\nAverage response time - %f\n", avg.avg_response_time);
    printf("Throughput - %f\n", avg.throughput);
    return 0;
}
