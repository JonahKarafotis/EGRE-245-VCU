#include <stdio.h>
#include <stdlib.h>
#include "rat.h"

rat createRat(int n, int d) {
    if (d == 0) {
        fprintf(stderr, "Denominator cannot be zero\n");
        exit(EXIT_FAILURE);
    }
    rat r = malloc(sizeof(struct rtype));
    if (r == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    r->n = n;
    r->d = d;
    return r;
}

bool wellFormed(const rat r) {
    return r != NULL && r->d != 0;
}

rat norm(const rat r) {

    if (wellFormed(r) == false) {
        fprintf(stderr, "The value is not well-formed\n");
        exit(EXIT_FAILURE);
    }
    if (r->n == 0) {
        r->d = 1;
        return r;
    }
    if (r->d < 0) {
        r->n = -r->n;
        r->d = -r->d;
    }
    return r;
}

rat reduce(const rat r) {
    if (wellFormed(r) == false) {
        fprintf(stderr, "The value is not well-formed\n");
        exit(EXIT_FAILURE);
    }
    int a = r->n;
    int b = r->d;
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    r->n /= a;
    r->d /= a;
    norm(r);
    return r;
}

int cmp(const rat r1, const rat r2) {
    if (wellFormed(r1) == false || wellFormed(r2) == false) {
        fprintf(stderr, "One or both values are not well-formed\n");
        exit(EXIT_FAILURE);
    }
    reduce(r1);
    reduce(r2);
    int lhs = r1->n * r2->d;
    int rhs = r2->n * r1->d;
    if (lhs < rhs) return -1;
    if (lhs > rhs) return 1;
    return 0;
}

rat add(const rat r1, const rat r2) {
    if (wellFormed(r1) == false || wellFormed(r2) == false) {
        fprintf(stderr, "One or both values are not well-formed\n");
        exit(EXIT_FAILURE);
    }
    int n = r1->n * r2->d + r2->n * r1->d;
    int d = r1->d * r2->d;
    return reduce(createRat(n, d));
}

rat sub(const rat r1, const rat r2) {
    if (wellFormed(r1) == false || wellFormed(r2) == false) {
        fprintf(stderr, "One or both values are not well-formed\n");
        exit(EXIT_FAILURE);
    }
    int n = r1->n * r2->d - r2->n * r1->d;
    int d = r1->d * r2->d;
    return reduce(createRat(n, d));
}

rat mul(const rat r1, const rat r2) {
    if (wellFormed(r1) == false || wellFormed(r2) == false) {
        fprintf(stderr, "One or both values are not well-formed\n");
        exit(EXIT_FAILURE);
    }
    int n = r1->n * r2->n;
    int d = r1->d * r2->d;
    return reduce(createRat(n, d));
}

rat divide(const rat r1, const rat r2) {
    if (wellFormed(r1) == false || wellFormed(r2) == false) {
        fprintf(stderr, "One or both values are not well-formed\n");
        exit(EXIT_FAILURE);
    }
    if (r2->n == 0) {
        fprintf(stderr, "Cannot divide by zero\n");
        exit(EXIT_FAILURE);
    }
    int n = r1->n * r2->d;
    int d = r1->d * r2->n;
    return reduce(createRat(n, d));
}

rat inverse(const rat r) {
    if (wellFormed(r) == false) {
        fprintf(stderr, "The value is not well-formed\n");
        exit(EXIT_FAILURE);
    }
    if (r->n == 0) {
        fprintf(stderr, "Cannot invert a zero numerator\n");
        exit(EXIT_FAILURE);
    }
    return reduce(createRat(r->d, r->n));
}


char *toString(const rat r) {
    char *str = malloc(24);

    if (str == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // this just allows us to store the rational number in the form of 
    // "numerator/denominator" and return it as a string
    snprintf(str, 24, "%d/%d", r->n, r->d);

    return str;
}
