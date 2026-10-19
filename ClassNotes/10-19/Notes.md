# October 19 - Linked List Operations

Based on `Canvas/begre246.pdf`, Chapter 7, lecture slides 110-121 (PDF pages 473-574). The PDF repeats slides for animation steps. The functions below expand the lecture's pointer operations into study examples, including allocation checks and empty-list cases; they are not verbatim lecture code.

## 1. How a singly linked list works

A linked list stores data in **nodes**, which do not need to be next to each other in memory. Each node contains data and a pointer to the next node.

```text
head -> [2 | next] -> [4 | next] -> [6 | NULL]
```

- `head` holds the address of the first node; `head == NULL` means the list is empty.
- The last node's `next` is `NULL`.
- `p->data` accesses a node's data; `p->next` accesses its link.
- Never dereference `NULL`. Check a pointer before using `->`.
- A cursor is simply a pointer to the node where we are currently working.

These definitions and headers apply to all singly linked list examples below:

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef int listItem;
struct node {
    listItem data;
    struct node *next;
};
typedef struct node *llist;
```

This matches the lecture's notation: `llist` is already a pointer type. Thus, `llist *head` is a pointer to the caller's head pointer (`struct node **`). The structure tag `node` lets the structure refer to itself.

### Traversal

```c
void printList(llist head)
{
    for (llist p = head; p != NULL; p = p->next)
        printf("%d\n", p->data);
}
```

Use a separate cursor so traversing does not lose the caller's head. Reaching a node by position or searching for a value takes O(n) time in the worst case.

## 2. Functions that change the head

C passes arguments by value. Assigning to a function's local copy of `head` does not update the caller's variable.

Two common solutions:

1. Return the new head and assign it: `head = function(head);`.
2. Pass the head's address: `function(&head);`, then update `*head`.

The mutation functions below use the second approach. Their `head` argument must be a valid address such as `&list`, even when `list == NULL`. Insert functions return `false` if allocation fails; removal functions return `false` if nothing can be removed.

## 3. Insert at the front

```text
Before: head -----------------> [2] -> [4] -> NULL
Step 1: newNode -> [1] --------> [2] -> [4] -> NULL
Step 2: head ---> [1] ---------> [2] -> [4] -> NULL
```

```c
bool insertFront(llist *head, listItem value)
{
    llist newNode = malloc(sizeof *newNode);
    if (newNode == NULL)
        return false;

    newNode->data = value;
    newNode->next = *head;    // Preserve the original list.
    *head = newNode;          // Make the new node the first node.
    return true;
}
```

**Order matters:** connect the new node to the old head before changing the head. This also works for an empty list because the new node's `next` becomes `NULL`. Time: O(1).

## 4. Insert after a known node (middle or end)

Lecture slides 115-116 show insertion at the end and in the middle. Both can use this operation when we already have the preceding node:

```c
bool insertAfter(llist p, listItem value)
{
    if (p == NULL)
        return false;

    llist newNode = malloc(sizeof *newNode);
    if (newNode == NULL)
        return false;

    newNode->data = value;
    newNode->next = p->next;
    p->next = newNode;
    return true;
}
```

For `2 -> 4 -> 6`, inserting `100` after the node containing `2` gives `2 -> 100 -> 4 -> 6`.

Save the original successor in `newNode->next` first. If `p` is the tail, its successor is `NULL`, so the same function appends. Inserting after a known node takes O(1); finding that node may take O(n).

### Insert at the back starting from the head

```c
bool insertBack(llist *head, listItem value)
{
    if (*head == NULL)
        return insertFront(head, value);

    llist p = *head;
    while (p->next != NULL)
        p = p->next;
    return insertAfter(p, value);
}
```

Without a stored tail pointer, appending requires traversal: O(n). A maintained tail pointer allows O(1) appends, but must be updated whenever the tail changes.

## 5. Remove nodes

The key pattern is **save the node, reconnect the list, then free the removed node**. The examples assume nodes were allocated dynamically and belong to this list.

### Remove the front

```c
bool removeFront(llist *head)
{
    if (*head == NULL)
        return false;

    llist victim = *head;
    *head = victim->next;
    free(victim);
    return true;
}
```

For `2 -> 4 -> 6`, the new head points to `4`. Removing the only node makes the head `NULL`. Time: O(1).

### Remove the node after a known node

This is the operation shown on lecture slide 117. Here `p` points to the node **before** the one being deleted.

```c
bool removeAfter(llist p)
{
    if (p == NULL || p->next == NULL)
        return false;

    llist victim = p->next;
    p->next = victim->next;
    free(victim);
    return true;
}
```

```text
Before: p -> [2] -> [4] -> [6] -> NULL
After:  p -> [2] --------> [6] -> NULL
                    free [4]
