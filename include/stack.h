#ifndef STACK_H
#define STACK_H

typedef struct stack_t{

    long data ; 
    struct stack_t *next ; 
}stack ; 

stack *head  ; 


void push(stack **, long);
void print_stack(stack *);
long pop(stack **);
int isEmpty(stack *);
#endif 

