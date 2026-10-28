# Class Notes — October 28

**Reminder: Quiz on Monday.**

## Generic programming

Based on [slides 150–153](../../Canvas/begre246.pdf#page=694), especially **slide 152** (PDF pages 696–697).

- The idea is to write one container that can work with different types of data.
- A stack still uses the same push/pop behavior whether it holds integers, floats, strings, or other objects.
- The slides describe generic programming as determining the data types when the program runs instead of at compile time. More precisely for this C example, the stack accepts addresses of different object types; **the caller must still know the correct type when reading each item**.
- In C, this example simulates generic behavior using `void *` pointers.

### What is a void pointer?

A `void *` is a pointer with no associated pointed-to data type. It stores an address and can be converted to and from a pointer to an object type, such as `int *` or `float *`.

```c
int number = 42;
void *p = &number;          // Store the address of an int
printf("%d\n", *(int *)p); // Convert to int *, then dereference
```

- `&number` means “the address of number.”
- `(int *)p` treats the stored address as a pointer to an integer.
- `*(int *)p` reads the integer at that address.
- We cannot directly dereference a `void *`; we must first convert it to the appropriate pointer type.
- A `void *` does not record the original type or make an incorrect cast safe.

## Generic stack interface — slide 151

The key changes are that `push` accepts a `void *` and `top` returns a `void *`. The stack's internal representation stays hidden from the caller.

### stackGENERIC.h

```c
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
```

`typedef struct stackType *stack;` makes `stack` a pointer type. `stack s` holds the address of the stack structure. In `createStack(void)`, `void` means the function takes no arguments; in `void *`, it describes a generic object pointer.

## Generic stack implementation — slide 152

This is the slide's fixed-capacity array implementation, formatted for readability. The allocation check is an added safeguard; the slide does not include it.

### stackGENERIC.c

```c
#include <stdio.h>
#include <stdlib.h>
#include "stackGENERIC.h"

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
```

### How the implementation works

| Part | Meaning |
| --- | --- |
| `void *data[STACKSIZE]` | Array of 10 generic pointers; stores addresses, not copies of the pointed-to objects |
| `int top` | Index of the most recently pushed item |
| `top = -1` | Empty stack; no valid top index |
| `data[++s->top] = p` | Increment the top index first, then store the address |
| `s->top--` | Logically remove the top item by lowering the index |
| `data[s->top]` | Return the address at the current top without removing it |
| `top == STACKSIZE - 1` | Full stack; with capacity 10, the final valid index is 9 |

- **LIFO: Last In, First Out.** The most recently pushed item is removed first.
- The first push changes `top` from `-1` to `0`.
- The number of stored items is `s->top + 1`.
- `pop` returns no value. To use an item before removing it, call `top(s)` and then `pop(s)`.
- **Overflow:** pushing onto a full stack. **Underflow:** calling `top` or `pop` on an empty stack.
- `pop` does not erase the old array slot or free the object. Lowering the index makes that slot logically unused; a later push can overwrite it.
- Push, pop, top, and the empty/full checks each take O(1) time. This stack has a fixed capacity of 10 pointers and does not resize.

## How would we use this? — slide 153

**To get an item back out of this stack and use it, we must already know its data type.** `top(s)` returns a `void *`, which carries an address but no information identifying whether the object is an `int`, `float`, string, or another type. The caller must convert that address to the correct pointer type before accessing the object. The stack cannot determine the type for us.

In this example, we know the types because we know the order in which we pushed the items. If we could not predict that order, we would need to store type information alongside each item, such as a type tag, so we could choose the correct conversion when retrieving it. Guessing the type or using the wrong cast can cause undefined behavior.

The slide demonstrates one stack holding addresses of integers, floats, and a string. This version adds `freeStack(s)` at the end to release the stack allocation.

```c
#include <stdio.h>
#include "stackGENERIC.h"

int main(void) {
    int l1[] = {1, 2, 3};
    float l2[] = {10.0, 20.0, 30.0};
    char *str = "how now brown cow";
    stack s = createStack();

    for (int i = 0; i < 3; i++) {
        push(s, &l1[i]);
        push(s, &l2[i]);
    }
    push(s, str);

    printf("%s\n", (char *)top(s));
    pop(s);

    while (!isEmpty(s)) {
        printf("%.2f\n", *(float *)top(s));
        pop(s);
        printf("%d\n", *(int *)top(s));
        pop(s);
    }

    freeStack(s);
    return 0;
}
```

### Trace the stack

The following values describe the objects whose addresses are stored:

```text
Bottom -> 1, 10.0, 2, 20.0, 3, 30.0, "how now brown cow" <- Top
```

The string is printed and removed first. Then each loop iteration prints and removes a float followed by an integer.

```text
how now brown cow
30.00
3
20.00
2
10.00
1
```

- `push(s, &l1[i])` passes the address of an integer, not the integer itself.
- `push(s, &l2[i])` passes the address of a float.
- `push(s, str)` passes the string's character pointer. Using `&str` would instead pass the address of the pointer variable, which is a different type.
- `%s` needs a `char *`, so the string pointer is converted but not dereferenced in the call.
- `*(float *)top(s)` and `*(int *)top(s)` convert the address and then read the corresponding numeric value.
- This loop works because the caller knows the insertion order. After removing the string, every remaining pair is a float on top of an integer. `!isEmpty(s)` alone would not guarantee two items in a different stack.

### Memory and type responsibilities

**The stack manages its own structure; the caller manages the pointed-to objects.**

- `freeStack(s)` frees the stack structure, including its embedded pointer array. It does not free the objects whose addresses were pushed.
- The arrays in this example remain alive throughout their use in `main`, so their element addresses remain valid. Do not return from a function while keeping pointers to its local variables in a stack that will be used later.
- If an object was allocated with `malloc`, the caller must arrange to free it when it is no longer needed. Do not call `free` on these local arrays or the string literal.
- Because the stack stores addresses, changing an original object changes the value later read through its stored pointer.
- Using the wrong pointer type to read an object can cause undefined behavior. A generic stack provides flexibility but does not automatically check types.

### Is `free(s)` enough, or do we need to free `data` too?

**For our code, `free(s)` is enough to release all memory allocated by the stack.** `createStack` makes only one allocation:

```c
stack s = malloc(sizeof(struct stackType));
```

The structure contains `void *data[STACKSIZE]`, an array embedded directly inside the structure. Space for all 10 pointers is already included in `sizeof(struct stackType)`. It is not a separate allocation, so `free(s)` releases both the pointer array and the `top` member together.

```c
void freeStack(stack s) {
    free(s);  // Releases the whole structure, including its data array
}
```

**Do not call `free(s->data)` in this implementation.** The array was not separately allocated; passing it to `free` is invalid and causes undefined behavior.

There are three different things to distinguish:

| Memory | How it is stored in our code | What to free |
| --- | --- | --- |
| Stack structure `s` | Allocated with `malloc` | `free(s)` |
| Array `s->data` containing the pointers | Embedded inside the structure | Already released by `free(s)` |
| Objects addressed by those pointers | Stored somewhere else | Depends on how those objects were allocated and who owns them |

In our `main` example, `l1` and `l2` are local arrays with automatic storage duration, and `str` points to a string literal. **None of these objects should be passed to `free`.** The local arrays' storage ends automatically when `main` returns; the string literal has static storage duration. Calling `freeStack(s)` is therefore enough for this example's explicit cleanup.

#### If we push an object allocated with `malloc`

Freeing a pointer does not automatically free the object it points to. If the caller separately allocates an integer, the caller must also arrange to free that integer:

```c
int *value = malloc(sizeof *value);
if (value == NULL) {
    freeStack(s);
    exit(EXIT_FAILURE);
}
*value = 42;
push(s, value);

// Assume this item is still on top and no other code needs it.
int *saved = top(s);
printf("%d\n", *saved);
pop(s);          // Removes the stored pointer; does not free the integer
free(saved);     // Releases the separately allocated integer
freeStack(s);    // Releases the stack structure
```

Only free an object once, when no code still needs it. A generic stack cannot safely free every stored address automatically: some may refer to local variables, string literals, shared objects, or the same allocation more than once.

#### What if `data` were a separately allocated array?

If the structure instead used `void **data` and creation separately allocated that array, cleanup would need to release both allocations:

```c
// Alternative design only; this is NOT our slide 152 structure.
struct stackType {
    void **data;
    int top;
};

// During creation, after allocating s and checking for failure:
s->data = malloc(STACKSIZE * sizeof *s->data);
// Check for allocation failure before using s->data.

// During cleanup, after successful creation:
free(s->data);   // Free the separately allocated pointer array first
free(s);         // Then free the structure holding its address
```

Even in this alternative, freeing the pointer array does not free the objects it points to. **Match cleanup to the actual allocations and ownership, not simply to the number of pointer variables.**

## Things to remember for review

- Why use `void *`? To let the same stack implementation accept addresses of different object types.
- What is stored? Addresses, not independent copies of the objects.
- How is empty represented? `top == -1`.
- How is full represented? `top == 9` for this 10-slot stack.
- Does `top(s)` remove an item? No; `pop(s)` removes it.
- Who knows the actual type? The caller, which must convert the pointer correctly before reading the object.
- Does freeing the stack free every pointed-to object? No.
