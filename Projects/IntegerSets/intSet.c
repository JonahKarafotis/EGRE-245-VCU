/* Project 3: Integer Sets
 * Name: Jonah Karafotis
 */

#include "intSet.h"
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Starting array size; add() grows it when all slots are occupied. */
#define INITIAL_CAPACITY 4

/* intSet is a pointer to this structure, as declared in intSet.h.
 * Keeping the fields here hides the storage details from other files.
 * Only items[0] through items[count - 1] are part of the set.
 */
struct intsetType {
    int *items;     // Points to a separately allocated array of integers.
    int count;      // Number of values currently stored.
    int capacity;   // Number of array slots currently allocated.
};

intSet createSet(void)
{
    /* Allocate the structure, not just a pointer. sizeof *s measures the
     * pointed-to structure without actually dereferencing s.
     */
    intSet s = malloc(sizeof *s);

    // malloc returns NULL if it cannot allocate the requested memory.
    if (s == NULL) {
        fprintf(stderr, "error: unable to allocate set\n");
        exit(EXIT_FAILURE);
    }

    /* Allocate the integer array separately. sizeof *s->items is sizeof(int).
     * The -> operator accesses a field through a pointer to a structure.
     */
    s->items = malloc(INITIAL_CAPACITY * sizeof *s->items);

    if (s->items == NULL) {
        free(s); // Release the structure if its array could not be allocated.
        fprintf(stderr, "error: unable to allocate set array\n");
        exit(EXIT_FAILURE);
    }

    // The array has space available, but no values have been added yet.
    s->count = 0;
    s->capacity = INITIAL_CAPACITY;

    return s; // Give the caller the pointer; the allocations outlive this function.
}

void destroySet(intSet s)
{
    if (s == NULL) {
        return;
    }

    // Free the array first, while s still exists and holds its address.
    free(s->items);
    free(s); // Then free the structure itself; the caller must not use it again.
}

void clear(intSet s) {
    // Forget all stored values without freeing or shrinking the array.
    s->count = 0;
}

int card(const intSet s) {
    // Cardinality means the number of distinct values in the set.
    return s->count;
}

bool equals(const intSet s1, const intSet s2) {
    // Different counts guarantee inequality. Otherwise, check that every
    // value in s1 also appears in s2; neither input is modified.
    if (s1->count != s2->count) {
        return false;
    }

    for (int i = 0; i < s1->count; i++) {
        if (!contains(s2, s1->items[i])) {
            return false;
        }
    }

    return true;
}

bool contains(const intSet s, int item) {
    // Search only the occupied slots; unused capacity is not part of the set.
    for (int i = 0; i < s->count; i++) {
        if (s->items[i] == item) {
            return true;
        }
    }
    return false;
}

int largest(const intSet s) {
    // Scan the occupied slots, keeping the largest value seen so far.
    if (s->count == 0) {
        fprintf(stderr, "error: largest called on an empty set\n");
        exit(EXIT_FAILURE);
    }
    // Start with an actual member so negative values are handled correctly.
    int max = s->items[0];
    for (int i = 1; i < s->count; i++) {
        if (s->items[i] > max) {
            max = s->items[i];
        }
    }
    return max;
}

int smallest(const intSet s) {
    // Scan the occupied slots, keeping the smallest value seen so far.
    if (s->count == 0) {
        fprintf(stderr, "error: smallest called on an empty set\n");
        exit(EXIT_FAILURE);
    }
    // Start with an actual member rather than assuming a minimum of zero.
    int min = s->items[0];
    for (int i = 1; i < s->count; i++) {
        if (s->items[i] < min) {
            min = s->items[i];
        }
    }
    return min;
}

