/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, OCaml, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>

void swap1(int i, int j) {
    int t;
    printf("----swap1---\n");
    printf("address of i:%0x,value of i:%d\n", &i,i);
    printf("address of i:%0xvalue of j:%d\n", &j,j);
    t=i;
    i=j;
    j=t;
    printf("address of i:%0x,value of i:%d\n", &i,i);
    printf("address of i:%0xvalue of j:%d\n", &j,j);
    printf("----swap1---\n");
}

void swap2(int *i, int *j) {
    int t;
    printf("----swap2---\n");
    printf("address of i:%0x,value of i:%d\n", i,*i);
    printf("address of i:%0xvalue of j:%d\n", j,*j);
    t=*i;
    *i=*j;
    *j=t;
    printf("address of i:%0x,value of i:%d\n", i,*i);
    printf("address of i:%0xvalue of j:%d\n", j,*j);
    printf("----swap2---\n");
}

int main()
{
    int a=4;
    int b=5;
    
    printf("Calling swap1\n");
    printf("address of a:%0x, value of a:%d\n", &a,a);
    printf("address of b:%0x, value of b:%d\n", &b,b);
    swap1(a,b);
    printf("address of a:%0x, value of a:%d\n", &a,a);
    printf("address of b:%0x, value of b:%d\n", &b,b);
    
    printf("Calling swap2\n");
    printf("address of a:%0x, value of a:%d\n", &a,a);
    printf("address of b:%0x, value of b:%d\n", &b,b);
    swap2(&a,&b);
    printf("address of a:%0x, value of a:%d\n", &a,a);
    printf("address of b:%0x, value of b:%d\n", &b,b);
    return 0;
}
