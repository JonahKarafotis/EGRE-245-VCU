#include <stdio.h>
#include <stdlib.h>

int main() {

    // Create a pointer to an integer
    int *ptr;

    // Create a dynamic "no-name" integer variable
    ptr = malloc(sizeof(int));

    // Put the value 10 into that dynamic variable
    *ptr = 10;

    // Print the value stored there
    printf("Value: %d\n", *ptr);

    // Print the memory address
    printf("Address: %p\n", (void *)ptr);

    // Change the value through the pointer
    *ptr = 25;

    printf("New value: %d\n", *ptr);

    // Give the memory back when finished
    free(ptr);

    return 0;
}