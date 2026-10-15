# Linked Lists in C — Beginner Notes

A **linked list** stores a sequence of values in separate objects called **nodes**. Each node holds a value and a pointer that tells us where the next node is located.

Unlike an array, the nodes do not need to sit next to one another in memory. We follow their pointers to move through the sequence.

## 1. Start with an array comparison

An array stores its elements in consecutive memory locations:

```text
Array:       [10][20][30]
Index:         0   1   2
```

A singly linked list stores one forward link in each node:

```text
head
 │
 ▼
[10 | next] ──► [20 | next] ──► [30 | NULL]
```

The arrows represent pointer values, not copies of the nodes.

- `head` points to the first node.
- `next` points to the following node.
- The last node's `next` is `NULL`, meaning there is no following node.
- An empty list has `head == NULL`.

The order comes from the links, not from the nodes' physical addresses. A list is not automatically sorted: its values appear in the order you arrange the links.

| Operation | Array | Singly linked list with only a head pointer |
|---|---|---|
| Read element at index `i` | Direct access: O(1) | Follow links: O(n) in general |
| Search for a value | O(n) for an unsorted array | O(n) |
| Insert at the beginning | Shift existing elements: O(n) | Change a few pointers: O(1) |
| Append | O(1) if capacity exists; resizing can cost O(n) | O(n) to find the last node |
| Remove the first element | Shift remaining elements: O(n) | Change head and free one node: O(1) |

Here, **n** is the number of elements. O(1) means the work does not grow with the list length; O(n) means it can grow proportionally to the length. A tail pointer can make appending O(1), but it must be maintained correctly.

## 2. The pointer basics you need

```c
int number = 10;
int *p = &number;
```

- `number` stores an integer.
- `&number` gives the address of that integer.
- `p` stores that address.
- `*p` accesses the integer at that address.

In `int *p`, the `*` declares a pointer. In an expression such as `*p = 20`, it dereferences the pointer to access the pointed-to object.

A pointer is not the object itself. Copying a pointer makes two pointers refer to the same object; it does not create a second object.

Never dereference a null, uninitialized, or freed pointer. It does not refer to an object you can safely access.

## 3. Define a node

```c
typedef struct Node {
    int value;
    struct Node *next;
} Node;
```

Each `Node` contains:

1. `value`: the integer stored in this node.
2. `next`: the address of another node, or `NULL`.

Inside the definition, `struct Node *next` refers to the structure being defined. It is legal because a pointer has a known size even though the complete structure is not yet defined. A node cannot contain another entire node of its own type directly: that would require endlessly nested storage.

After the declaration, `Node` is a shorter name for `struct Node`.

```c
Node *head = NULL;  /* An empty list: no nodes have been created yet. */
```

### Accessing fields: `.` versus `->`

```c
Node example = {10, NULL};
Node *p = &example;

printf("%d\n", example.value); /* Structure object: use dot. */
printf("%d\n", p->value);      /* Pointer to structure: use arrow. */
```

`p->value` means the same thing as `(*p).value`.

## 4. Allocate a node with `malloc`

For these examples, include:

```c
#include <stdbool.h> /* bool, true, false */
#include <stdio.h>   /* printf, fprintf */
#include <stdlib.h>  /* malloc, free, exit */
```

```c
static Node *createNode(int value)
{
    Node *node = malloc(sizeof *node);

    if (node == NULL) {
        fprintf(stderr, "error: unable to allocate node\n");
        exit(EXIT_FAILURE);
    }

    node->value = value;
    node->next = NULL;
    return node;
}
```

`malloc(sizeof *node)` allocates enough bytes for one whole `Node`, including its integer, pointer, and any padding. `sizeof node` would measure only the pointer, so it is not the correct expression here.

In this expression, `sizeof` determines the size from the type; it does not actually dereference the not-yet-initialized pointer. No cast is needed for `malloc` in C when `<stdlib.h>` is included.

`malloc` gives you uninitialized storage. Assign both fields before using them. The allocated node continues to exist after `createNode` returns, until it is freed. In contrast, returning the address of a local automatic variable would leave a dangling pointer when the function ends.

`static` makes this helper function private to its C source file.

## 5. Traverse: visit every node

```c
static void printList(const Node *head)
{
    const Node *current = head;

    while (current != NULL) {
        printf("%d -> ", current->value);
        current = current->next;
    }

    puts("NULL");
}
```

For `10 -> 20 -> 30 -> NULL`:

1. `current` initially points to the node containing `10`.
2. Print its value.
3. Follow its `next` pointer to `20`.
4. Repeat until `current` becomes `NULL`.

Use a separate traversal pointer so you retain the original head. `const Node *` prevents modifying a node's fields through that pointer, but still lets the pointer variable advance.

There is no `head[i]` shortcut for reaching the ith linked node. Pointer arithmetic does not follow links.

## 6. Search for a value

```c
static bool contains(const Node *head, int target)
{
    for (const Node *current = head;
         current != NULL;
         current = current->next) {
        if (current->value == target) {
            return true;
        }
    }
    return false;
}
```

