- We will have a quiz on tuesday on dynamic variables

- What is a dynamic variable?
    A variable that is defined at runtime
    normally pointed to via a pointer and defined using malloc (Memory Allocate)

- Derefrencing a Null pointer in C will cause a C program to crash
    int *ptr = NULL;
    // ...
    if (ptr != NULL)
    // do something

- Dynamic arrays Allow for different sized arrays

    * This Code is not working and asks for to many entries than available in the array * 
    * Fix this code and send code to proffessor to ask about what to study for on quiz *

    Example:
        int main() {

            // Create a pointer called list and dynamically allocate
            // enough memory to store LIST_SIZE integers
            int *list = (int *) malloc(sizeof(int) * LIST_SIZE);
            int i;

            // Loop through the array and get a value for each position
            for (i = 0; i < LIST_SIZE; i++)
                scanf("%d", &list[i]);

            // Loop through the array and print each value
            for (i = 0; i < LIST_SIZE; i++)
                printf("%d ", list[i]);

            // Move to a new line after printing the array
            printf("\n");

            // Release the dynamically allocated memory
            free(list);
        }

- The "." operator has a hogher precedence than that "*"
    In order to get the value of a part of a structure that is refrenced by a pointer you must
    (*p).number
    or
    p->number // this is just saying from p enter the structure and get the value of number

    * What if we wanted to input a number and replace p.number? *
        Could we use scanf(%d, p->number)
        or does this need to be the address of p->number
    