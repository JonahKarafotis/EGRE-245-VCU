#include <stdio.h>
#include <stdlib.h>

#define LIST_SIZE 3

int main() {
    printf("Enter %d integers to store in the list\n",LIST_SIZE);
    int *list = (int *) malloc(sizeof(int)*LIST_SIZE),i;
    printf("Pointer List has been created\n");

    // Assign our Valuse to the list
    for (i=0; i < LIST_SIZE; i++) {
        scanf("Enter integer %d: ", &list[i]);
        printf("We are at index %d\n",i);
        }

    // Print our List
    for (i=0; i < LIST_SIZE; i++) printf("%d ",list[i]);
    printf("\n");
    }