An empty list skips the loop and returns `false`. An unsuccessful search through a nonempty list visits every node.

## 7. Insert at the beginning

A straightforward design returns the potentially changed head:

```c
static Node *pushFront(Node *head, int value)
{
    Node *node = createNode(value);
    node->next = head; /* Connect the new node to the existing list first. */
    return node;       /* The caller makes the new node the head. */
}
```

Call it like this:

```c
head = pushFront(head, 10);
head = pushFront(head, 20);
```

Result:

```text
head ──► [20] ──► [10] ──► NULL
```

The most recently inserted value comes first. Inserting into an empty list also works because the old head is `NULL`.

**Keep the returned pointer.** Calling `pushFront(head, 20)` without assigning its result loses the newly allocated node and does not update the caller's head.

### Why not simply assign `head` inside a void function?

C passes arguments by value, including pointers. A function receiving `Node *head` gets a copy of the caller's pointer. It can change an existing node through that copy, but assigning the copy does not change the caller's pointer variable.

Two common ways to update the caller's head are:

- Return the updated head and assign it, as above.
- Pass the address of the head using a pointer to a pointer.

The second form looks like this (an alternative, not needed in the complete example below):

```c
static void pushFrontWithAddress(Node **head, int value)
{
    Node *node = createNode(value);
    node->next = *head;
    *head = node;
}

/* Call: pushFrontWithAddress(&head, 20); */
```

Here `head` points to the caller's pointer variable. `*head` accesses that variable, allowing the function to replace its value.

## 8. Insert at the end

```c
static Node *append(Node *head, int value)
{
    Node *node = createNode(value);

    if (head == NULL) {
        return node; /* The new node is both first and last. */
    }

    Node *current = head;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = node; /* Connect the old last node to the new last node. */
    return head;
}
```

Call with `head = append(head, 30);`. Retaining the return value is essential when the list was empty.

Notice the traversal condition: `current->next != NULL` stops **at the last node**, where we need to change a link. Using `current != NULL` would instead advance past the last node.

## 9. Insert after a known node

If `position` is a valid pointer to a node already in the list:

```c
Node *node = createNode(25);
node->next = position->next;
position->next = node;
```

Before:

```text
[position: 20] ──► [30]
```

After:

```text
[position: 20] ──► [25] ──► [30]
```

**Assignment order matters.** Preserve the old connection in `node->next` before overwriting `position->next`. Reversing these two assignments can make the new node point to itself, creating a cycle.

Once you already have `position`, this insertion takes O(1) time. Searching for `position` may still take O(n).

## 10. Remove the first matching value

Deletion has two jobs: unlink the node and release its memory.

```c
static Node *removeFirst(Node *head, int target)
{
    Node *previous = NULL;
    Node *current = head;

    while (current != NULL && current->value != target) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        return head; /* Empty list or target not found: nothing changes. */
    }

    if (previous == NULL) {
        head = current->next; /* Removing the first node changes head. */
    } else {
        previous->next = current->next; /* Bypass a middle or last node. */
    }

    free(current);
    return head;
}
```

Call: `head = removeFirst(head, 20);`.

For a middle node:

```text
Before: previous ──► current ──► next node
After:  previous ─────────────► next node
                      current is freed
```

This removes only the first match if duplicates exist. A linked list can contain duplicates unless you explicitly prevent them.

The `&&` condition uses short-circuit evaluation: C checks `current != NULL` first and only accesses `current->value` if that check succeeds.

After `free(current)`, do not read `current->next` or any other field. The node's lifetime has ended even if the pointer variable still holds its old address.

## 11. Free the whole list

```c
static Node *destroyList(Node *head)
{
    while (head != NULL) {
        Node *next = head->next; /* Save the connection before freeing. */
        free(head);
        head = next;
    }
    return NULL;
}
```

Call: `head = destroyList(head);`. This releases every node and resets the caller's head to `NULL`.

`free(head)` alone frees just one node. It does not recursively follow `next`. Setting `head = NULL` alone also does not free anything; if no other pointer reaches the nodes, their memory leaks.

Any other pointers into the destroyed list are now dangling. Resetting head does not fix those other pointers.

## 12. A complete runnable example

Save the following as a separate practice file, such as `linked_list_demo.c`. It includes all the definitions it needs; do not paste earlier function definitions into the same file again.

