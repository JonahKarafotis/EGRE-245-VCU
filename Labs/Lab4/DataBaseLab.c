#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for strcpy
#include <ctype.h> // for isspace

#define TEXT_LENGTH 250
// #define MAX_DB_SIZE 1000
#define EXPANSION_SIZE 2
#define MAX_DESCRIPTION_SIZE 160

int arraySize = 0;
int listSize = 0;

typedef struct wType {
    int id; // widget id number
    int num; // number in stock
    float cost; // our unit cost
    char *des; // description of item
} widget;

// void initDB(widget *list[],int n) {
//     if (n < 1) return;
//     for(int i=0; i < n; i++)
//         list[i] = NULL;
// }

// int count(widget *list[], int n) {
//     if (n <= 0) return 0;
//         int c = 0;
//         for(int i = 0; i < n; i++)
//             if(list[i] != NULL) c++;
//     return c;
// }

widget *newItem(int new_id, int new_num, float new_cost, char *new_des) {
    // Creates a pointer to with a size of a widget struct.
    widget *tmp = malloc(sizeof(widget)); // <-- make sure you understand this!
    // Assigns the values of the new item to the struct fields.
    tmp->id = new_id;
    tmp->num = new_num;
    tmp->cost = new_cost;
    // Allocates memory for our new description.
    tmp->des = malloc(strlen(new_des)+1); // <-- make sure you understand this!
    // Copies the new description into the struct field.
    strcpy(tmp->des,new_des);
    // Returns the pointer to the new item.
    return tmp;
    }

void printItem(widget *item) {
    // Prints the field of the item struct.
    printf("stock number: %d\n", item->id);
    printf("number in stock: %d\n", item->num);
    // Prints the cost with 2 decimal places since it is a float and this is a dollar amount.
    printf("unit cost: %.2f\n", item->cost);
    printf("description: %s\n", item->des);
    }

// Prints every widget currently stored in the database.
void printDB(widget *db[]) {

    // If there are no items, print that the database is empty.
    if (listSize <= 0) {
        printf("<database empty>\n");
        return;
    }

    // Only loop through the number of items actually stored.
    for (int i = 0; i < listSize; i++) {

        printf("Item #%d\n", i + 1);

        printItem(db[i]);

        printf("\n");
    }
}


/*
1. Define your database array db as a pointer (initially set to NULL). 
You will not need to modify function newItem

*/
int main(void) {

    // db is now a pointer instead of a fixed-size array.
    // It will point to a dynamically allocated array
    // containing pointers to widgets.
    widget **db = NULL;

    // Start the database with EXPANSION_SIZE available spaces.
    arraySize = EXPANSION_SIZE;

    db = malloc(arraySize * sizeof(widget *));

    // Make sure malloc worked.
    if (db == NULL) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    // Number of items the user wants to enter.
    int total;

    printf("# items in database: %d\n", listSize);

    printf("enter how many items? ");
    scanf("%d", &total);

    // We cannot have negative or zero items.
    if (total <= 0) {

        free(db);

        exit(EXIT_SUCCESS);
    }

    // Variables used to store user input.
    int id;
    int num;
    float cost;
    char des[MAX_DESCRIPTION_SIZE];
    char ch;

    // des currently contains an empty string.
    des[0] = '\0';

    // Ask the user for each widget.
    for (int i = 0; i < total; i++) {
        printf("Item #%d\n", i + 1);
        printf(" stock number: ");
        scanf("%d", &id);
        printf(" # in stock: ");
        scanf("%d", &num);
        printf(" unit cost: ");
        scanf("%f", &cost);
        printf(" description: ");

        while (isspace(ch = getchar()));
        ungetc(ch, stdin);
        scanf("%[^\n]", des);


        // Check if the database array is full.
        if (listSize >= arraySize) {

            printf("Expanding database from %d to %d.\n",
                   arraySize,
                   arraySize + EXPANSION_SIZE);

            // Create a new, larger array.
            widget **temp = malloc(
                (arraySize + EXPANSION_SIZE) * sizeof(widget *)
            );

            // Make sure malloc worked.
            if (temp == NULL) {
                printf("Memory allocation failed.\n");
                exit(EXIT_FAILURE);
            }

            // Copy all existing widget pointers
            // from the old array into the new array.
            for (int j = 0; j < listSize; j++) {

                temp[j] = db[j];
            }

            // Delete the old pointer array.
            free(db);

            // Make db point to the new larger array.
            db = temp;

            // Update the size of the allocated array.
            arraySize += EXPANSION_SIZE;
        }


        // Store the new widget in the next available position.
        db[listSize] = newItem(id, num, cost, des);

        // Increase the number of items currently stored.
        listSize++;
    }


    printf("\nDatabase contents:\n\n");

    printDB(db);


    // Free each widget and its dynamically allocated description.
    for (int i = 0; i < listSize; i++) {

        free(db[i]->des);

        free(db[i]);
    }

    // Free the dynamically allocated database array.
    free(db);

    return 0;
}