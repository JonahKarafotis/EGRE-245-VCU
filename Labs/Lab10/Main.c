#include <stdio.h>  
#include <stdlib.h>  
#include <limits.h>  
#include "stack.h" 
int main(void) 
{  
    stack s1 = createStack(), s2;
    printf("stack #1: ");
    printStack(s1);
    printf("\n");
    for(int i = 2; i <= 14; i+=2)
    push(s1,i);   
    printf("stack #2: ");
    printStack(s1);
    printf("\n");
    printf("get(2) = %d\n",get(s1,2));
    printf("get(6) = %d\n",get(s1,6));
    printf("get(16) = %d\n",get(s1,16));
    printf("get(-1) = %d\n",get(s1,-1));
    printf("stack #3: ");
    printStack(s1);
    printf("\n");
    pop(s1);
    pop(s1);
    printf("stack #4: ");
    printStack(s1);
    printf("\n");
    put(s1,100,2);
    put(s1,200,99);
    printf("stack #5: ");
    printStack(s1);
    printf("\n");
    s2 = createStack();
    put(s2,1000,5);
    printf("stack #6: ");
    printStack(s2);
    printf("\n");
    destroyStack(s1);
    destroyStack(s2);
    return 0;
}
