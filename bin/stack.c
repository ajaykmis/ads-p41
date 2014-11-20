#include<stdio.h>
#include<stdlib.h>
#include "stack.h"

void push (stack **head, long data ) {

    stack *ptr = (stack *) malloc(sizeof(stack *));
    ptr->data = data ; 
    ptr->next = *head ;
    *head = ptr ; 
}

int isEmpty (stack *head) {

    if (head == NULL){
        return 1 ; 
    }
    else {
        return 0 ; 
    }

}

void print_stack(stack *head){

    stack *ptr = head ; 
    while (ptr) {
        printf("%ld ", ptr->data);
        ptr = ptr->next ; 
    }
    printf("\n");
}

long pop (stack **head) {
   
    if (*head == NULL) {
        return INT32_MIN ; 
    }
    stack *temp = *head ; 
    long data = (*head)->data ; 
    *head = (*head)->next ;
    free(temp);
    return data ; 
}


