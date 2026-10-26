# Quiz Review - October 26, 2026

## Quiz 4 Review: Reversing a Singly Linked List

Source: [Quiz 4 solution](Quiz4_Sol-1.pdf), pages 1-2. The PDF heading says EGRE246.

- A singly linked list is made of nodes. Each node stores data and a pointer to the next node.
- The last node's `next` pointer is `NULL`.
- To reverse the list, change the direction of every `next` pointer and update the head.
- The nodes and their data stay in memory; only the links change.

```c
struct Node {
    int data;
    struct Node *next;
};
```

### Why use a pointer to a pointer?

```c
void reverse(struct Node **head_ref);
// In main:
reverse(&head);
```

- `head` is a `struct Node *`: it holds the address of the first node.
- `&head` is the address of the head pointer itself.
- `head_ref` receives that address, so assigning to `*head_ref` changes `head` in `main`.
- Passing only `head` would give the function a copy of the pointer. Reassigning that local copy would not update the caller's head.

### Corrected reversal function

**The supplied solution contains errors.** It sets `ptr = cur` and then `ptr->next = pre`. Because `ptr` and `cur` point to the same node, this overwrites `cur->next` before the original next node is saved. The first iteration then moves `cur` to `NULL`, losing access to the rest of the list through the head. The PDF also omits a semicolon after `cur = cur->next` and the final closing brace of the function.

Use `ptr` to save the original next node before changing the link:

```c
void reverse(struct Node **head_ref) {
    struct Node *cur, *pre, *ptr;

    cur = *head_ref;
    pre = NULL;

    while (cur != NULL) {
        ptr = cur->next;  // Save the next node before overwriting the link
        cur->next = pre;  // Reverse the current node's link
        pre = cur;        // Grow the reversed portion by one node
        cur = ptr;        // Move to the next original node
    }

    *head_ref = pre;      // The original last node is now the head
}
```

The function assumes `head_ref` points to a valid head variable, as in `reverse(&head)`. The list itself may be empty (`head == NULL`).

### Roles of the three pointers

| Pointer | Meaning |
| --- | --- |
| `cur` | Current node being processed; start of the unreversed portion |
| `pre` | Previous node; head of the already reversed portion |
| `ptr` | Saved next node; keeps the remaining list reachable |

**Remember the order: save next, reverse link, move previous, move current.** Copying a pointer does not copy the node it points to.

### Trace: 1 -> 2 -> 3 -> NULL

| Stage | Reversed portion (`pre`) | Remaining portion (`cur`) |
| --- | --- | --- |
| Start | `NULL` | `1 -> 2 -> 3 -> NULL` |
| After iteration 1 | `1 -> NULL` | `2 -> 3 -> NULL` |
| After iteration 2 | `2 -> 1 -> NULL` | `3 -> NULL` |
| After iteration 3 | `3 -> 2 -> 1 -> NULL` | `NULL` |

At the end, `cur == NULL`. Assign `pre` to `*head_ref` so the caller can reach the reversed list. The original head becomes the tail and points to `NULL`.

### Understanding the quiz's test program

- `addFront(&head, value)` inserts a node at the beginning:
  1. Allocate a node and store its data.
  2. Set `new_node->next = *head_ref` to link it to the old head.
  3. Set `*head_ref = new_node` to make it the new head.
- Adding `5`, `4`, `3`, `2`, then `1` at the front produces `1 -> 2 -> 3 -> 4 -> 5 -> NULL`.
- `printList` receives a copy of the head pointer and walks through the list without changing the caller's head.
- With the corrected reversal, the output is:

```text
Original list:
1 -> 2 -> 3 -> 4 -> 5 -> NULL
Reversed list:
5 -> 4 -> 3 -> 2 -> 1 -> NULL
```

### Study reminders

- Save `cur->next` before changing it; otherwise, you lose the path to unprocessed nodes.
- Empty list: the loop does not run and the head remains `NULL`.
- One-node list: the node remains the head and its `next` stays `NULL`.
- Reversal takes O(n) time and O(1) extra space. It does not allocate or free nodes.
- In a complete program, check whether `malloc` succeeds in `addFront` and free all allocated nodes when finished. These steps are omitted from the supplied example.
