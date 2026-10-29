// Lab 9: Doubly Linked List Implementation
#include <stdio.h>
#include <stdlib.h>

// The structure of a linked list node
struct node {
    int data;
    struct node* next;
    struct node* prev;
};

typedef struct node *dllist;

// Function to create a new Null or empty linked list
dllist createDList(void) {
    return NULL;
}

// prints contents of list with no newline
void printDList(dllist list) {
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
dllist addAtEnd(const dllist l, int n) {
    // Create a new node to hold the value n.
    // This node will become the last item in the list.
    struct node *newNode = (struct node*)malloc(sizeof(struct node));
    // Stop safely if memory cannot be allocated instead of using a NULL pointer.
    if (newNode == NULL) {
        fprintf(stderr, "Unable to allocate a list node.\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = n;   // Store the integer value in the new node
    newNode->next = NULL; // Make sure it points to nothing yet
    newNode->prev = NULL; // Make sure it points to nothing yet

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

    newNode->prev = current; // Make the new node point back to the current last node

    // Return the original head of the list, since the list pointer has not changed.
    return l;
}

dllist removeAtFront(dllist l) {
    // Only remove a node if the list is not empty.
    if (l != NULL) {
        // Save the address of the first node so we can free it later.
        struct node *ptr = l;

        // Move the list head to the second node.
        // This effectively removes the first node from the list.
        l = l->next;
        // The new head has no previous node; disconnect it from the node being freed.
        if (l != NULL) {
            l->prev = NULL; // Make the new head node point back to nothing
        }

        // Free the memory used by the original first node.
        free(ptr);
    }

    // Return the updated head of the list.
    // If the list was empty, it returns NULL.
    return l;
}

// Print without a newline, using the backward links to visit nodes in reverse.
void revPrintDList(dllist list) {
    if (list == NULL) {
        printf("NULL");
    } else {
        struct node* current = list;
        // First locate the last node using the forward links.
        while (current->next != NULL) {
            current = current->next;
        }
        // Walk from the last node to the first through each previous pointer.
        while (current != NULL) {
            printf("%d -> ", current->data);
            current = current->prev;
        }
        printf("NULL");
    }
}

int main(void) {
    // Verify that both printing functions handle an empty list.
    dllist dl = createDList();
    printf("newly created list: "); printDList(dl); printf("\n");
    printf("empty list reversed: "); revPrintDList(dl); printf("\n");
    // Appending must connect each new node in both directions.
    dl = addAtEnd(dl,2);
    printf("after adding 2: "); printDList(dl); printf("\n");
    dl = addAtEnd(dl,4);
    printf("after adding 4: "); printDList(dl); printf("\n");
    dl = addAtEnd(dl,6);
    printf("after adding 6: "); printDList(dl); printf("\n");
    printf("list reversed: "); revPrintDList(dl); printf("\n");
    dl = removeAtFront(dl);
    printf("after remove front item: ");printDList(dl); printf("\n");
    printf("remaining list reversed: "); revPrintDList(dl); printf("\n");
    // Remove down to a single node and check its backward link through printing.
    dl = removeAtFront(dl);
    printf("single item reversed: "); revPrintDList(dl); printf("\n");
    // Free the final node, then verify that removing from an empty list is safe.
    dl = removeAtFront(dl); dl = removeAtFront(dl);
    dl = removeAtFront(dl);
    printf("after removing all items: "); printDList(dl); printf("\n");
    printf("after removing all items reversed: "); revPrintDList(dl); printf("\n");
    return 0;
}

