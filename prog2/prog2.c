/* COP 3502C Assignment 2
This program is written by: Kyle Crisp */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "leak_detector_c.h"

#define MAXCHAR 51
#define MAXGROUNDS 10

typedef struct groundInfo
{
    int groundNumber;
    char *groundName;
    int numSoldiers;
    int numToSkip;
    int numKeepAlive;
} groundInfo;

typedef struct killQueue
{
    struct soldierNode *head;
    struct soldierNode *tail;
    struct groundInfo *queueInformation;
    int numElements;
} killQueue;

typedef struct soldierNode
{
    int soldierNum;
    struct soldierNode *next;
    struct soldierNode *last;
} soldierNode;



int readNumGrounds(FILE* input)
{
    int numGrounds;
    
    fscanf(input, "%d", &numGrounds);

    return numGrounds;
}

struct groundInfo* allocateArrayOfGrounds(int numGrounds)
{
    struct groundInfo *groundInfo = (struct groundInfo*) malloc(numGrounds * (sizeof(struct groundInfo)));

    return groundInfo;
}

struct killQueue** allocateArrayOfQueues(void)
{
    killQueue **newQueueList = (struct killQueue**) malloc(MAXGROUNDS * sizeof(struct killQueue*));

    return newQueueList;
}

char* allocateNameLength(int stringLen)
{
    char *newString = (char*) malloc(stringLen * (sizeof(char)));

    return newString;
}

struct groundInfo readGroundData(FILE* input)
{
    struct groundInfo groundInfo;
    char groundName[MAXCHAR];

    fscanf(input, "%d %s %d %d %d", &groundInfo.groundNumber, groundName, 
                                    &groundInfo.numSoldiers, &groundInfo.numToSkip, 
                                    &groundInfo.numKeepAlive);
    
    groundInfo.groundName = allocateNameLength(strlen(groundName));
    strcpy(groundInfo.groundName, groundName);

    return groundInfo;
}

struct killQueue* initQueue(void)
{
    struct killQueue *initialQueue = (struct killQueue*) malloc(sizeof(struct killQueue));

    // numElements is used as a catch to break in phaseTwo
    initialQueue->numElements = 0;

    initialQueue->head = NULL;
    initialQueue->tail = NULL;

    return initialQueue;
}

struct killQueue enQueue(killQueue queue, int soldierNumber)
{
    soldierNode *soldier = (struct soldierNode*) malloc(sizeof(struct soldierNode));
    soldierNode *traverseNode;

    soldier->soldierNum = soldierNumber;
    soldier->last = NULL;
    soldier->next = NULL;

    if(queue.head == NULL)  // Block for queues with no elements
    {
        queue.head = soldier;
        queue.tail = soldier;
    }
    else    // Otherwise insert element at end of list
    {
        traverseNode = queue.head;

        while(traverseNode != queue.tail)
            {
                traverseNode = traverseNode->next;      // Begins traversing to the queue tail
            }
        
        traverseNode->next = soldier;

        // Links new element to tail
        soldier->next = queue.head;
        soldier->last = traverseNode;

        // Makes new element the tail, and connects the circular list together
        queue.tail = soldier;
        queue.head->last = queue.tail;
    }

    soldier = NULL;
    free(soldier);

    return queue;
}

struct killQueue* fillQueue(int numSoldiers)
{
    struct killQueue* queueToFill = (struct killQueue*) malloc(numSoldiers * (sizeof(struct killQueue)));

    // Using double loop so the queue is filled with descending ints from numSoldiers to 0
    for(int i = 0; i < numSoldiers; i++)
    {
        for(int soldierNum = numSoldiers; soldierNum > 0; soldierNum--)
        {
            queueToFill[i] = enQueue(queueToFill[i], soldierNum);
        }
    }

    return queueToFill;
}

void display(killQueue queue, FILE *output)
{
    soldierNode *traverseNode;

    traverseNode = queue.head;

    while(traverseNode != queue.tail)   // Travels the queue and prints each value
    {
        printf(" %d", traverseNode->soldierNum);
        fprintf(output, " %d", traverseNode->soldierNum);
        traverseNode = traverseNode->next;
    }

    // Because the loop stops just before the tail,
    // a final printf is used to print the tails' data
    printf(" %d", queue.tail->soldierNum);
    fprintf(output, " %d", queue.tail->soldierNum);
    
}

