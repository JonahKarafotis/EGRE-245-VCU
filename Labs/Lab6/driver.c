#include <stdio.h>
#include "string.h"

int main(void) {

    // Test 1: Normal string
    printf("Test 1: Normal string\n");

    string s1 = screate();
    scopy(s1, "hi mom");

    printf("Expected length: 6\n");
    printf("Actual length:   %d\n", slen(s1));

    printf("Expected string: hi mom\n");
    printf("Actual string:   ");
    printString(s1);
    printf("\n\n");


    // Test 2: Empty string
    printf("Test 2: Empty string\n");

    string s2 = screate();
    scopy(s2, "");

    printf("Expected length: 0\n");
    printf("Actual length:   %d\n", slen(s2));

    printf("Expected string: \n");
    printf("Actual string:   ");
    printString(s2);
    printf("\n\n");


    // Test 3: One character
    printf("Test 3: One character\n");

    string s3 = screate();
    scopy(s3, "A");

    printf("Expected length: 1\n");
    printf("Actual length:   %d\n", slen(s3));

    printf("Expected string: A\n");
    printf("Actual string:   ");
    printString(s3);
    printf("\n\n");


    // Test 4: String with spaces
    printf("Test 4: String with spaces\n");

    string s4 = screate();
    scopy(s4, "Hello world from C");

    printf("Expected length: 18\n");
    printf("Actual length:   %d\n", slen(s4));

    printf("Expected string: Hello world from C\n");
    printf("Actual string:   ");
    printString(s4);
    printf("\n\n");


    // Test 5: Numbers and symbols
    printf("Test 5: Numbers and symbols\n");

    string s5 = screate();
    scopy(s5, "Test123!?");

    printf("Expected length: 9\n");
    printf("Actual length:   %d\n", slen(s5));

    printf("Expected string: Test123!?\n");
    printf("Actual string:   ");
    printString(s5);
    printf("\n\n");


    // Test 6: Copy a new value into an existing string
    printf("Test 6: Re-copy into existing string\n");

    string s6 = screate();

    scopy(s6, "first");
    printf("First value:     ");
    printString(s6);
    printf("\n");

    scopy(s6, "second");

    printf("Expected length: 6\n");
    printf("Actual length:   %d\n", slen(s6));

    printf("Expected string: second\n");
    printf("Actual string:   ");
    printString(s6);
    printf("\n");


    // Test 7: Compare two strings
    printf("\nTest 7: Compare two strings\n");
    string s7 = screate();
    scopy(s7, "hello");
    string s8 = screate();
    scopy(s8, "world this is a new string");
    printf("Comparing 'hello' and 'world': %d\n", scmp(s7, s8));

    string s9 = screate();
    scopy(s9, "hello");
    printf("Comparing 'hello' and 'hello': %d\n", scmp(s7, s9));

    // Test 8: Concatenate two strings
    printf("\nTest 8: Concatenate two strings\n");
    string s10 = screate();
    scopy(s10, "Hello, ");
    printf("First string: ");
    printString(s10);
    printf("\n");
    string s11 = screate();
    scopy(s11, "world this is a new string!");
    printf("Second string: ");
    printString(s11);
    printf("\n");
    scat(s10, s11);
    printf("Concatenated string: ");
    printString(s10);
    printf("\n");

    // Test 9: Convert string to uppercase
    printf("\nTest 9: Convert string to uppercase\n");
    string s12 = screate();
    scopy(s12, "Hello, World AAAHHHHH. lowercase!");
    string s13 = sToUpper(s12);
    printf("Uppercase string: ");
    printString(s13);
    printf("\n");

    // Test 10: Convert string to lowercase
    printf("\nTest 10: Convert string to lowercase\n");
    string s14 = screate();
    scopy(s14, "Hello, World AAAHHHHH. UPPERCASE!");
    string s15 = sToLower(s14);
    printf("Lowercase string: ");
    printString(s15);
    printf("\n"); 

    return 0;
}