#include <stdio.h>
#include <stdlib.h>

typedef struct monster {
    int id;
    char name[64];
    char element[64];
    int population;
    double weight;
} monster;

typedef struct {
    long long int compares;
    long long int copies;
} sort_results;

monster* allocateSpaceForData(int numMonsters)
{
    monster *monsterData = (struct monster*) malloc(numMonsters * sizeof(struct monster));

    return monsterData;
}

void readFile(FILE *infile, monster *monsterData, int numMonsters)
{

    for(int i = 0; i < numMonsters; i++)
    {
        fscanf(infile, "%s %s %d %lf", monsterData[i].name, monsterData[i].element, 
                                      &monsterData[i].population, &monsterData[i].weight);
    }

}

void selectionSort(int *data, int n)
{
    int temp, minIndex;

    for(int i = 0; i < n - 1; i++)
    {
        minIndex = i;

        for(int j = i + 1; j < n; j++)
        {
            if(data[minIndex] > data[j])
            {
                minIndex = j;
            }
        }

        temp = data[i];
        data[i] = data[minIndex];
        data[minIndex] = temp;
    }
}

void printData(monster *monsterData, int numMonsters)
{
    for(int i = 0; i < numMonsters; i++)
    {
        printf("%s %s %d %lf\n",  monsterData[i].name, monsterData[i].element, 
                                monsterData[i].population, monsterData[i].weight);
    }
}

int main(void)
{
    FILE *infile, *outfile;
    monster *monsterData;
    int numMonsters1 = 100000;

    infile = fopen("100K.txt", "r");

    if(infile == NULL)
    {
        printf("Please provide a valid input file.\n");
    }
    else
    {
        monsterData = allocateSpaceForData(numMonsters1);

        readFile(infile, monsterData, numMonsters1);


        // printData(monsterData, numMonsters1);
        
    }

}