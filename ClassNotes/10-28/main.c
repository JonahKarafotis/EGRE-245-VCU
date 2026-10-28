#include <stdio.h>
#include "stackGeneric.h"

int main(void) {
    int l1[] = {1, 2, 3};
    float l2[] = {10.0, 20.0, 30.0};
    char *str = "how now brown cow";
    stack s = createStack();

    for (int i = 0; i < 3; i++) {
        push(s, &l1[i]);
        push(s, &l2[i]);
    }
    push(s, str);

    printf("%s\n", (char *)top(s));
    pop(s);

    while (!isEmpty(s)) {
        printf("%.2f\n", *(float *)top(s));
        pop(s);
        printf("%d\n", *(int *)top(s));
        pop(s);
    }

    freeStack(s);
    return 0;
}