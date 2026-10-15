# Abstract Data Types (ADTs)
## EGRE 246 — Chapter 6: Data Structures and ADTs

These notes follow the organization and terminology used in the EGRE 246 lecture material. The chapter explains ADTs mainly by building a **List ADT in C**.

---

# 1. Data Types

A **data type** consists of two things:

1. A collection of possible values
2. A set of basic operations that can be performed on those values

For example, the C `int` data type has:

- a legal range of integer values, from `INT_MIN` to `INT_MAX`
- operations such as:
  - `+`
  - `-`
  - `>=`
  - `=`

Traditionally, basic data types are treated as **atomic entities**, meaning the programmer normally thinks of them as single values rather than as objects that are divided into smaller component parts.

---

# 2. What Is an Abstract Data Type?

An **Abstract Data Type (ADT)** is a data type whose implementation details are hidden from the programmer using it.

The important idea is:

> The programmer should know **what the type does** and **which operations are available**, without needing to know **how those operations are implemented internally**.

The lecture defines an ADT as a data type that does not give the programmer access to the details of how its values and operations are implemented.

## Main idea

An ADT separates:

- **what the data type does**
- from
- **how the data type does it**

This separation is one of the most important ideas in the chapter.

---

# 3. Data Types vs. Data Structures

The lecture also distinguishes a **data type** from a **data structure**.

## Data Type

A data type has:

- a collection of values
- operations that work on those values

Example:

```c
int
```

The `int` type has integer values and operations such as addition, subtraction, and comparison.

## Data Structure

A **data structure** is an abstract description of a way to organize data so that operations can be performed on it efficiently.

Like a data type, a data structure also has:

- values
- operations on those values

However, data structures are normally built from component parts and can be decomposed into those parts.

Examples include:

- arrays
- lists of strings

A data structure also contains **relationships or associations between its elements**.

For example, an array contains:

- multiple values
- a relationship between those values based on their positions
- operations that work on the array

---

# 4. Example Data Structure: A List

The chapter uses a **list** as the main example for explaining ADTs.

A list represents a countable number of **ordered values**.

Important properties of a list:

- the list may be empty
- a non-empty list has a first item
- it has a sequence of ordered items
- it has a last item
- the same value may appear more than once
- items can be fetched
- the list can be updated

For example:

```text
[10, 20, 30, 20]
```

The value `20` appears more than once, which is allowed.

The concept of a list and the operations performed on the list are independent of the exact type of value stored inside it.

The lecture describes this kind of structure as a **container**.

## Container

A container is a type-independent data structure used to:

- store data
- organize data
- access data

To define a list data structure, the chapter says that we need:

1. type definitions for the values stored in the list
2. function definitions for the operations performed on the list

---

# 5. Defining the List Item Type

The chapter first creates an abstraction for the type of value stored in the list.

```c
typedef int listItem;
```

Now the rest of the program can use:

```c
listItem
```

instead of directly using:

```c
int
```

This makes the list easier to change.

For example, if the list were later changed to store another type, the program would have one central definition to modify and could then be recompiled.

---

# 6. Defining a Special "Non-Item"

The lecture also introduces a special value that represents an invalid list item:

```c
#define NULL_ITEM INT_MIN
```

This can be useful when a function must return a `listItem`, but an error occurs or the requested item does not exist.

For example:

```c
listItem getItem(list l, int pos);
```

could return:

```c
NULL_ITEM
```

if `pos` is invalid.

## Important limitation

The lecture asks what drawback comes from choosing `INT_MIN` as the special non-item value.

The consequence is that if `INT_MIN` is being reserved to mean "not a valid item," then it cannot safely be used as an ordinary list value without creating ambiguity.

---

# 7. Defining the List Structure

The lecture initially demonstrates the list using a fixed-size array.

```c
typedef struct listType *list;
```

This means `list` is a typedef for a pointer to a `struct listType`.

A structure can then be defined similar to:

```c
#define LIST_SIZE 100

struct listType {
    listItem data[LIST_SIZE];
    int count;
    int maxSize;
};
```

The important pieces are:

- `data` — stores the list items
- `count` — tracks how many items are currently in the list
- `maxSize` — tracks the maximum capacity

Now list variables can be declared with:

```c
list l1, l2, l3;
```

However, these are pointer variables. They do not automatically point to a valid list object.

Therefore, the program needs an operation that **creates** a valid list before it is used.

That operation becomes the list's constructor:

```c
list createList(void);
```

---

# 8. Categories of Container Operations

The chapter groups container operations into several categories.

| Category | Purpose |
|---|---|
| **Constructor** | Creates a data container and normally allocates its memory |
| **Destructor** | Destroys a container and deallocates its memory |
| **Getter** | Fetches data from a container |
| **Setter / Mutator** | Changes the state or contents of a container |
| **Predicate** | Returns a Boolean value describing some property of the container |
| **Reporter** | Reports useful information about the container |

Understanding these categories is useful because many ADTs use the same general pattern.

---

# 9. Constructor

The list constructor creates a valid list.

Prototype:

```c
list createList(void);
```

Usage:

```c
list l = createList();
```

After this call, `l` should refer to a valid empty list.

---

# 10. Destructor

The destructor destroys the list and releases its allocated memory.

Prototype:

```c
void destroyList(list);
```

Usage:

```c
destroyList(l);
```

After the list has been destroyed, it should no longer be used.

---

# 11. Getter

A getter retrieves an item from the list.

Prototype:

```c
listItem getItem(list, int);
```

Example:

```c
listItem item = getItem(l, 0);
```

Index `0` represents the first item in the list.

If the requested position does not exist, the chapter's implementation returns:

```c
NULL_ITEM
```

---

# 12. Setters / Mutators

Mutators change the contents or state of the list.

The lecture gives several examples.

## `addItem`

Adds an item to the end of the list.

Prototype:

```c
int addItem(list, listItem);
```

Example:

```c
// l = [1, 2, 3]

int i = addItem(l, 99);

// l = [1, 2, 3, 99]
// i = 3
```

The returned integer represents the index of the newly added item.

---

## `insertItem`

Inserts an item at a specified position.

Prototype:

```c
int insertItem(list, int, listItem);
```

Example:

```c
// l = [1, 2, 3]

int i = insertItem(l, 1, 99);

// l = [1, 99, 2, 3]
// i = 1
```

Because the new item is inserted at index `1`, existing elements at and after that position must move one position to the right.

---

## `removeItem`

Removes the item stored at a specified position.

Prototype:

```c
bool removeItem(list, int);
```

Example:

```c
// l = [1, 2, 3]

bool success = removeItem(l, 0);

// l = [2, 3]
// success = true
```

When an item is removed from an array-based list, the items after it must shift left to fill the empty location.

---

## `clearList`

Removes all items from the list.

Prototype:

```c
void clearList(list);
```

Example:

```c
// l = [1, 2, 3]

clearList(l);

// l = []
```

The list still exists, but it is now empty.

---

## `setItem`

Replaces an existing list item.

Prototype:

```c
bool setItem(list, int, listItem);
```

Example:

```c
// l = [1, 2, 3]

bool success = setItem(l, 2, 99);

// l = [1, 2, 99]
// success = true
```

---

# 13. Predicates

A predicate answers a true/false question about the container.

## `listEmpty`

Prototype:

```c
bool listEmpty(list);
```

Example:

```c
// l = [1, 2, 3]

bool isEmpty = listEmpty(l);

// isEmpty = false
```

---

## `listFull`

Prototype:

```c
bool listFull(list);
```

Example:

```c
// l = [1, 2, 3]

bool isFull = listFull(l);

// isFull = false
```

For the fixed-size array version of the list, this operation tells the programmer whether the list has reached its maximum capacity.

---

# 14. Reporters

Reporters return useful information about the list.

## `listSize`

Returns the current number of items.

Prototype:

