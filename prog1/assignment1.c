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
    int population;
    
    fscanf(inFile, "%d %*s", monsterCount);

    // Creates an array of structures
    monsterList = (struct monster**) malloc(*monsterCount * (sizeof(struct monster)));

    for(int i = 0; i < *monsterCount; i++)
    {
        // Fills monster list structures with data from each monster
        fscanf(inFile, "%s %s %d", name, element, &population);
        monsterList[i] = createMonster(name, element, population);
    }
  
    return monsterList;
}

// Fills a placeholder structure with information from a
// region structure
region* retrieveRegion(region **regionList, int regionIndex)
{
    // Allocates space to structures
    region *foundRegion = (struct region*) malloc(sizeof(struct region));
    foundRegion->monsters = (struct monster**) malloc((regionList[regionIndex]->nmonsters) * sizeof(struct monster*));

    // Fills structures with data
    for(int i = 0; i < regionList[regionIndex]->nmonsters; i++)
        foundRegion->monsters[i] = regionList[regionIndex]->monsters[i];

    foundRegion->name = regionList[regionIndex]->name;
    foundRegion->nmonsters = regionList[regionIndex]->nmonsters;
    foundRegion->total_population = regionList[regionIndex]->total_population;

    return foundRegion;
}

// Takes read data from file and fills a 
// new region structure accordingly
region* createRegion(char *name, int nMonsters, int *monsterCount, char **rMonsterName, monster **monsterList)
{
    int i, j, nameLen, popTotal = 0;

    nameLen = strlen(name) + 1;

    // Allocates memory for each member of structure
    region *newRegion = (struct region*) malloc(sizeof(struct region));
    newRegion->name = (char*) malloc(nameLen * sizeof(char));
    newRegion->monsters = (struct monster**) malloc(nMonsters * sizeof(struct monster*));


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


    // Frees rMonsterName, as it is no longer
    // needed anywhere else in the program
    for(int i = 0; i < nMonsters; i++)
        free(rMonsterName[i]);
    free(rMonsterName);

    return newRegion;
}

// Reads data from input file
// Allocates data for new region structure array
// Calls createRegion to fill array of structures
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
            rMonsterName[j] = (char*) malloc((strlen(monsterName) + 1) * sizeof(char));
            strcpy(rMonsterName[j], monsterName);
        }

        // Fills array of structures with appropriate data pertaining to each region
        regionList[i] = createRegion(regionName, nMonsters, monsterCount, rMonsterName, monsterList);

    }

    return regionList;
}

// Creates itinerary structure and fills it with data
itinerary* createItinerary(int nRegions, int nCaptures)
{

    // Allocates memory for structure
    itinerary *newItinerary = (struct itinerary*) malloc(sizeof(struct itinerary));

    // Fills structure
    newItinerary->nregions = nRegions;
    newItinerary->captures = nCaptures;

    // Allocates memory for array of regions
    // Region structures to be filled by createRegion
    newItinerary->regions = (struct region**) malloc(nRegions * sizeof(struct region*));

    return newItinerary;
}

// Creates trainer structure and fills it with data
trainer* createTrainer(char *name)
{
    // Allocates memory for new trainer structure
    trainer *newTrainer = (struct trainer*) malloc(sizeof(struct trainer));

    int nameLen = (strlen(name) + 1);

    // Allocates and fills trainer name
    newTrainer->name = (char*) malloc(nameLen * sizeof(char));
    strcpy(newTrainer->name, name);

    return newTrainer;
}

// Reads data from input file and allocates
// memory for this data
// Calls createTrainer
trainer** readTrainers(FILE* inFile, int *trainerCount, int **nCaptures, int **nRegions, char ****tRegionNames)
{
    trainer **trainerList;
    int i;
    int *tempCaptures, *tempRegions;
    char ***tempRegionNames, regionName[MAXCHAR], trainerName[MAXCHAR];

    fscanf(inFile, "%d %*s", trainerCount);

    // Allocates necessary memory for temporary data holds
    trainerList = (struct trainer **) malloc(*trainerCount * sizeof(struct trainer*));
    tempCaptures = (int*) malloc(*trainerCount * sizeof(int));
    tempRegions = (int*) malloc(*trainerCount * sizeof(int));
    tempRegionNames = (char***) malloc(*trainerCount * sizeof(char**));

    // Reads lines for each trainer
    for(int i = 0; i < *trainerCount; i++)
    {
        fscanf(inFile, "%s", trainerName);
        fscanf(inFile, "%d %*s", &tempCaptures[i]);
        fscanf(inFile, "%d %*s", &tempRegions[i]);

        tempRegionNames[i] = (char**) malloc(tempRegions[i] * sizeof(char*));

        // Reads and stores region names
        for(int j = 0; j < tempRegions[i]; j++)
        {

            fscanf(inFile, "%s", regionName);
            tempRegionNames[i][j] = (char*) malloc((strlen(regionName)+ 1) * sizeof(char));
            strcpy(tempRegionNames[i][j], regionName);     
        }

        // Creates trainer structure
        trainerList[i] = createTrainer(trainerName);

    }

    // Transfers data to passed variables,
    // So values are accesible in main/other function(s)
    *nCaptures = tempCaptures;
    *nRegions = tempRegions;
    *tRegionNames = tempRegionNames;


    return trainerList;
}

