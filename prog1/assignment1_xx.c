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

region* createRegion(char *name, int nMonsters, int monsterCount, char **rMonsterName, monster **monsterList)
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
        for(j = 0; j < monsterCount; j++)
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

region** readRegions(FILE* inFile, int *regionCount, int monsterCount, monster **monsterList)
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

    return regionList;
}


itinerary* createItinerary(int *nRegions, region **regionList, int regionCount, int *nCaptures, char *tRegionNames, monster **monsterList, int monsterCount)
{
    int i, j, k, rMonsterCount;
    char **monsterNames;

    printf("monsterCount: %i\n", monsterCount);
    printf("regionCount within createIT: %i\n", regionCount);

    itinerary *newItinerary = (struct itinerary*) malloc(sizeof(struct itinerary));
    newItinerary->nregions = (int) malloc(sizeof(int));
    newItinerary->regions = (struct region**) malloc(*nRegions * sizeof(struct region));
    newItinerary->captures = (int) malloc(sizeof(int));

    newItinerary->nregions = *nRegions;
    newItinerary->captures = *nCaptures;



    // Loops through the number of regions specified by trainer
    for(i = 0; i < newItinerary->nregions; i++)
    {
        printf("nregions: %i\n", newItinerary->nregions);
        
        //printf("Region: %s\n Monsters in region:\n\n", tRegionNames[i]);
        // Iterates through entire *main* region list
        for(j = 0; j < regionCount; j++)
        {

            // Compares trainer's region with *main* region list
            if(strcmp(&tRegionNames[i], regionList[j]->name) == 0)
            {
                monsterNames = (char**) malloc(regionList[j]->nmonsters * sizeof(char*));

                // Generates list of monsters found in each region
                for(k = 0; k < regionList[j]->nmonsters; k++)
                {
                    monsterNames[k] = (char*) malloc((strlen(regionList[j]->monsters[k]->name) + 1) * sizeof(char*));
                    strcpy(monsterNames[k], regionList[j]->monsters[k]->name);
                    printf("from within createIT: %s\n", monsterNames[k]);
                }

                newItinerary->regions[i] = createRegion(regionList[j]->name, regionList[j]->nmonsters, monsterCount, monsterNames, monsterList);
                
            }
        }
    }


    return newItinerary;
}



trainer* createTrainer(char *name)
{
    int i, j, nameLen, tempRCount;

    nameLen = strlen(name) + 1;

    trainer *newTrainer = (struct trainer*) malloc(sizeof(struct trainer));
    
    newTrainer->name = (char*) malloc(nameLen * sizeof(char));
    newTrainer->visits = (struct itinerary*) malloc(sizeof(struct itinerary));

    strcpy(newTrainer->name, name);

    return newTrainer;
}


trainer** readTrainers(FILE* inFile, int *trainerCount, char **tRegionNames, int *nRegions, int *nCaptures)
{
    trainer **trainerList;

    char trainerName[MAXCHAR], regionName[MAXCHAR];

    fscanf(inFile, "%d %*s", trainerCount);
    //printf("Trainer Count: %i\n\n", *trainerCount);

    trainerList = (struct trainer**) malloc(*trainerCount * sizeof(struct trainer));
    nRegions = (int *) malloc(*trainerCount * sizeof(int));
    nCaptures = (int*) malloc(*trainerCount * sizeof(int));


    for(int i = 0; i < *trainerCount; i++)
    {
        fscanf(inFile, "%s", trainerName);
        fscanf(inFile, "%d %*s", &nCaptures[i]);
        fscanf(inFile, "%d %*s", &nRegions[i]);

        tRegionNames = (char**) malloc(nRegions[i] * sizeof(char*));

        for(int j = 0; j < (nRegions[i]); j++)
        {
            fscanf(inFile, "%s", regionName);
            tRegionNames[i][j] = (char) malloc((strlen(regionName) + 1) * sizeof(char));
            strcpy(&tRegionNames[i][j], regionName);

        }

        printf("\n");

        trainerList[i] = createTrainer(trainerName);

    }


    return trainerList;
}



int main(void)
{
    monster **monsterList;
    region **regionList;
    trainer **trainerList;
    FILE *input, *output;
    int monsterCount, regionCount, trainerCount;
    int **nRegions, **nCaptures;
    char region[MAXCHAR], type[MAXCHAR], strText[MAXCHAR];
    char **tRegionNames;
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
        regionList = readRegions(input, &regionCount, monsterCount, monsterList);
        printf("regionCount after readRegions: %i\n", regionCount);
        trainerList = readTrainers(input, &trainerCount, tRegionNames, *nRegions, *nCaptures);
        for(int i = 0; i < trainerCount; i++)
        {
            for(int j = 0; j < regionCount; j++)
            {
                trainerList[i]->visits[j] = *createItinerary(nRegions[i], regionList, regionCount, nCaptures[i], &tRegionNames[i][j], monsterList, monsterCount);
            }
        }
        
    }


    return 0;
}
