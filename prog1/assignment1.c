        /* COP 3502C Assignment 1
This program is written by: Kyle Crisp */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "leak_detector_c.h"

#define MAXCHAR 51

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
monster* createMonster(char *name, char *element, int *population)
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
    newMonster->population = *((int*) malloc(sizeof(int)));

    // Assigns values to structure members
    strncpy(newMonster->name, name, nameLen);
    strncpy(newMonster->element, element, elementLen);
    newMonster->population = *population;

    return newMonster;
}

// Function reads data from input file,
// allocates space for array of structures,
// and calls createMonster to fill structures.
monster** readMonsters(FILE* inFile, int *monsterCount)
{
    monster **monsterList, *monster;
    char name[MAXCHAR], element[MAXCHAR];
    int population;
    

    fscanf(inFile, "%d %*s", monsterCount);

    // Creates an array of structures
    monsterList = (struct monster**) malloc(*monsterCount * (sizeof(struct monster)));

    for(int i = 0; i < *monsterCount; i++)
    {
        // Fills monster list structures with data from each monster
        fscanf(inFile, "%s %s %d", name, element, &population);
        monsterList[i] = createMonster(name, element, &population);
    }
  
    return monsterList;
}


region* retrieveRegion(region **regionList, int regionIndex)
{
    region *foundRegion = (struct region*) malloc(sizeof(struct region));

    foundRegion->monsters = (struct monster**) malloc((regionList[regionIndex]->nmonsters) * sizeof(struct monster*));

    for(int i = 0; i < regionList[regionIndex]->nmonsters; i++)
    {
        foundRegion->monsters[i] = regionList[regionIndex]->monsters[i];
    }
    
    foundRegion->name = regionList[regionIndex]->name;
    foundRegion->nmonsters = regionList[regionIndex]->nmonsters;
    foundRegion->total_population = regionList[regionIndex]->total_population;

    return foundRegion;
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
                
                newRegion->monsters[i] = createMonster(monsterList[j]->name, monsterList[j]->element, &monsterList[j]->population);
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

    for(int i = 0; i < nMonsters; i++)
        free(rMonsterName[i]);

    free(rMonsterName);

    return regionList;
}

itinerary* createItinerary(int nRegions, int nCaptures)
{
    itinerary *newItinerary = (struct itinerary*) malloc(sizeof(struct itinerary));

    newItinerary->nregions = nRegions;
    newItinerary->captures = nCaptures;

    newItinerary->regions = (struct region**) malloc(nRegions * sizeof(struct region*));

    return newItinerary;
}

trainer* createTrainer(char *name)
{
    trainer *newTrainer = (struct trainer*) malloc(sizeof(struct trainer));
    int nameLen = (strlen(name) + 1);

    newTrainer->name = (char*) malloc(nameLen * sizeof(char));
    strcpy(newTrainer->name, name);
    newTrainer->visits = (struct itinerary*) malloc(sizeof(struct itinerary));


    return newTrainer;
}

trainer** readTrainers(FILE* inFile, int *trainerCount, int **nCaptures, int **nRegions, char ****tRegionNames)
{
    trainer **trainerList;
    int i;
    int *tempCaptures, *tempRegions;
    char ***tempRegionNames, regionName[MAXCHAR];

    char trainerName[MAXCHAR];

    fscanf(inFile, "%d %*s", trainerCount);

    trainerList = (struct trainer **) malloc(*trainerCount * sizeof(struct trainer*));
    tempCaptures = (int*) malloc(*trainerCount * sizeof(int));
    tempRegions = (int*) malloc(*trainerCount * sizeof(int));
    tempRegionNames = (char***) malloc(*trainerCount * sizeof(char**));

    for(int i = 0; i < *trainerCount; i++)
    {
        fscanf(inFile, "%s", trainerName);
        fscanf(inFile, "%d %*s", &tempCaptures[i]);
        fscanf(inFile, "%d %*s", &tempRegions[i]);

        tempRegionNames[i] = (char**) malloc(tempRegions[i] * sizeof(char*));

        for(int j = 0; j < tempRegions[i]; j++)
        {

            fscanf(inFile, "%s", regionName);
            tempRegionNames[i][j] = (char*) malloc((strlen(regionName)+ 1) * sizeof(char));
            strcpy(tempRegionNames[i][j], regionName);     
        }

        trainerList[i] = createTrainer(trainerName);

    }

    *nCaptures = tempCaptures;
    *nRegions = tempRegions;
    *tRegionNames = tempRegionNames;


    return trainerList;
}

float estimateCaptures(int trainerCapture, int total_population, int monsterPop)
{
    float percentPop = (float) monsterPop/total_population;
    float expectCapture = percentPop * trainerCapture;

    expectCapture = round(expectCapture);

    return expectCapture;
}

void freeMonsterList(monster **monsterList, int monsterCount)
{
    for(int i = 0; i < monsterCount; i++)
    {
        free(monsterList[i]->name);
        free(monsterList[i]->element);
        //free(monsterList[i]->population);
        free(monsterList[i]);
    }

    free(monsterList);
}

void freeRegionList(region **regionList, int regionCount)
{
    for(int i = 0; i < regionCount; i++)
    {
        for(int j = 0; j < regionList[i]->nmonsters; j++)
            free(regionList[i]->monsters[j]);

        free(regionList[i]);
    }

    free(regionList);
}