```c
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

static Node *createNode(int value)
{
    Node *node = malloc(sizeof *node);
    if (node == NULL) {
        fprintf(stderr, "error: unable to allocate node\n");
        exit(EXIT_FAILURE);
    }
    node->value = value;
    node->next = NULL;
    return node;
}

static Node *pushFront(Node *head, int value)
{
    Node *node = createNode(value);
    node->next = head;
    return node;
}

static Node *append(Node *head, int value)
{
    Node *node = createNode(value);
    if (head == NULL) return node;
    Node *current = head;
    while (current->next != NULL) current = current->next;
    current->next = node;
    return head;
}

static bool contains(const Node *head, int target)
{
    for (const Node *current = head; current != NULL; current = current->next) {
        if (current->value == target) return true;
    }
    return false;
}

static void printList(const Node *head)
{
    for (const Node *current = head; current != NULL; current = current->next) {
        printf("%d -> ", current->value);
    }
    puts("NULL");
}

static Node *removeFirst(Node *head, int target)
{
    Node *previous = NULL;
    Node *current = head;
    while (current != NULL && current->value != target) {
        previous = current;
        current = current->next;
    }
    if (current == NULL) return head;
    if (previous == NULL) head = current->next;
    else previous->next = current->next;
    free(current);
    return head;
}

static Node *destroyList(Node *head)
{
    while (head != NULL) {
        Node *next = head->next;
        free(head);
        head = next;
    }
    return NULL;
}

int main(void)
{
    Node *head = NULL;
    printList(head);

    head = pushFront(head, 20);
    head = pushFront(head, 10);
    head = append(head, 30);
    printList(head);

    printf("Contains 20? %s\n", contains(head, 20) ? "yes" : "no");
    head = removeFirst(head, 20);
    printList(head);

    head = removeFirst(head, 10); /* Remove the head. */
    head = removeFirst(head, 99); /* Missing value: no change. */
    printList(head);

    head = destroyList(head);
    printList(head);
    return EXIT_SUCCESS;
}
```

Compile and run from the directory containing the practice file:

```sh
cc -std=c11 -Wall -Wextra linked_list_demo.c -o linked_list_demo
./linked_list_demo
```

Expected output:

```text
NULL
10 -> 20 -> 30 -> NULL
Contains 20? yes
10 -> 30 -> NULL
30 -> NULL
NULL
```

## 13. Common mistakes and how to recognize them

| Mistake | Why it is a problem | Better approach |
|---|---|---|
| `Node *head;` without initialization | The pointer has an indeterminate value. | Start with `Node *head = NULL;`. |
| `malloc(sizeof node)` | Allocates space for a pointer, not necessarily a node. | Use `malloc(sizeof *node)`. |
| Accessing `current->value` before checking `current` | A null pointer cannot be dereferenced. | Check `current != NULL` first. |
| Forgetting to initialize `next` | Traversal may follow an invalid address. | Set it to a valid node or `NULL`. |
| Overwriting the only pointer to a node | The allocation becomes unreachable and leaks. | Preserve links before changing them. |
| Reading `node->next` after `free(node)` | Use-after-free: undefined behavior. | Save the next pointer first. |
| Freeing a node twice | Double-free: undefined behavior. | Give each allocation one clear owner. |
| Forgetting to advance during traversal | The loop never reaches the end. | Update `current = current->next`. |
| Ignoring a function's returned head | Insertions or removals at the beginning are lost. | Assign `head = function(head, ...)`. |
| Accidentally linking a node to itself | Traversal can loop forever. | Sketch the intended links and check assignment order. |

## 14. Other linked-list designs

### Doubly linked list

A node has both `next` and `previous` pointers:

```text
NULL ◄── [10] ◄──► [20] ◄──► [30] ──► NULL
```

You can move in either direction. Removing a known node is easier because you can reach its predecessor, but each node needs another pointer and every edit must maintain both directions.

### Circular linked list

The final node links back to the first rather than to `NULL`. Useful for repeated cycles, but a traversal must stop when it returns to its starting point. A loop that waits for `NULL` will not terminate in a valid nonempty circular list.

### List structure with head, tail, and count

A larger implementation may put bookkeeping into another structure:

```c
typedef struct {
    Node *head;
    Node *tail;
    size_t count;
} List;
```

A tail pointer supports fast appending; a stored count supports fast length queries. The tradeoff is extra rules: removing the last node must update tail, and every insertion/removal must update count. For an empty list, both head and tail should be `NULL` and count should be zero.

## 15. When linked lists are useful

- A **stack** can push and pop at the head.
- A **queue** can append at a tail and remove at the head.
- A sequence can grow a node at a time without resizing a single contiguous array.
- Insertion after a known node does not require shifting later values.

Arrays often use less memory and run faster for sequential access because nearby elements are stored together. Linked lists add a pointer and allocation overhead for every node. Choose based on the operations you need, rather than assuming linked lists are always faster.

**Connection to Project 3:** Your Integer Sets assignment specifically requires an expandable array. These linked-list notes explain a different representation; do not replace that project's required array with a linked list.

## 16. Practice checklist

Trace these on paper, drawing every changed pointer:

1. Insert `10` into an empty list.
2. Insert `5` at the beginning and `20` at the end.
3. Remove the first node, then the last node.
4. Remove a value that is not present.
5. Remove the only node in a one-node list.
6. Insert duplicates and predict which one `removeFirst` removes.
7. Free the entire list and explain why the next pointer must be saved first.

Before each pointer update, ask: **What object does this pointer refer to now, what should it refer to next, and will I lose access to any allocated node?**