```

This also removes the tail when the victim's `next` is `NULL`. Time: O(1) once the predecessor is known.

### Remove the first node with a matching value

```c
bool removeValue(llist *head, listItem value)
{
    llist previous = NULL;
    llist current = *head;

    while (current != NULL && current->data != value) {
        previous = current;
        current = current->next;
    }
    if (current == NULL)
        return false;                    // Empty list or value not found.

    if (previous == NULL)
        *head = current->next;           // Removing the head.
    else
        previous->next = current->next;  // Bypass a middle or last node.

    free(current);
    return true;
}
```

This deletes only the **first** match. Searching makes the operation O(n) in the worst case. A singly linked node has no `prev`, so keep track of the predecessor while searching.

### Remove the back

```c
bool removeBack(llist *head)
{
    if (*head == NULL)
        return false;
    if ((*head)->next == NULL)
        return removeFront(head);

    llist p = *head;
    while (p->next->next != NULL)
        p = p->next;
    return removeAfter(p);
}
```

Stop at the second-to-last node so its `next` can be changed. Time: O(n). Even with a tail pointer, a singly linked list still needs to find the predecessor to remove the tail.

### Delete the entire list

```c
void clearList(llist *head)
{
    while (*head != NULL)
        removeFront(head);
}
```

Time: O(n). The caller's head ends as `NULL`. Setting `head = NULL` alone would lose access to allocated nodes without freeing them.

### Example calls

```c
llist list = NULL;
if (!insertFront(&list, 4) || !insertFront(&list, 2) ||
    !insertBack(&list, 6)) {
    clearList(&list);
    exit(EXIT_FAILURE);
}
// list: 2 -> 4 -> 6 -> NULL
removeValue(&list, 4);  // list: 2 -> 6 -> NULL
printList(list);
clearList(&list);
```

## 6. Doubly linked lists

Lecture slides 119-120 add a backward link:

```text
NULL <- [2] <-> [4] <-> [6] -> NULL
         ^               ^
        head            tail (if stored)
```

```c
struct dlnode {
    int data;
    struct dlnode *prev;
    struct dlnode *next;
};
typedef struct dlnode *dllist;
```

For a noncircular doubly linked list:

- The first node's `prev` is `NULL`; the last node's `next` is `NULL`.
- If `p->next != NULL`, then `p->next->prev` must equal `p`.
- If `p->prev != NULL`, then `p->prev->next` must equal `p`.

Every change must keep both directions consistent. The following functions maintain a head only; an implementation that also stores a tail must update it when necessary.

### Insert at the front

```c
bool insertFrontDoubly(dllist *head, int value)
{
    dllist newNode = malloc(sizeof *newNode);
    if (newNode == NULL)
        return false;

    newNode->data = value;
    newNode->prev = NULL;
    newNode->next = *head;
    if (*head != NULL)
        (*head)->prev = newNode;
    *head = newNode;
    return true;
}
```

The old head must now point backward to the new head. The `if` handles the empty-list case. Time: O(1).

### Insert after a known node

```c
bool insertAfterDoubly(dllist p, int value)
{
    if (p == NULL)
        return false;
    dllist newNode = malloc(sizeof *newNode);
    if (newNode == NULL)
        return false;

    newNode->data = value;
    newNode->prev = p;
    newNode->next = p->next;
    if (p->next != NULL)
        p->next->prev = newNode;
    p->next = newNode;
    return true;
}
```

Four links are involved when inserting between two nodes: the new node's `prev` and `next`, the predecessor's `next`, and the successor's `prev`. Unlike the middle-only slide example, the `if` also allows insertion after the tail. Time: O(1) when `p` is known.

### Worked example: add `100` between `2` and `4`

Use the `struct dlnode` definition and `insertAfterDoubly` function above. Suppose `head` points to this existing list:

```text
Before:
NULL <- [2] <-> [4] <-> [6] -> NULL
         ^
        head
