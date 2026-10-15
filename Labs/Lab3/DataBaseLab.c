#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for strcpy
#include <ctype.h> // for isspace

#define TEXT_LENGTH 250
#define MAX_DB_SIZE 1000
#define MAX_DESCRIPTION_SIZE 160

typedef struct wType {
    int id; // widget id number
    int num; // number in stock
    float cost; // our unit cost
    char *des; // description of item
} widget;

void initDB(widget *list[],int n) {
    if (n < 1) return;
    for(int i=0; i < n; i++)
        list[i] = NULL;
}

int count(widget *list[], int n) {
    if (n <= 0) return 0;
        int c = 0;
        for(int i = 0; i < n; i++)
            if(list[i] != NULL) c++;
    return c;
}

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

void printDB(widget *db[],int size) {
    int c = count(db, size); // c should store the number of actual widgets in the db
    if(c <= 0) { printf("<database empty>\n"); return; }

    for(int i = 0; i < c; i++) {
        printf("Item #%d\n",i+1);
        printItem(db[i]);
        printf("\n");
    }
}


int main(void){
    // Creates an array of pointers to widget structs with a size of MAX_DB_SIZE.
    widget *db[MAX_DB_SIZE];

    // Define the total integer variable to store the number of items the user wants to enter.
    int total;

    // Intilize an empty database with the size of our Max Size.
    initDB(db,MAX_DB_SIZE);

    // List the number of items in the database and ask the user how many items they want to enter.
    printf("# items in database: %d\n",count(db,MAX_DB_SIZE));
    printf("enter how many items? ");
    scanf("%d",&total);
    
    // We cannot have negative or zero items
    // so if the user enters a number less than or equal to zero we exit the program.
    if(total <= 0) exit(EXIT_SUCCESS);

    // Define the variables we will use to store the user input for each item.
    int id,num; 
    float cost; 
    char des[MAX_DESCRIPTION_SIZE], ch;

    // The first character of the description is the null terminator.
    // The null terminator marks the end of a string in C.
    // Since it is at index 0, des currently contains an empty string.
    des[0] = '\0';

    for(int i = 0; i < total; i++) {
        printf("Item #%d\n",i+1);
        printf(" stock number: "); scanf("%d",&id);
        printf(" # in stock: "); scanf("%d",&num);
        printf(" unit cost: "); scanf("%f",&cost);
        printf(" description: ")
        ;

    while(isspace(ch=getchar())) ; // see notes in handout on this
        ungetc(ch,stdin);
        scanf("%[^\n]",des);
        db[i] = newItem(id,num,cost,des);
    
}

printf("\nDatabase contents:\n\n");
printDB(db,total);
return 0;
}