```c
int listSize(list);
```

Example:

```c
// l = [1, 2, 3]

int n = listSize(l);

// n = 3
```

---

## `maxSize`

Returns the maximum number of items the list can currently contain.

Prototype:

```c
int maxSize(list);
```

Example:

```c
// l = [1, 2, 3]

int n = maxSize(l);
```

In the fixed-size example from the lecture, the value depends on the size chosen for the list.

---

# 15. Using a List Without Knowing Its Implementation

This is where the list begins to demonstrate the central ADT idea.

A programmer may use operations such as:

```c
list l = createList();

addItem(l, 1);
addItem(l, 2);
addItem(l, 3);

printf("%d\n", getItem(l, 0));

destroyList(l);
```

The programmer does not need to know exactly how:

- the items are stored
- the count is maintained
- insertion works internally
- memory is organized

The programmer only needs to understand the public operations.

The lecture emphasizes that an abstraction can be used without knowing how it is implemented when **what it does** is cleanly separated from **how it does it**.

---

# 16. Fixed-Array List Implementation

The lecture next demonstrates one possible implementation.

A fixed-size list can use a structure like:

```c
#define LIST_SIZE 10

struct listType {
    listItem data[LIST_SIZE];
    int count;
    int max;
};
```

The user of the ADT does not need to interact with these fields directly.

---

# 17. Creating the List

One implementation shown in the lecture is:

```c
list createList(void) {
    list l = malloc(sizeof(struct listType));

    l->count = 0;
    l->max = LIST_SIZE;

    return l;
}
```

This performs several tasks:

1. allocates enough memory for a `struct listType`
2. initializes the list as empty
3. records its maximum capacity
4. returns a pointer to the new list

This is why the function acts as a **constructor**.

---

# 18. Destroying the List

For the fixed-array version:

```c
void destroyList(list l) {
    free(l);
}
```

The structure itself was dynamically allocated, so it must eventually be released with `free()`.

This is the **destructor**.

---

# 19. Clearing the List

The lecture's simple fixed-array implementation clears the list by resetting the count:

```c
void clearList(list l) {
    l->count = 0;
}
```

The list object remains allocated.

Logically, it now contains zero items.

---

# 20. Size Operations

The current number of items can be reported with:

```c
int listSize(list l) {
    return l->count;
}
```

The maximum capacity can be reported with:

```c
int maxSize(list l) {
    return l->max;
}
```

The list can be tested for emptiness with:

```c
bool listEmpty(list l) {
    return (l->count == 0);
}
```

---

# 21. Adding an Item

A fixed-array implementation can add an item only if space remains.

The lecture's logic is:

```c
int addItem(list l, listItem item) {
    if (l->count < maxSize(l)) {
        l->data[l->count++] = item;
        return l->count - 1;
    } else {
        return -1;
    }
}
```

The expression:

```c
l->data[l->count++] = item;
```

stores the item at the current end of the list and then increases `count`.

If the list is full, the operation returns:

```c
-1
```

to indicate failure.

---

# 22. Inserting an Item

Insertion requires:

1. checking that the list still has space
2. checking that the requested position is valid
3. shifting later elements one position to the right
4. placing the new item
5. increasing the count

The lecture's algorithm is equivalent to:

```c
int insertItem(list l, int pos, listItem item) {
    if (l->count < maxSize(l) &&
        pos >= 0 &&
        pos <= listSize(l)) {

        for (int i = l->count - 1; i >= pos; i--) {
            l->data[i + 1] = l->data[i];
        }

        l->data[pos] = item;
        l->count++;

        return pos;
    }

    return -1;
}
```

The key concept is the shift to the right.

Example:

```text
Before:
[10, 20, 30]

Insert 99 at index 1

Shift:
30 -> index 3
20 -> index 2

After:
[10, 99, 20, 30]
```

---

# 23. Removing an Item

Removing an item requires the opposite shift.

The lecture's algorithm is equivalent to:

```c
bool removeItem(list l, int pos) {
    if (pos >= 0 && pos < l->count) {

        for (int i = pos + 1; i < l->count; i++) {
            l->data[i - 1] = l->data[i];
        }

        l->count--;
        return true;
    }

    return false;
}
```

Example:

```text
Before:
[10, 20, 30, 40]

Remove index 1

Shift:
30 -> index 1
40 -> index 2

After:
[10, 30, 40]
```

---

# 24. Getting an Item

The getter checks whether the requested index is valid.

```c
listItem getItem(list l, int pos) {
    if (pos >= 0 && pos < l->count) {
        return l->data[pos];
    } else {
        return NULL_ITEM;
    }
}
```

Valid positions range from:

```text
0
```

through:

```text
count - 1
```

If the position is invalid, the operation returns `NULL_ITEM`.

---

# 25. Setting an Item

The lecture uses a Boolean result to indicate whether replacement succeeds.

```c
bool setItem(list l, int pos, listItem item) {
    if (listSize(l) > 0 &&
        pos >= 0 &&
        pos < listSize(l)) {

        l->data[pos] = item;
        return true;
    }

    return false;
}
```

A setter changes an existing item but does not change the number of items in the list.

---

# 26. Turning the List Data Structure Into a List ADT

The lecture then explicitly asks how to turn the list data structure into a proper ADT.

The key goal is to **hide the implementation**.

The lecture notes that C cannot hide implementation details as completely as some other languages, but common practices can still provide strong separation between the public interface and private implementation.

The chapter uses several files to accomplish this.

---

# 27. Step 1 — Create an Item-Type Abstraction

Create a separate header file:

```text
itemType.h
```

Example:

```c
#ifndef ITEMTYPE_H
#define ITEMTYPE_H

#include <limits.h>

typedef int listItem;
#define NULL_ITEM INT_MIN

#endif
```

This file defines the type of item the container stores.

The important advantage is that the list operations can consistently use:

```c
listItem
```

instead of hard-coding `int` throughout the implementation.

---

# 28. Step 2 — Create the Public ADT Specification

The public header file is:

```text
list.h
```

This file contains the information programmers need in order to **use** the list.

It contains the public type and function declarations.

A version matching the lecture's interface is:

```c
#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include "itemType.h"

typedef struct listType *list;

list createList(void);
void destroyList(list);
void clearList(list);

listItem getItem(list, int);

int addItem(list, listItem);
int insertItem(list, int, listItem);

bool removeItem(list, int);
bool setItem(list, int, listItem);

int maxSize(list);
int listSize(list);

bool listEmpty(list);
bool listFull(list);

#endif
```

## Why is this important?

Notice what is **not** defined in the public header:

```c
struct listType {
    ...
};
```

The public code knows that `list` is a pointer to a `struct listType`, but it does not need access to the structure's internal fields.

That helps hide the representation of the list.

---

# 29. The Public Interface Is the ADT's "What"

The public header tells the programmer things such as:

- how to create a list
- how to destroy it
- how to add an item
- how to remove an item
- how to fetch an item
- how to replace an item
- how to ask for its size
- how to determine whether it is empty or full

It does **not** need to tell the programmer:

- whether the list uses an array
- whether it uses dynamically allocated memory
- how many structure fields exist
- how insertion shifts data
- how memory expansion works

This is the separation between the interface and implementation.

---

# 30. Step 3 — Create the Implementation File

The implementation is placed in:

```text
list.c
```

The lecture's fixed-size version contains the actual structure definition there:

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#include "itemType.h"
#include "list.h"

#define LIST_SIZE 10

