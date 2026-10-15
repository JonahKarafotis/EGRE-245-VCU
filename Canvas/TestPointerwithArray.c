/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, OCaml, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int numbers[10];
    int *pnum=NULL;
    int **ppnum=NULL;
    int i,j;
    
    for (i-0;i<10; i++)
        numbers[i] = 2*i;
    
    printf("Testing pointer with array --1\n");  
    pnum = numbers;
    ppnum = &pnum;
    
    for (i=0;i<10; i++)
        printf("%5d", pnum[i]);
    printf("\n");

    for (i=0;i<10; i++)
        printf("%5d", (*ppnum)[i]);
    printf("\n");
    
    for (i=0;i<10; i++)
        printf("%5d", *(*ppnum+i));
    printf("\n");
    
    
    printf("\nTesting pointer with array --2\n");    

    ppnum = (int **)malloc(5*sizeof(int *));
    for (i=0;i<5;i++)
        ppnum[i] = NULL;
    pnum = (int *)malloc(10*sizeof(int));
    for (i=0;i<10;i++)
        *(pnum+i) = i*5;
    ppnum[0]=numbers;
    ppnum[1]=pnum;
    for (j=0;j<2;j++) {
        for (i=0;i<10;i++)
            printf("%5d",ppnum[j][i]);
        printf("\n");
    }
    
    printf("\nTesting pointer with array --3\n");
    for (j=0;j<2;j++) {
        for (i=0;i<10;i++)
            printf("%5d",(*(ppnum+j))[i]);
        printf("\n");
    }

    printf("\nTesting pointer with array --4\n");
    for (j=0;j<2;j++) {
        for (i=0;i<10;i++)
            printf("%5d",*(*(ppnum+j)+i));
        printf("\n");
    }

  
    free(pnum);
    free(ppnum);
    return 0;
}