```

Here is the call to insert a new node after the first node:

```c
// Precondition: head points to the existing list 2 <-> 4 <-> 6.
dllist p = head;  // p points to the node containing 2.
if (!insertAfterDoubly(p, 100)) {
    fprintf(stderr, "Could not insert the new node.\n");
}
```

The function allocates a new node, puts `100` in its `data` field, and connects it between `p` and the original `p->next`. In this example, those neighbors contain `2` and `4`.

| Step | Statement inside the function | What it connects |
| --- | --- | --- |
| 1 | `newNode->prev = p;` | New node points backward to `2`. |
| 2 | `newNode->next = p->next;` | New node points forward to the original successor, `4`. |
| 3 | `p->next->prev = newNode;` | Node `4` points backward to the new node. |
| 4 | `p->next = newNode;` | Node `2` points forward to the new node. |

Step 3 runs only when `p->next != NULL`, as checked in the function. This avoids dereferencing `NULL` when inserting after the tail.

```text
After a successful insertion:
NULL <- [2] <-> [100] <-> [4] <-> [6] -> NULL
         ^
        head
```

**Why does the order matter?** Until the last step, `p->next` still identifies the original successor (`4`). If you set `p->next = newNode` too early, later statements using `p->next` would refer to the new node instead of `4`. Save the original successor before replacing that link.

The head stays the same because the new node is inserted after it. No existing data is moved; only links change. The insertion takes O(1) because `p` already identifies the insertion position. If you first search for a value, that search can take O(n).

To add at the very front instead, call `insertFrontDoubly(&head, 100)`. Passing `&head` allows that function to replace the head pointer. If the list is empty, use this front-insertion function because there is no existing node to pass to `insertAfterDoubly`.

### Remove a known node

```c
void removeNodeDoubly(dllist *head, dllist victim)
{
    if (victim == NULL)
        return;
    // Precondition: victim belongs to this list.
    if (victim->prev != NULL)
        victim->prev->next = victim->next;
    else
        *head = victim->next;

    if (victim->next != NULL)
        victim->next->prev = victim->prev;
    free(victim);
}
```

This handles the head, middle, tail, and only-node cases. Because `victim->prev` provides the predecessor, deleting a known node takes O(1). Searching for a node by value still takes O(n). Any other pointer to a freed node becomes dangling and must not be dereferenced.

## 7. When and why to choose each structure

Lecture slide 121 connects the choice to the operations the program performs most often.

| Need | Useful choice | Why |
| --- | --- | --- |
| Frequent access by index, such as item 500 | Array / dynamic array | O(1) indexed access; a linked list must follow links. |
| Frequent insertion/removal at the front | Singly linked list | O(1) changes to the head; useful for a stack. |
| Frequent insertion/removal after a known cursor | Singly linked list | Update a few links without shifting later items. |
| Frequent forward/backward movement | Doubly linked list | `next` and `prev` support a two-way cursor, such as a playlist. |
| Frequent removal of a known node | Doubly linked list | The predecessor is available immediately. |
| Frequent insertion/removal at both ends | Doubly linked list with head and tail | O(1) changes at either end; useful for a deque. |
| Growth/shrinkage with local edits | Linked list may help | Allocate/free individual nodes without moving existing elements. |

### Class rules: reasons for choosing a list data structure

Choose a structure based on the operations your program performs most often. A **cursor** is a pointer to the current node where you are working.

#### Frequent random requests: dynamic array

Here, “random requests” means accessing arbitrary positions, such as item 3, then item 500, then item 12. A dynamic array stores elements contiguously, so an index directly identifies an element in O(1) time. A linked list must follow links from a known node to reach the requested position, which can take O(n).

**Example:** a program that repeatedly looks up a student's score by its array index benefits from a dynamic array. Inserting or removing elements in the middle may still require shifting elements.

#### Many operations at a cursor: linked list

If a cursor already points to the node where work is happening, a singly linked list can insert after it or remove the node after it in O(1) time. These operations reconnect links instead of shifting all later elements as an array would.

**Example:** while processing a list of tasks, keep a cursor at the current task and repeatedly insert new tasks immediately after it.

The location matters: removing the cursor's own node in a singly linked list generally requires its predecessor. Searching for the cursor's position can also take O(n); the constant-time advantage applies once the needed node is known.

#### Many operations at a two-way cursor: doubly linked list

A two-way cursor needs to move both forward and backward. A doubly linked list provides `next` and `prev`, so each move to an adjacent node takes O(1). It also allows insertion before or after a known node, and removal of that node, without searching for its predecessor.

**Example:** a playlist cursor can move to the next or previous song and remove the current song while reconnecting its neighbors.

The cost is an extra pointer in each node and more links to maintain whenever the list changes.

#### Frequent resizing of the list: linked list

**Class connection:** in the past, we used a dynamic array when the list needed to resize. For frequent resizing, the class recommendation is a linked list because it can grow or shrink without reallocating the entire backing array or copying existing elements when capacity changes.

A linked list grows by allocating nodes and shrinks by unlinking and freeing nodes. Existing nodes do not need to move, and the list does not need one larger contiguous memory block as it grows. This can make frequent size changes more efficient, especially when the insertion or removal location is already known.

**Example:** a task list whose entries are frequently added and removed can grow and shrink one node at a time.

The class rule highlights this flexibility. Finding an insertion or removal location may still take O(n), and each allocation has overhead. Dynamic arrays also resize: appending is typically amortized O(1), though an occasional capacity increase requires allocating a larger block and copying elements. Consider the actual operations as well as how often the size changes.

**Tradeoffs:** singly linked lists store one link per node; doubly linked lists store two and require more pointer updates. Both have allocation overhead and usually poorer cache locality than arrays. Dynamic arrays also support growth and often perform better for sequential processing. Frequent resizing alone does not guarantee a linked list is the best choice.

**Complexity reminder:** an O(1) pointer update does not include an O(n) search to locate the insertion or deletion position. These costs use the usual data-structure model, treating allocation/free as individual operations.

## 8. Pointer mistakes to watch for

- Overwriting a link before saving the rest of the list.
- Forgetting to update the caller's head when the first node changes.
- Reading `victim->next` after `free(victim)`; save or use the link first.
- Unlinking a node without freeing it, causing a memory leak.
- Freeing a node twice, or dereferencing any pointer to a freed node.
- Updating `next` but forgetting the matching `prev` in a doubly linked list.
- Assuming `malloc` succeeds or allocating `sizeof(pointer)` instead of `sizeof *pointer`.
- Forgetting empty-list, one-node, head, tail, and missing-value cases.

## 9. Reversing a doubly linked list

The original reversal notes are preserved below. Their `Node` definition is a separate doubly linked representation; use it independently from the singly linked `struct node` above.

A singly linked list supports forward traversal: starting at the head, each node points to the next node. From a node in the middle, you can continue toward the tail, but you cannot move backward toward the head.

A doubly linked list gives each node links to both its next and previous nodes. This allows traversal in either direction, at the cost of storing and maintaining an additional pointer in every node.

### Reversal algorithm

To reverse the list in place, swap each node's `next` and `prev` links. Follow the original `next` link as you go; after swapping, that link is available as `prev`. The new head is the node that was originally the tail.

```c
typedef struct node {
    int data;
    struct node *prev;
    struct node *next;
} Node;

