#include <stdio.h>
#include <stdlib.h>


int main()
{
    int a=10;
    int *p=NULL;
    int **pp=NULL;
    int ***ppp=NULL;
    

    p=&a;
    pp=&p;
    printf("Test pointer with local variable a:\n");
    printf("value of a is %d, the address of a is 0x%0x\n",a,&a);
    printf("Address of Pointer p is 0x%0x, Pointer p point to an address is 0x%0x, the value of that address is %d\n",&p,p,*p);
    printf("Address of double pointer is 0x%0x,Double Pointer pp point to an address 0x%0x, the value of double pointer is 0x%0x\n, the content is %d\n ",&pp, pp,*pp,**pp);
    printf("size of pointer p is %d,size of double pointer pp is %d, sizeof of int is %d\n",sizeof(p),sizeof(pp),sizeof(int));    
    printf("\n\n");

    printf("Test pointer with dynamic allocation:\n");
    p=(int *)malloc(sizeof(int));
    *p=20;
    pp=&p;
    printf("Address of Pointer p is 0x%0x, Pointer p point to an address is 0x%0x, the value of that address is %d\n",&p,p,*p);
    printf("Address of double pointer is 0x%0x,Double Pointer pp point to an address 0x%0x, the value of double pointer is 0x%0x\n, the content is %d\n ",&pp, pp,*pp,**pp);
    printf("size of pointer p is %d,size of double pointer pp is %d, sizeof of int is %d\n",sizeof(p),sizeof(pp),sizeof(int));
    //free(p);
    printf("\n\n");

    
    printf("\n\nTest double pointer with dynamic allocation:\n");
    pp=(int **)malloc(sizeof(int *));
    *pp = p;
    printf("Address of double pointer is 0x%0x,Double Pointer pp point to an address 0x%0x, the value of double pointer is 0x%0x\n, the content is %d\n ",&pp, pp,*pp,**pp);
    free(p);
    free(pp);
    return 0;
}