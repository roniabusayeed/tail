#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


/**
 * Reads an entire line from stream returning the address of
 * the buffer containing the text. The buffer is null-terminated
 * and includes the newline character if one was found.
 * Returns NULL on failure to read a line (including end of file
 * condition). Sets *line_number to line number of the line in stream.
 * The buffer is heap allocated and the caller function is should
 * to free the memory to avoid memory leak.
 */
char* readline(FILE* stream, int* line_number);


/**
 * Checks if all the characters of a string are digit.
 */
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

        // Remember filename
        infile = argv[2];
    }

    // Open file for reading
    FILE* inptr = fopen(infile, "r");
    if (!inptr)
    {
        printf("Couldn't open file\n");
        return 1;
    }

    // Load lines in a dynamic array
    const size_t init_capacity = 10;
    const float increment_factor = 1.25f;
    
    char** list = malloc(sizeof(char*) * init_capacity);
    
    size_t capacity = init_capacity;
    size_t size = 0;

    char* line;
    int line_number;    // Throwaway variable.
    while ((line = readline(inptr, &line_number)) != NULL)
    {
        if (size == capacity)
        {
            capacity = ceil(capacity * increment_factor);
            list = realloc(list, sizeof(char*) * capacity);
        }

        list[size++] = line;
    }

    // Print last n_lines lines.
    // Get index of the first line to print.
    int cursor = 0;
    if (n_lines < size)
    {
        cursor = size - n_lines;
    }

    while (cursor < size)
    {
        printf("%s", list[cursor++]);
    }



    // Close file
    fclose(inptr);
}


char* readline(FILE* stream, int* line_number)
{
    static int s_line_number = 1;

    const size_t init_capacity = 10;
    char* line = malloc(sizeof(char) * init_capacity);
    // Guard against insufficient memory.
    if (!line)
    {
        printf("Insufficient memory\n");
        *line_number = -1;
        return NULL;
    }
    const float increment_factor = 1.25f;   
    size_t capacity = init_capacity;
    size_t size = 0;
    int c;
    while ((c = fgetc(stream)) != EOF)
    {
        // Increase buffer capacity if size exceeds it.
        if (size == capacity)
        {
            capacity = ceil(capacity * increment_factor);
            line = realloc(line, capacity);
            // Guard against insufficient memory.
            if (!line)
            {
                printf("Insufficient memory\n");
                *line_number = -1;
                return NULL;
            }
        }

        line[size++] = c;
        if (c == '\n')
            break;
    }

    // If characters are in buffer return buffer.
    if (size)
    {
        line[size] = 0;
        *line_number = s_line_number++;
        return line;
    }
    
    // Characters are not in buffer. So, free memory and return NULL.   
    free(line);
    *line_number = -1;
    return NULL;    
}
