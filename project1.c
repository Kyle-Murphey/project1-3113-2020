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

/* data structure for the instructions */
typedef struct exel
{
    int pid; //id
    int burst; //time on cpu
    int priority; //priority
} exel;

/*
 * calculates the various statistics needed for this project
 * @param:numInstr = # of instructions
 * @param:execElems = # of pids
 * @param:elements = array of exel instructions
 * @param:nvcSwitch = nonvolcont switches
 * @param:vcSwitch = volcont switches
 * @param:waitingTime = time spent in ready queue
 * @param:turnaround = average completion time
 * @param:responseTime = average time to start running
 * @param:throughput = amount of work for processor
 */
void calculate(int numInstr, int execElems, exel elements[], int* nvcSwitch, int* vcSwitch, double* waitingTime, double* turnaround, double* responseTime, double* throughput)
{
    int ids[64] = {0}; //holds to unique ids

    // NV and V switches //
    for (int i = 0; i < numInstr; ++i)
    {
        //check for continuous elements
        if (elements[i].pid != elements[i + 1].pid)
        {
            for (int j = 0; j < 64; ++j)
            {
                //process runs later, nonvol
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
    //loop through # of pids
    for (int i = 0; i < execElems; ++i)
    {
        tempSum = 0;
        //loop through the # of instructions to compare pids
        for (int j = 0; j < numInstr; ++j)
        {
            //add up the waiting time
            if (elements[j].pid != ids[i])
            {
                tempSum += elements[j].burst;
            }
            //match found
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
            //adding time until first occurance
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

/*
 * reads in a file from the CL and stores the instructions into a struct. The data is sent to a function be calculated.
 */
int main(int argc, char** argv)
{
    byte input; //input byte from file
    int lineNumber = 0; //keeps track of first 2 lines since they're not instructions
    FILE * file; //file to read
    int execElems = 0; //number of IDs
    int numInstr = 0; //number of instructions
    int flag = FALSE; //flag for second line to separate the 2 inputs

    exel elements[32768]; // 2^15 p
    int numPid = 0; //keeps track of current pid index
    int firstElement = TRUE; //flag for first line of instructions
    int vcSwitch = 0; //voluntary context switch
    int nvcSwitch = 0; //non-voluntary context switch
    double throughput = 0.0;
    double turnaround = 0.0;
    double waitingTime = 0.0;
    double responseTime = 0.0;
    double cpu = 100.00;

    //check if file can be opened/exists
    if(!(file = fopen(argv[1], "r"))){
        printf("Whoopsie: %s\n", strerror(errno));
        exit(1);
    }

    //main loop through file
    while (((input = fgetc(file)) != (byte)EOF))
    {
        //for incrementing for first 2 lines
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
                    //stores entire value into execElems
                    execElems = atoi(&input);
                    while (((input = fgetc(file)) != ' '))
                    {
                        execElems = (execElems * 10) + atoi(&input);
                    }
                    flag = TRUE;
                }
                else
                {
                    //stores entire value into numInstr
                    numInstr = atoi(&input);
                    while (((input = fgetc(file)) != '\n'))
                    {
                        numInstr = (numInstr * 10) + atoi(&input);
                    }
                    flag = FALSE;
                    ++lineNumber;
                }
            }
            //start of instructions
            else
            {
                //first bit from instructions
                elements[numPid].pid = atoi(&input);
                //loop through the rest of the content
                while(((input = fgetc(file)) != (byte)EOF))
                {
                    //pid storage after first iterations
                    if (firstElement == FALSE)
                    {
                        elements[numPid].pid = atoi(&input);
                    }
                    else
                    {
                        firstElement = FALSE;
                    }

                    //storing entire value into pid
                    if (input != ' ' && input != '\n')
                    {
                        while (((input = fgetc(file)) != ' '))
                        {
                            elements[numPid].pid = (elements[numPid].pid * 10) + atoi(&input);
                        }
                    }

                    input = fgetc(file);
                    //storing entire value into burst
                    elements[numPid].burst = atoi(&input);
                    while (((input = fgetc(file)) != ' '))
                    {
                        elements[numPid].burst = (elements[numPid].burst * 10) + atoi(&input);
                    }

                    input = fgetc(file);
                    //storing entire value into priority
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

                //calculates the statistics
                calculate(numInstr, execElems, elements, &nvcSwitch, &vcSwitch, &waitingTime, &turnaround, &responseTime, &throughput);
                //prints stats
                printf("%d\n%d\n%.2f\n%.2f\n%.2f\n%.2f\n%.2f\n", vcSwitch, nvcSwitch,  cpu, throughput, turnaround, waitingTime, responseTime);
            }
        }
    }
    fclose(file); //close file

    return 0;
}
