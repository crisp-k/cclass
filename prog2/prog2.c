#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCHAR 51

typedef struct fileInfo
{
    int groundNumber;
    char *groundName;
    int numSoldiers;
    int numToSkip;
    int numKeepAlive;
} fileInfo;

typedef struct ground
{
    char *groundName;
    int numSoldiers, groundNumber;
} ground;

typedef struct soldierNode
{
    int soldierNum;
    struct soldierNode *next;
    struct soldierNode *last;
} soldierNode;

typedef struct killQueue
{
    struct soldierNode *head;
    struct soldierNode *tail;
} killQueue;



int readNumGrounds(FILE* input)
{
    int numGrounds;
            
    fscanf(input, "%d", &numGrounds);


    return numGrounds;
}

struct ground* allocateArrayOfGrounds(int numGrounds)
{
    struct ground *groundArray = (struct ground*) malloc(numGrounds * (sizeof(struct ground)));

    return groundArray;
}

struct fileInfo* allocateArrayOfFileInfo(int numGrounds)
{
    struct fileInfo *fileInfo = (struct fileInfo*) malloc(numGrounds * (sizeof(struct fileInfo)));

    return fileInfo;
}

struct fileInfo readFile(FILE* input)
{
    struct fileInfo groundInfo;
    char groundName[MAXCHAR];

    fscanf(input, "%d %s %d %d %d", groundInfo.groundNumber, groundName, 
                                    groundInfo.numSoldiers, groundInfo.numToSkip, 
                                    groundInfo.numKeepAlive);
    
    groundInfo.groundName = (char*) malloc (strlen(groundName) * (sizeof(char)));
    strcpy(groundInfo.groundName, groundName);

    return groundInfo;
}



int main(void)
{
    FILE *input, *output;
    char groundName[MAXCHAR];
    int numGrounds, groundNumber, numSoldiers;
    int numToSkip, numKeepAlive;

    ground *executionGrounds;
    fileInfo *fileInfo;

    input = fopen("int.txt", "r");
    output = fopen("out.txt", "w");

    if(input == NULL)
    {
        printf("Please provide a valid input file.\n");
    }
    else
    {

        numGrounds = readNumGrounds(input);
        printf("%d\n", numGrounds);

        executionGrounds = allocateArrayOfStruct(numGrounds);
    

        // for(int i = 0; i < numGroups; i++)
        // {
        //     fscanf(input, "%d %s %d %d %d", &groundNumber, groundName, &numSoldiers, &numToSkip, &numKeepAlive);
        //     printf("%d %s %d %d %d\n", groundNumber, groundName, numSoldiers, numToSkip, numKeepAlive);
        // }
    }

    return 0;
}