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
    struct node *newNode = (struct node*)malloc(sizeof(struct node));
    newNode->data = n;
    newNode->next = NULL;

    if (l == NULL) {
        return newNode;
    }

    struct node *current = l;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = newNode;

    return l;
}

llist removeAtFront(llist l) {
    if (l != NULL) {
        struct node *ptr = l;
        l = l->next;
        free(ptr);
    }
    return l;
}



int main() {
    llist l = createList();
    printList(l); printf("\n");
    l = addAtEnd(l,2);
    l = addAtEnd(l,4);
    l = addAtEnd(l,6);
    printList(l); printf("\n");
    l = removeAtFront(l);
    printList(l); printf("\n");
    return 0;
}