// Performs calculations on population
float estimateCaptures(int trainerCapture, int total_population, int monsterPop)
{
    // Calculates un-rounded number of expected captures
    float percentPop = (float) monsterPop/total_population;
    float expectCapture = percentPop * trainerCapture;

    // Rounds number to nearest whole number
    expectCapture = round(expectCapture);

    return expectCapture;
}

// Frees memory allocated for monsterList
void freeMonsterList(monster **monsterList, int monsterCount)
{
    for(int i = 0; i < monsterCount; i++)
    {
        free(monsterList[i]->name);
        free(monsterList[i]->element);
        free(monsterList[i]);
    }

    free(monsterList);
}

// Frees memory allocated for regionList
void freeRegionList(region **regionList, int regionCount)
{
    for(int i = 0; i < regionCount; i++)
    {
        for(int j = 0; j < regionList[i]->nmonsters; j++)
        {
            free(regionList[i]->monsters[j]->name);
            free(regionList[i]->monsters[j]->element);
            free(regionList[i]->monsters[j]);
        }
        free(regionList[i]->monsters);
        free(regionList[i]->name);
        free(regionList[i]);

    }

    free(regionList);
}

// Frees memory allocated for trainerList
void freeTrainerList(trainer ** trainerList, int trainerCount)
{
    for(int i = 0; i < trainerCount; i++)
    {
        for(int j = 0; j < trainerList[i]->visits->nregions; j++)
        {
            free(trainerList[i]->visits->regions[j]->monsters);
            free(trainerList[i]->visits->regions[j]);
        }

        free(trainerList[i]->visits->regions);
        free(trainerList[i]->visits);
        free(trainerList[i]->name);
        free(trainerList[i]);
    }

    free(trainerList);
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
    int i, j, k;
    float ***expectedCapture;
    char region[MAXCHAR], type[MAXCHAR], strText[MAXCHAR];
    char ***tRegionNames;
    

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
        // Calls functions that read and store appropriate data with
        // corrosponding structure lists
        monsterList = readMonsters(input, &monsterCount);
        regionList = readRegions(input, &regionCount, &monsterCount, monsterList);
        trainerList = readTrainers(input, &trainerCount, &nCaptures, &nRegions, &tRegionNames);

        // Allocates memory to store all expectedCapture values
        expectedCapture = (float***) malloc(trainerCount * sizeof(float**));

        // Finishes filling trainerList's missing data
        // Prints out data to terminal and output file
        // Steps through each trainer
        for(i = 0; i < trainerCount; i++)
        {
            // Creates itinerary structure within trainer structure
            trainerList[i]->visits = createItinerary(nRegions[i], nCaptures[i]);

            // expectedCapture allocation cont.
            expectedCapture[i] = (float**) malloc(nRegions[i] * sizeof(float*));

            printf("%s\n", trainerList[i]->name);
            fprintf(output, "%s\n", trainerList[i]->name);

            // Steps through each region
            for(j = 0; j < nRegions[i]; j++)
            {
                for(k = 0;  k < regionCount; k++)
                {
                    // Fills region data for itinerary structure within trainer structures
                    if(strcmp(tRegionNames[i][j], regionList[k]->name) == 0)
                    {
                        trainerList[i]->visits->regions[j] = retrieveRegion(regionList, k);
                    
                    }
                }

                printf("%s\n", trainerList[i]->visits->regions[j]->name);
                fprintf(output, "%s\n", trainerList[i]->visits->regions[j]->name);

                // expectedCapture allocation cont.
                expectedCapture[i][j] = (float*) malloc((trainerList[i]->visits->regions[j]->nmonsters) * sizeof(float));

                // Performs calculations and prints out to console and out.txt
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
            // out.txt formatting check
            if(i < trainerCount - 1)
            {
                printf("\n");
                fprintf(output, "\n");
            }
        }

        // Frees nested memory
        for(i = 0; i < trainerCount; i++)
        {
            for(j = 0; j < nRegions[i]; j++)
            {
                free(tRegionNames[i][j]);
                free(expectedCapture[i][j]);
            }
            free(expectedCapture[i]);
            free(tRegionNames[i]);
        }

        // Frees rest of memory
        freeMonsterList(monsterList, monsterCount);
        freeRegionList(regionList, regionCount);
        freeTrainerList(trainerList, trainerCount);
        free(expectedCapture);
        free(nRegions);
        free(nCaptures);
        free(tRegionNames);

        fclose(input);
        fclose(output);
    }

    return 0;
}
