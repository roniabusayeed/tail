#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>


bool str_isdigit(const char* str)
{
    for (int i = 0, n = strlen(str); i < n; i++)
    {
        if (!isdigit(str[i]))
            return false;
    }
    return true;
}

int main(int argc,const char* argv[])
{
    // Ensure proper usage
    if (argc != 2 && argc != 3)
    {
        printf("Usage %s [-n] <file>\n", argv[0]);
        return 1;
    }

    const int default_n_lines = 10;
    int n_lines;
    const char* infile = NULL;

    if (argc == 2)
    {
        n_lines = default_n_lines;
        infile = argv[1];
    }
    else 
    {   
        // argc == 3. Optioal argument provided.

        // Read optional argument: -n
        // Move past dash sign.
        if (argv[1][0] == '-')
        {
            argv[1]++;
        }

        // Convert argument to an integer.
        if (str_isdigit(argv[1]))
        {
            n_lines = atoi(argv[1]);
        }
        else
        {
            printf("tail: Invalid value of n\n");
            printf("Usage: %s [-n] <file>\n", argv[0]);
            return 1;
        }

        // Read filename
        infile = argv[2];
    }

    // Open file for reading
    FILE* inptr = fopen(infile, "r");
    if (!inptr)
    {
        printf("Couldn't open file\n");
        return 1;
    }

    // Read file...

    // Close file
    fclose(inptr);
}