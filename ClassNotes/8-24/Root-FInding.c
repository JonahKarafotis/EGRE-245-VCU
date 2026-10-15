#include <stdio.h>
#include <math.h>

/*
    ============================================================
    NEWTON-RAPHSON ROOT FINDING PROGRAM
    ============================================================

    The Newton-Raphson method is used to find a value of x
    where a function equals zero.

    In other words, we are trying to solve:

        f(x) = 0

    For this example, our function is:

        f(x) = x^2 - 2

    Therefore, we want to solve:

        x^2 - 2 = 0

    The answer should be:

        x = sqrt(2)
        x ≈ 1.414214


    Newton-Raphson uses the formula:

                      f(x)
        newX = x - --------
                     f'(x)

    where:

        x     = our current guess
        f(x)  = the value of the function at our current guess
        f'(x) = the derivative of the function
        newX  = our improved guess
*/


/*
    ============================================================
    FUNCTION: calculateFunctionValue
    ============================================================

    This function calculates:

        f(x) = x^2 - 2

    It takes a value of x as input and returns the result of
    plugging that x into the function.

    Example:

        If x = 2:

        f(2) = 2^2 - 2
             = 4 - 2
             = 2
*/
double calculateFunctionValue(double x)
{
    return (x * x) - 2.0;
}


/*
    ============================================================
    FUNCTION: calculateDerivativeValue
    ============================================================

    Newton-Raphson also requires the derivative of our function.

    Original function:

        f(x) = x^2 - 2

    Derivative:

        f'(x) = 2x

    This function calculates the derivative at whatever
    x value we give it.

    Example:

        If x = 2:

        f'(2) = 2(2)
              = 4
*/
double calculateDerivativeValue(double x)
{
    return 2.0 * x;
}


/*
    ============================================================
    MAIN PROGRAM
    ============================================================
*/
int main(void)
{
    /*
        currentGuess stores our current estimate for the root.

        The user will provide the first value.

        For example:

            currentGuess = 1
    */
    double currentGuess;


    /*
        nextGuess stores the improved estimate calculated using
        the Newton-Raphson formula.
    */
    double nextGuess;


    /*
        functionValue will store:

            f(currentGuess)
    */
    double functionValue;


    /*
        derivativeValue will store:

            f'(currentGuess)
    */
    double derivativeValue;


    /*
        tolerance determines how accurate we want our answer to be.

        0.000001 means we will stop when two consecutive guesses
        differ by less than one-millionth.
    */
    const double tolerance = 0.000001;


    /*
        maxIterations prevents the program from running forever
        if Newton-Raphson fails to find a root.
    */
    const int maxIterations = 100;


    /*
        iterationNumber keeps track of how many times we have
        performed the Newton-Raphson calculation.
    */
    int iterationNumber;


    /*
        Ask the user for an initial guess.

        Newton-Raphson needs somewhere to start.

        For this problem, entering 1 or 2 would both be
        reasonable starting guesses.
    */
    printf("Newton-Raphson Root Finding Method\n");
    printf("----------------------------------\n");

    printf("We are finding a root of:\n");
    printf("f(x) = x^2 - 2\n\n");

    printf("Enter an initial guess for x: ");

    scanf("%lf", &currentGuess);


    /*
        ========================================================
        NEWTON-RAPHSON LOOP
        ========================================================

        We repeatedly improve our guess until:

        1. We get close enough to the root

        OR

        2. We reach the maximum number of iterations
    */
    for (iterationNumber = 1;
         iterationNumber <= maxIterations;
         iterationNumber++)
    {

        /*
            Calculate the value of the original function
            using our current guess.

            This gives us:

                f(x)
        */
        functionValue = calculateFunctionValue(currentGuess);


        /*
            Calculate the value of the derivative using
            our current guess.

            This gives us:

                f'(x)
        */
        derivativeValue = calculateDerivativeValue(currentGuess);


        /*
            ----------------------------------------------------
            CHECK FOR DIVISION BY ZERO
            ----------------------------------------------------

            Newton-Raphson uses:

                         f(x)
                x - -------------
                         f'(x)

            Because f'(x) is in the denominator, we cannot
            allow it to equal zero.

            We use fabs() because the derivative could be
            positive or negative.

            fabs() means "floating-point absolute value."

            For example:

                fabs(-3.5) = 3.5
                fabs(3.5)  = 3.5
        */
        if (fabs(derivativeValue) < 0.0000001)
        {
            printf("\nERROR: The derivative is too close to zero.\n");
            printf("Newton-Raphson cannot continue because this ");
            printf("would cause division by zero.\n");

            return 1;
        }


        /*
            ====================================================
            NEWTON-RAPHSON FORMULA
            ====================================================

            Mathematical formula:

                          f(x)
            x_new = x - --------
                         f'(x)

            In our program:

            currentGuess = x
            functionValue = f(x)
            derivativeValue = f'(x)

            Therefore:
        */
        nextGuess = currentGuess -
                    (functionValue / derivativeValue);


        /*
            Print information about this iteration.

            This lets us see Newton-Raphson getting closer
            and closer to the root.
        */
        printf("\nIteration %d\n", iterationNumber);

        printf("Current guess:       %.6f\n", currentGuess);

        printf("f(x):                %.6f\n", functionValue);

        printf("f'(x):               %.6f\n", derivativeValue);

        printf("New guess:           %.6f\n", nextGuess);


        /*
            ----------------------------------------------------
            CHECK IF WE FOUND THE ROOT
            ----------------------------------------------------

            We compare the new guess and the old guess.

            If:

                |nextGuess - currentGuess| < tolerance

            then the guesses are barely changing anymore.

            That means Newton-Raphson has converged on the root.

            fabs() gives us the absolute value of the difference.
        */
        if (fabs(nextGuess - currentGuess) < tolerance)
        {
            printf("\n==================================\n");
            printf("ROOT FOUND!\n");
            printf("==================================\n");

            printf("Approximate root = %.6f\n", nextGuess);

            /*
                We return 0 to indicate that the program
                completed successfully.
            */
            return 0;
        }


        /*
            ----------------------------------------------------
            PREPARE FOR THE NEXT ITERATION
            ----------------------------------------------------

            Our newly calculated guess now becomes our
            current guess.

            For example:

                Iteration 1:
                    currentGuess = 1
                    nextGuess = 1.5

                Then:

                    currentGuess = 1.5

                Iteration 2 uses 1.5 to calculate another
                improved guess.
        */
        currentGuess = nextGuess;
    }


    /*
        If the program gets here, it means we completed all
        100 iterations without reaching our desired tolerance.
    */
    printf("\nNewton-Raphson did not converge after %d iterations.\n",
           maxIterations);

    return 1;
}