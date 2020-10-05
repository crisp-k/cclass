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


    if(queue.head == NULL)
    {
        queue.head = soldier;
        queue.tail = soldier;
    }
    else
    {
        traverseNode = queue.head;
        while(traverseNode != queue.tail)
            {
                traverseNode = traverseNode->next;
            }

        traverseNode->next = soldier;
        queue.tail = soldier;
        soldier->next = queue.head;
        soldier->last = traverseNode;
        queue.head->last = soldier;
        
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

void display(killQueue queue)
{
    soldierNode *traverseNode;

    traverseNode = queue.head;

    while(traverseNode != queue.tail)
    {
        printf(" %d", traverseNode->soldierNum);
        traverseNode = traverseNode->next;
    }

    printf(" %d", queue.tail->soldierNum);
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

void formattedDisplay(killQueue **groundQueueList)
{
    for(int i = 0; i < MAXGROUNDS; i++)
    {
        if(!isEmpty(groundQueueList[i]))
        {
            printf("\n%i %s", groundQueueList[i]->queueInformation->groundNumber,
                              groundQueueList[i]->queueInformation->groundName);
            display(*groundQueueList[i]);
        }
    }
}

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

    while(leadingNode != queue->tail)
    {
        trailingNode->next = trailingNode->last;
        trailingNode->last = leadingNode;
        trailingNode = leadingNode;
        leadingNode = leadingNode->next;
    }
    trailingNode->next = trailingNode->last;
    trailingNode->last = leadingNode;

    leadingNode->next = trailingNode;
    leadingNode->last = queue->head;

    queue->tail = queue->head;
    queue->head = leadingNode;
}

void phaseOne(killQueue *queue)
{
    soldierNode *killNode;
    soldierNode *deadSoldier = (soldierNode*) malloc(sizeof(soldierNode));
    int soldiers = queue->queueInformation->numSoldiers;
    int keepAlive = queue->queueInformation->numKeepAlive;
    int killSkip = queue->queueInformation->numToSkip;

    killNode = queue->head;

    for(int i = 1; i < (killSkip - 1); i++)
    {
        killNode = killNode->next;
    }

    deadSoldier = killNode->next;
    killNode->next = killNode->next->next;
    deadSoldier->next->last = killNode;

    printf("\nSoldier# %d executed\n", deadSoldier->soldierNum);
    free(deadSoldier);

    queue->numElements -= 1;

    // For loop conditional check is -1 due to the initial execution
    // before this kill block begins
    for(int i = 0; i < ((soldiers - keepAlive) - 1); i++)
    {
        for(int j = 0; j < (killSkip - 1); j++)
        {
            killNode = killNode->next;
        }
        
        deadSoldier = killNode->next;
        killNode->next = deadSoldier->next;
        deadSoldier->next->last = killNode;
        
        if(deadSoldier == queue->head)
        {
            queue->head = deadSoldier->next; 
        }
        else if(deadSoldier == queue->tail)
        {
            queue->tail = killNode;
        }

        printf("Soldier# %d executed\n", deadSoldier->soldierNum);

        free(deadSoldier);
        queue->numElements -= 1;
    }

    printf("Number of elements in queue: %i\n", queue->numElements);
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
        if(!isEmpty(groundQueueList[i]))
        {
            temp = peek(groundQueueList[i]);
            printf("peak: %i from line %i\n", temp, i);
            if(max < temp)
            {
                max = temp;
                groundIndex = i;
            }
            else if(max == temp)
            {
                findLowestGroundNumber = true;
            }
        }
    }

    if(findLowestGroundNumber)
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

    printf("Line to dequeue from: %i\n", groundIndex);

    return groundIndex;
}

void deQueue(killQueue *queue)
{
    soldierNode *deadSoldier;

    deadSoldier = queue->head;

    queue->head = queue->head->next;
    if(queue->head != queue->tail)
    {
        queue->head->last = deadSoldier->last;
        deadSoldier->last->next = queue->head;
    }

    free(deadSoldier);    
    queue->numElements -= 1;

    printf("Executed Soldier %i", deadSoldier->soldierNum);
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

void phaseTwo(killQueue **groundQueueList)
{
    bool lastSoldier = false;
    int groundIndex = 0;

    while(countQueuesWithMembers(groundQueueList) >= 1)
    {
        
        groundIndex = findGroundIndexOfMax(groundQueueList);

        deQueue(groundQueueList[groundIndex]);
        printf(" from line %i\n", groundIndex);

        printf("%i\n", groundQueueList[groundIndex]->numElements);

        if(countQueuesWithMembers(groundQueueList) == 1 && groundQueueList[groundIndex]->numElements == 1)
            break;
    }
        printf("Soldier %i from line %i will survive\n", groundQueueList[groundIndex]->head->soldierNum,
                                                         groundIndex);
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

        // Reads information into ground info structure
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
                    groundQueueList[i]->numElements = groundQueueList[i]->queueInformation->numSoldiers;
                }                
            }
        }   
        
        printf("\nInitial nonempty lists status");
        for(int i = 0; i < MAXGROUNDS; i++)
        {
            if(!isEmpty(groundQueueList[i]))
            {
                printf("\n%i %s", groundQueueList[i]->queueInformation->groundNumber,
                                  groundQueueList[i]->queueInformation->groundName);
                display(*groundQueueList[i]);

                reverseQueueOrder(groundQueueList[i]);
            }
            
        }
        
        printf("\n\nAfter ordering nonempty lists status");
        for(int i = 0; i < MAXGROUNDS; i++)
        {
            if(!isEmpty(groundQueueList[i]))
            {
                printf("\n%i %s", groundQueueList[i]->queueInformation->groundNumber,
                                  groundQueueList[i]->queueInformation->groundName);
                display(*groundQueueList[i]);
                
                phaseOne(groundQueueList[i]);
            }
        }
        phaseTwo(groundQueueList);   

    }
    return 0;
}