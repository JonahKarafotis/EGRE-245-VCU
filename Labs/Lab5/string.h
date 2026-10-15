#define MAX_STRING_SIZE 128

typedef struct stringType *string;

struct stringType {
    int length;
    char data[MAX_STRING_SIZE];
};

string screate(); // returns a newly created string
void scopy(string s,char *c);// copies a null-terminated C string c to s
int slen(string); // returns the length of a string
void printString(string); // prints a string to standard output w/o newline