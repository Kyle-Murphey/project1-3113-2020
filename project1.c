#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

/*
 *  Project 1
 * Kyle Murphey
 * 04/03/20
 */

#define TRUE 1
#define FALSE 0

/* unsigned char */
typedef unsigned char byte;

int main(int argc, char** argv)
{
    int input;
    int lineNumber = 0;
    FILE * file;

    if(!(file = fopen("D:/Users/kyle_/CLionProjects/project1/test", "r"))){
        printf("Whoopsie: %s\n", strerror(errno));
        exit(1);
    }

    while (((input = fgetc(file)) != EOF))
    {
        if (input == '\n' && lineNumber < 2)
        {
            ++lineNumber;
        }
        else
        {
            if (lineNumber == 0)
            {
                // # of processes available to run, but in this case it is always 1
            }
            else if (lineNumber == 1)
            {

            }
            else
            {
                printf("%c", input);
            }
        }
    }
    fclose(file);

    return 0;
}
