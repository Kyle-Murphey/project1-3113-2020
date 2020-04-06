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
    byte input;
    int lineNumber = 0;
    FILE * file;
    int execElems = 0;
    int numInstr = 0;
    int flag = FALSE;
    int pid;
    int burst;
    int priority;

    if(!(file = fopen("test.txt", "r"))){
        printf("Whoopsie: %s\n", strerror(errno));
        exit(1);
    }

    while (((input = fgetc(file)) != (byte)EOF))
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
                if (flag == FALSE)
                {
                    execElems = atoi(&input);
                    while (((input = fgetc(file)) != ' '))
                    {
                        execElems = (execElems * 10) + atoi(&input);
                    }
                    flag = TRUE;
                }
                else
                {
                    numInstr = atoi(&input);
                    while (((input = fgetc(file)) != '\n'))
                    {
                        numInstr = (numInstr * 10) + atoi(&input);
                    }
                    flag = FALSE;
                    ++lineNumber;
                }
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
