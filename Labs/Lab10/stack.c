#include "stack.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

struct stackType {
    int items[STACK_CAPACITY];
    int count;
};

stack createStack(void)
{
    stack s = malloc(sizeof *s);
    if (s != NULL)
        s->count = 0;
    return s;
}

void destroyStack(stack s)
{
    free(s);
}

int size(stack s)
{
    return s == NULL ? 0 : s->count;
}

bool push(stack s, int x)
{
    return put(s, x, size(s));
}

int pop(stack s)
{
    if (size(s) == 0)
        return INT_MIN;
    return s->items[--s->count];
}

void printStack(stack s)
{
    printf("[");
    for (int i = 0; i < size(s); ++i) {
        if (i > 0)
            printf(", ");
        printf("%d", s->items[i]);
    }
    printf("]");
}

int get(stack s, int p)
{
    if (p < 0 || p >= size(s))
        return INT_MIN;
    return s->items[p];
}

bool put(stack s, int x, int p)
{
    if (s == NULL || p < 0 || s->count == STACK_CAPACITY)
        return false;

    if (p > s->count)
        p = s->count;

    /* Shift toward the top to leave room at position p. */
    for (int i = s->count; i > p; --i)
        s->items[i] = s->items[i - 1];

    s->items[p] = x;
    ++s->count;
    return true;
}