Node *ReverseLinkList(Node *head)
{
    Node *current = head;
    Node *new_head = NULL;

    while (current != NULL) {
        Node *original_next = current->next;

        current->next = current->prev;
        current->prev = original_next;

        new_head = current;
        current = original_next;
    }

    return new_head;
}
```

The function returns the new head, so update the caller's head pointer:

```c
head = ReverseLinkList(head);
```

An empty list (`head == NULL`) is handled naturally: the loop does not run, and the function returns `NULL`.

Time: O(n); extra space: O(1). If your list also stores a tail pointer, the old head becomes the new tail.

## 10. Reversing a singly linked list using a double pointer

A reversal function can accept the **address of the head pointer** and change the caller's head directly. It does not need to return the new head.

This example uses the singly linked `struct node` from Section 1 (with `data` and `next` only), independently of the doubly linked `Node` example in Section 9.

```c
void reverseList(struct node **head)
{
    if (head == NULL)
        return;  // No address of a head pointer was provided.

    struct node *previous = NULL;
    struct node *current = *head;

    while (current != NULL) {
        struct node *nextNode = current->next; // Save the rest of the list.
        current->next = previous;             // Reverse this node's link.
        previous = current;                   // Grow the reversed portion.
        current = nextNode;                   // Continue through the original list.
    }

    *head = previous; // Update the caller's head to the original last node.
}
```

### Why use `struct node **head`?

If the caller declares `struct node *list`, then:

| Expression | Meaning |
| --- | --- |
| `list` | Pointer to the first node. |
| `&list` | Address of the caller's head pointer; type `struct node **`. |
| `head` inside the function | A copy of that address. |
| `*head` inside the function | The caller's actual head pointer, which the function can change. |

Because `llist` already means `struct node *`, the signature could also be written `void reverseList(llist *head)`. These parameter types are equivalent.

Call it with the address of the list pointer:

```c
// Assume list currently contains: 2 -> 4 -> 6 -> NULL
reverseList(&list);
// list now contains: 6 -> 4 -> 2 -> NULL
```

Do not write `list = reverseList(&list)`: this function returns `void`. Its result is communicated by changing `*head`.

### Trace the pointer changes

The three working pointers have different jobs: `previous` tracks the reversed portion, `current` is the node being processed, and `nextNode` saves the unprocessed portion before its link is overwritten.

```text
Start:         previous -> NULL        current -> 2 -> 4 -> 6 -> NULL
After node 2:  previous -> 2 -> NULL    current -> 4 -> 6 -> NULL
After node 4:  previous -> 4 -> 2 -> NULL    current -> 6 -> NULL
After node 6:  previous -> 6 -> 4 -> 2 -> NULL    current -> NULL
Finish:        *head = previous
```

The old head (`2`) becomes the tail because its `next` is set to `NULL` on the first iteration. Save `current->next` **before** reversing the link, or you lose the route to the remaining nodes.

An empty list (`*head == NULL`) skips the loop and stays empty; a one-node list stays the same. Passing `NULL` instead of a head-pointer address is handled by the initial guard. No nodes are allocated or freed, and their data stays unchanged. Time: O(n); extra space: O(1).

This algorithm is for a singly linked list. A doubly linked reversal must also update every `prev` link, as shown in Section 9. If a separate tail pointer is stored, update it to the old head as well.

## 11. Program example: a list of lists - pharmacy medications

Source: Chapter 7, slide 122, PDF pages 575-580 of `Canvas/begre246.pdf`. These pages repeat the same problem statement for the lecture presentation. The slide asks how to design the data structures; the proposed design below explains how to apply the preceding selection rules.

### The problem from the lecture

Write a program that maintains a list of medications (pills) available at a pharmacy. Each medication can come in several sizes and types, so each drug has its own list of available forms.

The lecture's example drug, **CUREZIT**, comes in these forms:

- 5 mg
- 10 mg
- 25 mg
- 50 mg
- 50 mg Extended Release
- 100 mg Extended Release

The program has three important requirements:

1. Store the drugs in **alphabetical order**.
2. The forms (sizes and types) of an individual drug **do not change often**.
3. New drugs are introduced and older drugs are discontinued **frequently**.

A form needs both a strength and a type: `50 mg` and `50 mg Extended Release` are different entries even though their strengths match.

### Choose a structure for each level

**Outer list: use a linked list of drugs.** The collection of drugs changes frequently. Once the insertion location is found, adding a drug requires reconnecting links rather than shifting later drug records. Removing a discontinued drug similarly unlinks its node. Keep this list alphabetical by comparing names while searching for the insertion position.

**Inner list: use an array of forms for each drug.** A drug's forms change infrequently, so an array is a reasonable choice: it is compact, easy to traverse, and supports indexed access. A dynamically allocated array lets different drugs have different numbers of forms. Store a count with the array so the program knows how many entries it contains.

“List of lists” describes the organization of the information; it does **not** require both levels to be linked lists. Here, a linked list at the outer level and arrays at the inner level match the different patterns of change.

```text
Outer linked list (alphabetical drug names):
head -> [Drug A | forms | next] -> [CUREZIT | forms | next] -> [Drug Z | forms | NULL]
                 |                          |                         |
                 v                          v                         v
         Drug A's form array        CUREZIT's form array       Drug Z's form array
                                    [  5 mg, regular          ]
                                    [ 10 mg, regular          ]
                                    [ 25 mg, regular          ]
                                    [ 50 mg, regular          ]
                                    [ 50 mg, extended release ]
                                    [100 mg, extended release ]
