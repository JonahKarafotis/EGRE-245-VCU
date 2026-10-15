# Double Pointers in C

A double pointer is needed when a program must work with the address of a
pointer itself. This is useful when a function needs to change the pointer,
such as allocating memory for it, making it point to a different variable, or
setting it to `NULL`. A regular pointer can change the data it points to, but
it cannot change the original pointer variable in the calling function.

A **double pointer** is a pointer that stores the address of another pointer.

```c
int value = 10;
int *ptr = &value;
int **double_ptr = &ptr;
```

The variables are connected like this:

```text
double_ptr -> ptr -> value
			  |      |
			address   10
```

## Meaning of `*`

- `ptr` is the address of `value`.
- `*ptr` is the value stored at that address: `10`.
- `double_ptr` is the address of `ptr`.
- `*double_ptr` is `ptr`.
- `**double_ptr` is the value reached through `ptr`: `10`.

## Basic example

```c
#include <stdio.h>

int main(void) {
	int value = 10;
	int *ptr = &value;
	int **double_ptr = &ptr;

	printf("value = %d\n", value);
	printf("*ptr = %d\n", *ptr);
	printf("**double_ptr = %d\n", **double_ptr);

	**double_ptr = 25;
	printf("value after change = %d\n", value);

	return 0;
}
```

Because `**double_ptr` reaches `value`, changing `**double_ptr` also changes
`value`.

## Using a double pointer in a function

A double pointer is useful when a function needs to change where a pointer
points. A single pointer parameter only changes a local copy of the pointer.

```c
#include <stdio.h>
#include <stdlib.h>

void create_number(int **number) {
	*number = malloc(sizeof(int));

	if (*number != NULL) {
		**number = 42;
	}
}

int main(void) {
	int *number = NULL;

	create_number(&number);

	if (number != NULL) {
		printf("number = %d\n", *number);
		free(number);
		number = NULL;
	}

	return 0;
}
```

Important steps:

1. `number` is an `int *`.
2. `&number` is an `int **`, so it can be passed to `create_number`.
3. `*number` changes the original pointer in `main`.
4. `**number` changes the integer stored in the allocated memory.
5. Always check `malloc` and call `free` when the memory is no longer needed.

## Common mistakes

```c
int **ptr;
```

This declares a double pointer, but it does not give it a valid address to
use. Initialize pointers before dereferencing them:

```c
int **ptr = NULL;
```

Do not use `*ptr` or `**ptr` while the pointer is `NULL`. Dereferencing a
`NULL` pointer can crash the program.