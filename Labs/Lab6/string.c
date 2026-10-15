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

int scmp(string s1, string s2) {
    int i = 0;
    // Increments through the characters of both strings until a difference is found or the end of one string is reached.
    while(i < slen(s1) && i < slen(s2)) {
        if(s1->data[i] != s2->data[i]) {
            return s1->data[i] - s2->data[i];
        }
        i++;
    }
    return 0;
}

string scat(string dest, string src) {
    int dest_len = slen(dest);
    int src_len = slen(src);
    if(dest_len + src_len >= MAX_STRING_SIZE) {
        // Handle error: concatenation would exceed max size
        return NULL;
    }
    for(int i = 0; i < src_len; i++) {
        dest->data[dest_len + i] = src->data[i];
    }
    dest->length = dest_len + src_len;
    return dest;
}

string sToUpper(string s) {

    string upper = screate();

    for (int i = 0; i < s->length; i++) {

        // Leters are stored with a number value in the ASCII table. 
        // The difference between a lowercase letter and its uppercase counterpart is 32. 
        // So, to convert a lowercase letter to uppercase, we can subtract 32 from its ASCII value.
        if (s->data[i] >= 'a' && s->data[i] <= 'z') {
            upper->data[i] = s->data[i] - 32; // Convert to uppercase by subtracting 32
        }
        else {
            upper->data[i] = s->data[i];
        }
    }

    upper->length = s->length;

    return upper;
}

string sToLower(string s) {
    string new_str = screate();
    for(int i = 0; i < slen(s); i++) {
        char c = s->data[i];
        // Leters are stored with a number value in the ASCII table. 
        // The difference between a lowercase letter and its uppercase counterpart is 32. 
        // So, to convert a lowercase letter to uppercase, we can add 32 from its ASCII value.
        if(c >= 'A' && c <= 'Z') {
            new_str->data[i] = c + 32; // Convert to lowercase by adding 32
        } else {
            new_str->data[i] = c;
        }
    }
    new_str->length = slen(s);
    return new_str;
}