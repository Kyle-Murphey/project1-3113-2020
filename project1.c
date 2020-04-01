#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 *  Project 1
 * Kyle Murphey
 * 04/03/20
 */

#define TRUE 1
#define FALSE 0
#define BUFF_SIZE 10

/* unsigned char */
typedef unsigned char byte;

int main(int argc, char** argv)
{
    byte input[BUFF_SIZE] = {0};
    int line[4] = {0};
    int lineNumber = 0;
    FILE * file;

    file = fopen("test.txt", "r");

    while (((fgets(input, BUFF_SIZE, file)) != NULL))
    {
        if (lineNumber == 0)
        {
            // # of processes available to run, but in this case it is always 1
            ++lineNumber;
        }
        else if (lineNumber == 1)
        {
            ++lineNumber;
        }
        else
        {
            printf("%s", input);
        }
    }
    fclose(file);

    return 0;
}
