# Class Notes — November 2

## Chapter 9 — Trees

Based on the course [PowerPoint PDF](../../Canvas/begre246.pdf#page=700), beginning with **slide 154, “Rationale for Trees.”** These starter notes cover slides 154–173; they do not indicate how far we have reached in class.

## Rationale for trees — slides 154–155

- Searching for an item is a common task, so the efficiency of searching matters.
- Two approaches are **sequential search** and **binary search**.
- To compare them, count the **key comparisons** needed as the number of items, `n`, grows.
- Timing alone can be misleading: results depend on the computer, compiler, other running programs, and the input. Comparison counts help explain the algorithms independently of a particular machine.
- **Key:** the value we are trying to find or the field used to identify an item.

## Sequential search — slides 156–157

[Slides 156–157](../../Canvas/begre246.pdf#page=707)

Examine each item in order until we find the key or reach the end of the list. The list does not need to be sorted.

| Search result | Best case | Worst case | Average case |
| ---           | ---       | ---        | ---          |
| Key is present| 1         | n          | (n + 1) / 2  |
| Key is absent | n         | n          | n            |

- A key in the first position requires 1 comparison; a key in position 5 requires 5 comparisons.
- An unsuccessful search must examine every item.
- For a successful search, assuming each position is equally likely:

```text
Average = (1 + 2 + ... + n) / n
        = [n(n + 1) / 2] / n
        = (n + 1) / 2
```

### Why (n + 1) / 2 instead of n / 2?

The possible comparison counts go from **1 to n**, so their average is halfway between **1 and n**: `(1 + n) / 2`. Even finding the first item requires one comparison.

For example, with 4 items:

```text
Position:      1  2  3  4
Comparisons:   1  2  3  4

Average = (1 + 2 + 3 + 4) / 4 = 2.5
        = (4 + 1) / 2
```

Using `n / 2` gives 2, which is **half a comparison lower** than the actual average. The extra `+1` accounts for the comparison counts starting at 1 rather than 0. An average can be fractional even though each individual search uses a whole number of comparisons.

This assumes the item is present and each position is equally likely. If the item is absent, sequential search requires all `n` comparisons.

**Growth:** average and worst-case sequential search are O(n). Doubling the list size roughly doubles the work. The best successful case is O(1).

## Binary search — slide 158

[Example on slide 158](../../Canvas/begre246.pdf#page=721)

**Binary search requires a sorted list.** Compare the key to the middle item, then discard the half that cannot contain it.

1. Set `low = 0` and `high = n - 1`.
2. Find the middle index: `mid = low + (high - low) / 2`.
3. If `list[mid] == key`, the search succeeds.
4. If `list[mid] < key`, search the upper half: `low = mid + 1`.
5. Otherwise, search the lower half: `high = mid - 1`.
6. Repeat while `low <= high`. If `low > high`, the key is absent.

The slides use `(low + high) / 2`; the expression above avoids overflow when computing the midpoint for valid nonnegative array bounds. Integer division drops the fractional part.

### Slide example: find 12

```text
Index:  0  1  2  3   4   5   6   7   8
Value:  2  4  6  8  10  12  14  16  18
```

| Step | low | high | mid | Middle value | Action |
| --- | --- | --- | --- | --- | --- |
| 1   | 0   | 8   | 4   | 10  | 10 < 12, so low becomes 5 |
| 2   | 5   | 8   | 6   | 14  | 14 > 12, so high becomes 5 |
| 3   | 5   | 5   | 5   | 12  | Found |

This visits 3 array positions. In the slide's code, it performs **5 key comparisons**: two at each unsuccessful middle item and one equality comparison at the matching item.

## Why binary search is faster — slides 159–163

[Comparison-tree analysis](../../Canvas/begre246.pdf#page=741)

- Each middle item can be represented as a node in a comparison tree.
- Going down a level means choosing a smaller remaining search interval.
- Each unsuccessful middle-item check uses two key comparisons in the slide's implementation: `==`, then `<`.
- The number of levels grows logarithmically because each step removes about half the remaining candidates.

For a **perfect binary tree**, with all levels filled:

| Levels (the slides call this height) | Nodes   |
| ---                                  | ---     |
| 1                                    | 1       |
| 2                                    | 3       |
| 3                                    | 7       |
| 4                                    | 15      |
| 5                                    | 31      |
| k                                    | 2^k - 1 |

```text
n = 2^k - 1
n + 1 = 2^k
k = log2(n + 1)
```

Here `lg` means log base 2. The slides approximate the comparison count as **2 lg n**, since there are roughly `lg n` levels and up to two key comparisons at each level.

### Binary search comparison counts — slide 163

The following table matches [slide 163](../../Canvas/begre246.pdf#page=759), using the slide's approximate comparison counts:

| Case | Best case | Worst case | Average case |
| --- | --- | --- | --- |
| Item is present | 1 | 2 lg n | 2 lg n |
| Item is not present | 2 lg n | 2 lg n | 2 lg n |

The slides describe little difference between average and worst cases, or between unsuccessful and successful searches. The best successful case takes just one comparison because the key matches the first middle item.

**Precision note:** `2 lg n` is an approximation, not an exact count for every list size or search. Exact counts depend on the path and implementation; a successful final equality check uses only one comparison. The repeated entries in the table summarize the slide's approximation rather than asserting identical counts for every search.

**Connection to Big O:** the slide's approximate count, `2 lg n`, has **O(log n)** growth. Big O describes how the work grows as `n` increases and drops constant multipliers such as `2`. Keep `2 lg n` when discussing the slide's comparison-count estimate; use O(log n) when describing its growth rate. The best successful case is O(1).

## Comparison-count experiment — slide 164

[Slide 164 code](../../Canvas/begre246.pdf#page=762)

The example creates a sorted array containing `0` through `1,999,999` and searches for a key near 93% of the way through it.

- `SIZE = 2000000`.
- The key expression gives `1863440`.
- Sequential search reaches that key after **1,863,441 comparisons**, because indexing starts at 0.
- Binary search needs only a small number of middle-item checks because it repeatedly halves the search interval.
- The example functions return the comparison count when successful and `-1` when unsuccessful. They do **not** return the found index; an unsuccessful return also does not report its comparison count.

The important distinction is **linear growth versus logarithmic growth**, rather than one specific timing result.

## Issues with binary search on an array — slide 165

[Slide 165](../../Canvas/begre246.pdf#page=764)

The PowerPoint identifies two issues with using binary search on a list stored in an array:

1. **It only works on sorted lists.** The list must be sorted before searching, and updates must preserve that order.
2. **It suffers from costly insertions and deletions.** Inserting or deleting an item can require shifting many array elements, taking O(n) time even though searching is O(log n).

### Can we combine binary-search efficiency with linked structures?

The PowerPoint asks whether we can get the efficiency of binary search along with the more efficient insertions and deletions of linked structures. **Yes — with trees!**

Trees combine a branching search structure with linked nodes. A binary search tree uses comparisons to choose a left or right subtree, narrowing the search. Insertions and deletions update links instead of shifting array elements.

**Qualification:** a binary search tree's performance depends on its height. A balanced tree supports O(log n) search, insertion, and deletion; a badly unbalanced tree can become a chain and require O(n) work.

## Introduction to trees — slide 166

[Slide 166](../../Canvas/begre246.pdf#page=770)

| Term          | Meaning |
| ---           | --- |
| Node / vertex | An item in the tree |
| Edge          | A connection from a parent to a child |
| Root          | The distinguished starting node; it has no parent |
| Parent        | The node directly above a child |
| Child         | A node connected directly below its parent |
| Leaf          | A node with no children |
| Subtree       | A node together with all of its descendants |
| Path          | A sequence of connected nodes |

- Every node except the root has exactly one parent.
- There is exactly one path from the root to any particular node.
- Any node can be viewed as the root of its own subtree.

## Binary trees — slide 167

[Slide 167](../../Canvas/begre246.pdf#page=777)

- A **binary tree** allows each node to have at most two children: a **left child** and a **right child**.
- A node may have 0, 1, or 2 children.
- A **full binary tree** has exactly two children at every non-leaf node.
- “Full” does not mean every level is filled. The fully filled tree used in the height calculation above is a **perfect** binary tree.

## Binary search trees (BSTs) — slide 168

[Slide 168](../../Canvas/begre246.pdf#page=781)

A BST adds an ordering rule to a binary tree. For **every node**:

- All values in its left subtree are less than the node's value.
- All values in its right subtree are greater than the node's value.
- The slide's BST definition does not allow duplicate items.

The rule applies to the **entire subtree**, not just the immediate children. Items need comparable keys so we can decide which direction to search.

```text
        10
       /  \
      4    14
     / \   / \
    2   8 12 18
```

To search for `12`: compare with `10`, go right to `14`, then go left to `12`. Reaching a missing child means the key is absent.

## Binary search tree traversals — slides 169–173

[Traversal patterns, slide 169](../../Canvas/begre246.pdf#page=787) · [Recursive code, slide 173](../../Canvas/begre246.pdf#page=810)

**Traversal** means visiting every node in a tree in a defined order. Unlike searching for one key, a complete traversal processes all nodes.

At each node, the slides identify three tasks:

- **V — Visit:** process the current node's data, for example by printing it.
- **L — Left:** traverse the entire left subtree.
- **R — Right:** traverse the entire right subtree.

| Traversal | Slide notation | Order of tasks |
| --- | --- | --- |
| Preorder | VLR | Visit the node, traverse left, traverse right |
| Inorder | LVR | Traverse left, visit the node, traverse right |
| Postorder | LRV | Traverse left, traverse right, visit the node |

**The same pattern applies recursively at every node.** L and R mean completing the corresponding subtree, not just visiting its immediate child. “Pre,” “in,” and “post” describe when we visit the current node relative to its subtrees.

### Worked example

Using the numeric BST from our notes (an added example):

```text
        10
       /  \
      4    14
     / \   / \
    2   8 12 18
```

| Traversal | Visit order |
| --- | --- |
| Preorder — VLR | 10, 4, 2, 8, 14, 12, 18 |
| Inorder — LVR | 2, 4, 8, 10, 12, 14, 18 |
| Postorder — LRV | 2, 8, 4, 12, 18, 14, 10 |

For inorder, finish the left subtree (`2, 4, 8`), visit the root (`10`), then finish the right subtree (`12, 14, 18`).

**Inorder traversal of a BST visits the keys in ascending order.** This relies on the BST ordering rule; an arbitrary binary tree does not necessarily produce sorted output.

### Passing a function to the traversal — slides 171–172

[Function pointers, slide 171](../../Canvas/begre246.pdf#page=792) · [Example, slide 172](../../Canvas/begre246.pdf#page=808)

The slides introduce **function pointers** so the traversal can accept a function describing what “visit” should do.

- A function name such as `printItem`, used without calling it, supplies a pointer to that function.
- `void (*f)(itemType)` declares `f` as a pointer to a function that accepts an `itemType` and returns nothing.
- Calling `f(t->data)` processes the current node's data. We can pass a different function to perform a different action while keeping the traversal order the same.
- Parentheses matter: `int (*p)(float)` is a pointer to a function returning `int`; `int *p(float)` declares a function returning an `int *`.

### Recursive traversal code — slide 173

The following follows the slide's traversal logic, with the callback parameter explicitly typed as `itemType` (as in the interface on slide 175). `itemType` must be defined by the surrounding program; this is a snippet, not a standalone program.

```c
struct node {
    itemType data;
    struct node *left, *right;
};

void preOrder(struct node *t, void (*f)(itemType)) {
    if (t != NULL) {
        f(t->data);               // V
        preOrder(t->left, f);     // L
        preOrder(t->right, f);    // R
    }
}

void inOrder(struct node *t, void (*f)(itemType)) {
    if (t != NULL) {
        inOrder(t->left, f);      // L
        f(t->data);               // V
        inOrder(t->right, f);     // R
    }
}

void postOrder(struct node *t, void (*f)(itemType)) {
    if (t != NULL) {
        postOrder(t->left, f);    // L
        postOrder(t->right, f);   // R
        f(t->data);               // V
    }
}
```

- **Base case:** if `t == NULL`, return without processing anything. This handles an empty tree or a missing child.
- Each recursive call handles a smaller subtree and passes along the same visit function `f`.
- Moving the visit call before, between, or after the two recursive calls changes the traversal order.
- **Added complexity note:** each complete traversal takes O(n) time if visiting each node takes O(1). Recursive stack space is O(h), where `h` is the tree height: O(log n) for a balanced tree and O(n) for a chain.

## Quick review

- Why is successful sequential search about n / 2 comparisons on average? Each position is equally likely, so average the counts 1 through n.
- What must be true before binary search works? The list must be sorted.
- What does `lg n` mean? Log base 2 of n.
- Why is binary search O(log n)? Each step removes about half the remaining candidates.
- Why use a tree? To combine efficient searching with linked insertion and deletion.
- Does any binary tree qualify as a BST? No; a BST must also satisfy the ordering rule at every node.
- What controls BST performance? Its height; balance keeps search paths short.

- What are the three traversal orders? Preorder VLR, inorder LVR, and postorder LRV.
- Which traversal produces sorted keys in a BST? Inorder.
- What stops recursive traversal? Reaching a `NULL` child.

**Next slide:** 174, Implementing Binary Search Trees.
