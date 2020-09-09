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
    int population;                                         // i possibly not needed?
    

    fscanf(inFile, "%d %*s", monsterCount);

    // Creates an array of structures
    monsterList = (struct monster**) malloc(*monsterCount * (sizeof(struct monster)));

    for(int i = 0; i < *monsterCount; i++)
    {
        // Fills monster list structures with data from each monster
        fscanf(inFile, "%s %s %d", name, element, &population);
        monsterList[i] = createMonster(name, element, population);
    }

/*
    printf("\n\nFROM MONSTERLIST: -----------\n\n");

   for(i = 0; i < *monsterCount; i ++)                                 // X
    {
        printf("%s %s %d\n", monsterList[i]->name, monsterList[i]->element, monsterList[i]->population);
    }

    printf("\n");                                                       // X

*/
  
    return monsterList;
}

region* createRegion(char *name, int nMonsters, int *monsterCount, char **rMonsterName, monster **monsterList)
{
    int i, j, nameLen, popTotal = 0;

    nameLen = strlen(name) + 1;

    // Allocates memory for each member of structure
    region *newRegion = (struct region*) malloc(sizeof(struct region));
    newRegion->name = (char*) malloc(nameLen * sizeof(char));
    newRegion->nmonsters = (int) malloc(sizeof(int));
    newRegion->total_population = (int) malloc(sizeof(int));
    newRegion->monsters = (struct monster**) malloc(nMonsters * sizeof(struct monster));


    // Assigns values to structure members
    strncpy(newRegion->name, name, nameLen);
    newRegion->nmonsters = nMonsters;
   
    // Steps through list of monster's names in region
    for(i = 0; i < newRegion->nmonsters; i++)
    {
        
        // Compares monster name with names in monster master list
        for(j = 0; j < *monsterCount; j++)
        {
            // Assigns values to region monsters structure
            // if/when the monster's name is found in monsterList
            if(strcmp(rMonsterName[i], monsterList[j]->name) == 0)
            {
                
                newRegion->monsters[i] = createMonster(monsterList[j]->name, monsterList[j]->element, monsterList[j]->population);
                newRegion->monsters[i]->population = monsterList[j]->population;
                popTotal += monsterList[j]->population;
            }
        }
    }

    newRegion->total_population = popTotal;  

    return newRegion;
}

region** readRegions(FILE* inFile, int *regionCount, int *monsterCount, monster **monsterList)
{
    region **regionList;
    monster **regionMonsters;
    char regionName[MAXCHAR], monsterName[MAXCHAR], **rMonsterName;
    int nMonsters;

    // Gets number of regions
    fscanf(inFile, "%d %*s", regionCount);
    //printf("%i regions\n\n", *regionCount);        // X

    regionList = (struct region**) malloc(*regionCount * sizeof(struct region));

    // Iterates through regions
    for(int i = 0; i < *regionCount; i++)
    {
        // Reads data for region[i]
        fscanf(inFile, "%s", regionName);
        fscanf(inFile, "%d %*s", &nMonsters);

        // Allocates memory for array of strings holding list of
        // monsters found in each region
        rMonsterName = (char**) malloc(nMonsters * sizeof(char*));

        // Fills array of strings with each monster name
        for(int j = 0; j < nMonsters; j++)
        {
            
            fscanf(inFile, "%s", monsterName);
            rMonsterName[j] = (char*) malloc((strlen(monsterName) + 1) * sizeof(char*));
            strcpy(rMonsterName[j], monsterName);
        }

        // Fills array of structures with appropriate data pertaining to each region
        regionList[i] = createRegion(regionName, nMonsters, monsterCount, rMonsterName, monsterList);

    }
/*
    printf("\n\nInfo from newRegion!:-----------\n\n\n");
    for(int i = 0; i < *regionCount; i++)
    {
        printf("%s\n", regionList[i]->name);
        printf("%i monsters\n", regionList[i]->nmonsters);
        
        for(int j = 0; j < regionList[i]->nmonsters; j++)
        {
            printf("%s\n", regionList[i]->monsters[j]->name);
        }
        printf("Total population: %i\n\n", regionList[i]->total_population);
    }
*/

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


    return 0;
}
