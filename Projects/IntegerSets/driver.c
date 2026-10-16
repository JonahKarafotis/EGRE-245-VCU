/* Local tests for Project 3. Submit intSet.c only, not this driver.
 * Build: cc -std=c11 -Wall -Wextra intSet.c driver.c -o driver
 * Run: ./driver
 * Error cases (run separately because the ADT must exit):
 *   ./driver --empty-largest
 *   ./driver --empty-smallest
 * Each error case must terminate and print a message containing "error".
 */
#include "intSet.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int checks = 0;
static int failures = 0;

/* Keep testing after a failed check so one run reveals multiple problems. */
static void check(bool condition, const char *description)
{
    checks++;
    printf("%s: %s\n", condition ? "PASS" : "FAIL", description);
    if (!condition) failures++;
}

static intSet makeSet(const int values[], int count)
{
    intSet s = createSet();
    for (int i = 0; i < count; i++) add(s, values[i]);
    return s;
}

/* Check membership independently of output ordering and formatting. */
static bool hasValues(intSet s, const int expected[], int count)
{
    if (card(s) != count) return false;
    for (int i = 0; i < count; i++) {
        if (!contains(s, expected[i])) return false;
    }
    return true;
}

static void checkArray(intSet s, const int expected[], int count,
                       const char *description)
{
    int *values = toArray(s);
    bool matches = card(s) == count && (count == 0 || values != NULL);
    if (matches) {
        for (int i = 0; i < count; i++) {
            if (values[i] != expected[i]) matches = false;
        }
    }
    check(matches, description);
    free(values);
}

static void checkString(intSet s, const char *expected, const char *description)
{
    char *text = toString(s);
    bool matches = text != NULL && strcmp(text, expected) == 0;
    check(matches, description);
    if (!matches) printf("  Expected: %s\n  Actual:   %s\n", expected,
                         text != NULL ? text : "(NULL)");
    free(text);
}

int main(int argc, char *argv[])
{
    /* These modes let an external process verify termination and error output. */
    if (argc == 2 && (strcmp(argv[1], "--empty-largest") == 0 ||
                      strcmp(argv[1], "--empty-smallest") == 0)) {
        intSet empty = createSet();
        if (strcmp(argv[1], "--empty-largest") == 0) (void)largest(empty);
        else (void)smallest(empty);
        destroySet(empty);
        puts("FAIL: empty-set query returned instead of terminating");
        return EXIT_SUCCESS;
    }
    if (argc != 1) {
        fprintf(stderr, "Usage: %s [--empty-largest|--empty-smallest]\n", argv[0]);
        return EXIT_FAILURE;
    }

    intSet s = createSet();
    intSet empty = createSet();
    check(isEmpty(s) && card(s) == 0, "new set is empty with cardinality zero");
    check(equals(s, empty), "two empty sets are equal");
    check(!contains(s, 9), "empty set contains no values");
    remove_(s, 9);
    check(isEmpty(s), "removing from an empty set does nothing");
    checkString(s, "{}", "empty string representation");
    checkArray(s, NULL, 0, "empty array conversion (no elements to read)");

    add(s, 9); add(s, -52); add(s, 17); add(s, 9);
    const int ordered[] = {-52, 9, 17};
    check(hasValues(s, ordered, 3), "add accepts negatives and ignores duplicates");
    check(!isEmpty(s) && !contains(s, 100), "nonempty and missing-value queries");
    check(smallest(s) == -52 && largest(s) == 17, "minimum and maximum");
    checkArray(s, ordered, 3, "array values are ascending");
    checkString(s, "{-52,9,17}", "string is ascending with no spaces");
    intSet same = makeSet(ordered, 3);
    check(equals(s, same) && equals(same, s), "equality ignores insertion order");
    intSet different = makeSet((int[]){-52, 9, 18}, 3);
    check(!equals(s, different) && !equals(s, empty), "unequal sets are rejected");

    int *copy = toArray(s);
    if (copy != NULL) copy[0] = 12345;
    check(hasValues(s, ordered, 3), "changing returned array leaves set unchanged");
    free(copy);

    remove_(s, 9);
    check(hasValues(s, (int[]){-52, 17}, 2), "remove middle value");
    remove_(s, 999);
    check(card(s) == 2, "remove missing value does nothing");
    remove_(s, -52); remove_(s, 17);
    check(isEmpty(s), "remove first and last remaining values");

    /* Descending insertion forces growth and challenges ascending output. */
    for (int i = 100; i >= -100; i--) add(s, i);
    bool retained = card(s) == 201;
    int expected[201];
    for (int i = 0; i < 201; i++) {
        expected[i] = i - 100;
        if (!contains(s, expected[i])) retained = false;
    }
    check(retained, "array growth preserves all 201 values");
    checkArray(s, expected, 201, "ascending array after repeated growth");
    clear(s); clear(s);
    check(isEmpty(s) && card(s) == 0 && !contains(s, 100), "clear is repeatable");
    add(s, INT_MAX); add(s, INT_MIN); add(s, 0);
    check(smallest(s) == INT_MIN && largest(s) == INT_MAX, "integer boundary values");
    checkArray(s, (int[]){INT_MIN, 0, INT_MAX}, 3, "boundary values sorted correctly");
    char boundaryText[128];
    snprintf(boundaryText, sizeof boundaryText, "{%d,0,%d}", INT_MIN, INT_MAX);
    checkString(s, boundaryText, "string handles full integer range");
    clear(s); add(s, -7);
    checkString(s, "{-7}", "cleared set can be reused as a singleton");
    check(smallest(s) == -7 && largest(s) == -7, "singleton minimum and maximum");

    intSet a = makeSet((int[]){5, 1, 3}, 3);
    intSet b = makeSet((int[]){4, 5, 3}, 3);
    intSet u = union_(a, b);
    intSet in = intersect(a, b);
    intSet ab = diff(a, b);
    intSet ba = diff(b, a);
    check(hasValues(u, (int[]){1, 3, 4, 5}, 4), "union membership");
    checkArray(u, (int[]){1, 3, 4, 5}, 4, "union ascending output");
    check(hasValues(in, (int[]){3, 5}, 2), "intersection membership");
    check(hasValues(ab, (int[]){1}, 1), "A minus B");
    check(hasValues(ba, (int[]){4}, 1), "B minus A");
    check(hasValues(a, (int[]){1, 3, 5}, 3) &&
          hasValues(b, (int[]){3, 4, 5}, 3), "set operations preserve input values");
    add(u, 99);
    check(!contains(a, 99) && !contains(b, 99), "operation result is independent");

    intSet results[] = {union_(a, empty), union_(empty, a),
        intersect(a, empty), intersect(empty, a), diff(a, empty),
        diff(empty, a), union_(a, a), intersect(a, a), diff(a, a), intersect(ab, ba)};
    check(equals(results[0], a) && equals(results[1], a), "union with empty on either side");
    check(isEmpty(results[2]) && isEmpty(results[3]), "intersection with empty on either side");
    check(equals(results[4], a) && isEmpty(results[5]), "difference with empty on either side");
    check(equals(results[6], a) && equals(results[7], a) && isEmpty(results[8]),
          "operations with the same input set");
    check(isEmpty(results[9]), "disjoint sets have empty intersection");
    for (unsigned i = 0; i < sizeof results / sizeof results[0]; i++) destroySet(results[i]);
    destroySet(a); destroySet(b); destroySet(u); destroySet(in);
    destroySet(ab); destroySet(ba); destroySet(s); destroySet(empty);
    destroySet(same); destroySet(different);

    printf("\n%d/%d checks passed; %d failed.\n", checks - failures, checks, failures);
    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
