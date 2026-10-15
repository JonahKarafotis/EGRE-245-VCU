#include <stdio.h>
#include <math.h>

double squareRoot(long x)
{
    if (x < 0)
        return NAN;

    if (x == 0)
        return 0;

    double prev;
    int k = 0;
    int kmax = 1000;
    double s = 1;

    for (k = 0; k < kmax; k++)
    {
        prev = s;

        // Newton's Method for square root
        s = s - ((s * s - x) / (2 * s));

        if (prev == s)
            break;
    }

    return s;
}


double cubeRoot(long x)
{
    if (x == 0)
        return 0;

    double prev;
    int k = 0;
    int kmax = 1000;
    double s = 1;

    for (k = 0; k < kmax; k++)
    {
        prev = s; 

        // Newton's Method for cube root
        s = s - ((s * s * s - x) / (3 * s * s));

        if (prev == s)
            break;
    }

    return s;
}


int main()
{
    int choice;
    long n;

    while (1)
    {
        printf("What would you like to solve?\n");
        printf("1. Square Root\n");
        printf("2. Cube Root\n");
        printf("Enter your choice: ");

        // Make sure the user actually entered an integer
        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Please enter 1 or 2.\n\n");

            // Remove invalid input from the input buffer
            while (getchar() != '\n');

            continue;
        }

        // Make sure the integer is either 1 or 2
        if (choice == 1 || choice == 2)
        {
            break;
        }

        printf("Invalid choice. Please enter 1 or 2.\n\n");
    }

    printf("Enter the value you would like to find the root of: ");
    scanf("%ld", &n);

    switch (choice)
    {
        case 1:
            printf("Square root of %ld is %f\n",
                   n, squareRoot(n));
            break;

        case 2:
            printf("Cube root of %ld is %f\n",
                   n, cubeRoot(n));
            break;
    }

    return 0;
}