```

`Drug A` and `Drug Z` are placeholders to illustrate alphabetical order. Each drug node owns its own array; the `next` link connects drug nodes, not individual forms.

### Example C representation

This is a proposed representation for studying the design, rather than code supplied on the slide. The name limit is an example implementation choice.

```c
#include <stddef.h>
#include <stdbool.h>

#define DRUG_NAME_CAPACITY 64

typedef struct {
    int strengthMg;
    bool extendedRelease;
} DrugForm;

typedef struct drugNode {
    char name[DRUG_NAME_CAPACITY];
    DrugForm *forms;          // Dynamically allocated array of this drug's forms.
    size_t formCount;        // Number of valid entries in the array.
    struct drugNode *next;   // Next drug in alphabetical order.
} DrugNode;
```

For CUREZIT, `formCount` would be `6`. Its forms array would hold the strength/type pairs `(5, false)`, `(10, false)`, `(25, false)`, `(50, false)`, `(50, true)`, and `(100, true)`. The Boolean models the two types in this example; a program with more types could use an enumeration instead.

### How the program would operate

- **Find a drug:** traverse the outer list and compare names. With a consistently sorted list, stop when you find the drug or pass where its name would appear. Searching is O(n) in the worst case.
- **Add a drug:** find its alphabetical position, allocate its node and forms array, fill them, and link the node into the outer list. Handle insertion at the head, middle, or end. Define a policy for duplicate names, such as updating the existing record.
- **Display a drug's forms:** find its node, then loop through `forms[0]` to `forms[formCount - 1]`. This inner traversal takes O(k) for k forms.
- **Change a drug's forms:** update an existing array entry, or resize/replace that drug's array if the number of forms changes. For an exact-sized replacement, copying the existing forms takes O(k); this is acceptable here because such changes are infrequent.
- **Discontinue a drug:** unlink its node, free its forms array, then free the drug node. Save the next drug pointer before freeing the node if continuing traversal.
- **Delete the entire inventory:** visit every drug, free each forms array, and then free each drug node. Finally set the inventory head to `NULL`.

**Key lesson:** choose the data structure separately for each level. Frequent changes to the drug collection favor a linked list; relatively stable collections of forms favor arrays. Finding a drug or insertion position still takes time, even though reconnecting links is O(1) once the position is known.

## 12. A new data structure: the stack

Source: Chapter 8, lecture slides 123-124 (PDF pages 581-591) of `Canvas/begre246.pdf`. The explanations and examples below expand on the lecture definitions.

### What is a stack?

A **stack** stores an ordered collection of entries, with insertion and removal restricted to one end called the **top**. The entries have the same element type (the lecture calls this a homogeneous structure).

A stack follows **LIFO: Last In, First Out**. The most recently added item is the first one removed. Think of a stack of plates: put a new plate on top and take the top plate off first.

“Ordered” here means there is a first item available, then a second, and so on. It does not mean the values are sorted numerically or alphabetically.

```text
Push 10, then 20, then 30:

       TOP
        |
        v
      [ 30 ]  <- Added last; removed first.
      [ 20 ]
      [ 10 ]  <- Added first; removed last.

