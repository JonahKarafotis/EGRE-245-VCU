#include <stdio.h>
#include <stdlib.h>

int swap(int *x, int *y) {
    int temp = *x;
    *x = *y;
    *y = temp;
    return 0;
}

int main(void) {
    int a = 10;
    int b = 5;
    swap(&a, &b);
    printf("%d\n", a);
    printf("%d", b);
    return 0;
}