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
    printf("value of a is %d, the address of a is %p\n",a,&a);
    printf("Address of Pointer p is %p, Pointer p point to an address is %p, the value of that address is %d\n",&p,p,*p);
    printf("Address of double pointer is %p,Double Pointer pp point to an address %p, the value of double pointer is %p\n, the content is %d\n ",&pp, pp,*pp,**pp);
    printf("size of pointer p is %zu,size of double pointer pp is %zu, sizeof of int is %zu\n",sizeof(p),sizeof(pp),sizeof(int));    
    printf("\n\n");

    printf("Test pointer with dynamic allocation:\n");
    p=(int *)malloc(sizeof(int));
    *p=20;
    pp=&p;
    printf("Address of Pointer p is %p, Pointer p point to an address is %p, the value of that address is %d\n",&p,p,*p);
    printf("Address of double pointer is %p,Double Pointer pp point to an address %p, the value of double pointer is %p,\n the content is %d\n ",&pp, pp,*pp,**pp);
    printf("size of pointer p is %zu,size of double pointer pp is %zu, sizeof of int is %zu\n",sizeof(p),sizeof(pp),sizeof(int));
    //free(p);
    printf("\n\n");

    
    printf("\n\nTest double pointer with dynamic allocation:\n");
    pp=(int **)malloc(sizeof(int *));
    *pp = p;
    printf("Address of double pointer is %p,Double Pointer pp point to an address %p, the value of double pointer is %p\n, the content is %d\n ",&pp, pp,*pp,**pp);
    free(p);
    free(pp);
    return 0;
}