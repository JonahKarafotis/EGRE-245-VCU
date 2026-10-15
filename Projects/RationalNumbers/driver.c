
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include "rat.h"

// Used for tests that intentionally cause the program to exit.
// Supported on macOS and Linux.
#ifndef _WIN32
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#endif

static int passed = 0;
static int failed = 0;

// -----------------------------------------------------
// Test reporting
// -----------------------------------------------------

void check(const char *name, bool success) {
    if (success) {
        printf("[PASS] %s\n", name);
        passed++;
    } else {
        printf("[FAIL] %s\n", name);
        failed++;
    }
}

// -----------------------------------------------------
// 1. createRat and wellFormed
// -----------------------------------------------------

void testCreateRat(int n, int d) {
    rat r = createRat(n, d);

    char name[100];
    snprintf(name, sizeof(name),
             "createRat(%d, %d)", n, d);

    check(name, r != NULL &&
                r->n == n &&
                r->d == d);

    free(r);
}

void testWellFormed(void) {
    rat r = createRat(2, 3);

    check("wellFormed(2/3)", wellFormed(r));
    check("wellFormed(NULL)", !wellFormed(NULL));

    r->d = 0;
    check("wellFormed(2/0)", !wellFormed(r));

    r->n = 0;
    check("wellFormed(0/0)", !wellFormed(r));

    free(r);
}

// -----------------------------------------------------
// 2. toString
// -----------------------------------------------------

void testToString(int n, int d, const char *expected) {
    // createRat rejects zero denominators.
    // Create a legal fraction first, then change it.
    rat r = createRat(n, d == 0 ? 1 : d);
    r->d = d;

    char *actual = toString(r);

    char name[128];
    snprintf(name, sizeof(name),
             "toString(%d/%d)", n, d);

    bool success = actual != NULL &&
                   strcmp(actual, expected) == 0;

    check(name, success);

    if (!success) {
        printf("       Expected: \"%s\"\n", expected);
        printf("       Actual:   \"%s\"\n",
               actual == NULL ? "(null)" : actual);
    }

    free(actual);
    free(r);
}

// -----------------------------------------------------
// 3. norm and reduce
// -----------------------------------------------------

typedef struct {
    int n;
    int d;
    int expectedN;
    int expectedD;
} UnaryCase;

void testUnary(
    const char *name,
    rat (*operation)(const rat),
    const UnaryCase cases[],
    size_t count
) {
    for (size_t i = 0; i < count; i++) {
        rat r = createRat(cases[i].n, cases[i].d);
        rat result = operation(r);

        char label[128];
        snprintf(label, sizeof(label),
                 "%s(%d/%d)", name,
                 cases[i].n, cases[i].d);

        bool success =
            result == r &&
            result->n == cases[i].expectedN &&
            result->d == cases[i].expectedD;

        check(label, success);

        if (!success) {
            printf("       Expected: %d/%d\n",
                   cases[i].expectedN,
                   cases[i].expectedD);

            printf("       Actual:   %d/%d\n",
                   result->n, result->d);
        }

        // norm and reduce should return the same object.
        if (result != r) free(result);
        free(r);
    }
}

// -----------------------------------------------------
// 4. cmp
// -----------------------------------------------------

typedef struct {
    int n1, d1;
    int n2, d2;
    int expected;
} CmpCase;

void testComparisons(
    const CmpCase cases[],
    size_t count
) {
    for (size_t i = 0; i < count; i++) {
        rat a = createRat(cases[i].n1, cases[i].d1);
        rat b = createRat(cases[i].n2, cases[i].d2);

        int actual = cmp(a, b);

        char name[128];
        snprintf(name, sizeof(name),
                 "cmp(%d/%d, %d/%d)",
                 cases[i].n1, cases[i].d1,
                 cases[i].n2, cases[i].d2);

        check(name, actual == cases[i].expected);

        if (actual != cases[i].expected) {
            printf("       Expected: %d\n",
                   cases[i].expected);
            printf("       Actual:   %d\n", actual);
        }

        free(a);
        free(b);
    }
}

// -----------------------------------------------------
// 5. Arithmetic operations
// -----------------------------------------------------

typedef struct {
    int n1, d1;
    int n2, d2;
    int expectedN, expectedD;
} MathCase;

