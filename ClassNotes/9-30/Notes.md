# Data Types and Data Structures

## Data Type

A **data type** is a set of possible values and the operations that can be performed on those values.

For example, the `int` type represents whole-number values. C allows operations such as addition and comparison on integers.

In the traditional view, a data type is **atomic**: a programmer uses its values without treating them as smaller component parts.

## Abstract Data Type (ADT)

An **abstract data type (ADT)** describes what values and operations are available without exposing how they are implemented.

This separation lets a programmer use an ADT without needing to know its internal details.

## Data Structure

A **data structure** is a way of organizing data so that particular operations can be performed efficiently.

Like a data type, a data structure has values and operations. Unlike an atomic data type, a data structure is usually made from components that can be examined or combined. Its components have relationships that define how the data is organized.

Examples:

- A list of strings organizes multiple string values in a sequence.
- An array stores multiple values in indexed positions.

Data structures can be built from other data types and structures. We will study several useful data structures this semester.


## Lists

A **list** is an ordered collection of values. The same value may appear more than once, and a list may be empty or contain one or more items.

List operations commonly let a program access items, identify the first and last items, and update the list. These operations describe the list independently of the type of values it stores. For example, a list could contain integers, characters, or strings.

In C, a list implementation is a **container**: a data structure that stores, organizes, and provides access to values. To implement a list, define:

- The types used to represent the list and its values.
- Functions that perform the list operations.


### Choosing a List Item Type

Use a type alias such as `listItem` wherever list operations refer to an item. This makes the implementation easier to adapt: change the alias and any related definitions, then recompile.

For example, if list items are integers, `INT_MIN` can be used as a special value to mean “no valid item.” The `<limits.h>` header defines `INT_MIN`.

```c
#include <limits.h>

typedef int listItem;
#define NULL_ITEM INT_MIN
```

This special value is called a **sentinel**. A sentinel is convenient when an operation needs to report that it could not return a valid item. For example, a function might return `NULL_ITEM` when a requested position does not exist.

The problem is that the function's return value now has two possible meanings: `INT_MIN` could be a real integer stored in the list, or it could mean “no item.” A caller cannot tell which meaning was intended:

```c
listItem item = getItem(values, index);
if (item == NULL_ITEM) {
	/* The lookup failed, or the stored item was INT_MIN. */
}
```

If the program reserves `INT_MIN` as the sentinel, it cannot also use that value as ordinary list data without risking confusion. The sentinel must also suit the chosen `listItem` type. Another design is to return a success/failure status and write the item through an output parameter. This keeps every integer available as list data.

### Storing List Values

One simple implementation stores list items in a fixed-size array. The array has room for at most `LIST_SIZE` items:

```c
#define LIST_SIZE 100

typedef struct listType *list;

struct listType {
	listItem data[LIST_SIZE];
	int count;
	int maxSize;
};
```

- `data` stores the items. Its fixed capacity is `LIST_SIZE`.
- `count` records how many items are currently in the list.
- `maxSize` records the capacity chosen for this list; it must not exceed `LIST_SIZE`.

The `list` type is a pointer to the structure. A creation function can allocate and initialize the structure, and list operations can use the pointer to work on the same list. Declaring `list values;` alone does not create a list; the pointer must receive a valid value from a creation function before it is used. If the list is dynamically allocated, it must eventually be released by a destruction function.

## Container Operations

Container operations are functions that create, access, change, or report information about a data structure. In C, a function signature shows its return type, name, and parameter types. For example:

```c
bool setItem(list values, int index, listItem item);
```

This function returns a `bool` (`true` or `false`). It receives a `list`, an `int` index, and a `listItem` value. The `list` and `listItem` types are defined in the sections above. Include `<stdbool.h>` when using `bool` in C.

### Create and Destroy

```c
list createList(void);
void destroyList(list values);
```

- `createList` takes no arguments (`void` inside the parentheses) and returns a `list`. It creates and initializes the container, allocating memory if needed.
- `destroyList` takes a `list` and returns nothing (`void`). It releases resources owned by that list. Do not use the list after destroying it.

```c
list values = createList();
destroyList(values);
```

### Read and Change Items

```c
listItem getItem(list values, int index);
int addItem(list values, listItem item);
int insertItem(list values, int index, listItem item);
bool setItem(list values, int index, listItem item);
bool removeItem(list values, int index);
void clearList(list values);
```

- `getItem` takes a list and a zero-based index, then returns the item at that position as a `listItem`. Index `0` means the first item. If an invalid index is reported using `NULL_ITEM`, a real item equal to the sentinel is ambiguous, as explained above.
- `addItem` takes a list and an item, appends the item, and returns its index as an `int`.
- `insertItem` takes a list, an index, and an item. It inserts the item at that position and returns the inserted item's index as an `int`.
- `setItem` takes a list, an index, and a replacement item. It returns `true` if the item was replaced, or `false` if the index was invalid.
- `removeItem` takes a list and an index. It removes the item there and returns `true` if successful, or `false` if the index was invalid.
- `clearList` takes a list and removes all its items. It returns nothing, and the list can still be reused.