bool isEmpty(killQueue *queue)
{
    if(queue->numElements == 0) 
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Function copies information into the killQueue queues
// This is done because the killQueue array is in the desired order in relation to ground numbers,
// while the original groundInfo array is not in order in relation to ground numbers
struct groundInfo* groundInformationTransfer(struct groundInfo initialInfo)
{
    groundInfo* queueInformation = (struct groundInfo*) malloc(sizeof(struct groundInfo));

    queueInformation->groundName = allocateNameLength(strlen(initialInfo.groundName + 1));

    strcpy(queueInformation->groundName, initialInfo.groundName);
    queueInformation->groundNumber = initialInfo.groundNumber;
    queueInformation->numKeepAlive = initialInfo.numKeepAlive;
    queueInformation->numSoldiers = initialInfo.numSoldiers;
    queueInformation->numToSkip = initialInfo.numToSkip;

    return queueInformation;
}

void reverseQueueOrder(killQueue *queue)
{
    soldierNode *leadingNode, *trailingNode;

    leadingNode = queue->head->next;
    trailingNode = queue->head;

    while(leadingNode != queue->tail) // Traverses through queue and swaps the node pointers to reverse the order
    {
        trailingNode->next = trailingNode->last;
        trailingNode->last = leadingNode;
        trailingNode = leadingNode;
        leadingNode = leadingNode->next;
    }
    trailingNode->next = trailingNode->last; // Swaps node pointers for tail and node just before tail
    trailingNode->last = leadingNode;        
                                            
    leadingNode->next = trailingNode;
    leadingNode->last = queue->head;

    queue->tail = queue->head;  // Swaps queue and tail designations
    queue->head = leadingNode;
}

void phaseOne(killQueue *queue, FILE *outfile)
{
    soldierNode *killNode;
    soldierNode *deadSoldier;

    // Creates new variables to store information for readability purposes
    int soldiers = queue->queueInformation->numSoldiers;
    int keepAlive = queue->queueInformation->numKeepAlive;
    int killSkip = queue->queueInformation->numToSkip;

    killNode = queue->head; // Points to head...

    // and begins moving through queue based on the number
    // of soldiers to skip in a given ground.
    // This intial loop is for the first execution because
    // killNode is pointing to the head to begin with, and that counts as a step
    for(int i = 1; i < (killSkip - 1); i++)
    {                                          
        killNode = killNode->next;
    }

    // Isolates deadSoldier node by moving node pointers
    deadSoldier = killNode->next;
    killNode->next = killNode->next->next;
    deadSoldier->next->last = killNode;


    printf("\nSoldier# %d executed\n", deadSoldier->soldierNum);
    fprintf(outfile, "\nSoldier# %d executed\n", deadSoldier->soldierNum);

    free(deadSoldier);
    queue->numElements -= 1; // Counter to keep track of how many elements are in the queue currently



    // For loop conditional check is -1 due to the initial execution
    // before this kill block begins
    for(int i = 0; i < ((soldiers - keepAlive) - 1); i++)
    {
        for(int j = 0; j < (killSkip - 1); j++)
        {
            killNode = killNode->next;
        }
        
        // Isolates deadSoldier node by moving node pointers
        deadSoldier = killNode->next;
        killNode->next = deadSoldier->next;
        deadSoldier->next->last = killNode;


        if(deadSoldier == queue->head)
        {
            queue->head = deadSoldier->next;    // Move head designation to one node infront of deadSoldier
        }
        else if(deadSoldier == queue->tail)
        {
            queue->tail = killNode; // Move tail designation to one node behind deadSoldier
        }

        printf("Soldier# %d executed\n", deadSoldier->soldierNum);
        fprintf(outfile, "Soldier# %d executed\n", deadSoldier->soldierNum);

        free(deadSoldier);
        queue->numElements -= 1;
    }
}

int peek(killQueue *queue)
{
    if(!isEmpty(queue))
    {
        return queue->head->soldierNum;
    }
    else
    {
        return -1;
    }  
}

int findGroundIndexOfMax(killQueue **groundQueueList)
{
    int temp, max, groundIndex;
    bool findLowestGroundNumber;
    max = -1;


    for(int i = 0; i < MAXGROUNDS; i++)
    {
        if(!isEmpty(groundQueueList[i]))    // If queue has members
        {
            temp = peek(groundQueueList[i]);  

            if(max < temp) // Find max value
            {
                max = temp;
                groundIndex = i;
            }
            else if(max == temp)
            {
                findLowestGroundNumber = true;  // Conditional to account for when the queue array needs to be looped again
            }
        }
    }

    if(findLowestGroundNumber)  // Finds first ground with desired max value
    {
        for(int i = 0; i < MAXGROUNDS; i++)
        {
            if(!isEmpty(groundQueueList[i]))
            {
                if(peek(groundQueueList[i]) == max)
                {
                    return i;
                }
            }
        }  
    } 

    return groundIndex;
}

// deQueue used in phaseTwo
void deQueue(killQueue *queue, FILE *output)
{
    soldierNode *deadSoldier;

    deadSoldier = queue->head;

    queue->head = queue->head->next;
    if(queue->head != queue->tail)  // Conditonal to manage deletion of nodes that are neither the head or tail
    {
        queue->head->last = deadSoldier->last;
        deadSoldier->last->next = queue->head;
    }

    free(deadSoldier);    
    queue->numElements -= 1;

    printf("Executed Soldier %i", deadSoldier->soldierNum);
    fprintf(output, "Executed Soldier %i", deadSoldier->soldierNum);
}

int countQueuesWithMembers(killQueue **groundQueueList)
{ 
    int count = 0;

    for(int i = 0; i < MAXGROUNDS; i++)
    {
        if(!isEmpty(groundQueueList[i]))
            count++;
    }

    return count;
}

void phaseTwo(killQueue **groundQueueList, FILE *output)
{
    bool lastSoldier = false;
    int groundIndex = 0;

    while(!lastSoldier)
    {
        groundIndex = findGroundIndexOfMax(groundQueueList); // Finds index of array with max soldier nubmer

        deQueue(groundQueueList[groundIndex], output);  // Executes that soldier
        printf(" from line %i\n", groundIndex);
        fprintf(output, " from line %i\n", groundIndex);

        // Checks if we have found the survivor or not
        if(countQueuesWithMembers(groundQueueList) == 1 && groundQueueList[groundIndex]->numElements == 1)
        {
            lastSoldier = true;
        }   
    }

        printf("\nSoldier %i from line %i will survive\n", groundQueueList[groundIndex]->head->soldierNum,
                                                           groundIndex);
        fprintf(output, "\nSoldier %i from line %i will survive\n", groundQueueList[groundIndex]->head->soldierNum,
                                                                    groundIndex);

        free(groundQueueList[groundIndex]->head);
}


void formattedOutput(killQueue **groundQueueList, FILE *output)
{
    for(int i = 0; i < MAXGROUNDS; i++)
    {
        if(!isEmpty(groundQueueList[i]))
        {
            printf("\n%i %s", groundQueueList[i]->queueInformation->groundNumber,
                              groundQueueList[i]->queueInformation->groundName);
            fprintf(output, "\n%i %s", groundQueueList[i]->queueInformation->groundNumber,
                                       groundQueueList[i]->queueInformation->groundName);                  
            display(*groundQueueList[i], output);
        }
    }
}
int main(void)
{
    atexit(report_mem_leak);

    FILE *input, *output;
    int numGrounds;

    groundInfo *groundInformation;
    killQueue **groundQueueList;

    input = fopen("int.txt", "r");
    output = fopen("out.txt", "w");

    if(input == NULL)
    {
        printf("Please provide a valid input file.\n");
    }
    else
    {
        // Allocates an array of structures to hold information for each ground
        numGrounds = readNumGrounds(input);
        groundInformation = allocateArrayOfGrounds(numGrounds);

        // Allocates data for a maximum of 10 Queues/Grounds
        // This is allocated to 10 regardless of input
        groundQueueList = allocateArrayOfQueues();

        // Reads information into ground info structure
        for(int i = 0; i < numGrounds; i++)
        {
            groundInformation[i] = readGroundData(input);
        }


        // Initalizes each queue with appropiate data read from input file
        // Places queue in index specified by ground number
        for(int i = 0; i < MAXGROUNDS; i++)
        {
            groundQueueList[i] = initQueue();
            
            for(int j = 0; j < numGrounds; j++)
            {
                if(i == groundInformation[j].groundNumber)
                {
                    groundQueueList[i] = fillQueue(groundInformation[j].numSoldiers);
                    groundQueueList[i]->queueInformation = groundInformationTransfer(groundInformation[j]);
                    groundQueueList[i]->numElements = groundQueueList[i]->queueInformation->numSoldiers;
                }                
            }
        }   

        printf("Initial nonempty lists status");
        fprintf(output, "\nInitial nonempty lists status");
        formattedOutput(groundQueueList, output);

        for(int i = 0; i < MAXGROUNDS; i++)
            if(!isEmpty(groundQueueList[i]))
                reverseQueueOrder(groundQueueList[i]);
        
        printf("\n\nAfter ordering nonempty lists status");
        fprintf(output, "\n\nAfter ordering nonempty lists status");
        formattedOutput(groundQueueList, output);

        printf("\nPhase1 execution\n");
        fprintf(output, "\n\nPhase1 execution\n");
        // Begin PhaseOne
        for(int i = 0; i < MAXGROUNDS; i++)
        {
            if(!isEmpty(groundQueueList[i]))
            {
                printf("Line# %i %s\n", i, groundQueueList[i]->queueInformation->groundName);
                fprintf(output, "\nLine# %i %s", i, groundQueueList[i]->queueInformation->groundName);
                phaseOne(groundQueueList[i], output); 
            }
        }

        printf("\nPhase2 execution\n");
        fprintf(output, "\nPhase2 execution\n");
        phaseTwo(groundQueueList, output);   

    }
    return 0;
}