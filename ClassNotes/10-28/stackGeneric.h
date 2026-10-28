#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

typedef struct stackType *stack;

stack createStack(void);
void freeStack(stack s);
void push(stack s, void *p);
void pop(stack s);
void *top(stack s);
bool isEmpty(stack s);
bool isFull(stack s);

#endif