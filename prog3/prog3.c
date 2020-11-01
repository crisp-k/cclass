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


void printData(monster *monsterData, int numMonsters)
{
    for(int i = 0; i < numMonsters; i++)
    {
        printf("%s %s %d %lf\n",  monsterData[i].name, monsterData[i].element, 
                                monsterData[i].population, monsterData[i].weight);
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

monster* allocateSpaceForData(int numMonsters)
{
    monster *monsterData = (struct monster*) malloc(numMonsters * sizeof(struct monster));

    return monsterData;
}

monster *copyToTempArray(monster *monsterData, int numMonsters)
{
    monster *tempArray = allocateSpaceForData(numMonsters);

    for(int i = 0; i < numMonsters; i++)
    {
        strcpy(tempArray[i].name, monsterData[i].name);
        strcpy(tempArray[i].element, monsterData[i].element);
        tempArray[i].id = monsterData[i].id;
        tempArray[i].population = monsterData[i].population;
        tempArray[i].weight = monsterData[i].weight;
    }

    return tempArray;
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

void swap(monster *a, monster *b)
{
    monster temp = *a;
    *a = *b;
    *b = temp;
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
            // compares++;
        }

        swap(&data[i], &data[minIndex]);
        // temp = data[i];
        // data[i] = data[minIndex];
        // data[minIndex] = temp;

        // copies += 3;
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
                swap(&data[j], &data[j + 1]);
                // temp = data[j];
                // data[j] = data[j + 1];
                // data[j + 1] = temp;
                // copies += 3;
            }
            // compares++;
        }
    }
}

void insertionSort(monster *data, int n, int criteria)
{
    int value, j;
    monster temp;

    for(int i = 1; i < n; i++)
    {
       temp = data[i];
       // copies++;

        for(j = i - 1; j >= 0; j--)
        {
            if(compareTo(&temp, &data[j], criteria) <= 0)
                data[j + 1] = data[j];
                // copies++;
            else 
                break;  

            // compares++  ;
        }

        data[j + 1] = temp;
        // copies++;
    }

}

void merge(monster *data, int left, int middle, int right, int criteria)
{
    int i, j, k;
    int  leftLength, rightLength;
    monster *LTemp, *RTemp;
    
    leftLength = middle - left + 1;
    rightLength = right - middle;

    LTemp = (monster*) malloc(leftLength * sizeof(monster));
    RTemp = (monster*) malloc(rightLength * sizeof(monster));

    for(i = 0; i < leftLength; i++)
        LTemp[i] = data[left + i];
    
    for(i = 0; i < rightLength; i++)
        RTemp[i] = data[middle + 1 + i];

    i = 0;
    j = 0;
    k = left;

    while(i < leftLength && j < rightLength)
    {
        if(compareTo(&LTemp[i], &RTemp[j], criteria) <= 0)
        {
            data[k] = LTemp[i];
            i++;
        }
        else
        {
            data[k] = RTemp[j];
            j++;
        }

        k++;

    }

    while(i < leftLength)
    {
        data[k] = LTemp[i];
        i++;
        k++;
    }

    while(j < rightLength)
    {
        data[k] = RTemp[j];
        j++;
        k++;
    }

    free(LTemp);
    free(RTemp);
}

void mergeSort(monster *data, int left, int right, int criteria)
{
    if(left < right)
    {
        int middle = (left + right) / 2;

        mergeSort(data, left, middle, criteria);

        mergeSort(data, middle + 1, right, criteria);

        merge(data, left, middle, right, criteria);

    }
}

int partition(monster *data, int low, int high, int criteria)
{
    int i = low + rand() % (high - low + 1);
    swap(&data[low], &data[i]);

    int lowpos = low;
    low++;

    while(low <= high)
    {
        // while(low <= high && values[low] <= values[lowpos]) low++;
        while(low <= high && compareTo(&data[low], &data[lowpos], criteria) <= 0) low++;
        // while(high >= low && values[high] > values[lowpos]) high--;
        while(high >= low && compareTo(&data[high], &data[lowpos], criteria) > 0) high--;

        if(low < high)
            swap(&data[low], &data[high]);
    }

    swap(&data[lowpos], &data[high]);

    return high;
}

void quickSort(monster *data, int low, int high, int criteria)
{
    if(low < high)
    {
        int split = partition(data, low, high, criteria);
        quickSort(data, low, split - 1, criteria);
        quickSort(data, split + 1, high, criteria);
    }
}

void selectionWrapper(monster *monsterData, int numMonsters, int criteria)
{
            monster *tempData = copyToTempArray(monsterData, numMonsters);

            printData(tempData, 25);
            selectionSort(tempData, numMonsters, criteria);
            
            printf("\n\n\n");
            printData(tempData, numMonsters);
            printf("\n\n\n");

            free(tempData);
}

void bubbleWrapper(monster *monsterData, int numMonsters, int criteria)
{
            monster *tempData = copyToTempArray(monsterData, numMonsters);

            printData(tempData, 25);
            bubbleSort(tempData, numMonsters, criteria);
            
            printf("\n\n\n");
            printData(tempData, numMonsters);
            printf("\n\n\n");

            free(tempData);
}

void insertionWrapper(monster *monsterData, int numMonsters, int criteria)
{
            monster *tempData = copyToTempArray(monsterData, numMonsters);

            printData(tempData, 25);
            insertionSort(tempData, numMonsters, criteria);
            
            printf("\n\n\n");
            printData(tempData, numMonsters);
            printf("\n\n\n");

            free(tempData);
}

void mergeWrapper(monster *monsterData, int numMonsters, int criteria)
{
            monster *tempData = copyToTempArray(monsterData, numMonsters);

            printData(tempData, 25);
            mergeSort(tempData, 0, numMonsters - 1, criteria);
            
            printf("\n\n\n");
            printData(tempData, numMonsters);
            printf("\n\n\n");

            free(tempData);
}

void quickWrapper(monster *monsterData, int numMonsters, int criteria)
{
            monster *tempData = copyToTempArray(monsterData, numMonsters);

            printData(tempData, 25);
            quickSort(tempData, 0, numMonsters, criteria);
            
            printf("\n\n\n");
            printData(tempData, numMonsters);
            printf("\n\n\n");

            free(tempData);
}


int main(void)
{
    int numFiles = 6;
    char fileNames[][10] = {"10K.txt", "20K.txt", "30K.txt", "40K.txt", "50K.txt", "60K.txt"};
    int numMonsters[] = {10000, 20000, 30000, 40000, 50000, 60000};
    FILE *infile, *outfile;
    monster *monsterData, *sortedData;

    for(int i = 0; i < 1; i++)
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

            // selectionWrapper(monsterData, numMonsters[i], 1);
            // bubbleWrapper(monsterData, numMonsters[i], 1);
            // insertionWrapper(monsterData, numMonsters[i], 2);
            mergeWrapper(monsterData, numMonsters[i], 1);
            // quickWrapper(monsterData, numMonsters[i], 1);

            free(monsterData);


            fclose(infile);
        }
    }

}