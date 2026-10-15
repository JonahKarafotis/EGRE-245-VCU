#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_SIZE 100

typedef struct {
    int number;
    char name[NAME_SIZE+1];
    int on_hand;
    } Part;

int main() 
    {
    // Create a pointer to a Part structure and dynamically allocate memory for it
    Part *p = (Part *) malloc(sizeof(Part));

    // Assigne values to the members of the structure using the pointer
    (*p).number = 10221; strcpy((*p).name,"widget 5"); (*p).on_hand = 1200;

    // Print out the values of the members of the structure using the pointer
    printf("%d: %s (# in stock: %d)\n",(*p).number,(*p).name,(*p).on_hand);
    }