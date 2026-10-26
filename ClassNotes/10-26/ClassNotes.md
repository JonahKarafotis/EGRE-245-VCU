# Class Notes - October 26, 2026

## Implementing a Stack as a Linked List

Source: [Course PDF, slide 130/191](../../Canvas/begre246.pdf#page=643), Chapter 8: Stacks and Queues (PDF page 643).

### Main idea from slide 130

- Treat the front (or rear) of the linked list as the **top of the stack**.
- Insert and remove only at the top. This makes the algorithms simpler because there is only one position to handle.

**Best choice for a singly linked list: use the front (head) as the stack's top, and perform both push and pop there.** This requires the least work because both operations change only a few pointers without walking through the list.

- **Push at the front:** point the new node to the current head, then make the new node the head.
- **Pop at the front:** move the head to the next node, then free the old head.
- Both operations take **O(1)** time regardless of the number of nodes. At the rear, insertion can be O(1) with a tail pointer, but removal is O(n) because a singly linked list cannot follow a link backward to find the previous node.

### How this works

The following explanations and implementation steps expand on the slide's idea.

- A stack follows **LIFO: Last In, First Out**. The most recently pushed item is the first one popped.
- For a singly linked list, using the **front as the top** makes both insertion and removal O(1); no traversal is needed.
- Using the rear of a singly linked list would require finding the previous node to remove the last node, which takes O(n), even if a tail pointer is available.
- The top pointer holds the address of the first node. An empty stack has a top pointer of `NULL`.

```text
Push 10:   top -> [10] -> NULL
Push 20:   top -> [20] -> [10] -> NULL
Push 30:   top -> [30] -> [20] -> [10] -> NULL
Pop:       top -> [20] -> [10] -> NULL  (remove 30)
```

### Stack operations

| Operation | What it does | Linked-list steps |
| --- | --- | --- |
| `push` | Adds an item at the top | Allocate a node, link it to the old top, update the top |
| `pop` | Removes the top item | Save the old top, advance the top, free the removed node |
| `top` / peek | Reads the top item without removing it | Return the first node's data |
| `isEmpty` | Checks whether the stack has no items | Test whether the top pointer is `NULL` |

In the course's stack interface, `pop` returns `void`: read the item using `top(s)` before calling `pop(s)` if you need its value.

### Pointer update order

These C fragments assume `top_ptr` is a `struct Node *` initialized to `NULL`, using the node structure from [the quiz review](QuizReview.md). They illustrate the operations inside the code that owns the top pointer.

**Push an integer `value`:**

```c
struct Node *new_node = malloc(sizeof *new_node);
if (new_node == NULL) {
    fprintf(stderr, "Error: stack allocation failed\n");
    exit(EXIT_FAILURE);
}
new_node->data = value;
new_node->next = top_ptr;  // Link to the current stack first
top_ptr = new_node;        // Then make the new node the top
```

This is the same front-insertion idea as `addFront` in the quiz review. Updating the top before saving the old link can lose the existing list or accidentally create a self-link.

**Pop the top node:**

```c
if (top_ptr == NULL) {
    fprintf(stderr, "Error: stack underflow\n");
    exit(EXIT_FAILURE);
}
struct Node *old_top = top_ptr;
top_ptr = top_ptr->next;  // Keep access to the remaining stack
free(old_top);            // Release the removed node
```

Use `<stdio.h>` and `<stdlib.h>` for these fragments. Never access `old_top->next` after freeing `old_top`. If the removed node was the only node, its `next` was `NULL`, so the stack becomes empty automatically.

### Connection to pointers and memory

- A function that directly changes the caller's top pointer can accept `struct Node **`, just like `addFront(&head, value)` and `reverse(&head)`.
- Another design stores the top pointer inside a stack structure and passes a pointer to that structure. The function then updates its top member through `->`.
- Popping or peeking at an empty stack is **stack underflow**; check before dereferencing the top pointer.
- A linked stack has no fixed array capacity, but each push can still fail if memory allocation fails.
- Each push allocates one node; each pop frees one node. Free all remaining nodes when destroying the stack.
- Push, pop, peek, and the empty check each take O(1) time with the top at the front. Storing n items takes O(n) memory, including one next pointer per node.

## Queues: First In, First Out

Source: [Course PDF, slides 131-149](../../Canvas/begre246.pdf#page=644), Chapter 8: Stacks and Queues (PDF pages 644-688). The explanations below summarize and expand on these slides.

### What is a queue?

- A queue stores an ordered sequence of items of the same type. Here, ordered means order of arrival, not sorted by value.
- A queue follows **FIFO: First In, First Out**. The oldest item is removed first, like people waiting in line.
- Insert new items at the **rear/back** and remove items from the **front**.
- **Queue underflow:** trying to remove or inspect the front of an empty queue.
- **Queue overflow:** trying to insert into a queue at its capacity. A dynamically allocated implementation can grow, but allocation can still fail.
- The overview slide has a typo saying a stack is FIFO; **a queue is FIFO and a stack is LIFO**.

```text
Start:             front -> [10] -> [20] <- back
Enqueue 30:        front -> [10] -> [20] -> [30] <- back
Dequeue:           front -> [20] -> [30] <- back  (remove 10)
```

### Queue operations and interface

| Operation | Purpose |
| --- | --- |
| `createQueue()` | Create and initialize an empty queue |
| `enqueue(q, item)` | Add an item at the back |
| `dequeue(q)` | Remove the item at the front |
| `front(q)` | Read the front item without removing it |
| `isEmpty(q)` | Check whether there are no items |
| `isFull(q)` | Check whether the current storage capacity is reached |
| `size(q)` | Return the number of items currently stored |
| `freeQueue(q)` | Release the queue and its allocated storage |

In the course interface, `dequeue` returns `void`. To use the removed value, call `front(q)` before `dequeue(q)`. Both require a nonempty queue.

### Where queues are used

- Print jobs waiting for a printer.
- CPU jobs waiting to be processed.
- Keyboard input waiting to be read.
- Client requests waiting for a server.

The slides' client-server simulation models arrivals and service over clock ticks. A client may arrive with a chosen probability each tick, and the server removes a waiting client at a specified processing interval. If arrivals outpace service, the queue grows. The queue's final size is the number of clients still waiting.

### Worked example: client-server request simulation

Source: [Slides 134-136: Client-Server Simulation](../../Canvas/begre246.pdf#page=656) (PDF pages 656-658).

Imagine requests arriving at a server faster than it can handle them. The queue stores pending requests in arrival order. New requests join the back with `enqueue`; at a service opportunity, the server reads the oldest request with `front` and removes it with `dequeue`.

The PDF uses clock ticks, a probability of arrival per tick, and a service interval. Its `CLIENT` value is always 0 because it only counts clients. **This adapted example uses integer request IDs so we can see FIFO order**, prints each event, uses fixed parameters instead of keyboard input, and frees the queue afterward. It uses the course's `queue.h` interface with `itemType` defined as `int`.

```c
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

int main(void) {
    queue q = createQueue();
    const int totalTicks = 6;
    const int processTime = 2;  // Serve at ticks 2, 4, 6, ...
    const double prob = 1.0;    // One arrival every tick for this trace
    int totalClients = 0;
    int served = 0;

    srand(9999);               // Fixed seed, as in the slides

    for (int tick = 1; tick <= totalTicks; tick++) {
        // Service happens BEFORE this tick's new arrival, as in the PDF.
        if (tick % processTime == 0 && !isEmpty(q)) {
            int requestID = front(q);  // Read before removing
            printf("Tick %d: serve request %d\n", tick, requestID);
            dequeue(q);
            served++;
        }

        // Random value in [0, 1); prob = 1 guarantees an arrival.
        double randomValue = rand() / (RAND_MAX + 1.0);
        if (randomValue < prob) {
            int requestID = ++totalClients;
            enqueue(q, requestID);
            printf("Tick %d: request %d joins the queue\n",
                   tick, requestID);
        }
        printf("  Waiting: %d\n", size(q));
    }

    printf("Total requests: %d\n", totalClients);
    printf("Requests served: %d\n", served);
    printf("Requests still waiting: %d\n", size(q));
    freeQueue(q);  // Free pending nodes and the queue structure
    return 0;
}
```

`createQueue` should check allocation failure in its implementation. The parameters must satisfy `totalTicks >= 0`, `processTime > 0`, and `0 <= prob <= 1`; these fixed values do. The random-number comparison above uses `<` and a value below 1 so probabilities 0 and 1 behave exactly as expected.

**Trace for the example:** the queue column lists front to back. Each tick checks for service first, then adds the new arrival.

| Tick | Request served | New request | Queue after the tick |
| --- | --- | --- | --- |
| 1 | None | 1 | `1` |
| 2 | 1 | 2 | `2` |
| 3 | None | 3 | `2 -> 3` |
| 4 | 2 | 4 | `3 -> 4` |
| 5 | None | 5 | `3 -> 4 -> 5` |
| 6 | 3 | 6 | `4 -> 5 -> 6` |

Final totals: **6 requests arrived, 3 were served, and 3 remain waiting**. Requests 1, 2, and 3 were served in arrival order. Freeing the pending requests at shutdown releases memory; it does not count them as served.

### Understanding the simulation's behavior

- `tick % processTime == 0` selects service ticks. With `processTime = 2`, the server can remove at most one request every two ticks.
- `isEmpty` prevents underflow if no request is waiting at a service tick.
- Changing `prob` to `0.6` gives each tick a 60% chance of one arrival. This is an average of 0.6 arrivals per tick, not a guarantee of exactly 60 arrivals in 100 ticks.
- With service every two ticks, the maximum average service rate is 0.5 requests per tick. A sustained average arrival rate above this tends to build a backlog.
- Service is checked before arrivals: a request arriving on a service tick cannot be served until a later service tick. If the queue was empty, that service opportunity is unused.
- This is a simplified simulation: one arrival at most per tick and one removal per service opportunity. It represents service by removing a request; it does not model a separate request currently being processed.
- Keeping the seed fixed makes repeated runs reproducible with the same C library. Exact random sequences may differ across platforms. The `prob = 1.0` trace above does not depend on the random sequence.

### Implementing a queue with a singly linked list

Source: [Slides 145-149](../../Canvas/begre246.pdf#page=684) (PDF pages 684-688).

**For the least work, enqueue at the rear and dequeue at the front. Keep both a front pointer and a back pointer.** Each operation then takes O(1) time without traversing the list.

- A back pointer lets insertion reach the last node directly.
- Removing the front only requires following its `next` pointer.
- Reversing these roles would require removing the rear. In a singly linked list, finding the previous node requires O(n) traversal, even with a back pointer.

The slide implementation stores:

```c
struct node {
    itemType data;
    struct node *next;
};

struct queueType {
    struct node *front, *back;
    int count;
};
```

`itemType` is the element type defined by the course's `itemType.h`. The public queue type is `typedef struct queueType *queue;`, so functions can update the structure's members through `q->front`, `q->back`, and `q->count`.

**Initialization:** set both pointers to `NULL` and `count` to 0.

**Enqueue steps:**

1. Allocate a new node, check allocation success, store the item, and set its `next` to `NULL`.
2. If empty, set both `front` and `back` to the new node.
3. Otherwise, link the old back's `next` to the new node, then update `back` to that node.
4. Increment `count`.

**Dequeue steps:**

1. Check for underflow before accessing `front`.
2. Save the old front pointer.
3. Move `front` to the next node.
4. If the queue is now empty, also set `back = NULL` so it does not point to freed memory.
5. Free the old front node and decrement `count`.

The one-node case matters: before removal, `front` and `back` point to the same node; after removal, both must be `NULL`.

- `front(q)` returns `q->front->data` after checking for underflow.
- `isEmpty(q)` checks `q->front == NULL`.
- `size(q)` returns the maintained `count`, so it takes O(1).
- The slide's linked-list `isFull(q)` always returns `false` because there is no fixed node limit. This does **not** guarantee that `malloc` will succeed; the example omits allocation checks.
- Destroying the queue requires freeing every node, then the queue structure. This takes O(n) time. The slides use a recursive helper; an iterative walk can avoid O(n) recursive call-stack use.

#### Complete PDF example: queueLL.c (slides 145-149)

This combines **all linked-list implementation examples from the five slides**, retaining their chained assignment, conditional expression, and recursive cleanup. Formatting and comments have been added; `createQueue(void)` explicitly declares that the function takes no arguments. The simpler function examples below are alternative explanations of the same operations, not additional definitions to paste into this file.

Use the same public queue interface as the array implementation. In the course file layout, `itemType.h` defines the item type (for example, `typedef int itemType;`), and `queue.h` includes it and declares `typedef struct queueType *queue;` plus the public functions. Protect both headers with include guards. If using the self-contained `queue.h` later in these notes, keep its `itemType` definition and omit the extra `itemType.h` include instead.

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "queue.h"
#include "itemType.h"

// Slide 145: node and queue structures
struct node {
    itemType data;
    struct node *next;
};

struct queueType {
    struct node *front, *back;
    int count;
};

// Slide 146: create the queue and free its storage
queue createQueue(void) {
    queue q = malloc(sizeof(struct queueType));
    q->front = q->back = NULL;
    q->count = 0;
    return q;
}

static void freeNodes(struct node *p) {
    if (p == NULL)
        return;
    else {
        freeNodes(p->next);
        free(p);
    }
}

void freeQueue(queue q) {
    freeNodes(q->front);
    free(q);
}

// Slide 147: insert at the rear
void enqueue(queue q, itemType itm) {
    if (isFull(q)) {
        printf("Error: queue overflow\n");
        exit(EXIT_FAILURE);
    } else {
        struct node *tp = malloc(sizeof(struct node));
        tp->data = itm;
        tp->next = NULL;
        if (isEmpty(q)) {
            q->front = q->back = tp;
        } else {
            q->back = q->back->next = tp;
        }
        q->count++;
    }
}

// Slide 148: remove or inspect the front
void dequeue(queue q) {
    if (isEmpty(q)) {
        printf("Error: queue underflow\n");
        exit(EXIT_FAILURE);
    } else {
        struct node *tp = q->front;
        q->front = (q->count == 1) ? (q->back = NULL)
                                 : q->front->next;
        free(tp);
        q->count--;
    }
}

itemType front(queue q) {
    if (isEmpty(q)) {
        printf("Error: queue underflow\n");
        exit(EXIT_FAILURE);
    } else {
        return q->front->data;
    }
}

// Slide 149: queue status and item count
bool isEmpty(queue q) { return q->front == NULL; }
bool isFull(queue q) { return false; }
int size(queue q) { return q->count; }
```

#### Reading the PDF's pointer expressions

**Slide 145 — structure:** each node owns a data value and a link to the next node. The queue structure owns the two endpoint pointers and a count. There is no array, capacity field, or circular index helper in this implementation.

**Slide 146 — create and destroy:** `q->front = q->back = NULL` initializes both ends to an empty list. `freeNodes` first recursively visits the next node, then frees the current one, so nodes are freed from the rear back toward the front. `freeQueue` frees the queue structure after its nodes. The helper is `static` because only this implementation needs it. Cleanup takes O(n) time and O(n) recursive call-stack space; an iterative loop can avoid that recursion for long queues.

**Slide 147 — enqueue on an empty queue:** `q->front = q->back = tp` makes both ends point to the first node. Its `next` is `NULL`.

**Slide 147 — enqueue on a nonempty queue:** assignment groups right to left. The slide's compact statement:

```c
q->back = q->back->next = tp;
```

has the effect of these two steps:

```c
q->back->next = tp;  // Link the old rear to the new node
q->back = tp;        // Then move the rear pointer
```

**Slide 148 — dequeue:** save the old front before changing the queue. The slide's conditional assignment handles both possible cases:

```c
if (q->count == 1) {
    q->back = NULL;
    q->front = NULL;  // Removing the only node empties both ends
} else {
    q->front = q->front->next;
}
```

Then free the saved node and decrement the count. `front(q)` only returns the oldest value; it does not remove or free anything.

**Slide 149 — status:** `isEmpty` tests the front pointer; `size` returns the maintained count. `isFull` always returns false because this implementation has no fixed capacity.

**Memory-allocation omission in the PDF:** the code above preserves the slide examples, which do not check either `malloc` result. A complete implementation must check `q == NULL` in `createQueue` and `tp == NULL` in `enqueue` before dereferencing them. The `isFull` test does not detect allocation failure. The alternative enqueue example below demonstrates the node allocation check.

#### Example sequence with the linked-list implementation

| Call | Front-to-back contents | Front data | Back data | Count |
| --- | --- | --- | --- | --- |
| `q = createQueue()` | Empty | None | None | 0 |
| `enqueue(q, 10)` | `10 -> NULL` | 10 | 10 | 1 |
| `enqueue(q, 20)` | `10 -> 20 -> NULL` | 10 | 20 | 2 |
| `front(q)` | Unchanged; returns 10 | 10 | 20 | 2 |
| `dequeue(q)` | `20 -> NULL` | 20 | 20 | 1 |
| `dequeue(q)` | Empty; both pointers are NULL | None | None | 0 |
| `isEmpty(q)` | Unchanged; returns true | None | None | 0 |
| `freeQueue(q)` | Queue structure released; do not use q afterward | — | — | — |

To use the linked-list version with the client program, compile `queueLL.c` instead of the array's `queue.c`:

```sh
cc -std=c11 -Wall -Wextra queueLL.c main.c -o queue_demo
./queue_demo
```

Compile one implementation at a time: both provide the same public function names. The client-server simulation can use either implementation through the same `queue.h` interface.

#### Linked-list queue: function-by-function implementation

This checked implementation expands the PDF examples. Use the same public `queue.h` interface as the circular array, but compile `queueLL.c` instead of `queue.c`. All operations require a valid queue returned by `createQueue`; `front` and `dequeue` also require a nonempty queue or they report underflow. The recursive original from the PDF remains above.

**1. Define nodes and the queue structure**

The queue structure stores pointers, not an item array. Each node is allocated separately when a request arrives. The front is the oldest item; the back is the newest.

```c
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

struct node {
    itemType data;
    struct node *next;
};

struct queueType {
    struct node *front;
    struct node *back;
    int count;
};
```

**2. createQueue: allocate the queue, not an array**

Allocate only the queue structure initially. Both endpoints start at NULL because no nodes exist yet. There is no CHUNKSIZE, capacity, or circular index in this implementation.

```c
queue createQueue(void) {
    queue q = malloc(sizeof *q);
    if (q == NULL) {
        fprintf(stderr, "Queue allocation failed\n");
        exit(EXIT_FAILURE);
    }
    q->front = NULL;
    q->back = NULL;
    q->count = 0;
    return q;
}
```

**3. isEmpty, isFull, and size: inspect the queue**

isEmpty checks the front pointer. isFull has no fixed node limit to check; it cannot predict malloc failure. (void)q marks its unused parameter intentionally. size returns the maintained count.

```c
bool isEmpty(queue q) { return q->front == NULL; }
bool isFull(queue q) {
    (void)q;
    return false;
}
int size(queue q) { return q->count; }
```

**4. enqueue: allocate a node and append it**

Check the new allocation before dereferencing it. On the first insertion, both endpoints refer to the same node. Otherwise link the old rear to the new node before moving back. No traversal is needed: O(1).

```c
void enqueue(queue q, itemType item) {
    struct node *new_node = malloc(sizeof *new_node);
    if (new_node == NULL) {
        fprintf(stderr, "Node allocation failed\n");
        exit(EXIT_FAILURE);
    }
    new_node->data = item;
    new_node->next = NULL;
    if (isEmpty(q)) {
        q->front = new_node;
        q->back = new_node;
    } else {
        q->back->next = new_node;
        q->back = new_node;
    }
    q->count++;
}
```

**5. front: read the oldest value without changing links**

Guard against underflow, then return the front node’s data. The node stays in the queue. Call this before dequeue if the caller needs the removed value.

```c
itemType front(queue q) {
    if (isEmpty(q)) {
        fprintf(stderr, "Queue underflow\n");
        exit(EXIT_FAILURE);
    }
    return q->front->data;
}
```

**6. dequeue: advance front, then free the old node**

Save the node, advance the front, clear back if the last node was removed, then free the saved node and update count. Never dereference old_front after free. O(1).

```c
void dequeue(queue q) {
    if (isEmpty(q)) {
        fprintf(stderr, "Queue underflow\n");
        exit(EXIT_FAILURE);
    }
    struct node *old_front = q->front;
    q->front = old_front->next;
    if (q->front == NULL) {
        q->back = NULL;
    }
    free(old_front);
    q->count--;
}
```

**7. freeQueue: release every node, then the structure**

This iterative version uses the same ownership rules as the PDF’s recursive freeNodes helper. It takes O(n) time and O(1) extra space. The caller must not use q after it is freed.

```c
void freeQueue(queue q) {
    while (!isEmpty(q)) {
        dequeue(q);
    }
    free(q);
}
```

**Example client (`main.c`)**

```c
#include "queue.h"
#include <stdio.h>

int main(void) {
    queue q = createQueue();
    enqueue(q, 10);
    enqueue(q, 20);
    enqueue(q, 30);
    printf("First: %d\n", front(q));  // 10; does not remove it
    dequeue(q);                       // Frees node containing 10
    printf("Next: %d\n", front(q));   // 20
    printf("Count: %d\n", size(q));   // 2
    freeQueue(q);                     // Frees nodes 20, 30, then q
    return 0;
}
```

Expected output: `First: 10`, `Next: 20`, `Count: 2`. Compile with `cc -std=c11 -Wall -Wextra queueLL.c main.c -o queue_demo`.

### C examples: enqueue, dequeue, front, and isEmpty

These examples use the linked-list structures above and the course's function names. For an integer queue, define `itemType` as `int` before the structures (`typedef int itemType;`). Include `<stdbool.h>`, `<stdio.h>`, and `<stdlib.h>`, and define `typedef struct queueType *queue;` after the structures. Each function assumes `q` points to an initialized queue.

**`isEmpty`: check whether the front pointer is NULL.** This does not change the queue.

```c
bool isEmpty(queue q) {
    return q->front == NULL;
}
```

**`enqueue`: add an item at the back.** Handle the empty queue by setting both pointers to the first node.

```c
void enqueue(queue q, itemType item) {
    struct node *new_node = malloc(sizeof *new_node);
    if (new_node == NULL) {
        fprintf(stderr, "Error: queue allocation failed\n");
        exit(EXIT_FAILURE);
    }
    new_node->data = item;
    new_node->next = NULL;

    if (isEmpty(q)) {
        q->front = new_node;
        q->back = new_node;
    } else {
        q->back->next = new_node;  // Link the old back to the new node
        q->back = new_node;        // Move the back pointer
    }
    q->count++;
}
```

**`front`: return the oldest item without removing it.** Check for underflow before accessing the node's data.

```c
itemType front(queue q) {
    if (isEmpty(q)) {
        fprintf(stderr, "Error: queue underflow\n");
        exit(EXIT_FAILURE);
    }
    return q->front->data;
}
```

**`dequeue`: remove and free the front node.** When the last node is removed, reset the back pointer too.

```c
void dequeue(queue q) {
    if (isEmpty(q)) {
        fprintf(stderr, "Error: queue underflow\n");
        exit(EXIT_FAILURE);
    }
    struct node *old_front = q->front;
    q->front = old_front->next;
    if (q->front == NULL) {
        q->back = NULL;
    }
    free(old_front);
    q->count--;
}
```

**Example calls for an integer queue:** place this code inside `main` after the definitions above. The queue structure is local here; its nodes are dynamically allocated by `enqueue` and freed by `dequeue`.

```c
struct queueType storage = {NULL, NULL, 0};
queue q = &storage;

printf("Empty: %d\n", isEmpty(q));  // 1 (true)
enqueue(q, 10);
enqueue(q, 20);
enqueue(q, 30);                     // Front-to-back: 10, 20, 30
printf("Front: %d\n", front(q));   // 10; queue stays unchanged

dequeue(q);                        // Remove 10
printf("Front: %d\n", front(q));   // 20
printf("Empty: %d\n", isEmpty(q));  // 0 (false)

while (!isEmpty(q)) {
    printf("Removing: %d\n", front(q));  // 20, then 30
    dequeue(q);
}
printf("Empty: %d\n", isEmpty(q));  // 1 (true)
```

All four operations take O(1) time. Remember: **enqueue adds, dequeue removes, front reads, and isEmpty checks**.

### Implementing a queue with an array

Source: [Slides 137-144](../../Canvas/begre246.pdf#page=659) (PDF pages 659-683).

**Main problem with a basic array queue: removing from the front either requires shifting the remaining items or leaves unused slots that a non-wrapping implementation cannot reuse.** A fixed-size array also needs a policy for when its capacity is reached.

#### Array example 1: fixed front, shift every remaining value after dequeue

```text
Index:             0    1    2    3    4
Before dequeue:   [10] [20] [30] [40] [50]
After removing 10 and shifting left:
                  [20] [30] [40] [50] [  ]
```

**Enqueue and dequeue walkthrough (capacity 5):** start with `front = 0`, `back = -1`. A dash below means a slot is not part of the live queue.

| Operation | Array at indices 0, 1, 2, 3, 4 | Front | Back |
| --- | --- | --- | --- |
| Enqueue 10 | `[10, -, -, -, -]` | 0 | 0 |
| Enqueue 20 | `[10, 20, -, -, -]` | 0 | 1 |
| Enqueue 30 | `[10, 20, 30, -, -]` | 0 | 2 |
| Dequeue 10; shift 20 and 30 left | `[20, 30, -, -, -]` | 0 | 1 |
| Enqueue 40 | `[20, 30, 40, -, -]` | 0 | 2 |

Insert at `++back`. When removing, copy `data[1]` to `data[0]`, then `data[2]` to `data[1]`, and continue through the old back; finally decrement `back`. **The shifting happens after dequeue; enqueue appends at the back without shifting.** The front stays at index 0.

Removing one request requires moving all four remaining requests. With n items, a dequeue may move n - 1 items, making it **O(n)**. Draining an entire queue this way takes O(n^2) total shifting work. This is costly for a server with many waiting requests.

#### Array example 2: move the front after dequeue, without wrapping

```text
Index:             0    1    2    3    4
Initially:        [10] [20] [30] [40] [50]   front = 0, back = 4
After 2 dequeues: [  ] [  ] [30] [40] [50]   front = 2, back = 4
```

**Enqueue and dequeue walkthrough (capacity 5):** start with `front = 0`, `back = -1`.

| Operation | Array at indices 0, 1, 2, 3, 4 | Front | Back |
| --- | --- | --- | --- |
| Enqueue 10, 20, 30 | `[10, 20, 30, -, -]` | 0 | 2 |
| Dequeue 10; increment front | `[-, 20, 30, -, -]` | 1 | 2 |
| Enqueue 40 | `[-, 20, 30, 40, -]` | 1 | 3 |
| Dequeue 20; increment front | `[-, -, 30, 40, -]` | 2 | 3 |
| Enqueue 50 | `[-, -, 30, 40, 50]` | 2 | 4 |
| Try to enqueue 60 | Cannot append: next index would be 5 | 2 | 4 |

Insert at `++back`; remove by incrementing `front`. **The front moves instead of the data.** Each successful enqueue or dequeue takes O(1), but this design does not wrap around to reuse earlier slots.

Only three items remain, but the back index is already at the last slot. A basic implementation that only increments `back` cannot enqueue another item, even though indices 0 and 1 are available. This is **an apparent full condition caused by the layout, not by the item count**. Writing to index 5 would be out of bounds; shifting the items back toward index 0 would reintroduce O(n) copying. Resetting the indices only when the queue becomes empty does not solve this while items remain.

#### Problem 3: fixed capacity and resizing

- A fixed-size array cannot accept more items once all slots are occupied. The program must reject the insertion, report overflow, or provide a way to grow.
- Allocating a much larger array in advance can reserve memory that is never used.
- Resizing requires allocating storage and copying the live items, which takes O(n) time for that expansion. Allocation may fail, and a wrapped queue must be copied in FIFO order.

#### Array example 3: wrap the queue around in the array

For the second example, wrap the back index to 0 and insert 60:

```text
Index:             0    1    2    3    4
After enqueue 60: [60] [  ] [30] [40] [50]   front = 2, back = 0
Logical FIFO order: 30 -> 40 -> 50 -> 60
```

**Continue with both back and front wrapping (capacity 5):**

| Operation | Array at indices 0, 1, 2, 3, 4 | Front | Back | Count |
| --- | --- | --- | --- | --- |
| Starting queue: 30, 40, 50 | `[-, -, 30, 40, 50]` | 2 | 4 | 3 |
| Enqueue 60; back wraps to 0 | `[60, -, 30, 40, 50]` | 2 | 0 | 4 |
| Dequeue 30 | `[60, -, -, 40, 50]` | 3 | 0 | 3 |
| Enqueue 70 | `[60, 70, -, 40, 50]` | 3 | 1 | 4 |
| Dequeue 40 | `[60, 70, -, -, 50]` | 4 | 1 | 3 |
| Dequeue 50; front wraps to 0 | `[60, 70, -, -, -]` | 0 | 1 | 2 |

The remaining FIFO order is `60 -> 70`. Both indices wrap independently; no values shift.

- Enqueue, after checking space: `back = (back + 1) % capacity`, store the item at `data[back]`, then increment `count`.
- Dequeue, after checking nonempty: `front = (front + 1) % capacity`, then decrement `count`.
- Read the oldest item at `data[front]` before dequeue if its value is needed.
- Empty: `count == 0`. Full: `count == capacity`.

In all diagrams, a dash means a logically unused slot. Dequeue does not have to erase the old integer; the indices and count determine which values are live.

**Circular indexing avoids shifting and reuses freed slots, so enqueue and dequeue take O(1) when no resizing is needed.** The physical array order may wrap, but the logical queue order stays FIFO. Circular indexing does not increase capacity: when every slot holds a live item, the queue is truly full.

The slides compare these three designs:

1. **Keep the front at index 0:** after removal, shift all remaining items left. This makes dequeue O(n).
2. **Advance the front without wrapping:** avoids shifting, but leaves unused space at the beginning as the indices move toward the end.
3. **Use a circular array:** wrap indices back to 0 after the last slot, reusing space freed by dequeues.

For capacity `capacity`, advance an index using:

```c
index = (index + 1) % capacity;
```

The array queue tracks a front index, a back index, an item count, and the allocated capacity. In the slides, the structure's `size` member is capacity, while the public `size(q)` function returns the item count.

- Initialize `front = 0`, `back = -1`, and `count = 0`.
- Enqueue: ensure capacity, advance `back` with wraparound, store the item, increment `count`.
- Dequeue: check for underflow, advance `front` with wraparound, decrement `count`. Shifting or clearing the old slot is unnecessary for the value types in the example.
- Empty means `count == 0`; full means `count == capacity`. The count distinguishes the two states.
- Circular-array enqueue and dequeue take O(1) when no resizing is needed. Growing the storage requires O(n) copying.

**Correction to the slide's resizing example:** `expandQ` copies physical array positions and changes capacity without preserving a wrapped queue's logical order; it also loses the old allocation without freeing it. A correct expansion copies the `count` live items in FIFO order, starting at the old front (`old_data[(old_front + i) % old_capacity]`), frees the old array after a successful allocation and copy, and resets `front = 0` and `back = count - 1` before using the new capacity.

### Header-file example: a circular-array queue module

Based on [slides 140-144](../../Canvas/begre246.pdf#page=679). This example uses the lecture's **dynamically allocated circular array**, with an initial capacity of `CHUNKSIZE = 20`. The expansion helper below preserves FIFO order even when the queue has wrapped.

Use three files: `queue.h` declares the interface, `queue.c` implements it with a circular array, and `main.c` uses it. The header tells callers which operations are available; the private structure in `queue.c` determines how the queue is stored.

#### queue.h: public types and function declarations

```c
#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

typedef int itemType;
typedef struct queueType *queue;  // Implementation is hidden in queue.c

queue createQueue(void);
void freeQueue(queue q);
void enqueue(queue q, itemType item);
void dequeue(queue q);
itemType front(queue q);
bool isEmpty(queue q);
bool isFull(queue q);
int size(queue q);

#endif
```

- The include guard prevents repeated declarations when the header is included more than once.
- `itemType` is `int` here so the example is self-contained. In the course layout, this type comes from `itemType.h`; use that header instead of redefining it when integrating with the course files.
- `queue` is a pointer to a structure whose members are hidden from the caller. Client code uses `enqueue(q, value)` rather than changing array indices directly.
- The declarations match the course interface: `dequeue` removes an item and returns no value; `front` reads without removing.

#### queue.c: private circular-array implementation

```c
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

#define CHUNKSIZE 20

struct queueType {
    itemType *data;
    int front, back;
    int count;  // Number of live items
    int size;   // Allocated array capacity
};

static void fail(const char *message) {
    fprintf(stderr, "%s\n", message);
    exit(EXIT_FAILURE);
}

// Private helper: wrap using this queue's current capacity.
static int increment(int index, queue q) {
    return (index + 1) % q->size;
}

queue createQueue(void) {
    queue q = malloc(sizeof(struct queueType));
    if (q == NULL) fail("Queue allocation failed");

    q->data = malloc(sizeof(itemType) * CHUNKSIZE);
    if (q->data == NULL) {
        free(q);
        fail("Queue data allocation failed");
    }
    q->front = 0;
    q->back = -1;
    q->size = CHUNKSIZE;
    q->count = 0;
    return q;
}

void freeQueue(queue q) {
    free(q->data);
    free(q);
}

bool isEmpty(queue q) { return q->count == 0; }
bool isFull(queue q) { return q->count == q->size; }
int size(queue q) { return q->count; }

// Private helper: copy live items in logical FIFO order.
static void expandQ(queue q) {
    int newSize = q->size + CHUNKSIZE;
    itemType *newData = malloc(sizeof(itemType) * newSize);
    if (newData == NULL) fail("Queue expansion failed");

    for (int i = 0; i < q->count; i++) {
        newData[i] = q->data[(q->front + i) % q->size];
    }
    free(q->data);
    q->data = newData;
    q->size = newSize;
    q->front = 0;
    q->back = q->count - 1;
}

void enqueue(queue q, itemType item) {
    if (isFull(q)) expandQ(q);
    q->back = increment(q->back, q);
    q->data[q->back] = item;
    q->count++;
}

itemType front(queue q) {
    if (isEmpty(q)) fail("Queue underflow");
    return q->data[q->front];
}

void dequeue(queue q) {
    if (isEmpty(q)) fail("Queue underflow");
    q->front = increment(q->front, q);
    q->count--;
}
```

**What `createQueue` initializes:**

1. `malloc(sizeof(struct queueType))` allocates the structure containing the pointer, indices, count, and capacity.
2. `malloc(sizeof(itemType) * CHUNKSIZE)` allocates a separate array with room for `CHUNKSIZE` items. `sizeof(itemType)` is the number of bytes per item; multiplying by the chunk size gives the total bytes needed.
3. `front = 0` sets the index where the first item will be read.
4. `back = -1` means no item has been inserted yet; the first call to `increment(-1, q)` produces index 0.
5. `size = CHUNKSIZE` records the array's capacity, while `count = 0` records that the queue is empty. **`q->size` is capacity; `size(q)` returns the number of stored items.**
6. Return `q` so the caller can use the initialized queue.

`increment` is `static`, making it internal to `queue.c`; it does not belong in `queue.h`. Both enqueue and dequeue call it and assign the returned index. It uses `q->size` so wrapping still works after expansion: with capacity 20, index 19 wraps to 0.

When full, `expandQ` increases capacity by `CHUNKSIZE`, copies live items in FIFO order, frees the old array, and resets the indices. This corrects the slide's resizing issue described above. Expansion takes O(n); operations that do not expand take O(1). Underflow or allocation failure exits the example. `freeQueue` must free **both allocations**, data first and structure second.

#### main.c: use the header to work with the queue

```c
#include "queue.h"
#include <stdio.h>

int main(void) {
    queue q = createQueue();  // Capacity 20, count 0
    enqueue(q, 10);
    enqueue(q, 20);
    enqueue(q, 30);
    printf("Front: %d\n", front(q));  // 10
    dequeue(q);                        // Remove 10
    enqueue(q, 40);
    printf("Size: %d\n", size(q));    // 3, not capacity 20

    while (!isEmpty(q)) {
        printf("%d ", front(q));
        dequeue(q);
    }
    printf("\n");
    freeQueue(q);
    return 0;
}
```

Compile the implementation and client together:

```sh
cc -std=c11 -Wall -Wextra queue.c main.c -o queue_demo
./queue_demo
```

Expected output:

```text
Front: 10
Size: 3
20 30 40
```

The five-slot diagrams earlier still illustrate wraparound; this implementation starts with 20 slots and expands when needed.

### Stack vs. queue: which end should change?

| Structure | Order | Insert at | Remove from | Efficient singly linked implementation |
| --- | --- | --- | --- | --- |
| Stack | LIFO | Front/top | Front/top | Head pointer; O(1) push and pop |
| Queue | FIFO | Rear/back | Front | Head and tail pointers; O(1) enqueue and dequeue |

A linked queue grows one node at a time and uses O(n) memory, including node pointers. A circular array uses its allocated capacity and avoids per-node pointers, but must handle a full array or resize it.
