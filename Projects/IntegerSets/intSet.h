#include  <stdbool.h>
#ifndef INTSET_H
#define INTSET_H

typedef struct intsetType *intSet;

// returns an intSet created at runtime using malloc
intSet createSet();

// frees up the memory associated with its argument
void destroySet(intSet);

// clears set to empty (does not free up memory)
void clear(intSet);

// returns the cardinality of the set
int card(const intSet);

// returns true if 2 sets are equal, false otherwise; two sets are equal if they contain
// all of the same values
bool equals(const intSet,const intSet); 

// retuns true if argument is in set, false otherwise
bool contains(const intSet,int);

// returns the largest item in the set; prints error messageand exits if set is empty
int largest(const intSet);

// returns the smallest item in the set; prints error message and exits if set is empty
int smallest(const intSet);

// adds item to s or does nothing if it is in set
void add(intSet s,int item); 

// removes item from s or does nothing if it is not in set
void remove_(intSet s,int item); 

// set union
intSet union_(const intSet, const intSet); 

// set intersection
intSet intersect(const intSet, const intSet); 

// set difference, i.e. s1 - s2
intSet diff(const intSet s1, const intSet s2); 

// returns true if the set is empty, false otherwise
bool isEmpty(const intSet); 

// returns an array representation of the set in ascending order
int *toArray(const intSet); 

// returns a string representation of the set in ascending order; e.g. an empty set returns "{}", 
// and a non-empty set containing the values 9,-52,17 would return the string "{-52,9,17}"
char *toString(const intSet); 

#endif