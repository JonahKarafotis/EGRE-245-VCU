#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

#define STACK_CAPACITY 100

typedef struct stackType *stack;

stack createStack(void);
void destroyStack(stack s);
int size(stack s);
bool push(stack s, int x);
/* Returns INT_MIN when the stack is empty. */
int pop(stack s);
void printStack(stack s);
/* Positions start at 0 at the bottom of the stack. */
int get(stack s, int p);
/* Positions beyond the current size append at the top. */
bool put(stack s, int x, int p);

#endif
