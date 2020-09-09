        /* COP 3502C Assignment 1
This program is written by: Kyle Crisp */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCHAR 50

typedef struct monster {
    int id;
    char *name;
    char *element;
    int population;
} monster;

typedef struct region {
    char *name;
    int nmonsters;
    int total_population;
    monster **monsters;
} region;

typedef struct itinerary {
    int nregions;
    region **regions;
    int captures;
} itinerary;

typedef struct trainer {
    char *name;
    itinerary *visits;
} trainer;

// Takes data read from input file and fills a monster structure
// with the appropiate data
monster* createMonster(char *name, char *element, int population)
{
    int nameLen, elementLen;

    // Assigns string lengths to a variable for ease of use
    nameLen = strlen(name) + 1;
    elementLen = strlen(element) + 1;

    // Allocates new memory for new monster structure as a whole
    monster *newMonster = (struct monster*) malloc(sizeof(struct monster));

    // Allocates memory space for each structure member
    newMonster->name = (char*) malloc(nameLen * sizeof(char));
    newMonster->element = (char*) malloc(elementLen * sizeof(char));
    newMonster->population = (int) malloc(sizeof(int));

    // Assigns values to structure members
    strncpy(newMonster->name, name, nameLen);
    strncpy(newMonster->element, element, elementLen);
    newMonster->population = population;

    return newMonster;
}

// Function reads data from input file,
// allocates space for array of structures,
// and calls createMonster to fill structures.
monster** readMonsters(FILE* inFile, int *monsterCount)
{
    monster **monsterList, *monster;
    char name[MAXCHAR], element[MAXCHAR];
    int i, population;                                         // i possibly not needed?
    

    fscanf(inFile, "%d %*s", monsterCount);

    // Creates an array of structures
    monsterList = (struct monster**) malloc(*monsterCount * (sizeof(struct monster)));

    for(i = 0; i < *monsterCount; i ++)
    {
        fscanf(inFile, "%s %s %d", name, element, &population);
        monsterList[i] = createMonster(name, element, population);
    }

/*
   for(i = 0; i < *monsterCount; i ++)                                 // X
    {
        printf("%s %s %d\n", monsterList[i]->name, monsterList[i]->element, monsterList[i]->population);
    }

    printf("\n");                                                       // X
*/

    return monsterList;
}

region* createRegion(char *name, int nMonsters, int population, monster **monsterList)
{
    region *newRegion;





    return newRegion;
}

region** readRegions(FILE* inFile, int *regionCount, int *monsterCount, monster **monsterList)
{
    region **regionList, *region;
    monster **regionMonsters;
    char regionName[MAXCHAR], monsterName[MAXCHAR], **rMonsterList;
    int nMonsters, population;

    // Gets number of regions
    fscanf(inFile, "%d %*s", regionCount);
    printf("%i regions\n\n", *regionCount);

    regionList = (struct region**) malloc(*regionCount * sizeof(struct region));

    // Iterates through regions
    for(int i = 0; i < *regionCount; i++)
    {
        // Reads data for region[i]
        fscanf(inFile, "%s", regionName);
        fscanf(inFile, "%d %*s", &nMonsters);

        printf("%s\n", regionName);
        printf("%d monsters\n", nMonsters);

        rMonsterList = (char**) malloc(nMonsters * sizeof(char*));

        for(int j = 0; j < nMonsters; j++)
        {
            fscanf(inFile, "%s", monsterName);
            rMonsterList[j] = (char*) malloc((strlen(monsterName) + 1) * sizeof(char));
            strcpy(rMonsterList[j], monsterName);

            printf("%s\n", rMonsterList[j]);
        }
        printf("\n\n");

    }

    return regionList;
}

/*

itinerary createItinerary(int nRegions, int captures, region **regions)
{

}

trainer* createTrainer(char *name, itinerary *visits)
{

}


trainer** readTrainer(FILE* infile, int *numTrainers)
*/


int main(void)
{
    monster **monsterList;
    region **regionList;
    FILE *input, *output;
    int monsterCount, regionCount;
    char region[MAXCHAR], type[MAXCHAR], strText[MAXCHAR];
    int numFound;

    // Opens file, and checks if the file is == NULL
    // If != NULL, begins reading data from input file
    input = fopen("in.txt", "r");
    if(input == NULL)
    {
        printf("Please provide a valid input file");
        exit(-1);
    }
    else
    {
        monsterList = readMonsters(input, &monsterCount);
        regionList = readRegions(input, &regionCount, &monsterCount, monsterList);
    }
    
/*
    printf("\n");
    fscanf(input, "%d %s", &numInt, strText);
    printf("%i %s\n\n", numInt, strText);

    for(i = 0; i < numInt; i ++)
    {
        fscanf(input, "%s %s %d", region, type, &numFound);
        printf("%s %s %d\n", region, type, numFound);
    }
    printf("\n");
*/
    

    return 0;
}
