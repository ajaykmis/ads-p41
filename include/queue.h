#ifndef QUEUE_H
#define QUEUE_H

typedef struct queue_t{
    long data ; 
    struct queue_t *next ; 
}queue ; 

queue *head ; 
queue *tail ; 

void enqueue(queue **, queue **, long);
void print_queue(queue *);
long delete_q(queue **, queue **);
int isEmpty(queue *);
#endif 

