/*
 * Programming Project #4 - Text Cross Referencing
 * Author: Jonah Karafotis
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORDS 10000
#define MAX_LINE_LENGTH 200

// Singly-linked list module, based on the supplied Canvas implementation.
struct node {
    int data;
    struct node *next;
};
typedef struct node *llist;

// Return the new head, or NULL on allocation failure; the old list is intact.
static llist InsertAtEnd(llist head, int value)
{
    llist newNode = malloc(sizeof *newNode);
    llist ptr;

    // Check for allocation failure
    if (newNode == NULL)
        return NULL;

    // Assign the value and set the next pointer to NULL
    newNode->data = value;
    newNode->next = NULL;

    // if the list is empty, return the new node as the head
    if (head == NULL)
        return newNode;

    // Traverse to the end of the list and append the new node
    ptr = head;
    while (ptr->next != NULL)
        ptr = ptr->next;
    ptr->next = newNode;
    return head;
}

static void FreeList(llist head)
{
    // Free each node in the list
    while (head != NULL) {
        llist next = head->next;
        free(head);
        head = next;
    }
}

// Each entry owns its word text and the nodes in its reference list. 
typedef struct {
    char word[MAX_LINE_LENGTH + 1];
    llist references;
    int last_line;
} Word;

typedef struct {
    Word *words;
    size_t unique_count;
    size_t total_count;
    int line_count;
} CrossReference;

// Return the matching entry, or NULL when the word has not been seen.
static Word *find_word(CrossReference *index, const char *word)
{
    size_t i;
    for (i = 0; i < index->unique_count; ++i) {
        if (strcmp(index->words[i].word, word) == 0)
            return &index->words[i];
    }
    return NULL;
}

// Count an occurrence and add its line number only if it is a new reference. 
static int record_word(CrossReference *index, const char *text)
{
    Word *word = find_word(index, text);
    llist updated;

    if (word == NULL) {
        if (index->unique_count == MAX_WORDS) {
            printf("Error: input exceeds %d unique words.\n", MAX_WORDS);
            return 0;
        }
        word = &index->words[index->unique_count++];
        strcpy(word->word, text);
        word->references = NULL;
        word->last_line = 0;
    }
    if (word->last_line != index->line_count) {
        updated = InsertAtEnd(word->references, index->line_count);
        if (updated == NULL) {
            printf("Error: unable to allocate a line reference.\n");
            return 0;
        }
        word->references = updated;
        word->last_line = index->line_count;
    }
    ++index->total_count;
    return 1;
}

// Normalize separators in this line only never retain pointers into it.
static int process_line(CrossReference *index, char *line)
{
    char *token;
    size_t i;
    for (i = 0; line[i] != '\0'; ++i) {
        if (!isalpha((unsigned char)line[i]))
            line[i] = ' ';
    }
    token = strtok(line, " ");
    while (token != NULL) {
        if (!record_word(index, token))
            return 0;
        token = strtok(NULL, " ");
    }
    return 1;
}

// A buffer accommodates 200 text characters, a newline, and a null byte.
static int read_input(FILE *input, CrossReference *index)
{
    // Read each line, check for overflow, and process it.
    // we use the maximum line length plus 2 to accommodate the newline and null terminator
    char line[MAX_LINE_LENGTH + 2];

    // Uses fgets to read each line from the input file, checking for errors and line length constraints.
    while (fgets(line, sizeof line, input) != NULL) {

        // Checks if the line exceeds the maximum length and does not contain a newline character, indicating that the line is too long.
        if (strlen(line) > MAX_LINE_LENGTH &&
            strchr(line, '\n') == NULL) {
            
            // If the line is too long, print an error message and return 0 to indicate failure.
            printf("Error: input line exceeds %d characters.\n",
                    MAX_LINE_LENGTH);
            return 0;
        }

        // increments the line count and checks and processes the line using the process_line function. 
        // If processing fails, it returns 0 to indicate failure.
        ++index->line_count;
        if (!process_line(index, line))
            return 0;
    }
    // This is just a check to see if there was an error reading the input file. 
    //If there was, it prints an error message and returns 0 to indicate failure.
    if (ferror(input)) {
        printf("Error: failed to read input file.\n");
        return 0;
    }
    return 1; 
}

static int compare_words(const void *left, const void *right)
{
    const Word *a = left;
    const Word *b = right;
    return strcmp(a->word, b->word);
}

static void print_index(const CrossReference *index)
{
    size_t i;
    printf("Proj. #4 - Jonah Karafotis\n\n");
    printf("%-15s References\n", "Word");
    printf("--------------- -------------\n");
    // Iterate through the unique words in the index and print each word along with its line references.
    for (i = 0; i < index->unique_count; ++i) {
        // create a refrencee equal to the words1s references and print the word and its references in a formatted manner.
        llist reference = index->words[i].references;
        printf("%-15s ", index->words[i].word);
        while (reference != NULL) {
            printf("%d", reference->data);
            reference = reference->next;
            if (reference != NULL)
                putchar(',');
        }
        putchar('\n');
    }
    printf("\nTotal unique words: %zu\n", index->unique_count);
    printf("Total words: %zu\n", index->total_count);
    printf("Total lines: %d\n", index->line_count);
}

static void free_index(CrossReference *index)
{
    size_t i;
    for (i = 0; i < index->unique_count; ++i)
        FreeList(index->words[i].references);
    free(index->words);
}

int main(int argc, char *argv[])
{
    FILE *input;
    CrossReference index = {NULL, 0, 0, 0};
    int success;

    // Ensure we are given exactly one command line argument (the name of the text file to process). 
    // If not, it prints a usage message and exits with a failure status.
    if (argc != 2) {
        printf("Usage: %s <text-file>\n", argv[0]);
        return EXIT_FAILURE;
    }
    // Assign the input to our input file specified in the command line argument.
    input = fopen(argv[1], "r");
    if (input == NULL) {
        printf("Error: could not open file %s.\n", argv[1]);
        return EXIT_FAILURE;
    }
    // Allocate memory for the words array in the CrossReference structure.
    index.words = malloc(MAX_WORDS * sizeof *index.words);
    if (index.words == NULL) {
        printf("Error: unable to allocate the word table.\n");
        fclose(input);
        return EXIT_FAILURE;
    }
    // Read the input file and build the cross-reference index. If successful, sort the words and print the index.
    success = read_input(input, &index);
    if (fclose(input) != 0) {
        printf("Error: failed to close input file.\n");
        success = 0;
    }

    // if the reading and processing of the input file was successful, the program sorts the words in the index using qsort and then prints the index.
    if (success) {
        // sorts the words in the index using qsort and then prints the index.
        qsort(index.words, index.unique_count, sizeof *index.words,
              compare_words);
        print_index(&index);
    }

    // free the allocated memory for the words and their associated reference lists, and return an appropriate exit status based on whether the processing was successful or not.
    free_index(&index);
    return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
