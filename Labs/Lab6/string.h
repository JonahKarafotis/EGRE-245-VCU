#define MAX_STRING_SIZE 128

typedef struct stringType *string;

struct stringType {
    int length;
    char data[MAX_STRING_SIZE];
};

// Lab 5 Implimentation of string.h
string screate(); // returns a newly created string
void scopy(string s,char *c);// copies a null-terminated C string c to s
int slen(string); // returns the length of a string
void printString(string); // prints a string to standard output w/o newline

// Lab 6 Implementation of string.h
int scmp(string s1, string s2); // compares if two strings are equal (returns 0 if equal, returns > 0 if ascii value of s1 > ascii value of s2, returns < 0 if ascii value of s1 < ascii value of s2)
string scat(string dest, string src); // Concatenates src to the end of dest and returns a pointer to the
string sToUpper(string); // converts all lowercase letters in string to upper case and produces a new string with the result
string sToLower(string); // converts all uppercase letters in string to lower case and produces a new string with the result
