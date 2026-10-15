// Lab 7: Linked List Implementation
#include <stdio.h>
#include <stdlib.h>


// The structure of a linked list node
struct node {
    int data;
    struct node* next;
};

typedef struct node *llist;

// Function to create a new Null or empty linked list
llist createList() {
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



int main() {
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
    return 0;
}

