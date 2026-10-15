#include <stdbool.h>
#ifndef RAT_H
#define RAT_H

struct rtype {
	int n,d; // numerator, denominator
};

typedef struct rtype *rat;
rat createRat(int,int); // creates and returns a rat number
rat norm(const rat); // returns normalized number
rat reduce(const rat); // reduces number to lowest terms
int cmp(const rat r1, const rat r2); // returns -1 if r1 < r2,
// 0 if r1 == r2,
// 1 if r1 > r2
rat add(const rat r1, const rat r2); // r1 + r2
rat sub(const rat r1, const rat r2); // r1 - r2
rat mul(const rat r1, const rat r2); // r1 * r2
rat divide(const rat r1, const rat r2); // r1 / r2
rat inverse(const rat); // returns argument inverted
bool wellFormed(const rat); // returns true if number is legal, false otherwise
char *toString(const rat); // returns string representation of argument
#endif