struct listType {
    listItem data[LIST_SIZE];
    int count;
    int max;
};
```

The implementations of the list functions are also placed in `list.c`.

This means the internal list representation is kept out of the public header.

---

# 31. Why Keeping the Structure in `list.c` Matters

Code that includes:

```c
#include "list.h"
```

can use:

```c
list
```

but does not need the complete definition of:

```c
struct listType
```

The lecture points out that the implementation file can be compiled separately and distributed as object code.

This makes the internal list structure hidden from the programmer using the ADT.

---

# 32. Step 4 — Compile and Use the ADT

The lecture shows two approaches.

## Compile the main program and ADT implementation together

```bash
gcc main.c list.c
```

## Compile the ADT separately

First:

```bash
gcc -c list.c
```

This creates an object file such as:

```text
list.o
```

Then compile the user's program with the compiled implementation:

```bash
gcc main.c list.o
```

The second method demonstrates the ADT idea especially well because the programmer can use the compiled implementation without needing to see the implementation source.

---

# 33. Example Program Using the List ADT

A program using the ADT can look like:

```c
#include <stdio.h>
#include "list.h"

int main(void) {
    int i;
    int n;

    list l = createList();

    for (i = 1; i <= 10; i++) {
        addItem(l, i);
    }

    for (i = 0; i < 10; i++) {
        printf("%d ", getItem(l, i));
    }

    printf("\n");

    if (removeItem(l, 3)) {
        printf("successfully removed item #3\n");
    }

    insertItem(l, 2, -99);

    i = 0;

    while ((n = getItem(l, i++)) != NULL_ITEM) {
        printf("%d ", n);
    }

    printf("\n");

    destroyList(l);

    return 0;
}
```

The important point is not the exact program output.

The important point is that the program uses only the **public ADT operations**.

It does not directly access fields such as:

```c
l->data
l->count
l->max
```

Those are implementation details.

---

# 34. Fixed-Size Array Limitation

The first list implementation uses:

```c
listItem data[LIST_SIZE];
```

This means its maximum capacity is fixed at compile time.

The lecture then asks whether this could become a problem.

Yes: if more items are needed than the compile-time capacity allows, the fixed array cannot grow automatically.

This motivates a **dynamic array** implementation.

---

# 35. Dynamic Arrays

The lecture describes a dynamic array as an array that can expand, and possibly contract, at runtime as needed.

Instead of this:

```c
listItem data[LIST_SIZE];
```

the structure can use a pointer:

```c
struct listType {
    listItem *data;
    int count;
    int max;
};
```

Now the storage for the actual list items can be allocated separately at runtime.

---

# 36. Dynamic List Initialization

For the dynamic version, the lecture initializes the list with no data array yet:

```c
list createList(void) {
    list l = malloc(sizeof(struct listType));

    l->data = NULL;
    l->count = 0;
    l->max = 0;

    return l;
}
```

So initially:

```text
data  -> NULL
count -> 0
max   -> 0
```

The array is allocated only when space is actually required.

---

# 37. Expanding the Dynamic Array

The lecture uses a fixed expansion amount:

```c
#define SIZE_INCREMENT 4
```

A helper function expands the internal array.

The important algorithm is:

1. allocate a new, larger array
2. copy the current items into it
3. free the old array
4. make the list point to the new array
5. increase the recorded maximum size

A version matching the lecture's logic is:

```c
static void expandArray(list l) {
    listItem *tptr =
        malloc(sizeof(listItem) *
               (l->max + SIZE_INCREMENT));

    for (int i = 0; i < l->count; i++) {
        tptr[i] = l->data[i];
    }

    free(l->data);

    l->data = tptr;
    l->max += SIZE_INCREMENT;
}
```

The function is declared:

```c
static
```

The lecture notes that a static function can only be used inside the file where it is defined.

That makes `expandArray()` an implementation helper rather than a public ADT operation.

---

# 38. Adding an Item With Dynamic Expansion

For the dynamic version, `addItem()` can check whether the current array is full.

If there is no room, it expands the array first.

```c
int addItem(list l, listItem item) {
    if (l->count >= maxSize(l)) {
        expandArray(l);
    }

    l->data[l->count++] = item;

    return l->count - 1;
}
```

The programmer using the ADT still calls:

```c
addItem(l, value);
```

exactly as before.

The internal implementation has changed, but the public interface has not.

This is one of the strongest demonstrations of the ADT concept in the chapter.

---

# 39. Inserting Into the Dynamic Array

The same idea applies to insertion.

Before shifting and inserting, the implementation can expand the array if necessary.

```c
int insertItem(list l, int pos, listItem item) {
    if (pos >= 0 && pos <= listSize(l)) {

        if (l->count >= maxSize(l)) {
            expandArray(l);
        }

        for (int i = l->count - 1; i >= pos; i--) {
            l->data[i + 1] = l->data[i];
        }

        l->data[pos] = item;
        l->count++;

        return pos;
    }

    return -1;
}
```

Again, the public operation remains:

```c
insertItem(...)
```

even though the internal memory-management strategy is different.

---

# 40. Destroying a Dynamic List

The dynamic version has two separate allocations:

1. the `struct listType`
2. the dynamic array referenced by `data`

Therefore both must be freed.

```c
void destroyList(list l) {
    free(l->data);
    free(l);
}
```

This is different from the earlier fixed-array version, where freeing the structure was enough because the array was stored directly inside the structure.

---

# 41. Why the Dynamic-Array Example Matters for ADTs

This example shows why abstraction is useful.

The implementation can change from:

```c
listItem data[LIST_SIZE];
```

to:

```c
listItem *data;
```

without forcing programs using the ADT to change the way they call:

```c
createList()
addItem()
insertItem()
getItem()
removeItem()
destroyList()
```

The **implementation changed**, but the **interface stayed the same**.

That is exactly the separation the lecture is trying to demonstrate.

---

# 42. Interface vs. Implementation

A useful way to organize the lecture's design is:

```text
PROGRAM USING THE LIST
        |
        v
     list.h
  Public interface
        |
        v
     list.c
  Implementation
        |
        v
