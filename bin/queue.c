#include<stdio.h>
#include<stdlib.h>
#include "queue.h"

void enqueue (queue **head, queue **tail,  long data ) {

    queue *ptr = NULL ; 
    ptr = (queue *) malloc(sizeof(queue));
    ptr->data = data ; 
    ptr->next = NULL ; // if head is NULL
    
    if(*head == NULL) {
        *head = ptr; 
        *tail = ptr; 
        return ;
    }
    
    (*tail)->next  = ptr ; 
     *tail = (*tail)->next ;  
}

int isEmpty (queue *head) {

    if (head == NULL){
        return 1 ; 
    }
    else {
        return 0 ; 
    }

}

void print_queue(queue *head){

    queue *ptr = head ; 
    while (ptr) {
        printf("%ld ", ptr->data);
        ptr = ptr->next ; 
    }
    printf("\n");
}

long delete_q (queue **head, queue **tail) {
   
    if (*head == NULL) {
        return INT32_MIN ; 
    }
    
    queue *temp = *head ; 
    long data = (*head)->data ; 
    if (*head == *tail) //single node
    {
        *tail = (*tail)->next ;  
    }
    *head = (*head)->next ;
    temp->next = NULL ;     
    free(temp);
    return data ; 
}