void testMath(
    const char *name,
    rat (*operation)(const rat, const rat),
    const MathCase cases[],
    size_t count
) {
    for (size_t i = 0; i < count; i++) {
        rat a = createRat(cases[i].n1, cases[i].d1);
        rat b = createRat(cases[i].n2, cases[i].d2);

        rat result = operation(a, b);

        // Compare mathematical values instead of requiring
        // an identical representation.
        // The test values are small enough for long long.
        bool valid = result != NULL && wellFormed(result);

        bool equivalent = valid &&
            (long long)result->n * cases[i].expectedD ==
            (long long)cases[i].expectedN * result->d;

        // All arithmetic results should be normalized.
        bool normalized = valid &&
            result->d > 0 &&
            (result->n != 0 || result->d == 1);

        char label[128];
        snprintf(label, sizeof(label),
                 "%s(%d/%d, %d/%d)",
                 name,
                 cases[i].n1, cases[i].d1,
                 cases[i].n2, cases[i].d2);

        bool success = equivalent && normalized;

        check(label, success);

        if (!success) {
            printf("       Expected value: %d/%d\n",
                   cases[i].expectedN,
                   cases[i].expectedD);

            if (result != NULL) {
                printf("       Actual:         %d/%d\n",
                       result->n, result->d);
            } else {
                printf("       Actual:         NULL\n");
            }
        }

        if (result != a && result != b) free(result);
        free(a);
        free(b);
    }
}

// -----------------------------------------------------
// 6. inverse
// -----------------------------------------------------

void testInverse(
    const UnaryCase cases[],
    size_t count
) {
    for (size_t i = 0; i < count; i++) {
        rat r = createRat(cases[i].n, cases[i].d);
        rat result = inverse(r);

        bool success =
            result != NULL &&
            result->n == cases[i].expectedN &&
            result->d == cases[i].expectedD;

        char label[128];
        snprintf(label, sizeof(label),
                 "inverse(%d/%d)",
                 cases[i].n, cases[i].d);

        check(label, success);

        if (!success && result != NULL) {
            printf("       Expected: %d/%d\n",
                   cases[i].expectedN,
                   cases[i].expectedD);
            printf("       Actual:   %d/%d\n",
                   result->n, result->d);
        }

        if (result != r) free(result);
        free(r);
    }
}

// -----------------------------------------------------
// 7. Tests that SHOULD terminate with an error
// -----------------------------------------------------
// These run in child processes so exit(EXIT_FAILURE)
// does not terminate the entire test program.
//
// This section works on macOS and Linux.

#ifndef _WIN32

void runInvalidOperation(int id) {
    rat a = createRat(1, 2);
    rat b = createRat(3, 4);

    switch (id) {
        case 0:
            createRat(1, 0);
            break;

        case 1:
            a->d = 0;
            norm(a);
            break;

        case 2:
            a->d = 0;
            reduce(a);
            break;

        case 3:
            a->d = 0;
            cmp(a, b);
            break;

        case 4:
            a->d = 0;
            add(a, b);
            break;

        case 5:
            a->d = 0;
            sub(a, b);
            break;

        case 6:
            a->d = 0;
            mul(a, b);
            break;

        case 7:
            a->d = 0;
            divide(a, b);
            break;

        case 8:
            b->n = 0;
            divide(a, b);
            break;

        case 9:
            a->n = 0;
            inverse(a);
            break;

        case 10:
            a->d = 0;
            inverse(a);
            break;
    }
}

void testInvalidOperations(void) {
    const char *names[] = {
        "createRat(1, 0) rejects zero denominator",
        "norm rejects malformed rational",
        "reduce rejects malformed rational",
        "cmp rejects malformed rational",
        "add rejects malformed rational",
        "sub rejects malformed rational",
        "mul rejects malformed rational",
        "divide rejects malformed rational",
        "divide rejects division by zero",
        "inverse rejects zero numerator",
        "inverse rejects malformed rational"
    };

    for (int i = 0; i < 11; i++) {
        // Prevent duplicated buffered output after fork.
        fflush(NULL);

        pid_t pid = fork();

        if (pid < 0) {
            check(names[i], false);
            continue;
        }

        if (pid == 0) {
            // Suppress expected error messages from child.
            freopen("/dev/null", "w", stderr);

            runInvalidOperation(i);

            // Reaching here means the function did NOT
            // terminate as expected.
            _exit(0);
        }

        int status = 0;
        pid_t waited = waitpid(pid, &status, 0);

        bool success =
            waited == pid &&
            WIFEXITED(status) &&
            WEXITSTATUS(status) != 0;

        check(names[i], success);
    }
}