Internal representation
(array, dynamic array, etc.)
```

The program should depend on the public interface.

It should not depend on the internal representation.

---

# 43. File Organization for the List ADT

The lecture's design can be summarized as:

```text
itemType.h
    |
    |-- defines listItem
    |-- defines NULL_ITEM
    |
    v
list.h
    |
    |-- defines public list type
    |-- declares public operations
    |
    v
main.c
    |
    |-- uses the list through list.h
```

The implementation is separate:

```text
list.c
    |
    |-- defines struct listType
    |-- implements the list operations
    |-- contains private helper functions
```

This organization helps separate the ADT's specification from its implementation.

---

# 44. Why `typedef struct listType *list;` Helps

The public header contains:

```c
typedef struct listType *list;
```

but the complete structure definition is kept in `list.c`.

This lets user code declare:

```c
list l;
```

and pass that list to public functions without needing to know the structure's fields.

For example:

```c
list l = createList();
addItem(l, 42);
destroyList(l);
```

The user knows that `l` represents a list.

The user does not need to know whether it internally contains:

```c
data
count
max
```

or whether a future implementation uses an entirely different representation.

---

# 45. Public Operations vs. Internal Operations

Public operations are declared in the header and are intended for the programmer using the ADT.

Examples:

```c
createList()
destroyList()
addItem()
insertItem()
removeItem()
getItem()
setItem()
listSize()
maxSize()
listEmpty()
listFull()
```

An internal helper such as:

```c
static void expandArray(list l);
```

is not part of the public interface.

It exists only to help implement the ADT.

That means a user of the ADT should not call it directly.

---

# 46. A Major ADT Design Principle

A program using an ADT should avoid code like:

```c
l->count = 4;
l->data[0] = 99;
```

because this depends directly on the internal structure.

Instead, the program should use the public interface:

```c
addItem(l, 99);
setItem(l, 0, 99);
listSize(l);
```

This lets the implementation change later without requiring the user program to be rewritten around private details.

---

# 47. What the Programmer Using the ADT Needs to Know

The programmer needs to know:

- the ADT's public type
- which operations exist
- what each operation expects
- what each operation returns
- what the operations do

The programmer does **not** need to know:

- the exact structure fields
- whether storage is fixed or dynamic
- how shifting is implemented
- how expansion is performed
- how internal bookkeeping works

---

# 48. Why ADTs Are Useful

Based on the chapter's examples, ADTs provide several practical advantages.

## 1. Implementation hiding

The user works with the public interface instead of manipulating internal fields.

## 2. Easier implementation changes

The fixed-array implementation can be replaced by a dynamic-array implementation while keeping the same public operations.

## 3. Cleaner program structure

The ADT can be separated into:

- public definitions
- implementation code
- user code

## 4. Reuse

Once the list has been implemented, multiple programs can use the same interface.

## 5. Reduced dependence on internal details

Programs become less tightly tied to one particular storage method.

---

# 49. ADT vs. Data Structure — Important Distinction

A **data structure** describes how data is organized and related.

An **ADT** emphasizes hiding the details of that organization from the programmer using it.

For the lecture's list example:

```text
Concept:
List

