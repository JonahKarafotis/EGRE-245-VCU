# Class Notes — November 9

## Chapter 9 — Binary Search Tree Traversal

Based on the course [PowerPoint PDF](../../Canvas/begre246.pdf#page=787), starting at **slide 169**. These notes cover **slides 169–183**: traversal, node representation, the BST interface, insertion, and removal.

## Different node traversal methods — slide 169

**Traversal** means visiting every node in a tree in a specific order. Visiting a node means processing its data, such as printing it. Searching for a single key can stop early; a complete traversal processes every node.

At each node, there are three tasks:

- **V — Visit** the current node.
- **L — Traverse the left subtree.**
- **R — Traverse the right subtree.**

| Method | Pattern | Steps at each node | When is the current node visited? |
| --- | --- | --- | --- |
| **Preorder** | **VLR** | Visit, left subtree, right subtree | Before its subtrees |
| **Inorder** | **LVR** | Left subtree, visit, right subtree | Between its subtrees |
| **Postorder** | **LRV** | Left subtree, right subtree, visit | After its subtrees |

**Apply the same pattern recursively at every node.** L and R mean completing the entire corresponding subtree, not just visiting the immediate child. All three methods finish the left subtree before starting the right subtree.

**Memory aid:** “pre,” “in,” and “post” tell us where **V** goes relative to **L** and **R**.

## Worked traversal example — slide 170

[Tree diagram on slide 170](../../Canvas/begre246.pdf#page=791)

The slide's tree has **Jim** as its root:

```text
                 Jim
              /       \
            Dot       Ron
           /   \     /   \
         Amy   Guy  Kay   Tim
           \   / \  / \   / \
           Ann Eva Jan Jon Kim Roy Tom
```

### Preorder — VLR

Visit Jim first, complete Dot's subtree, then complete Ron's subtree:

```text
Jim, Dot, Amy, Ann, Guy, Eva, Jan, Ron, Kay, Jon, Kim, Tim, Roy, Tom
```

### Inorder — LVR

Complete Dot's subtree, visit Jim, then complete Ron's subtree:

```text
Amy, Ann, Dot, Eva, Guy, Jan, Jim, Jon, Kay, Kim, Ron, Roy, Tim, Tom
```

**Inorder traversal of a BST produces sorted keys** — alphabetical order for this example. This follows from the BST rule: every key in the left subtree is smaller than the current key, and every key in the right subtree is larger. An arbitrary binary tree does not necessarily give sorted output.

### Postorder — LRV

Complete Dot's subtree, complete Ron's subtree, then visit Jim last:

```text
Ann, Amy, Eva, Jan, Guy, Dot, Jon, Kim, Kay, Roy, Tom, Tim, Ron, Jim
```

### How to trace a traversal

For the small subtree rooted at Guy:

```text
     Guy
    /   \
  Eva   Jan
```

- Preorder: **Guy, Eva, Jan**.
- Inorder: **Eva, Guy, Jan**.
- Postorder: **Eva, Jan, Guy**.

Treat that whole result as the traversal of Guy's subtree when working back up to Dot. Reaching a missing child does nothing; return to the calling node and continue its remaining steps.

## Passing functions into functions — slide 171

[Function pointers](../../Canvas/begre246.pdf#page=792)

The traversal decides **when** to visit each node. A function passed into the traversal decides **what to do** during a visit. This lets us reuse the traversal with different processing functions.

In C, we can store a function's address in a **function pointer**:

```c
int (*p2)(float n);
```

Read this as: `p2` is a pointer to a function that accepts a `float` and returns an `int`.

- A function name without a call, such as `printItem`, supplies a pointer to the function.
- `f(value)` and `(*f)(value)` both call the function through pointer `f`.
- **Parentheses matter:** `int (*p2)(float n)` declares a function pointer; `int *p2(float n)` declares a function returning a pointer to `int`.
- The pointer's parameter and return types must match the function being passed.

## Function pointer example — slide 172

[Slide 172 example](../../Canvas/begre246.pdf#page=808)

The slide uses an array to demonstrate passing an operation into another function. This expanded, runnable version follows the slide's example and uses `(void)` to explicitly declare functions that take no arguments:

```c
#include <stdio.h>  // Provides printf for displaying results.

void fun(void) {
    printf("this is fun!\n");  // This function takes no arguments and returns nothing.
}

void twoUp(int *n) {  // n receives the address of an integer, not a copy of its value.
    *n += 2;         // Dereference n and add 2 to the original integer at that address.
}

void threeUp(int *n) {
    *n += 3;         // Same idea as twoUp, but add 3 to the original integer.
}

// f: pointer to a function accepting an int * and returning void.
// data: the array to process (an array parameter is adjusted to a pointer in C).
// n: the number of elements; this is separate from the pointer parameter in twoUp.
void apply(void (*f)(int *), int data[], int n) {
    for (int i = 0; i < n; i++) {  // Process each array element in index order.
        f(&data[i]);  // Pass this element's ADDRESS to the function stored in f.
                     // If f is twoUp, this acts like twoUp(&data[i]).
                     // The called function changes the original array element.
    }
}

void printList(int data[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", data[i]);  // Print the current value of each element.
    }
    printf("\n");
}

int main(void) {
    void (*fp)(void);  // fp can point to a function with no arguments and no return value.
    fp = fun;         // Store fun's address. No () here: we are not calling fun yet.
    (*fp)();          // Call fun through fp; fp() would do the same thing.

    int list1[5] = {2, 4, 6, 8, 10};

    for (int i = 0; i < 5; i++) {
        threeUp(&list1[i]);  // Direct call: add 3 to each element using its address.
    }
    printList(list1, 5);     // Array now contains: 5 7 9 11 13.

    apply(twoUp, list1, 5);  // Pass twoUp's address, the array, and its length to apply.
                            // Inside apply, f points to twoUp for this call.
    printList(list1, 5);     // Each value increased by 2: 7 9 11 13 15.

    return 0;  // Report successful completion.
}
```

- `void (*f)(int *)` means `f` points to a function taking an `int *` and returning nothing.
- `apply` passes each element's address to `f`, so the function can change the original element.
- `apply(twoUp, list1, 5)` passes **the function**, without calling it at the point it is passed.

### Follow one callback call

For the first iteration of `apply(twoUp, list1, 5)`:

1. `f` points to `twoUp`, and `i` is 0.
2. `data[0]` currently contains 5, because the earlier loop already added 3.
3. `&data[0]` produces the address of that element.
4. `f(&data[0])` calls `twoUp` with that address.
5. Inside `twoUp`, pointer `n` points to that same element. `*n += 2` changes its value from 5 to 7.
6. Control returns to `apply`, which moves on to the next element.

**Two kinds of pointers are involved:** `f` points to executable function code; the `int *` passed to that function points to integer data. The callback's `void` return type means it returns no value; it still changes the array through the supplied address.

The slide starts with `{2, 4, 6, 8, 10}`, applies `threeUp` in a loop, then calls `apply(twoUp, list1, 5)`:

```text
Initial values:       2  4  6  8 10
After threeUp:        5  7  9 11 13
After apply(twoUp):   7  9 11 13 15
```

The tree traversals use the same idea: pass in a function and call it once for each node's data.

## Recursive traversal code — slide 173

[Traversal implementations](../../Canvas/begre246.pdf#page=810)

Each node stores data and pointers to its left and right children:

```c
struct node {
    itemType data;
    struct node *left, *right;
};
```

The following follows the slide's logic, with an explicit callback parameter type. The slide writes `void (*f)()`; here, `void (*f)(itemType)` makes the expected argument clear. `itemType` must be defined by the surrounding program.

```c
// t points to the root of the subtree handled by THIS call.
// f points to the action to perform when visiting a node, such as printItem.
void preOrder(struct node *t, void (*f)(itemType)) {
    if (t != NULL) {              // Only access data/children if a node exists.
        f(t->data);               // V: process this node BEFORE either subtree.
        preOrder(t->left, f);     // L: pause this call while the left subtree finishes.
        preOrder(t->right, f);    // R: after the left call returns, finish the right subtree.
    }
    // A NULL call skips the body. Every call returns to its caller when finished.
}

void inOrder(struct node *t, void (*f)(itemType)) {
    if (t != NULL) {              // NULL means an empty subtree: do nothing and return.
        inOrder(t->left, f);      // L: finish ALL of the left subtree before visiting t.
        f(t->data);               // V: process t after the left call has returned.
        inOrder(t->right, f);     // R: finish ALL of the right subtree next.
    }
    // In a BST, this places smaller values before t and larger values after t.
}

void postOrder(struct node *t, void (*f)(itemType)) {
    if (t != NULL) {              // Guard against dereferencing a missing node.
        postOrder(t->left, f);    // L: finish the left subtree; this call waits.
        postOrder(t->right, f);   // R: finish the right subtree; this call waits again.
        f(t->data);               // V: process t only AFTER both subtree calls return.
    }
    // The subtree's root is the last node visited within that subtree.
}
```

- **Base case:** `t == NULL`. Do nothing and return. This handles an empty tree and missing children.
- **Recursive step:** traverse the child subtrees, passing the same function `f` to each call.
- **Visit:** `f(t->data)` processes the current node's data.
- Moving the visit call before, between, or after the recursive calls changes the traversal order.
- A call such as `inOrder(root, printItem)` visits the BST in sorted order and uses `printItem` to process each value. The supplied function must accept an `itemType` and return `void`.

### Example setup for all three functions

These are **BST traversal functions**: they process every node rather than search for one target value. Use this simple BST and let the visit function print each integer:

```text
       4
      / \
     2   6
```

```c
// Define itemType as int BEFORE the struct node and traversal definitions above.
typedef int itemType;

// Define this callback before main. Its signature matches void (*f)(itemType).
void printItem(itemType value) {
    printf("%d ", value);  // Visiting a node prints its data, not its address.
}

// Inside main, after including <stdio.h> and defining the functions/struct:
struct node left  = {2, NULL, NULL};  // Leaf: both child pointers are NULL.
struct node right = {6, NULL, NULL};  // Another leaf.
struct node top   = {4, &left, &right}; // Root points to the two child nodes.
struct node *root = &top;             // Starting address passed to a traversal.

preOrder(root, printItem);   // Prints: 4 2 6
printf("\n");
inOrder(root, printItem);    // Prints: 2 4 6
printf("\n");
postOrder(root, printItem);  // Prints: 2 6 4
printf("\n");
```

**How recursion resumes:** each call has its own `t` parameter. Calling a traversal on node 2 does not change the waiting call's `t`, which still points to node 4. When the child call finishes, execution resumes at the next statement in the waiting parent call. The same callback pointer `f` is passed to every child call.

### Preorder workflow — visit, left, right

Start with `preOrder(root, printItem)`:

1. At **4**, `t != NULL`, so `f(t->data)` calls `printItem(4)`. Output: `4`.
2. Call `preOrder(t->left, f)` on **2**. The call at 4 waits before its right-subtree call.
3. At **2**, print 2. Its left call receives `NULL` and immediately returns; its right call also receives `NULL` and returns. Node 2's call is finished. Output: `4 2`.
4. Resume at **4**, then call `preOrder(t->right, f)` on **6**.
5. At **6**, print 6. Both NULL child calls return, then node 6's call returns to node 4.
6. Node **4** has no steps left, so the original call returns. Final output: **`4 2 6`**.

### Inorder workflow — left, visit, right

Start with `inOrder(root, printItem)`:

1. At **4**, call `inOrder(t->left, f)` on **2**. Nothing has been printed yet; node 4's visit waits for this call to finish.
2. At **2**, call its left subtree (`NULL`), which immediately returns.
3. Resume at **2** and print 2. Call its right subtree (`NULL`), then return to node 4. Output: `2`.
4. Resume at **4** and print 4. Then call its right subtree on **6**. Output: `2 4`.
5. At **6**, its NULL left call returns, then print 6, then its NULL right call returns.
6. Return from node 6 to node 4, then finish the original call. Final output: **`2 4 6`**.

Node 4 is reached first, but **reaching a node is not necessarily visiting it**. In inorder, the print statement runs only after its left subtree finishes.

### Postorder workflow — left, right, visit

Start with `postOrder(root, printItem)`:

1. At **4**, call the left subtree on **2**. Node 4 has not been printed.
2. At **2**, its left NULL call returns, then its right NULL call returns. Only now print 2 and return to node 4. Output: `2`.
3. Resume at **4** and call the right subtree on **6**. Node 4 still has not been printed.
4. At **6**, both NULL child calls return, then print 6 and return to node 4. Output: `2 6`.
5. Resume at **4**. Both subtrees are finished, so print 4 and return. Final output: **`2 6 4`**.

For a larger tree, each child call repeats this same workflow for its own subtree. A leaf still follows the full pattern; its subtree calls simply encounter `NULL` immediately.

### Additional study notes: cost and uses

- Each complete traversal visits all `n` nodes once: **O(n) time**, assuming O(1) work per visit.
- Recursion uses **O(h) stack space**, where `h` is the tree height: O(log n) for a balanced tree and O(n) for a chain.
- **Inorder** is useful for displaying BST keys in sorted order.
- **Postorder** is useful when children must be handled before their parent, such as freeing an entire tree. A deletion routine must free the node itself after its children; the data-only callback above does not by itself free tree nodes.

## Implementing binary search trees — slide 174

[Slide 174 — Implementing Binary Search Trees](../../Canvas/begre246.pdf#page=811)

The first step in implementing a BST is to **define a node**, just as we did for linked lists. Each node has three fields: the stored data and two pointers connecting it to other nodes.

| Structure | Data field | First pointer | Second pointer |
| --- | --- | --- | --- |
| Doubly linked list node | `data` | `prev`: previous node | `next`: next node |
| Binary search tree node | `data` | `left`: root of the left subtree | `right`: root of the right subtree |

A list's pointers connect items in a sequence. A tree's pointers create branches. Each child node can itself be the root of another subtree.

### Node definition

```c
struct node {
    itemType data;       // The value stored in this particular node.
    struct node *left;   // Address of the left child, or NULL if there is none.
    struct node *right;  // Address of the right child, or NULL if there is none.
};
```

The slide combines the pointer declarations as `struct node *left, *right;`. Splitting them above makes it clear that **both fields are pointers**.

- **`itemType`** stands for the type of data being stored. It must be defined elsewhere, for example with `typedef int itemType;`. This makes the node definition reusable for a chosen data type; it does not automatically allow mixed types in one tree.
- **`struct node *`** means a pointer to another node of this same structure type. The node stores the other node's address rather than containing a complete child node inside itself.
- **A leaf** has both `left == NULL` and `right == NULL`.
- **A node with one child** has one valid child pointer and one NULL pointer.
- **A node with two children** has two valid child pointers.
- A separate **root pointer** identifies where the tree starts. `root == NULL` represents an empty tree.

**C clarification:** the slide describes these fields as private. A plain C `struct` has no `private` access modifier. An implementation can hide its node definition inside a source file and expose operations through a header to keep the representation internal.

### Added example: connecting three nodes

With `itemType` defined as `int`, the following statements inside a function build the same small tree used in our traversal examples:

```c
struct node small = {2, NULL, NULL};  // Store 2; initially no children.
struct node large = {6, NULL, NULL};  // Store 6; initially no children.
struct node top = {4, &small, &large}; // Store 4 and link it to the two existing nodes.
struct node *root = &top;             // Keep the address of the starting node.

// root->data is 4: the data in the root node.
// root->left is &small: the address of the node containing 2.
// root->left->data is 2: follow the left link, then access that node's data.
// root->right->data is 6: follow the right link, then access its data.
// root->left->left is NULL: the node containing 2 has no left child.
```

```text
              root
                |
                v
        [ left | 4 | right ]
            /           \
           v             v
    [NULL | 2 | NULL] [NULL | 6 | NULL]
```

The pointer fields describe the tree's **shape**. To make it a **binary search tree**, we must also preserve its ordering rule: all keys in a node's left subtree are smaller, and all keys in its right subtree are larger, using the course's no-duplicates definition. The structure declaration alone does not enforce this rule.

The example nodes live within their enclosing scope; their addresses must not be used after their lifetimes end. Later insertion code can allocate nodes dynamically so that nodes remain available until explicitly freed.

## BST interface (`bst.h`) — slide 175

[Slide 175 — Implementing Binary Search Trees continued](../../Canvas/begre246.pdf#page=817)

The document separates the **interface** (operations callers can use) from the implementation (how nodes and links are managed). Its `bst.h` example declares these operations:

```c
#ifndef BST_H                  // Include guard: process these declarations only once.
#define BST_H

#include <stdbool.h>           // Defines bool, true, and false.
#include "itemType.h"           // Supplies the chosen itemType and related declarations.

typedef struct bstType *bst;   // bst is a POINTER type referring to a hidden tree structure.

bst createBST(void);           // Create an empty tree and return its handle.
void freeBST(bst);             // Release the tree's allocated storage when finished.
void insertItem(bst, itemType); // Insert an item while preserving BST ordering.
void removeItem(bst, itemType); // Remove an item from the tree.
bool contains(bst, itemType);  // Report whether the tree contains the requested item.

// Each traversal receives a tree and a callback that processes one stored item.
void preorder(bst, void (*f)(itemType));
void inorder(bst, void (*f)(itemType));
void postorder(bst, void (*f)(itemType));

int size(bst);                 // Return the number of items stored in the tree.
bool isEmpty(bst);             // Report whether the tree has no items.

#endif                        // End the include guard.
```

This follows the slide's declarations, with the includes placed inside the guard and `createBST(void)` explicitly declaring no parameters.

### Understanding the tree handle

- `typedef struct bstType *bst;` makes `bst` an alias for a pointer to `struct bstType`.
- In a declaration such as `bst tree;`, **tree is already a pointer** even though there is no visible `*` next to its name.
- The header does not reveal the fields of `struct bstType`. Callers use the functions instead of directly manipulating its internal representation. This is an **opaque type**.
- A `bst` handle and a `struct node *` serve different roles: the public handle represents the tree object; a node pointer identifies a particular node/subtree inside the implementation.
- These lines are **function declarations**, not definitions: they describe argument and return types but contain no function bodies.
- The public names `preorder`, `inorder`, and `postorder` differ in capitalization from the earlier recursive helpers `preOrder`, `inOrder`, and `postOrder`. C treats those as different names.

### Example use of the document's interface

The following added usage example calls the operations declared on slide 175. It assumes an integer `itemType`, a compatible `printItem` callback, and a linked BST implementation:

```c
bst tree = createBST();         // Obtain an empty tree through the public interface.
insertItem(tree, 4);            // First item becomes the root.
insertItem(tree, 2);            // Smaller than 4: goes in the left subtree.
insertItem(tree, 6);            // Larger than 4: goes in the right subtree.

bool found = contains(tree, 2); // true: 2 is stored in this tree.
int count = size(tree);         // 3 items have been inserted.
bool empty = isEmpty(tree);    // false: the tree contains items.
inorder(tree, printItem);      // With an integer-printing callback, prints 2 4 6.

removeItem(tree, 2);            // Remove the item 2; 4 and 6 remain.
freeBST(tree);                 // Release the tree; do not use its handle afterward.
```

## Insertion into a binary search tree — slide 176

[Slide 176 — Insertion into a Binary Search Tree](../../Canvas/begre246.pdf#page=818)

To insert item `n`, start at the root of tree/subtree `t`. Compare `n` with the current root's value and follow **one branch** at a time until an empty subtree is reached.

The document gives this recursive algorithm:

```text
insert item n into subtree t:
    if t is empty:
        return a one-node tree containing n  // Base case: this is the insertion spot.
    else if n < the value at t's root:
        return t with n inserted into its left subtree
                                            // Reapply the algorithm on the left.
    else if n > the value at t's root:
        return t with n inserted into its right subtree
                                            // Reapply the algorithm on the right.
    else:
        error — duplicates are not allowed  // Equality: do not add another node.
```

- **Empty tree:** the new node becomes the overall root.
- **Missing child:** the new node becomes that parent's child, starting as a leaf with two NULL child pointers.
- **Smaller item:** recurse left. **Larger item:** recurse right.
- **Equal item:** reject the duplicate; the course's BST does not store duplicate keys.
- Returning the updated subtree lets the caller keep or update the link to its root. In the next slide's C implementation, this appears as `t->left = insertNode(t->left, item)` or the corresponding assignment to `t->right`.

### Document example: insert 6

Slide 176 provides the algorithm; the document's worked numeric example appears immediately afterward on [slide 177](../../Canvas/begre246.pdf#page=820). It inserts **6** into this tree:

```text
Before insertion:             After insertion:

        7                            7
       / \                          / \
      2   9                        2   9
     / \                          / \
    1   5                        1   5
       /                            / \
      3                            3   6
```

| Step | Current node/subtree | Comparison | Next action |
| --- | --- | --- | --- |
| 1 | Root 7 | 6 < 7 | Recurse into the left subtree rooted at 2 |
| 2 | Node 2 | 6 > 2 | Recurse into the right subtree rooted at 5 |
| 3 | Node 5 | 6 > 5 | Recurse into its right subtree, which is empty |
| 4 | Empty subtree (`NULL`) | No value to compare | Create a node containing 6, with NULL left and right children |
| 5 | Returning to node 5 | New subtree root returned | Set node 5's right pointer to the new node |
| 6 | Returning through 2 and 7 | Their subtree roots stay the same | Preserve the existing links and finish insertion |

Only the path **7 → 2 → 5 → NULL** is followed. Nodes 1, 3, and 9 are not examined. After insertion, inorder traversal produces **1, 2, 3, 5, 6, 7, 9**, confirming the ordering is preserved.

If we try to insert **6 again**, we follow 7 → 2 → 5 → 6 and find equality. Slide 176 calls this an error; slide 177's implementation handles it by printing a warning and ignoring the duplicate.

**Added efficiency note:** insertion takes O(h) time, where h is the height of the tree. This is O(log n) for a balanced tree but can become O(n) for a chain. The basic insertion algorithm does not automatically rebalance the tree.

## Recursive insertion in C — slide 177

[Slide 177 — Insertion continued](../../Canvas/begre246.pdf#page=820)

The document implements slide 176's algorithm with a helper that **returns the root of the updated subtree**. This is different from the public `insertItem(bst, itemType)` interface, which returns `void`.

```c
// Slide logic with descriptive comments; assumes allocation succeeds.
struct node *insertNode(struct node *t, itemType item) {
    if (t == NULL) {                    // Empty subtree: create the new leaf here.
        t = malloc(sizeof(struct node)); // Allocate storage for ONE complete node.
        t->data = item;                 // Store the item in that new node.
        t->left = t->right = NULL;      // The new leaf has no children yet.
    } else if (itemcmp(item, t->data) < 0) {
        t->left = insertNode(t->left, item); // Insert left and save its returned root.
    } else if (itemcmp(item, t->data) > 0) {
        t->right = insertNode(t->right, item); // Insert right and save its returned root.
    } else {
        printf("Warning: duplicate item '%s' ignored\n", toItemString(item));
        // Equal key: leave this tree unchanged rather than create a duplicate.
    }
    return t;                          // Give the caller this subtree's root address.
}
```

- `itemcmp(a, b)` compares the chosen item type: negative means `a < b`, zero means equal, and positive means `a > b`. Do not assume its nonzero results must be exactly -1 or 1.
- `toItemString(item)` supplies a string representation for the warning's `%s`.
- `malloc` is declared in `<stdlib.h>`. A complete implementation must check for `NULL` before writing through the allocated pointer; the slide omits that check.
- At the overall node-root level, use `root = insertNode(root, item)`. Saving the return value is essential when an empty tree gains its first node.

### Follow the document's insertion of 6

Using the tree shown in the preceding section:

```text
insertNode(7, 6)
  6 < 7: update 7's left link using insertNode(2, 6)
    6 > 2: update 2's right link using insertNode(5, 6)
      6 > 5: update 5's right link using insertNode(NULL, 6)
        allocate node 6; return its address
      save that address in 5->right; return node 5
    save/retain node 5 in 2->right; return node 2
  save/retain node 2 in 7->left; return node 7
```

**Why assign the returned pointer?** Each call receives its own copy of `t`. Allocating a node changes that local pointer; the caller must save its returned address to connect the new node into the tree.

## Removing an item: three cases — slide 178

[Slide 178 — Removing an Item from a Binary Search Tree](../../Canvas/begre246.pdf#page=821)

Removal is harder than insertion because an internal node can hold entire subtrees together. We must preserve those subtrees and their ordering, while avoiding unnecessary increases in depth.

First locate the item using comparisons, just as in insertion: go left for a smaller key and right for a larger key. Once found, choose the case based on **the node's number of children**, not the number of descendants.

| Children | Removal strategy |
| --- | --- |
| 0 — leaf | Replace its incoming link with `NULL` |
| 1 | Replace its incoming link with its only child's address |
| 2 | Replace its data with the smallest key in its right subtree, then remove that minimum node |

The **incoming link** is the parent's child pointer, or the overall root pointer when removing the root. Removed node storage must also be released.

## Case 1: remove a leaf — slide 179

[Slide 179 — `removeNode(bst, 3)`](../../Canvas/begre246.pdf#page=828)

The slide starts with root 7, children 2 and 9, node 2's children 1 and 5, and node 5's left child 3:

```text
Before:                 After removing 3:
       7                       7
      / \                     / \
     2   9                   2   9
    / \                     / \
   1   5                   1   5
      /
     3
```

1. Follow comparisons: **7 → 2 → 5 → 3**.
2. Node 3 has no children, so its replacement subtree is `NULL`.
3. Set node 5's left pointer to `NULL` and release node 3's storage.
4. All other links remain the same. Inorder output is now **1, 2, 5, 7, 9**.

## Case 2: remove a node with one child — slide 180

[Slide 180 — `removeNode(bst, 5)`](../../Canvas/begre246.pdf#page=830)

This is a separate example starting from the original tree above, with node 3 still present:

```text
Before:                 After removing 5:
       7                       7
      / \                     / \
     2   9                   2   9
    / \                     / \
   1   5                   1   3
      /
     3
```

1. Follow **7 → 2 → 5** and find the target.
2. Node 5 has one child: node 3 on its left.
3. Return node 3's address as the replacement for the subtree previously rooted at 5.
4. Set `2->right` to that address and release node 5's storage.

Node 3 is **linked directly to node 2**; there is no need to copy every descendant. A sole right child would be promoted in the same way.

## Case 3: remove a node with two children — slide 181

[Slide 181 — `removeNode(bst, 2)`](../../Canvas/begre246.pdf#page=833)

The document's two-child example includes an extra node **4**, which is the right child of 3:

```text
Before:                 After removing 2:
       7                       7
      / \                     / \
     2   9                   3   9
    / \                     / \
   1   5                   1   5
      /                       /
     3                       4
      \
       4
```

1. Find node **2**, which has both left and right children.
2. Find the **smallest item in its right subtree**: start at 5, then follow left to **3**. This is node 2's **inorder successor**.
3. Copy item 3 into the node that previously held 2. That node keeps its existing child links for now.
4. Remove the original minimum node 3 from the right subtree. It has no left child, but it has right child **4**.
5. Replace the original node 3 with node 4: node 5's left pointer now points to 4.

Final inorder output: **1, 3, 4, 5, 7, 9**. The successor is larger than every item in the left subtree and smaller than the remaining items in the right subtree, so the BST ordering is preserved.

**The minimum node need not be a leaf.** It cannot have a left child, but it can have a right child, as this example shows.

## Recursive removal in C — slide 182

[Slide 182 — `removeNode`](../../Canvas/begre246.pdf#page=836)

The following is the slide's logic with comments. Like insertion, it returns the updated subtree root:

```c
struct node *removeNode(struct node *t, itemType item) {
    if (t == NULL) {                    // Empty subtree: the target was not found.
        printf("Warning: item '%s' not found\n", toItemString(item));
    } else if (itemcmp(item, t->data) < 0) {
        t->left = removeNode(t->left, item);   // Search left; save its updated link.
    } else if (itemcmp(item, t->data) > 0) {
        t->right = removeNode(t->right, item); // Search right; save its updated link.
    } else {                           // Comparison is zero: this is the target node.
        if (t->left != NULL && t->right != NULL) {
            t->data = findMin(t->right)->data; // Two children: copy successor's data.
            t->right = removeMin(t->right);   // Remove original successor; repair link.
        } else {
            t = (t->left != NULL) ? t->left : t->right;
            // One child: return that child. Leaf: both are NULL, so return NULL.
            // Slide omission: this loses the old node address without freeing it.
        }
    }
    return t;                          // Caller replaces its link with this address.
}
```

The conditional expression `condition ? a : b` chooses `a` when the condition is true and `b` otherwise. Here it selects the left child if present; otherwise it selects the right child (possibly NULL).

**Memory cleanup note:** the slide's zero/one-child branch updates the tree correctly but does not free the removed node. For integer data, the branch should retain the old address and release it:

```c
struct node *old = t;                   // Keep the node address before bypassing it.
t = (t->left != NULL) ? t->left : t->right; // Select the replacement subtree.
free(old);                             // Free the removed node, not its surviving child.
```

At the overall node-root level, use `root = removeNode(root, item)` because deleting the root can change the starting address. Public `removeItem` can manage that internal update for callers.

## Minimum-node helpers and item types — slide 183

[Slide 183 — `findMin`, `removeMin`, and `itemType.h`](../../Canvas/begre246.pdf#page=837)

### `findMin`: follow left links

The slide writes `return ((t->left==NULL)?t:findMin(t->left));`. Expanded for readability:

```c
static struct node *findMin(struct node *t) {
    // Precondition: t is not NULL. The two-child removal case guarantees this.
    if (t->left == NULL) {
        return t;                      // No smaller node exists to this node's left.
    }
    return findMin(t->left);           // Keep searching down the leftmost path.
}
```

- Returns a **node pointer**, not just its data. `findMin(t)->data` accesses the minimum item.
- In slide 181's right subtree, the workflow is **5 → 3 → return node 3**. Node 3's right child 4 does not affect which key is smallest.
- `static` on these functions gives them internal linkage: they are helpers available within their implementation source file.

### `removeMin`: bypass and free the minimum node

```c
static struct node *removeMin(struct node *t) {
    if (t != NULL) {                   // Empty subtree needs no changes.
        if (t->left != NULL) {
            t->left = removeMin(t->left); // Minimum is farther left; repair that link.
        } else {                      // This node IS the minimum: no left child.
            struct node *ptr = t;     // Remember the node that will be freed.
            t = t->right;             // Promote its right child, or NULL if a leaf.
#ifdef NEED_TO_FREE
            freeItemType(ptr->data);  // Optional cleanup for dynamically owned data.
#endif
            free(ptr);                // Release the minimum node's storage.
        }
    }
    return t;                         // Return the replacement subtree root.
}
```

For the slide 181 example: `removeMin(5)` calls `removeMin(3)`. Node 3 has no left child, so save its address, select its right child 4, free the old node 3, and return node 4. The waiting call sets **5->left = node 4** and returns node 5.

### The document's `itemType.h` example

```c
#ifndef ITEMTYPE_H
#define ITEMTYPE_H

#include <limits.h>                   // Supplies the minimum representable int.
#define NULLITEM INT_MIN              // Sentinel value chosen by this item interface.
// #define NEED_TO_FREE                // Disabled for this integer item type.

typedef int itemType;                  // This version stores integers.
void freeItemType(itemType);           // Type-specific cleanup operation.
int itemcmp(itemType, itemType);       // Type-specific key comparison.
char *toItemString(itemType);          // Type-specific string representation.

#endif
```

`NULLITEM` is an item-value sentinel; it is different from the **NULL pointer** marking an absent child. If the interface reserves `INT_MIN` as a sentinel, that value cannot also unambiguously represent ordinary stored data through that convention.

**Ownership detail:** with the slide's integer `itemType`, copying successor data copies an integer, so freeing the successor node does not invalidate the copied value. If items instead own dynamically allocated memory, the two-child case requires a consistent copy/transfer policy: copying the successor's pointer and then freeing its data in `removeMin` would leave a dangling pointer. The removed target's previous data also needs appropriate cleanup. The slide's optional macro alone does not solve those ownership issues.

**Added efficiency note:** insertion, removal, `findMin`, and `removeMin` follow paths whose lengths depend on tree height: O(h) time, O(log n) for balanced trees, and O(n) in the worst case for a chain. Their recursive implementations can use O(h) stack space. They do not rebalance the tree.

## Quick review

| Question | Answer |
| --- | --- |
| What are the three traversal patterns? | Preorder VLR, inorder LVR, postorder LRV |
| Which traversal visits the root first? | Preorder |
| Which traversal visits the root last? | Postorder |
| Which traversal produces sorted output for a BST? | Inorder |
| Does L mean only visiting the left child? | No; traverse the entire left subtree |
| What ends a recursive branch? | Reaching `NULL` |
| What does the function pointer control? | The action performed when a node is visited |
| Does a balanced tree make a complete traversal O(log n)? | No; visiting every node still takes O(n) time |
| What three fields make up a BST node? | `data`, `left`, and `right` |
| What does a child pointer store? | The address of the root node of that child subtree |
| How do we represent an empty tree? | A root pointer equal to `NULL` |
| Does defining the node structure enforce BST ordering? | No; tree operations must preserve the ordering rule |
| What does the typedef `bst` represent? | A pointer to the hidden `struct bstType` |
| Where does insertion create a new node? | At the empty subtree reached by following comparisons |
| What happens when insertion finds an equal key? | The duplicate is rejected |
| What does an insertion/removal helper return? | The updated subtree root pointer |
| How do we remove a node with one child? | Replace its incoming link with its child |
| What replaces a two-child node in the slides? | The minimum item from its right subtree |
| Can the minimum node have a right child? | Yes; promote that child when removing the minimum |

**Next slide:** 184.
