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

typedef struct exel
{
    int pid;
    int burst;
    int priority;
} exel;

void calculate(int numInstr, int execElems, exel elements[], int* nvcSwitch, int* vcSwitch, double* waitingTime, double* turnaround, double* responseTime, double* throughput)
{
    int ids[32] = {0};

    // NV and V switches //
    for (int i = 0; i < numInstr; ++i)
    {
        if (elements[i].pid != elements[i + 1].pid)
        {
            for (int j = 0; j < 32; ++j)
            {
                if (elements[i].pid == ids[j])
                {
                    ++*nvcSwitch;
                    break;
                }
                else if (ids[j] == 0)
                {
                    ids[j] = elements[i].pid;
                    ++*vcSwitch;
                    break;
                }
            }
        }
    }

    // Waiting Time and Turnaround //
    int tempSum = 0;
    for (int i = 0; i < execElems; ++i)
    {
        tempSum = 0;
        for (int j = 0; j < numInstr; ++j)
        {
            if (elements[j].pid != ids[i])
            {
                tempSum += elements[j].burst;
            }
            else
            {
                *waitingTime += tempSum;
                *turnaround += (tempSum + elements[j].burst);
                tempSum = 0;
            }
        }
    }
    *waitingTime /= (double)execElems;
    *turnaround /= (double)execElems;

    // Response Time //
    int tempResponseTime = 0;
    for (int i = 0; i < execElems; ++i)
    {
        for (int j = 0; j < numInstr; ++j)
        {
            if (elements[j].pid != ids[i])
            {
                tempResponseTime += elements[j].burst;
            }
            else
            {
                *responseTime += tempResponseTime;
                tempResponseTime = 0;
                break;
            }
        }
    }
    *responseTime /= (double)execElems;

    // Throughput //
    for (int i = 0; i < numInstr; ++i)
    {
        *throughput += elements[i].burst;
    }
    *throughput = execElems / *throughput;
}

int main(int argc, char** argv)
{
    byte input;
    int lineNumber = 0;
    FILE * file;
    int execElems = 0;
    int numInstr = 0;
    int flag = FALSE;

    if(!(file = fopen(*argv, "r"))){
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
                exel elements[32768]; // 2^15 p
                int numPid = 0;
                int firstElement = TRUE;
                int vcSwitch = 0;
                int nvcSwitch = 0;
                double throughput = 0.0;
                double turnaround = 0.0;
                double waitingTime = 0.0;
                double responseTime = 0;

                elements[numPid].pid = atoi(&input);
                while(((input = fgetc(file)) != (byte)EOF))
                {
                    if (firstElement == FALSE)
                    {

                        elements[numPid].pid = atoi(&input);
                    }
                    else
                    {
                        firstElement = FALSE;
                    }

                    if (input != ' ' && input != '\n')
                    {
                        while (((input = fgetc(file)) != ' '))
                        {
                            elements[numPid].pid = (elements[numPid].pid * 10) + atoi(&input);
                        }
                    }
                    input = fgetc(file);
                    elements[numPid].burst = atoi(&input);
                    while (((input = fgetc(file)) != ' '))
                    {
                        elements[numPid].burst = (elements[numPid].burst * 10) + atoi(&input);
                    }
                    input = fgetc(file);
                    elements[numPid].priority = atoi(&input);
                    while (((input = fgetc(file)) != (byte)EOF))
                    {
                        if (input == '\n')
                        {
                            break;
                        }
                        elements[numPid].priority = (elements[numPid].priority * 10) + atoi(&input);
                    }
                    ++numPid;
                }


                calculate(numInstr,execElems, elements, &nvcSwitch, &vcSwitch, &waitingTime, &turnaround, &responseTime, &throughput);


                printf("%d\n%d\n100.00\n%.2f\n%.2f\n%.2f\n%.2f\n", vcSwitch, nvcSwitch, throughput, turnaround, waitingTime, responseTime);
            }
        }
    }
    fclose(file);

    return 0;
}
