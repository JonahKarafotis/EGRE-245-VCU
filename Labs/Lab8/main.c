// Lab 8: Linked List Insertion
#include <stdio.h>
#include <stdlib.h>


// The structure of a linked list node
struct node {
    int data;
    struct node* next;
};

typedef struct node *llist;

// Function to create a new Null or empty linked list
llist createList(void) {
    return NULL;
}

// prints contents of list with no newline
void printList(llist list) {
    if (list == NULL) {
        printf("NULL");
    } else {
        struct node* current = list;
        while (current != NULL) {
            printf("%d -> ", current->data);
            current = current->next;
        }
        printf("NULL");
    }
}

// adds n to the end of list l and returns the new list
llist addAtEnd(const llist l, int n) {
    // Create a new node to hold the value n.
    // This node will become the last item in the list.
    struct node *newNode = (struct node*)malloc(sizeof(struct node));
    if (newNode == NULL) {
        fprintf(stderr, "Unable to allocate a list node.\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = n;   // Store the integer value in the new node
    newNode->next = NULL; // Make sure it points to nothing yet

    // If the list is empty, the new node is the first and only node.
    if (l == NULL) {
        return newNode;
    }

    // Start at the beginning of the list and walk to the last node.
    struct node *current = l;
    while (current->next != NULL) {
        current = current->next;  // Move forward until we reach the end
    }

    // Attach the new node after the current last node.
    current->next = newNode;

    // Return the original head of the list, since the list pointer has not changed.
    return l;
}

llist removeAtFront(llist l) {
    // Only remove a node if the list is not empty.
    if (l != NULL) {
        // Save the address of the first node so we can free it later.
        struct node *ptr = l;

        // Move the list head to the second node.
        // This effectively removes the first node from the list.
        l = l->next;

        // Free the memory used by the original first node.
        free(ptr);
    }

    // Return the updated head of the list.
    // If the list was empty, it returns NULL.
    return l;
}

llist add(llist l, int n, int pos) {
    // -- if pos < 0, print warning message and do
    // nothing (i.e. return l unaltered)
    if (pos < 0) {
        printf("Warning: position must be nonnegative; list unchanged.\n");
        return l;
    }

    // Create a new node to hold the value n.
    struct node *newNode = malloc(sizeof *newNode);
    // Check if memory allocation was successful
    if (newNode == NULL) {
        fprintf(stderr, "Unable to allocate a list node.\n");
        exit(EXIT_FAILURE);
    }

    newNode->data = n;

    // An empty list or position zero makes the new node the head.
    if (l == NULL || pos == 0) {
        newNode->next = l;
        return newNode;
    }

    // adds integer n to list l at position pos,
    // return this new list
    // where position 0 is the first item in list
    // -- if pos >=size of list, add n at end
    struct node *current = l;
    for (int i = 1; i < pos && current->next != NULL; i++) {
        current = current->next;
    }
    newNode->next = current->next;
    current->next = newNode;
    return l;
}

int main(void) {
    printf("Creating a new (NULL) linked list\n");
    llist l = createList();
    printList(l); printf("\n");
    printf("Adding 2 to the end of the list\n");
    l = addAtEnd(l,2);
    printList(l); printf("\n");
    printf("Adding 4 to the end of the list\n");
    l = addAtEnd(l,4);
    printList(l); printf("\n");
    printf("Adding 6 to the end of the list\n");
    l = addAtEnd(l,6);
    printList(l); printf("\n");
    printf("Removing the first element from the list\n");
    l = removeAtFront(l);
    printList(l); printf("\n");

    // Start fresh so the six Lab 8 cases are easy to follow.
    printf("\nRecursively removing all elements from our list\n");
    while (l != NULL) {
        l = removeAtFront(l);
        printList(l); printf("\n");
    }

    printf("\n(a) Adding 4 to an empty list at position 0\n");
    l = add(l, 4, 0);
    printList(l); printf("\n");

    printf("(b) Adding 2 at position 0 of a non-empty list\n");
    l = add(l, 2, 0);
    printList(l); printf("\n");

    printf("(c) Adding 3 in the middle at position 1\n");
    l = add(l, 3, 1);
    printList(l); printf("\n");

    printf("(d) Adding 6 at the end at position 3\n");
    l = add(l, 6, 3);
    printList(l); printf("\n");

    printf("(e) Trying to add 99 at negative position -1\n");
    l = add(l, 99, -1);
    printList(l); printf("\n");

    printf("(f) Adding 8 at position 100, beyond the list size\n");
    l = add(l, 8, 100);
    printList(l); printf("\n");

    while (l != NULL) {
        l = removeAtFront(l);
    }
    return 0;
}

