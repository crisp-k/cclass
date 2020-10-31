#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int compareTo(monster *m1, monster *m2, int criteria)
{
    switch (criteria)
    {
        case 1:
            if(strcmp(m1->name, m2->name) > 0)
                return 1;
            else
                return 0;
        case 2:
            if(m1->weight > m2->weight)
                return 1;
            else if(m1->weight < m2->weight)
                return -1;
            else 
                return 0;
        // case 3:

        //     break;
    }
    return 0;
}

void selectionSort(monster *data, int n, int criteria)
{
    monster temp; 
    int minIndex;

    for(int i = 0; i < n - 1; i++)
    {
        minIndex = i;

        for(int j = i + 1; j < n; j++)
        {
            if(compareTo(&data[minIndex], &data[j], criteria) == 1)
            {
                minIndex = j;
            }
        }

        temp = data[i];
        data[i] = data[minIndex];
        data[minIndex] = temp;
    }
}

void bubbleSort(monster *data, int n, int criteria)
{
    monster temp;
    for(int i = 0; i < n - 1; i++)
    {
        for(int j = 0; j < n - i - 1; j++)
        {
            if(compareTo(&data[j], &data[j + 1], criteria) == 1)
            {
                temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
        }
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

void freeData(monster *monsterData, int numMonsters)
{
    for(int i = 0; i < numMonsters; i++)
    {
        free(&monsterData);
    }
}

int isSorted(monster *monsterData, int length, int criteria)
{
    for(int i = 0; i <length - 1; i++)
    {
        switch (criteria)
            {
            case 1:
                if(strcmp(monsterData[i].name, monsterData[i + 1].name) > 0)
                    return 0;
            case 2:
                if(monsterData[i].weight > monsterData[i + 1].weight)
                    return 0;
                    
            // case 3:
                // break;
            }

    }

    return 1;
}

int main(void)
{
    int numFiles = 6;
    char fileNames[][10] = {"10K.txt", "20K.txt", "30K.txt", "40K.txt", "50K.txt", "60K.txt"};
    int numMonsters[] = {10000, 20000, 30000, 40000, 50000, 60000};
    FILE *infile, *outfile;
    monster *monsterData;

    for(int i = 0; i < numFiles; i++)
    {
        infile = fopen(fileNames[i], "r");

        if(infile == NULL)
        {
            printf("Please provide a valid input file.\n");
        }
        else
        {
            monsterData = allocateSpaceForData(numMonsters[i]);

            readFile(infile, monsterData, numMonsters[i]);

            printData(monsterData, 25);

            selectionSort(monsterData, numMonsters[i], 1);

            printf("\n\n\n");

            printData(monsterData, numMonsters[i]);

            free(monsterData);

            fclose(infile);

            printf("\n\n\n\n\n");
        }
    }

}