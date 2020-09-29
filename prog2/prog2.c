#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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


    if(queue.head == NULL)
    {
        queue.head = soldier;
        queue.tail = soldier;
    }
    else
    {
        traverseNode = queue.head;
        while(traverseNode->next != NULL)
            {
                traverseNode = traverseNode->next;
            }

        traverseNode->next = soldier;
        queue.tail = soldier;
        soldier->last = traverseNode;
        
    }
    
    return queue;
}

struct killQueue* fillQueue(int numSoldiers)
{
    struct killQueue* queueToFill = (struct killQueue*) malloc(numSoldiers * (sizeof(struct killQueue)));

    for(int i = 0; i < numSoldiers; i++)
    {
        for(int j = numSoldiers; j > 0; j--)
        {
            queueToFill[i] = enQueue(queueToFill[i], j);
        }
    }

    return queueToFill;
}

void display(soldierNode *node)
{
    while(node != NULL)
    {
        printf("Soldier Number: %d\n", node->soldierNum);
        node = node->next;
    }
}

bool isEmpty(killQueue *queue)
{
    if(queue->head == queue->tail)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*  ... Not working...  */
// void freeGroundInfoStructure(groundInfo *unusedStructure, int numStructs)
// {
//     for(int i; i < numStructs; i++)
//     {
//         free(unusedStructure[i].groundName);
//     }
//         free(unusedStructure);

//     return ;
// }


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

int main(void)
{
    FILE *input, *output;
    char groundName[MAXCHAR];
    int numGrounds, groundNumber;
    int numToSkip, numKeepAlive;

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

        for(int i = 0; i < numGrounds; i++)
        {
            groundInformation[i] = readGroundData(input);
        }

        // Initalizes each queue with appropiate data read from input file
        for(int i = 0; i < MAXGROUNDS; i++)
        {
            groundQueueList[i] = initQueue();
            
            for(int j = 0; j < numGrounds; j++)
            {
                if(i == groundInformation[j].groundNumber)
                {
                    groundQueueList[i] = fillQueue(groundInformation[j].numSoldiers);
                    groundQueueList[i]->queueInformation = groundInformationTransfer(groundInformation[j]);
                }                
            }
                //  freeGroundInfoStructure(groundInformation, numGrounds);     ...Not Working...

            if(!isEmpty(groundQueueList[i]))
            {
                printf("Ground name: %s\n", groundQueueList[i]->queueInformation->groundName);
                display(groundQueueList[i]->head);
            }
        }


        
        

    }

    return 0;
}