void freeTrainerList(trainer ** trainerList, int trainerCount)
{
    for(int i = 0; i < trainerCount; i++)
    {
        for(int j = 0; j < trainerList[i]->visits->nregions; j++)
            {
                free(trainerList[i]->visits->regions[j]);
            }
        
        free(trainerList[i]->visits);
        free(trainerList[i]);
    }

    //free(trainerList);
}

int main(void)
{
    atexit(report_mem_leak);

    monster **monsterList;
    region **regionList;
    trainer **trainerList;
    FILE *input, *output;
    
    int monsterCount, regionCount, trainerCount;
    int *nCaptures, *nRegions;
    float ***expectedCapture;
    char region[MAXCHAR], type[MAXCHAR], strText[MAXCHAR];
    char ***tRegionNames;
    int i, j, k;

    // Opens file, and checks if the file is == NULL
    // If != NULL, begins reading data from input file
    input = fopen("in.txt", "r");
    output = fopen("out.txt", "w");
    if(input == NULL)
    {
        printf("Please provide a valid input file");
        exit(-1);
    }
    else
    {
        monsterList = readMonsters(input, &monsterCount);
        regionList = readRegions(input, &regionCount, &monsterCount, monsterList);
        trainerList = readTrainers(input, &trainerCount, &nCaptures, &nRegions, &tRegionNames);

        expectedCapture = (float***) malloc(trainerCount * sizeof(float**));

        for(i = 0; i < trainerCount; i++)
        {
            trainerList[i]->visits = createItinerary(nRegions[i], nCaptures[i]);

            expectedCapture[i] = (float**) malloc(nRegions[i] * sizeof(float*));


            printf("%s\n", trainerList[i]->name);
            fprintf(output, "%s\n", trainerList[i]->name);


            for(j = 0; j < nRegions[i]; j++)
            {
                for(k = 0;  k < regionCount; k++)
                {
                    if(strcmp(tRegionNames[i][j], regionList[k]->name) == 0)
                    {
                        trainerList[i]->visits->regions[j] = retrieveRegion(regionList, k);
                    }
                }

                printf("%s\n", trainerList[i]->visits->regions[j]->name);
                fprintf(output, "%s\n", trainerList[i]->visits->regions[j]->name);

                expectedCapture[i][j] = (float*) malloc((trainerList[i]->visits->regions[j]->nmonsters) * sizeof(float));

                for(k = 0; k < trainerList[i]->visits->regions[j]->nmonsters; k++)
                {
                    expectedCapture[i][j][k] = estimateCaptures(trainerList[i]->visits->captures, 
                                                       trainerList[i]->visits->regions[j]->total_population,
                                                       trainerList[i]->visits->regions[j]->monsters[k]->population);
                    
                    if(expectedCapture[i][j][k] > 0){
                        printf("%.0f %s\n", expectedCapture[i][j][k], trainerList[i]->visits->regions[j]->monsters[k]->name);
                        fprintf(output, "%.0f %s\n", expectedCapture[i][j][k], trainerList[i]->visits->regions[j]->monsters[k]->name);
                    }
                }
            }

            if(i < trainerCount - 1)
            {
                printf("\n");
                fprintf(output, "\n");
            }
        }
    }


/*
    printf("trainerCount: %i\n", trainerCount);

    printf("-----INSIDE MAIN-------\n");
    printf("n captures for each trainer\n");
    for(i = 0; i < trainerCount; i++)
    {
        printf("%i ", nCaptures[i]);
    }

    printf("\n\nn regions for each trainer\n");
    for(i = 0; i < trainerCount; i++)
    {
        printf("%i ", nRegions[i]);
    }
    printf("\n");

    for(i = 0; i < trainerCount; i++)
    {
        for(j = 0; j < nRegions[i]; j++)
        {
            printf("tempRegionNames[%i][%i]: %s\n", i, j, tRegionNames[i][j]);
        }
    }

    for(i = 0; i < trainerCount; i++)
    {
        printf("%s\n", trainerList[i]->name);
    
        printf("trainerList[%i]->visits->nregions: %i\n", i, trainerList[i]->visits->nregions);
        printf("trainerList[%i]->visits->captures: %i\n", i , trainerList[i]->visits->captures);

        for(j = 0; j < nRegions[i]; j++)
        {
            printf("\ntrainerList[%i]->visits->regions[%i]->name: %s\n", i, j, trainerList[i]->visits->regions[j]->name);
            printf("trainerList[%i]->visits->regions[%i]->nmonsters: %i\n", i, j, trainerList[i]->visits->regions[j]->nmonsters);
            printf("trainerList[%i]->visits->regions[%i]->tot_pop: %i\n", i, j, trainerList[i]->visits->regions[j]->total_population);

            for(k = 0; k < trainerList[i]->visits->regions[j]->nmonsters; k++)
            {
               
                printf("trainerList[%i]->visits->regions[%i]->monsters: %s\n", i, j, trainerList[i]->visits->regions[j]->monsters[k]->name);
                printf("trainerList[%i]->visits->regions[%i]->monsters: %s\n", i, j, trainerList[i]->visits->regions[j]->monsters[k]->element);
                printf("trainerList[%i]->visits->regions[%i]->monsters: %i\n", i, j, trainerList[i]->visits->regions[j]->monsters[k]->population);
                printf("Expected Captures[%i][%i][%i]: %.2f\n", i, j, k, expectedCapture[i][j][k]);
            }
        }
    }
*/

    freeMonsterList(monsterList, monsterCount);
    freeRegionList(regionList, regionCount);
    freeTrainerList(trainerList, trainerCount);

    fclose(input);
    fclose(output);

    return 0;
}