Removal order: 30, 20, 10
```

Unlike a general list, the stack interface does not allow insertion or removal at arbitrary positions. All such operations happen at the top.

### Basic stack operations

| Operation | Purpose | Effect on the stack |
| --- | --- | --- |
| `push(s, item)` | Add an item at the top. | Size increases by one. |
| `pop(s)` | Remove the top item. | Size decreases by one. |
| `top(s)` | Read the top item. | No item is removed. |
| `isEmpty(s)` | Check whether the stack has no items. | No change. |
| `isFull(s)` | Check whether the implementation has reached its capacity. | No change. |

The lecture's interface separates **reading** from **removing**: `top` returns the item, while `pop` returns `void`. Some other stack interfaces combine these actions, so check the function declarations before using them.

### Trace an example

Stacks below are shown from bottom to top, with the top at the right.

| Step | Operation | Stack afterward | Result |
| --- | --- | --- | --- |
| 1 | Start empty | `[]` | `isEmpty(s)` is true. |
| 2 | `push(s, 10)` | `[10]` | Top is 10. |
| 3 | `push(s, 20)` | `[10, 20]` | Top is 20. |
| 4 | `top(s)` | `[10, 20]` | Returns 20; leaves it in place. |
| 5 | `pop(s)` | `[10]` | Removes 20. |
| 6 | `top(s)` | `[10]` | Returns 10. |
| 7 | `pop(s)` | `[]` | Removes 10; stack is empty. |

With the lecture's interface, reading and removing an item would look like this, assuming `s` is an already-created stack of integers:

```c
if (!isEmpty(s)) {
    int value = top(s); // Read before removing.
    pop(s);            // Remove the item just read.
    printf("Removed %d\n", value);
}
```

### Underflow and overflow

**Stack underflow** occurs when trying to remove an item from an empty stack. Reading `top` on an empty stack is also invalid under this interface. Check `isEmpty` before these operations, or follow the implementation's error-handling contract.

**Stack overflow** occurs when trying to push onto a stack that has reached its capacity. A fixed-size array stack can become full. A growing array or linked stack can expand, but allocation can still fail; dynamic storage is not unlimited.

### How a stack connects to linked lists and arrays

A stack describes the allowed operations and their behavior; an array or linked list can provide the underlying storage.

- **Array implementation:** keep a top index and add/remove at the occupied end. Push and pop take O(1) when capacity is available. Growing a dynamic array occasionally takes O(n) to copy items, giving typical appends amortized O(1) cost.
- **Singly linked implementation:** use the list's head as the top. Push inserts at the front; pop removes the front; top reads the head's data. Each takes O(1) in the usual data-structure model, with allocation failure handled on push.

```text
Linked stack:
top/head -> [30] -> [20] -> [10] -> NULL

