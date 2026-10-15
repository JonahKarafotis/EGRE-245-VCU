# Quiz 2 Review

## Q1: Create a dynamic array
- Ask the user for input and store it in a dynamically allocated array.
- Use `malloc` to allocate enough memory for the type and number of elements needed.
- Always check whether memory allocation succeeded before using the pointer.

```c
int *arr;
int n;

printf("Enter the number of elements: ");
scanf("%d", &n);

arr = malloc(n * sizeof(int));
if (arr == NULL) {
    printf("Memory allocation failed\n");
    return 1;
}
```

## Q2: Create a function that swaps two values
- The function should take pointers so it can modify the actual values in memory.
- If you pass the values themselves, the function only changes a local copy.

```c
void swap(int *x, int *y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}
```

```c
int main(void) {
    int a = 10;
    int b = 5;

    swap(&a, &b);

    printf("a = %d\n", a);
    printf("b = %d\n", b);

    return 0;
}
```

### Bonus: Swap without a third variable
This works, but it is less readable and can overflow for large integers.

```c
int a = 10;
int b = 5;

a = a + b;
b = a - b;
a = a - b;
```

This produces:
- `a = 10 + 5 = 15`
- `b = 15 - 5 = 10`
- `a = 15 - 10 = 5`

So the values are swapped successfully.

---

## Dynamic Memory in the Heap

```c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int *p1, *p2; // pointers stored in the stack

    p1 = malloc(sizeof(int)); // allocate memory in the heap
    p2 = malloc(sizeof(int)); // allocate another block in the heap

    *p1 = 2;
    *p2 = 3; // assign values to heap memory

    free(p1);
    free(p2);

    return 0;
}
```

### Important notes
- Local variables and pointers are usually stored on the stack.
- `malloc` allocates memory on the heap at runtime.
- The heap is used for dynamic memory that must be created while the program is running.
- `free()` releases the memory when it is no longer needed.

### Dangling pointer
After we call `free(p1)`, the memory is released, but the pointer still points to that address. This is called a dangling pointer.

- The data may still remain in memory for a short time.
- It is unsafe to use the pointer after freeing it.
- If another operation writes over that memory, we can no longer trust the old pointer.

So, after freeing memory, set the pointer to `NULL` if possible:

```c
free(p1);
p1 = NULL;
```

---

## Important question
When allocating memory, does it always go at the “top” of the heap?

Not necessarily. The exact address is controlled by the operating system and the memory allocator. It does not always mean “the top of the heap” in a simple stack-like way. The allocator may:
- reuse freed blocks,
- place blocks in different regions,
- manage memory in a more complex way than a simple last-in-first-out structure.

So, we should not assume that each new allocation goes to a predictable fixed location.

