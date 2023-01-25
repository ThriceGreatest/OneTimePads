#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to generate a key of the specified length using standard Unix
// randomization methods
void keygen(int keylength)
{
    // Make sure the keylength is positive
    if (keylength <= 0)
    {
        fprintf(stderr, "Error: keylength must be positive\n");
        return;
    }

    // Initialize a string with the 27 allowed characters
    char allowed_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    // Loop through the keylength and generate a random character for each iteration
    int i = 0;
    for (i; i < keylength; i++)
    {
        // Generate a random index between 0 and 26 (inclusive)
        int index = rand() % 26;

        // Print the character at the generated index to stdout
        printf("%c", allowed_chars[index]);
    }

    // Print a newline character at the end of the key
    printf("\n");
}

// Main function
int main(int argc, char *argv[])
{

    
    // Make sure the correct number of arguments is provided
    if (argc != 2)
    {
        fprintf(stderr, "Error: incorrect number of arguments\n");
        return 1;
    }

    srand((int)time(NULL));
    // Convert the keylength argument to an integer
    int keylength = atoi(argv[1]);

    // Generate the key using the keygen function
    keygen(keylength);

    return 0;
}