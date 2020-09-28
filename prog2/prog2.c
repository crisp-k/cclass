#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCHAR 51

typedef struct groundInfo
{
    int groundNumber;
    char *groundName;
    int numSoldiers;
    int numToSkip;
    int numKeepAlive;
} groundInfo;

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

struct groundInfo* allocateArrayOfGrounds(int numGrounds)
{
    struct groundInfo *groundInfo = (struct groundInfo*) malloc(numGrounds * (sizeof(struct groundInfo)));

    return groundInfo;
}

char* allocateNameLenghth(int stringLen)
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

    groundInfo *executionGrounds;

    input = fopen("int.txt", "r");
    output = fopen("out.txt", "w");

    if(input == NULL)
    {
        printf("Please provide a valid input file.\n");
    }
    else
    {
        numGrounds = readNumGrounds(input);
        executionGrounds = allocateArrayOfGrounds(numGrounds);

        for(int i = 0; i < numGrounds; i++)
        {
            executionGrounds[i] = readGroundData(input);
            printf("Ground Name: %s\nGround number: %d\nNumber of Soldiers: %d\n\n", executionGrounds[i].groundName,
                                                                                     executionGrounds[i].groundNumber,
                                                                                     executionGrounds[i].numSoldiers);
        }

        // for(int i = 0; i < numGroups; i++)
        // {
        //     fscanf(input, "%d %s %d %d %d", &groundNumber, groundName, &numSoldiers, &numToSkip, &numKeepAlive);
        //     printf("%d %s %d %d %d\n", groundNumber, groundName, numSoldiers, numToSkip, numKeepAlive);
        // }
    }

    return 0;
}