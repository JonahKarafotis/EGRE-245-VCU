#include "string.h"
#include <stdlib.h>
#include <stdio.h>

string screate() {
    string s = malloc(sizeof(struct stringType));
    if (s != NULL)
        s->length = 0;
    return s;
}

void printString(string s) {
    for(int i = 0; i < slen(s); i++)
        printf("%c",s->data[i]);
}

void scopy(string s,char *cstr) {
    // your stuff here, as needed
    int i = 0;
    while(cstr[i] != '\0' && i < MAX_STRING_SIZE) {
        // -- copy the i-th char in cstr to the
        s->data[i] = cstr[i];
        // i-th location in the data field in s
        // -- increment the size field in s
        i++;
    }
    s->length = i;
}

int slen(string s) {
    return s->length;
}