push 40: insert a new head containing 40.
pop:     remove the head, making the next node the new top.
```

Using the front of a singly linked list avoids searching for the predecessor needed to remove its last node. A doubly linked list is unnecessary for these basic stack operations.

### When would we use a stack?

Use a stack when the most recent unfinished item should be handled first:

- **Expression evaluation:** the lecture's next example uses separate stacks for numbers and operators while evaluating an expression.
- **Matching parentheses:** push opening parentheses and match them as closing parentheses arrive.
- **Undo history:** the latest action is the next one to undo.
- **Function calls:** nested calls return in reverse order of entry; the call stack tracks active calls.

**Remember:** `push` adds, `top` reads, `pop` removes, and LIFO determines which item is available next.

## 13. Application using stacks: expression evaluation

Source: Chapter 8, slide 125, “An Application Using Stacks: Expression Evaluation,” PDF pages 592-637 of `Canvas/begre246.pdf`. These pages show animation steps for the same example.

### Why use two stacks?

The lecture evaluates an arithmetic expression using:

- **`numStack`**: numbers and intermediate results.
- **`operStack`**: operators waiting to be evaluated, plus opening parentheses.

An operator cannot always be evaluated as soon as it is read. We may need to read its right operand or finish a higher-precedence operation first. Stacks keep those pending operations in the order needed to handle nested expressions.

For ordinary binary arithmetic, `*` and `/` have higher precedence than `+` and `-`. Parentheses specify a group that must be evaluated before applying surrounding operators.

### What does “perform an operation” mean?

For an operator at the top of `operStack`:

1. Read and pop the operator.
2. Read and pop the **right** operand from `numStack`.
3. Read and pop the **left** operand from `numStack`.
4. Calculate `left operator right`.
5. Push the result back onto `numStack`.

**Operand order matters:** for `10 - 3`, the first number popped is `3`, then `10`. Calculate `10 - 3`, not `3 - 10`. The same issue applies to division.

The lecture's `pop` removes an item without returning it, so “read and pop” means calling `top` before `pop`.

### Rules while reading the expression

| Input | Action |
| --- | --- |
| Number | Push its value onto `numStack`. |
| Operator | Evaluate pending operators with higher or equal precedence, then push the incoming operator. Stop at an opening parenthesis or an empty operator stack. |
| `(` | Push it onto `operStack` as a boundary for a group. |
| `)` | Evaluate pending operators until `(` is on top, then pop and discard that `(`. |
| End of input | Evaluate the remaining operators. The remaining number is the result. |
| Whitespace | Skip it. |

The slide shows a single precedence check. For a general implementation, **repeat** that check while the top operator has higher or equal precedence. This expanded rule applies to the left-associative operators `+`, `-`, `*`, and `/`; operators with different associativity need their own rule. Never evaluate `(` as an arithmetic operator.

### Worked lecture example: `(2 + 6) * 4`

In this table, each stack is shown **bottom to top**, with the top at the right.

| Input/event | Action | `numStack` | `operStack` |
| --- | --- | --- | --- |
| Start | Both stacks empty. | `[]` | `[]` |
| `(` | Push the opening parenthesis. | `[]` | `[(]` |
| `2` | Push 2. | `[2]` | `[(]` |
| `+` | Push +; the parenthesis blocks evaluation. | `[2]` | `[(, +]` |
| `6` | Push 6. | `[2, 6]` | `[(, +]` |
| `)` | Pop + and operands 6, then 2; push `2 + 6 = 8`. | `[8]` | `[(]` |
| Finish `)` | Pop and discard the matching `(`. | `[8]` | `[]` |
| `*` | Push *. | `[8]` | `[*]` |
| `4` | Push 4. | `[8, 4]` | `[*]` |
| End of input | Pop * and operands 4, then 8; push `8 * 4 = 32`. | `[32]` | `[]` |

The final result is **32**, the number on top of `numStack`.

### How precedence changes evaluation

For `2 + 6 * 4`, the incoming `*` has higher precedence than the pending `+`, so push it without evaluating `+`. At the end, evaluate `6 * 4 = 24` first, then `2 + 24 = 26`.

For `10 - 3 - 2`, the second `-` has equal precedence to the first. Evaluate `10 - 3 = 7` before pushing the second `-`, giving `7 - 2 = 5`. This preserves left associativity.

### Implementation details to remember

- Check whether a stack is empty before reading its top.
- Each binary operation requires two numbers; missing operands indicate an invalid expression.
- An unmatched parenthesis is an error. Do not pop indefinitely looking for a missing `(`.
- After valid input is fully evaluated, the operator stack is empty and exactly one number remains.
- Read a complete numeric token: `25` is one number, not separate operands `2` and `5`. Decimal and negative values require additional parsing rules.
- Handle division by zero, invalid tokens, and allocation/capacity failures. The slide's “skip character” step is suitable for spacing, but a real evaluator should reject unsupported symbols.

For n tokens, this method takes O(n) time and O(n) extra space in the worst case: each token is pushed and popped only a bounded number of times, assuming constant-time stack operations (or amortized constant-time pushes for a growing array).
