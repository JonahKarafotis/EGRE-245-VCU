/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, OCaml, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>

struct node{
    int data;
    struct node *next;
};
typedef struct node * llist;

llist InsertAtEnd(llist head, int i) {
      llist newNode, ptr;
      newNode = (llist)malloc(sizeof(struct node));
      newNode->data = i;
      newNode->next = NULL;
      if (head==NULL)  head = newNode;
      else {
        ptr = head;
        while (ptr->next!=NULL) ptr=ptr->next;
        ptr->next = newNode;
      }
      return head;  
}

int main()
{
    llist l1=NULL;
    llist newNode;
    llist ptr;
    int i;
    
/*    
    for (i<0;i<10;i++) {
        //insert a node into the linked list
        newNode=(llist) malloc (sizeof(struct node));
        newNode->data = i;
        newNode->next=NULL;
        if (l1==NULL) l1=newNode;
        else {
            ptr = l1;
            while (ptr->next!=NULL) ptr=ptr->next;
        
           ptr->next = newNode;
        }
    
    }
    */
    
    for (i=0;i<10;i++) 
        l1=InsertAtEnd(l1,i);
    
    ptr=l1;
    
    while (ptr!=NULL) {
        printf("%5d\n",ptr->data);
        ptr = ptr->next;
        
    }
    
    return 0;
}

