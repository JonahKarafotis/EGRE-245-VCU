#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int *p1,*p2; // Create objects in the stack
    p1 = (int *) malloc(sizeof(int)); // allocate memory in the heap to our pointer in the stack
    p2 = (int *) malloc(sizeof(int)); // allocate memory in the heap to our pointer in the stack
    *p1=2; *p2=3; // assign values to our memory in the heap 
    free(p1); free(p2);
}