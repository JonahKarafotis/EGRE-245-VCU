#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define CHUNK 10
#define DEBUG true

/*
The Goal of this lab is to implement a dynamic array of integers. 
The array should be able to grow as needed when new items are added. 
The `addItem` function will handle adding new items to the list, and the `printList` function will display the contents of the list.
*/

/*
This is a new structure called intList. 
It contains:
    - a pointer to an integer array
    - the current size of the list
    - the current count of items in the list.
*/
typedef struct {
    int *data;
    int listSize; // current number of cells in array
    int count; // current number of items in list
} intList;

// The purpose of the addItem function is to add a new integer item to the dynamic array.
void addItem(intList*,int);

// The purpose of the printList function is to print all the items in the dynamic array.
void printList(intList*);

// This is the main function that initializes the dynamic array, adds items to it, and prints the contents of the list.
int main(void) { 

    /*
    Creates a new intList Structure and allocates memory for it. 
    Initializes the data pointer to NULL and sets the count and listSize to 0.
    */
    intList *list = malloc(sizeof(intList));

    // Assignes values to our new intList structure using the arrow operator to assign values to the members of a structure refrenced by a pointer.
    list->data = NULL; list->count = list->listSize = 0;

    /*
    Adds items to the list and prints the list contents.

    This will fail to compile if the addItem function is not implemented correctly 
    as it is responsible for managing the dynamic memory allocation and resizing of the array when new items are added.
    */
    for (int i=1; i <= 112; i++)
        addItem(list,i*2);
    printList(list); printf("\n");
    return 0;
}

/*
addItem(intList *list, int item)

Arguments:
    - list: a pointer to an intList structure that holds the dynamic array and its metadata.
    - item: the integer value to be added to the dynamic array.

This function checks if the current count of items in the list has reached the allocated size of the array. 
If it has, the function reallocates memory to increase the size of the array by a defined chunk size (CHUNK). 
It then adds the new item to the array and increments the count of items in the list. 
If the memory allocation fails, it prints an error message and exits the program.
*/
void addItem(intList *list, int item) {

    // Check if we need to allocate more memory for the array
    if (list->count == list->listSize) {

        // Calculate the new size of the array by adding the defined chunk size (CHUNK) to the current list size.
        int newSize = list->listSize + CHUNK;

        // Make a new larger array
        int *newData = malloc(newSize * sizeof(int));

        // Check if malloc failed to allocate memory.
        // If malloc fails, it returns NULL.
        if (newData == NULL) {

            // Print an error message.
            fprintf(stderr, "Memory allocation failed\n");

            // Stop the program because newData cannot be used safely.
            exit(EXIT_FAILURE);
        }

        // Copy values from OLD array into NEW array
        for (int i = 0; i < list->count; i++) {
            newData[i] = list->data[i];
        }

        // Save the old array
        int *old = list->data;

        // Make the structure point to the new array
        list->data = newData;

        // Update the size
        list->listSize = newSize;

        // Free the old array
        free(old);
    }

    // Add the new item
    list->data[list->count] = item;

    // Increase number of items
    list->count++;
}

void printList(intList *list) {
    printf("List contents: ");
    for (int i=0; i<list->count; i++) {
        printf("%d ",list->data[i]);
    }
    printf("\n");
}