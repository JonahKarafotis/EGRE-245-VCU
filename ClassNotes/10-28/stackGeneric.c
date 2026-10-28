#include <stdio.h>
#include <stdlib.h>
#include "stackGeneric.h"

#define STACKSIZE 10

struct stackType {
    void *data[STACKSIZE];
    int top;
};

stack createStack(void) {
    stack s = malloc(sizeof(struct stackType));
    if (s == NULL) {
        fprintf(stderr, "Error: stack allocation failed\n");
        exit(EXIT_FAILURE);
    }
    s->top = -1;
    return s;
}

void freeStack(stack s) {
    free(s);
}

void push(stack s, void *p) {
    if (isFull(s)) {
        printf("Error: stack overflow\n");
        exit(EXIT_FAILURE);
    }
    s->data[++s->top] = p;
}

void pop(stack s) {
    if (isEmpty(s)) {
        printf("Error: stack underflow\n");
        exit(EXIT_FAILURE);
    }
    s->top--;
}

void *top(stack s) {
    if (isEmpty(s)) {
        printf("Error: stack underflow\n");
        exit(EXIT_FAILURE);
    }
    return s->data[s->top];
}

bool isEmpty(stack s) {
    return s->top == -1;
}

bool isFull(stack s) {
    return s->top == STACKSIZE - 1;
}