For `addItem` and `insertItem`, the functions return the item's index on success. The implementation must define what they return if the list is full or the index is invalid. A common choice is `-1`, because a valid list index is never negative; callers should check for failure before using the returned index.

### Check and Report

```c
bool listEmpty(list values);
bool listFull(list values);
int listSize(list values);
int maxSize(list values);
```

- `listEmpty` returns `true` if the list has no items; otherwise, it returns `false`.
- `listFull` returns `true` if the list has reached its capacity; otherwise, it returns `false`.
- `listSize` returns the current number of items as an `int`.
- `maxSize` returns the list's capacity as an `int`.

### Example Workflow

This example assumes the declarations above and their implementations are available. It creates a list, adds and inserts values, reads and updates items, checks the list, and then releases it:

```c
#include <stdbool.h>
#include <stdlib.h>

int main(void) {
	list values = createList();
	if (values == NULL) {
		return 1;
	}

	addItem(values, 10);
	addItem(values, 30);
	/* values: [10, 30] */

	int insertedIndex = insertItem(values, 1, 20);
	/* values: [10, 20, 30]; insertedIndex: 1 */

	listItem middle = getItem(values, 1);  /* middle: 20 */
	bool changed = setItem(values, 1, 25); /* values: [10, 25, 30] */
	bool removed = removeItem(values, 0);  /* values: [25, 30] */

	int count = listSize(values);     /* count: 2 */
	int capacity = maxSize(values);   /* capacity: at most LIST_SIZE */
	bool empty = listEmpty(values);   /* false */
	bool full = listFull(values);     /* depends on the capacity */

	clearList(values); /* values: [] */
	destroyList(values);
	return 0;
}
```

## Using the List

The list implementation can be compiled in a separate source file, such as `list.c`. A client program includes `list.h` and calls the functions declared there; it does not need to know how the list stores its items. To hide the representation, put the `list` pointer declaration in the header and keep the full `struct listType` definition in `list.c`.

### Public Interface (`list.h`)

These declarations tell the client which operations are available. The function bodies belong in `list.c`. Each function's return type appears before its name, and its parameter types appear inside the parentheses.

```c
#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <limits.h>

typedef int listItem;
#define NULL_ITEM INT_MIN
typedef struct listType *list;

/* Create and destroy the container. */
list createList(void);
void destroyList(list values);

/* Read or change items in the container. */
void clearList(list values);
listItem getItem(list values, int index);
int addItem(list values, listItem item);
int insertItem(list values, int index, listItem item);
bool removeItem(list values, int index);
bool setItem(list values, int index, listItem item);

/* Check the container or get information about it. */
int maxSize(list values);
int listSize(list values);
bool listEmpty(list values);
bool listFull(list values);

#endif
```

#### Create and Destroy

- `list createList(void)` takes no arguments and returns a new, initialized list. It returns `NULL` if creating the list fails.
- `void destroyList(list values)` releases the list's allocated memory. Do not use `values` after destroying it.

#### Read and Change Items

- `listItem getItem(list values, int index)` returns the item at a zero-based index. It returns `NULL_ITEM` if the index is invalid; note that this makes a real item equal to the sentinel ambiguous.
- `int addItem(list values, listItem item)` appends an item and returns its index, or `-1` if the list is full.
- `int insertItem(list values, int index, listItem item)` inserts an item before the item at `index` and returns the new index, or `-1` if insertion fails. An index equal to the current list size appends the item.
- `bool setItem(list values, int index, listItem item)` replaces the item at `index`; it returns `true` on success and `false` for an invalid index.
- `bool removeItem(list values, int index)` removes the item at `index`; it returns `true` on success and `false` for an invalid index.
- `void clearList(list values)` removes all items but keeps the list available for reuse.

#### Check and Report

- `bool listEmpty(list values)` returns `true` when the list contains no items.
- `bool listFull(list values)` returns `true` when the list has reached its capacity.
- `int listSize(list values)` returns the number of items currently stored.
- `int maxSize(list values)` returns the list's capacity.

### Client Program (`main.c`)

This program creates a list, adds the integers 1 through 10, and prints them using the public functions. It accesses the list through its `list` handle rather than accessing the internal array.

```c
#include "list.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	list values = createList();
	if (values == NULL) {
		return EXIT_FAILURE;
	}

	for (int value = 1; value <= 10; ++value) {
		if (addItem(values, value) == -1) {
			destroyList(values);
			return EXIT_FAILURE;
		}
	}

	for (int index = 0; index < listSize(values); ++index) {
		printf("%d ", getItem(values, index));
	}
	printf("\n");

	destroyList(values);
	return EXIT_SUCCESS;
}
```

Compile the client and implementation together; the header is included by both source files but is not compiled separately:

```sh
clang -std=c11 -Wall -Wextra main.c list.c -o use_list
```


## Bonus: `static` in C

The meaning of `static` depends on where it is used. It does **not** make a value constant or prevent it from changing.

- At file scope, `static` limits a variable or function's visibility to the source file where it is declared.
- Inside a function, a `static` local variable keeps its value between calls to that function.

Use `const` when the goal is to prevent code from modifying an object through a particular name. This is not a general security mechanism; other code may still be able to change the object through another route.