#endif

// =====================================================
// MAIN TEST DRIVER
// =====================================================

int main(void) {

    printf("\n====================================\n");
    printf("      RATIONAL NUMBER ADT TESTS\n");
    printf("====================================\n");

    // -------------------------------------------------
    // CREATE RAT
    // -------------------------------------------------

    printf("\n--- createRat ---\n");

    testCreateRat(2, 3);
    testCreateRat(-2, 3);
    testCreateRat(2, -3);
    testCreateRat(-2, -3);
    testCreateRat(0, 5);
    testCreateRat(7, 1);

    // -------------------------------------------------
    // WELL FORMED
    // -------------------------------------------------

    printf("\n--- wellFormed ---\n");
    testWellFormed();

    // -------------------------------------------------
    // TO STRING
    // -------------------------------------------------

    printf("\n--- toString ---\n");

    testToString(2, 3, "2/3");
    testToString(-2, 3, "-2/3");
    testToString(2, -3, "2/-3");
    testToString(-2, -3, "-2/-3");
    testToString(0, 1, "0/1");
    testToString(0, -5, "0/-5");
    testToString(5, 0, "5/0");
    testToString(0, 0, "0/0");
    testToString(123456, 789012, "123456/789012");

    // Test very large integers.
    char expectedLarge[80];
    snprintf(expectedLarge, sizeof(expectedLarge),
             "%d/%d", INT_MIN, INT_MIN);

    testToString(INT_MIN, INT_MIN, expectedLarge);

    // -------------------------------------------------
    // NORMALIZATION
    // -------------------------------------------------

    printf("\n--- norm ---\n");

    const UnaryCase normCases[] = {
        { 2,  3,  2, 3 },
        { 2, -3, -2, 3 },
        {-2,  3, -2, 3 },
        {-2, -3,  2, 3 },
        { 0,  5,  0, 1 },
        { 0, -5,  0, 1 },
        { 6,  8,  6, 8 }
    };

    testUnary("norm", norm,
              normCases,
              sizeof(normCases) / sizeof(normCases[0]));

    // -------------------------------------------------
    // REDUCTION
    // -------------------------------------------------

    printf("\n--- reduce ---\n");

    const UnaryCase reduceCases[] = {
        { 6,   8,   3, 4 },
        {-6,   8,  -3, 4 },
        { 6,  -8,  -3, 4 },
        {-6,  -8,   3, 4 },
        { 0,   8,   0, 1 },
        { 0,  -8,   0, 1 },
        { 8,   8,   1, 1 },
        { 8,   1,   8, 1 },
        {-8,   1,  -8, 1 },
        { 1,  -1,  -1, 1 },
        { 3,   7,   3, 7 },
        {18,  24,   3, 4 },
        {15,   5,   3, 1 },
        {100, 250,  2, 5 }
    };

    testUnary("reduce", reduce,
              reduceCases,
              sizeof(reduceCases) / sizeof(reduceCases[0]));

    // -------------------------------------------------
    // COMPARISON
    // -------------------------------------------------

    printf("\n--- cmp ---\n");

    const CmpCase cmpCases[] = {
        { 1, 2,   1, 2,   0 },
        { 1, 2,   2, 4,   0 },
        { 1, 2,   2, 3,  -1 },
        { 2, 3,   1, 2,   1 },
        {-1, 2,   1, 3,  -1 },
        {-1, 2,  -1, 3,  -1 },
        { 0, 5,  -1, 2,   1 },
        { 0, 5,   0,-3,   0 },
        { 1,-2,   1, 3,  -1 },
        {-1,-2,   1, 3,   1 },
        { 1,-2,  -1, 2,   0 },
        { 3,-4,   1, 2,  -1 },
        { 5, 6,   4, 5,   1 }
    };

    testComparisons(cmpCases,
                    sizeof(cmpCases) / sizeof(cmpCases[0]));

    // -------------------------------------------------
    // ADDITION
    // -------------------------------------------------

    printf("\n--- add ---\n");

    const MathCase addCases[] = {
        { 1, 2,   1, 3,   5, 6 },
        { 1, 2,   1, 2,   1, 1 },
        { 2, 3,  -5, 6,  -1, 6 },
        { 2,-3,   5, 6,   1, 6 },
        {-2,-3,   1, 6,   5, 6 },
        { 0, 5,   2, 3,   2, 3 },
        {-3, 7,   3, 7,   0, 1 },
        { 0,-5,   0, 3,   0, 1 },
        { 2, 4,   2, 4,   1, 1 },
        { 7,-9,   2, 3,  -1, 9 }
    };

    testMath("add", add,
             addCases,
             sizeof(addCases) / sizeof(addCases[0]));

    // -------------------------------------------------
    // SUBTRACTION
    // -------------------------------------------------

    printf("\n--- sub ---\n");

    const MathCase subCases[] = {
        { 1, 2,   1, 3,   1, 6 },
        { 1, 2,   3, 4,  -1, 4 },
        { 1, 2,   1, 2,   0, 1 },
        {-2, 3,   1, 3,  -1, 1 },
        { 3,-4,   1,-4,  -1, 2 },
        { 0, 9,   2, 3,  -2, 3 },
        {-3, 5,  -3, 5,   0, 1 },
        { 1,-2,   1, 2,  -1, 1 },
        { 5, 6,  -1, 6,   1, 1 },
        { 0,-5,   0, 3,   0, 1 }
    };

    testMath("sub", sub,
             subCases,
             sizeof(subCases) / sizeof(subCases[0]));

    // -------------------------------------------------
    // MULTIPLICATION
    // -------------------------------------------------

    printf("\n--- mul ---\n");

    const MathCase mulCases[] = {
        { 1, 2,   2, 3,   1, 3 },
        {-2, 3,   3, 4,  -1, 2 },
        { 1,-2,   2, 3,  -1, 3 },
        {-2,-3,   3, 4,   1, 2 },
        { 0, 3,   5, 7,   0, 1 },
        { 7, 1,   1, 7,   1, 1 },
        {-2,-3,  -3, 4,  -1, 2 },
        { 3,-5,  -5,-6,  -1, 2 },
        { 0,-3,   2, 7,   0, 1 },
        { 4, 9,   9, 4,   1, 1 }
    };

    testMath("mul", mul,
             mulCases,
             sizeof(mulCases) / sizeof(mulCases[0]));

    // -------------------------------------------------
    // DIVISION
    // -------------------------------------------------

    printf("\n--- divide ---\n");

    const MathCase divideCases[] = {
        { 1, 2,   3, 4,   2, 3 },
        {-1, 2,   3, 4,  -2, 3 },
        { 1,-2,   3, 4,  -2, 3 },
        {-1,-2,  -3, 4,  -2, 3 },
        { 0, 7,   2, 3,   0, 1 },
        { 5, 6,   5, 6,   1, 1 },
        { 2,-3,  -4,-5,  -5, 6 },
        {-2, 3,   4,-5,   5, 6 },
        { 4, 5,   2, 5,   2, 1 },
        { 1, 7,   1, 7,   1, 1 }
    };

    testMath("divide", divide,
             divideCases,
             sizeof(divideCases) / sizeof(divideCases[0]));

    // -------------------------------------------------
    // INVERSE
    // -------------------------------------------------

    printf("\n--- inverse ---\n");

    const UnaryCase inverseCases[] = {
        { 2,  3,   3, 2 },
        {-2,  3,  -3, 2 },
        { 2, -3,  -3, 2 },
        {-2, -3,   3, 2 },
        { 1,  7,   7, 1 },
        { 7,  1,   1, 7 },
        { 5, -8,  -8, 5 }
    };

    testInverse(inverseCases,
                sizeof(inverseCases) / sizeof(inverseCases[0]));

    // -------------------------------------------------
    // INVALID OPERATIONS
    // -------------------------------------------------

    printf("\n--- Invalid operation tests ---\n");

#ifndef _WIN32
    testInvalidOperations();
#else
    printf("Skipped: requires macOS or Linux.\n");
#endif

    // -------------------------------------------------
    // FINAL RESULTS
    // -------------------------------------------------

    printf("\n====================================\n");
    printf("             RESULTS\n");
    printf("====================================\n");

    printf("Tests passed: %d\n", passed);
    printf("Tests failed: %d\n", failed);
    printf("Total tests:  %d\n", passed + failed);

    if (failed == 0) {
        printf("\nALL TESTS PASSED!\n");
    } else {
        printf("\nSome tests failed. Review the output.\n");
    }

    return failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