void add(intSet s, int item) {
    // A set cannot contain duplicates, so ignore an item already present.
    if (contains(s, item)) {
        return; // Item already exists in the set
    }

    if (s->count == s->capacity) {
        // Grow without overflowing the capacity or allocation size.
        size_t limit = SIZE_MAX / sizeof *s->items;
        if (limit > INT_MAX) limit = INT_MAX;
        if ((size_t)s->capacity >= limit) {
            fprintf(stderr, "error: set capacity limit reached\n");
            exit(EXIT_FAILURE);
        }
        int capacity = (size_t)s->capacity > limit / 2
            ? (int)limit : s->capacity * 2;
        int *items = realloc(s->items, (size_t)capacity * sizeof *s->items);
        if (items == NULL) {
            fprintf(stderr, "error: unable to allocate set array\n");
            exit(EXIT_FAILURE);
        }
        s->items = items;
        s->capacity = capacity;
    }

    // Shift larger values right so the occupied slots stay in ascending order.
    int pos = s->count;
    while (pos > 0 && s->items[pos - 1] > item) {
        s->items[pos] = s->items[pos - 1];
        pos--;
    }
    s->items[pos] = item;
    s->count++;
}

void remove_(intSet s, int item) {
    // Find the item, shift later values left to close the gap, and reduce count.
    // If no slot matches, leave the set unchanged.
    for (int i = 0; i < s->count; i++) {
        if (s->items[i] == item) {
            for (int j = i; j < s->count - 1; j++) {
                s->items[j] = s->items[j + 1];
            }
            s->count--;
            return;
        }
    }
}

intSet union_(const intSet s1, const intSet s2) {
    // Build a new set from both inputs. add() prevents repeated values.
    // The caller owns the result and should eventually destroySet(result).
    intSet result = createSet();

    for (int i = 0; i < s1->count; i++) {
        add(result, s1->items[i]);
    }

    for (int i = 0; i < s2->count; i++) {
        add(result, s2->items[i]);
    }

    return result;
}

intSet intersect(const intSet s1, const intSet s2) {
    // Build a new set containing only values present in both input sets.
    intSet result = createSet();

    for (int i = 0; i < s1->count; i++) {
        if (contains(s2, s1->items[i])) {
            add(result, s1->items[i]);
        }
    }

    return result;
}

intSet diff(const intSet s1, const intSet s2) {
    // Build s1 minus s2: keep values from s1 that do not appear in s2.
    // Reversing the arguments can produce a different result.
    intSet result = createSet();

    for (int i = 0; i < s1->count; i++) {
        if (!contains(s2, s1->items[i])) {
            add(result, s1->items[i]);
        }
    }

    return result;
}

bool isEmpty(const intSet s) {
    // An allocated set can still be empty: it has no occupied slots.
    return s->count == 0;
}

int *toArray(const intSet s) {
    // Allocate an independent copy so the caller can use it without changing
    // the set. The caller must free the returned array.
    // An empty set has no elements to copy; free(NULL) is safe for the caller.
    if (s->count == 0) return NULL;
    int *array = malloc((size_t)s->count * sizeof *array);
    if (array == NULL) {
        fprintf(stderr, "error: unable to allocate array\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < s->count; i++) {
        array[i] = s->items[i];
    }

    return array;
}

char *toString(const intSet s) {
    // Create a separately allocated C string; the caller must free it.
    if (s->count == 0) {
        char *emptySet = malloc(3 * sizeof *emptySet);
        if (emptySet == NULL) {
            fprintf(stderr, "error: unable to allocate string\n");
            exit(EXIT_FAILURE);
        }
        emptySet[0] = '{';
        emptySet[1] = '}';
        emptySet[2] = '\0'; // Every C string ends with a null terminator.
        return emptySet;
    }

    // snprintf(NULL, 0, ...) counts the number of characters needed for each
    // integer, including a minus sign when necessary, without writing output.
    size_t length = 2; // For '{' and '}'
    for (int i = 0; i < s->count; i++) {
        length += (size_t)snprintf(NULL, 0, "%d", s->items[i]) + 1;
    }
    length--; // No comma after the last number.

    char *str = malloc(length + 1); // +1 for null terminator
    if (str == NULL) {
        fprintf(stderr, "error: unable to allocate string\n");
        exit(EXIT_FAILURE);
    }

    str[0] = '{';
    size_t pos = 1;
    for (int i = 0; i < s->count; i++) {
        // Write each number into the remaining space in the string.
        pos += (size_t)snprintf(str + pos, length + 1 - pos, "%d", s->items[i]);
        if (i < s->count - 1) {
            str[pos++] = ',';
        }
    }
    str[pos++] = '}';
    str[pos] = '\0';

    return str;
}