Possible implementation:
Fixed-size array

Another possible implementation:
Dynamic array
```

A program using the ADT should be able to work through the same public operations even if the implementation changes.

---

# 50. The Most Important Idea From the Chapter

The central idea can be summarized as:

```text
ADT = public behavior and operations
      without requiring the user to know
      the internal implementation
```

Or, in the language emphasized throughout the lecture:

```text
WHAT it does
     vs.
HOW it does it
```

The list example demonstrates this by separating:

```text
itemType.h  -> item abstraction
list.h      -> public specification/interface
list.c      -> private implementation
main.c      -> program using the ADT
```

The programmer works through the public operations, while the representation can remain hidden and can even change later.

---

# 51. Quick Review

## Data type

A collection of values together with operations on those values.

## Data structure

A way of organizing related data so operations can be performed on it.

## ADT

A data type or data structure whose implementation details are hidden from the programmer using it.

## Container

A type-independent structure for storing, organizing, and accessing data.

## Constructor

Creates the container.

```c
createList()
```

## Destructor

Destroys the container and releases memory.

```c
destroyList()
```

## Getter

Retrieves data.

```c
getItem()
```

## Setter / Mutator

Changes the container.

```c
addItem()
insertItem()
removeItem()
clearList()
setItem()
```

## Predicate

Returns true or false about the container.

```c
listEmpty()
listFull()
```

## Reporter

Returns information about the container.

```c
listSize()
maxSize()
```

---

# 52. Final Study Summary

For EGRE 246, remember these points:

1. A **data type** consists of values and operations.
2. A **data structure** organizes multiple related values and provides operations on them.
3. An **ADT hides implementation details** from the programmer using it.
4. The lecture demonstrates ADTs with a **list**.
5. A list is an ordered container that may contain duplicate values.
6. `listItem` abstracts the type of item stored in the list.
7. `typedef struct listType *list;` lets the public interface refer to a list without exposing the full structure definition.
8. Container operations are grouped into constructors, destructors, getters, setters/mutators, predicates, and reporters.
9. `list.h` contains the public specification.
10. `list.c` contains the implementation.
11. The implementation may be separately compiled.
12. A fixed-size array implementation can later be changed to a dynamic-array implementation.
13. The program using the ADT should not need to change simply because the implementation changes.
14. The core ADT principle is to separate **what the object does** from **how it does it**.

---

## Core Mental Model

```text
            USER PROGRAM
                 |
                 | calls public operations
                 v
        +-------------------+
        |     LIST ADT      |
        |-------------------|
        | createList()      |
        | addItem()         |
        | insertItem()      |
        | removeItem()      |
        | getItem()         |
        | setItem()         |
        | listSize()        |
        | destroyList()     |
        +-------------------+
                 |
                 | implementation hidden
                 v
        +-------------------+
        | Internal Storage  |
        |-------------------|
        | fixed array       |
        |       OR          |
        | dynamic array     |
        |       OR          |
        | another method    |
        +-------------------+
```

The user should interact with the **top box**.

The ADT implementation controls the **bottom box**.
