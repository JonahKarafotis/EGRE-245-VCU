#include <stdio.h>
#include <stdlib.h>
#define MAX_STRING_SIZE 100
int main() {
char *str = (char *) malloc(sizeof(char)*MAX_STRING_SIZE+1);
printf("string? ");
scanf("%s",str);
printf("string entered = %s\n",str);
}