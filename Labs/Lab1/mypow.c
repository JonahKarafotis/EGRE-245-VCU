#include "mypow.h"
#include <math.h>
float mypow(int x, int y) {

    // Problem:
    // I Need to Implement a Function That takes x and multiplies it by itself y times.

    // Solution:
    // To do this I will use a sum integer that holds our current sum of the multiplication and loop through multiplying sum by iself y times.
    

    // Edge Case:
    // If y is equal to 0 than we need to return 1

    if (y == 0) {
        return 1;
    }

    // Edge Case:
    // If y is less than 0 than we need to return 1 divided by the result of x multiplied by itself y times. 
    // This is because any number raised to the power of a negative number is equal to 1 divided by that number raised to the power of the positive version of that number.
    if (y < 0) {

        //Edge Case:
        // If x is equal to 0 than we need to return NAN because we are raising 0 to a negative power which is undefined.
        if (x == 0) {
            return NAN;
        }

        // If y is less than 0 than we need to return 1 divided by the result of x multiplied by itself y times.
        // This is because any number raised to the power of a negative number is equal to 1 divided by that number raised to the power of the positive version of that number.
        return 1 / mypow(x, -y); // Call the function again with the positive version of y and return 1 divided by that result.
    }

    // Main Flow For the Function
    int sum; // Declare my sum integer
    sum = x; // Set sum equal to x

    for (int i = 1; i < y; i++) {
        sum = sum * x;
    }
    
    return sum;
    }