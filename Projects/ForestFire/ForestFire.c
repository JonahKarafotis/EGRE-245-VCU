/*
Author: Jonah Karafotis
Class: EGRE 246
Project: Forest Fire Simulation

The Goal of this project is to simulate a forest fire using a grid-based approach. 
The simulation will model the spread of fire through a forest represented as a 2D grid
where each cell can be in one of several states: empty, tree, or burning. 
The simulation will allow for the visualization of the fire's progression through several generations.

Intended Process:
    1. Read the forest size, random seed, probability, and initial forest
    configuration from the input file.
    2. Store the forest in a two-dimensional array.
    3. Print the initial forest and simulation information.
    4. While there is still fire in the forest:
        - Check each cell from left to right and top to bottom.
        - If a tree has a neighboring fire, use the given probability
            to determine if it catches fire.
        - Change currently burning cells to empty cells.
        - Create the next generation of the forest.
    5. Continue until there are no burning cells remaining.
    6. Print the final forest and simulation statistics.
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Maps the cell states to integers for easier readability.
#define EMPTY 0
#define TREE  1
#define FIRE  2

typedef struct {
    char *commands;
    int numberOfLines;
} InputData;

// Print the Visible Forest to the console.
void printForest(int *forest, int rows, int columns, int totalColumns) {
    
    // Print column numbers.
    // This works by printing the column number modulo 10
    // which will give us the last digit of the column number.
    printf("   ");
    for (int j = 1; j <= columns; j++) {
        printf("%d", j % 10);
    }
    printf("\n");

    // Print top border.
    printf("  +");
    for (int j = 0; j < columns; j++) {
        printf("-");
    }
    printf("+\n");

    // Print the actual forest.
    for (int i = 1; i <= rows; i++) {

        printf("%d|", i);

        for (int j = 1; j <= columns; j++) {

            int cell = forest[i * totalColumns + j];

            if (cell == EMPTY)
                printf(".");
            else if (cell == TREE)
                printf("T");
            else if (cell == FIRE)
                printf("F");
        }

        printf("| %d\n", i);
    }

    // Print bottom border.
    printf("  +");
    for (int j = 0; j < columns; j++) {
        printf("-");
    }
    printf("+\n");

    // Print column numbers again.
    printf("   ");
    for (int j = 1; j <= columns; j++) {
        printf("%d", j % 10);
    }
    printf("\n\n");
}

// Inputs:
//      fileName - name of the input file
//      commands - pointer to the dynamically allocated character array
//
// Outputs:
//      commands will contain all lines read from the file
void readInitialForest(char *fileName, InputData *inputData) {

    // Open the file whose name was passed into this function.
    FILE *inputFile = fopen(fileName, "r");

    // Make sure the file opened successfully.
    if (inputFile == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Each line can contain up to 10 characters.
    // +1 gives us room for the null terminator.
    char line[11];

    int numberOfLines = 0;

    // ---------------------------------------------------------
    // First pass:
    // Count how many lines are in the file.
    // This is needed so we know how much memory to allocate for the commands array.
    // ---------------------------------------------------------

    // fgets 
    // Description: reads one line at a time from the file. 
    // inputs: 
    //      line - pointer to the character array where the read line will be stored
    //      sizeof(line) - the maximum number of characters to read (including the null terminator)
    //      inputFile - pointer to the file from which to read
    while (fgets(line, sizeof(line), inputFile) != NULL) {
        numberOfLines++;
    }

    // ---------------------------------------------------------
    // Allocate enough memory to hold every line.
    //
    // 10 characters per line
    // +1 for the final null terminator
    // ---------------------------------------------------------

    inputData->commands = malloc((10 * numberOfLines + 1) * sizeof(char));

    // Checks to see if malloc failed to allocate memory. If malloc fails, it returns NULL.
    if (inputData->commands == NULL) {
        printf("Error allocating memory.\n");
        fclose(inputFile);
        return;
    }

    // Rewind
    // Description: Resets the file position indicator for the stream pointed to by inputFile to the beginning of the file.
    // Inputs: 
    //      inputFile - pointer to the file stream to rewind
    rewind(inputFile);

    // Keeps track of where the next line should be stored in the commands array.
    int position = 0;

    // ---------------------------------------------------------
    // Second pass:
    // Read each line and copy it into commands.
    // ---------------------------------------------------------

    //fgets
    // Description: reads one line at a time from the file.
    // inputs:
    //      line - pointer to the character array where the read line will be stored
    //      sizeof(line) - the maximum number of characters to read (including the null terminator)
    //      inputFile - pointer to the file from which to read
    // Example:
    //      If line contains:
    //
    //      "T 5 8\n"
    //
    //      then strlen(line) returns 6 because the characters are:
    //
    //      'T' ' ' '5' ' ' '8' '\n'
    //
    //      The '\0' is still stored after them, but is not counted.
    while (fgets(line, sizeof(line), inputFile) != NULL) {

        //strlen
        // Description: calculates the length of the string stored in line, excluding the null terminator
        // inputs:
        //      line - pointer to the character array whose length is to be calculated
        int lineLength = strlen(line);

        // Go through the line one character at a time until it hits the linelength.
        for (int i = 0; i < lineLength; i++) {

            // Copy the current character from "line"
            // into the next available spot in the dynamically
            // allocated commands array.
            inputData->commands[position] = line[i];

            // Move position forward so the next character will be stored in the next open location.
            position++;
            // Example of data would look like this:
        }
    }

    // Add the null terminator to make commands a valid C string.
    (inputData->commands)[position] = '\0';

    // Close the file when finished reading it.
    fclose(inputFile);
}

void applyInitialCommands(char *commands, int *forest, int rows, int columns, int totalColumns, int *trees) {

    // Start at the beginning of the commands string.
    // this was confusing to me for a while but pointers to char is just a pointer to the start of the memory address.
    // So this is just a pointer to the start of the memory address of the commands string and we know that each of our lines
    // are seperated by newline characters so we can just move the pointer forward until we hit a newline character and then
    // move it forward one more time to get to the next line.
    char *current = commands; 

    // Skip the first 4 lines:
    // rows, columns, random seed, probability
    for (int i = 0; i < 4; i++) {

        // Move forward until we find the newline.
        while (*current != '\n') {
            current++; // Move to the next memory address in the commands string.
        }

        // Move past the newline to the start of the next line.
        current++;
    }

    // Now current points to the first command.
    while (*current != '\0') {

        char command;
        int row;
        int column;

        // Read the first character on the current line.
        command = *current;

        // Q means stop processing commands.
        if (command == 'Q') {
            break;
        }

        // A means fill the entire visible forest with trees.
        if (command == 'A') {

            for (int i = 1; i <= rows; i++) {
                for (int j = 1; j <= columns; j++) {
                    forest[i * totalColumns + j] = TREE;
                }
            }
        }

        // T, F, and E all have a row and column after them.
        else {

            sscanf(current, "%c %d %d", &command, &row, &column);

            if (command == 'T') {
                forest[row * totalColumns + column] = TREE;
            }

            else if (command == 'F') {
                forest[row * totalColumns + column] = FIRE;
            }

            else if (command == 'E') {
                forest[row * totalColumns + column] = EMPTY;
            }
        }

        // Move current pointer to the end of the line.
        // In this case we finished processing the line but our pointer 
        // is still pointing at our proccessed command. So we push the pointer
        // forward until we either hit the \0 end array signal or the new line signal.
        while (*current != '\n' && *current != '\0') {
            current++;
        }

        // If we're sitting on a newline, move past it.
        if (*current == '\n') {
            current++;
        }
    }
    *trees = 0;

    for (int i = 1; i <= rows; i++) {
        for (int j = 1; j <= columns; j++) {

            if (forest[i * totalColumns + j] == TREE) {
                (*trees)++;
            }
        }
    }
}

int runGeneration(int *forest, int *nextForest, int rows, int columns, int totalColumns, double probability, int *currentTrees) {
    // This function will implement the logic for running a single generation of the forest fire simulation.
    // It will check each cell in the forest and apply the rules for spreading fire and updating the state of each cell.
    // The results will be stored in nextForest, which will then be copied back to forest at the end of the generation.

    // while (there is still a burning cell in the forest) {
    //
    //     Check each cell from left to right and
    //     top to bottom.
    //
    //     If the current cell contains a tree:
    //         Check its neighboring cells for fire.
    //
    //         If a neighboring cell is burning:
    //             Use the probability to determine whether
    //             the tree catches fire.
    //
    //     If the current cell is already burning:
    //         Change it to an empty cell in nextForest.
    //
    //     Store all changes in nextForest so that changing
    //     one cell does not affect other cells during the
    //     same generation.
    //
    //     After every cell has been checked:
    //         Copy/swap nextForest into forest.
    //
    //     Print the new generation if required.
    // }
    // Convert the percentage from something like 75
    // into a decimal probability like 0.75.
    
    double fireProbability = probability / 100.0;

    // This will tell main whether the NEW generation
    // still contains any fire.
    int fireExists = 0;

    // Go through only the visible forest.
    // We start at 1 and stop at rows/columns so that
    // the empty buffer around the forest is not changed.
    for (int i = 1; i <= rows; i++) {

        for (int j = 1; j <= columns; j++) {

            // Find the location of the current cell
            // in our one-dimensional array.
            int position = i * totalColumns + j;


            // -------------------------------------------------
            // Case 1: The current cell is EMPTY.
            // Empty cells stay empty.
            // -------------------------------------------------
            if (forest[position] == EMPTY) {

                nextForest[position] = EMPTY;
            }


            // -------------------------------------------------
            // Case 2: The current cell is FIRE.
            // Fire burns out after one generation,
            // so it becomes EMPTY.
            // -------------------------------------------------
            else if (forest[position] == FIRE) {

                nextForest[position] = EMPTY;
            }


            // -------------------------------------------------
            // Case 3: The current cell is a TREE.
            // Check its four neighbors for fire.
            // -------------------------------------------------
            else if (forest[position] == TREE) {

                int hasFireNeighbor = 0;

                // Check above.
                if (forest[(i - 1) * totalColumns + j] == FIRE) {
                    hasFireNeighbor = 1;
                }

                // Check below.
                if (forest[(i + 1) * totalColumns + j] == FIRE) {
                    hasFireNeighbor = 1;
                }

                // Check left.
                if (forest[i * totalColumns + (j - 1)] == FIRE) {
                    hasFireNeighbor = 1;
                }

                // Check right.
                if (forest[i * totalColumns + (j + 1)] == FIRE) {
                    hasFireNeighbor = 1;
                }


                // If there is at least one neighboring fire,
                // determine whether this tree catches fire.
                if (hasFireNeighbor == 1) {

                    // Generate a random number from 0.0 to 1.0.
                    double randomNumber =
                        (double)rand() / (double)RAND_MAX;

                    // If the random number falls within our
                    // probability, this tree catches fire.
                    if (randomNumber <= fireProbability) {

                        nextForest[position] = FIRE;

                        // Since the next generation contains fire,
                        // the simulation will need another generation.
                        fireExists = 1;
                    }

                    // Otherwise the tree survives.
                    else {

                        nextForest[position] = TREE;
                    }
                }

                // If there is no neighboring fire,
                // the tree simply stays a tree.
                else {

                    nextForest[position] = TREE;
                }
            }
        }
    }


    // ---------------------------------------------------------
    // Copy the completed next generation back into forest.
    // ---------------------------------------------------------

    for (int i = 1; i <= rows; i++) {

        for (int j = 1; j <= columns; j++) {

            int position = i * totalColumns + j;

            forest[position] = nextForest[position];
        }
    }


    // 1 means there is still fire.
    // 0 means the fire is completely gone.
    *currentTrees = 0;

    for (int i = 1; i <= rows; i++) {
        for (int j = 1; j <= columns; j++) {

            if (forest[i * totalColumns + j] == TREE) {
                (*currentTrees)++;
            }
        }
    }

    return fireExists;
}

// This is where the program begins execution.
int main(int argc, char *argv[]) {

    // ---------------------------------------------------------
    // 0. Define the variables and capture the input file from the command line argument
    //    passed and save the file pointer to a variable for later
    //    use. Define some basic mapped variables for easier readability.
    // ---------------------------------------------------------

    int rows;
    int columns;
    double probability;
    int randomSeed;
    int generation = 0;
    int trees;


    // debug mode options
    int debugMode = 0;
    int choice;

    printf("\n\nForest Fire Simulation\n");
    printf("Programmed by Jonah Karafotis\n");
    printf("EGRE 246\n\n");

    while (1)
    {
        printf("Would You Like To Enter DEBUG MODE?\n");
        printf("1. Yes\n");
        printf("2. No\n");
        printf("Enter your choice (1 or 2): ");

        // scanf returns 1 if one integer is successfully read.
        // If it does not return 1, the input was invalid or unavailable.
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
    printf("\n");

    // Turn debug mode on or off based on the user's choice.
    if (choice == 1) {
        debugMode = 1;
    }
    else {
        debugMode = 0;
    }

    

    InputData *inputData = malloc(sizeof(InputData));

    readInitialForest(argv[1], inputData);

    // ---------------------------------------------------------
    // 1. Read the first 4 lines of the input file for
    //    the forest dimensions, random seed, probability,
    //    and initial forest configuration.
    // ---------------------------------------------------------

    // Take in and parse the first 4 lines of the input file.
    // This will give us the forest dimensions, random seed, probability, and initial forest configuration.
    rows = inputData->commands[0];
    columns = inputData->commands[1];
    randomSeed = inputData->commands[2];
    probability = inputData->commands[3];

    // Use sscanf to parse the first 4 lines of the input file.
    sscanf(inputData->commands, "%d %d %d %lf",
       &rows, &columns, &randomSeed, &probability);
    
    if (debugMode == 1) {
        printf("DEBUG: Rows = %d, Columns = %d, Random Seed = %d, Probability = %f\n", rows, columns, randomSeed, probability);
    }

    // Add 2 to the rows and columns to account for the border.
    int totalRows = rows + 2;
    int totalColumns = columns + 2;

    // ---------------------------------------------------------
    // 2. Store the forest in a dynamically allocated
    //    two-dimensional grid.
    // ---------------------------------------------------------

    // Dynamically allocate memory for the forest and nextForest arrays.
    int *forest = malloc(totalRows * totalColumns * sizeof(int));
    int *nextForest = malloc(totalRows * totalColumns * sizeof(int));

    // Make sure malloc was successful.
    if (forest == NULL || nextForest == NULL) {
        printf("Error allocating memory.\n");

        free(forest);
        free(nextForest);

        return 1;
    }
 
    // Initialize every cell in the forest to EMPTY.
    for (int i = 0; i < totalRows; i++) {
        for (int j = 0; j < totalColumns; j++) {
            forest[i * totalColumns + j] = EMPTY;
        }
    }

    // Initialize every cell in the nextForest to EMPTY.
    for (int i = 0; i < totalRows; i++) {
        for (int j = 0; j < totalColumns; j++) {
            nextForest[i * totalColumns + j] = EMPTY;
        }
    }

    // ---------------------------------------------------------
    // 3. Print the initial forest and simulation information.
    // ---------------------------------------------------------
    if (debugMode == 1){
        printf("\nInitialized Forest\n");
        printForest(forest, rows, columns, totalColumns);
        printf("\n\nInitialized nextForest\n");
        printForest(nextForest, rows, columns, totalColumns);
    }

    applyInitialCommands(inputData->commands, forest, rows, columns, totalColumns, &trees);

    printf("rows: %d\n", rows);
    printf("cols: %d\n", columns);
    printf("trees: %d\n", trees);
    printf("seed: %d\n", randomSeed);
    printf("Probability: %lf\n\n",probability);

    printf("Current Generation: %d\n\n", generation);

    printForest(forest, rows, columns, totalColumns);

    printf("Total Burned: 0 (0.0%%)\n");
    printf("Trees Left: %d\n", trees);

    // ---------------------------------------------------------
    // 4. Continue creating generations while fire remains.
    // ---------------------------------------------------------
    
    // Seed our random number Generator
    srand(randomSeed);

    int fireExists = 1;
    int currentTrees = 0;
    while (fireExists) {


        fireExists = runGeneration(forest, nextForest, rows, columns, totalColumns, probability, &currentTrees);
        if (debugMode == 1) {
            printf("\nCurrent Generation: %d\n", generation);
            printForest(forest, rows, columns, totalColumns);
            int burnedTrees = trees - currentTrees;

            float percentBurned = ((float)burnedTrees / trees) * 100.0;

            printf("Total Burned: %d (%.1f%%)\n", burnedTrees, percentBurned);
            printf("Trees Left: %d\n", currentTrees);
        }
        generation++;
    }

    // ---------------------------------------------------------
    // 6. Print the final forest and simulation statistics.
    // ---------------------------------------------------------
    printf("\nCurrent Generation: %d\n\n", generation);
    printForest(forest, rows, columns, totalColumns);
    
    int burnedTrees = trees - currentTrees;

    float percentBurned = ((float)burnedTrees / trees) * 100.0;

    printf("Total Burned: %d (%.1f%%)\n", burnedTrees, percentBurned);
    printf("Trees Left: %d\n", currentTrees);

    // The arrays were created using malloc, so their memory
    // must be released before the program ends.
    free(forest);
    free(nextForest);
    free(inputData);

    return 0;
}
