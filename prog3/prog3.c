#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "leak_detector_c.h"

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

void print_clocks(clock_t clocks)
{
    printf("Total time taken %lfs second\n", ((double) clocks) / CLOCKS_PER_SEC);
}

// Function is used for the sake of printing to csv file
double calculateTime(clock_t clocks)
{
    return ((double) clocks) / CLOCKS_PER_SEC;
}

char *getCriteriaString(int criteria)
{
    switch(criteria)
    {
        case 1:
            return "name";
        case 2:
            return "weight";
        case 3:
            return "name and weight";
        default:
            return "error";
    }
}

int isSorted(monster *monsterData, int length, int criteria)
{
    for(int i = 0; i <length - 1; i++)
    {
        switch (criteria)
            {
            
            // Sorted by name check
            case 1:
                if(strcmp(monsterData[i].name, monsterData[i + 1].name) == 1)
                    return 0;
                    break;

            // Sorted by weight check
            case 2:
                if(monsterData[i].weight > monsterData[i + 1].weight)
                    return 0;
                    break; 

            // Sorted by name and weight check       
            case 3:
                if(strcmp(monsterData[i].name, monsterData[i + 1].name) == 1 && monsterData[i].weight > monsterData[i + 1].weight)
                    return 0;
                    break;
            }
    }

    return 1;
}

monster* allocateSpaceForData(int numMonsters)
{
    monster *monsterData = (struct monster*) malloc(numMonsters * sizeof(struct monster));

    return monsterData;
}

monster* copyToTempArray(monster *monsterData, int numMonsters)
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
        // Sort by name comparison
        case 1:
            if(strcmp(m1->name, m2->name) > 0)
                return 1;
            else
                return 0;

        // Sort by weight comparison
        case 2:
            if(m1->weight > m2->weight)
                return 1;
            else if(m1->weight < m2->weight)
                return -1;
            else 
                return 0;

        // Sort by name and weight comparison
        case 3:
            if(strcmp(m1->name, m2->name) > 0)
                return 1;
            else if(strcmp(m1->name, m2->name) == 0 && compareTo(m1, m2, 2) == 1)
                return 1;
            else
                return 0;
            break;
    }
    return 0;
}

void swap(monster *a, monster *b)
{
    monster temp = *a;
    *a = *b;
    *b = temp;
}

void selectionSort(monster *data, int n, int criteria, sort_results *sortData)
{
    int minIndex;

    // Re-adjusts boundary to unsorted subarray
    for(int i = 0; i < n - 1; i++)
    {
        // Finds minimum value in array
        minIndex = i;
        for(int j = i + 1; j < n; j++)
        {
            if(compareTo(&data[minIndex], &data[j], criteria) == 1)
            {
                minIndex = j;
            }
            sortData->compares++;
        }

        // When minimum value is found, swaps i value with new minimum value
        swap(&data[i], &data[minIndex]);
        sortData->copies += 3;

    }
}

void bubbleSort(monster *data, int n, int criteria, sort_results *sortData)
{

    for(int i = 0; i < n - 1; i++)
    {
        // Steps through array and swaps values when the next
        // Value in an array is out of place
        // Creates a 'falling' effect of values
        for(int j = 0; j < n - i - 1; j++)
        {
            if(compareTo(&data[j], &data[j + 1], criteria) == 1)
            {
                swap(&data[j], &data[j + 1]);
                sortData->copies += 3;
            }
            sortData->compares++;
        }
    }
}

void insertionSort(monster *data, int left, int right, int criteria, sort_results *sortData)
{
    int j;
    monster temp;

    for(int i = left; i < right; i++)
    {
       temp = data[i + 1];
       sortData->copies++;

        // Moves values one index ahead that are greater than temp value
        for(j = i - 1; j >= left; j--)
        {
            if(compareTo(&temp, &data[j], criteria) <= 0)
            {
                data[j + 1] = data[j];
                sortData->copies++;
            }
            else
                break;  

            sortData->compares++;
        }

        data[j + 1] = temp;
    }

}

void merge(monster *data, int left, int middle, int right, int criteria, sort_results *sortData)
{
    int i, j, k;
    int  leftLength, rightLength;
    monster *LTemp, *RTemp;

    leftLength = middle - left + 1;
    rightLength = right - middle;

    // Temp array creation
    LTemp = (monster*) malloc(leftLength * sizeof(monster));
    RTemp = (monster*) malloc(rightLength * sizeof(monster));

    // Fills temp arrays with sub-array information
    for(i = 0; i < leftLength; i++)
        LTemp[i] = data[left + i];
        sortData->copies++;
    
    for(i = 0; i < rightLength; i++)
        RTemp[i] = data[middle + 1 + i];
        sortData->copies++;

    i = 0;
    j = 0;
    k = left;

    while(i < leftLength && j < rightLength)
    {
        // Fills original array based on which value in the
        // left or right array is less than the other
        if(compareTo(&LTemp[i], &RTemp[j], criteria) <= 0)
        {
            data[k] = LTemp[i];
            i++;
            sortData->copies++;
        }
        else
        {
            data[k] = RTemp[j];
            j++;
            sortData->copies++;
        }

        k++;
        sortData->compares++;

    }

    // Dumps the rest of remaining values into the array
    while(i < leftLength)
    {
        data[k] = LTemp[i];
        i++;
        k++;
        sortData->copies++;
    }

    while(j < rightLength)
    {
        data[k] = RTemp[j];
        j++;
        k++;
        sortData->copies++;
    }

    free(LTemp);
    free(RTemp);
}

void mergeSort(monster *data, int left, int right, int criteria, sort_results *sortData)
{
    // Recursive division of array into sub arrays
    if(left < right)
    {
        int middle = (left + right) / 2;

        mergeSort(data, left, middle, criteria, sortData);

        mergeSort(data, middle + 1, right, criteria, sortData);

        // Actual function doing the sorting
        // Puts the sub arrays back together
        merge(data, left, middle, right, criteria, sortData);

    }
}

// Merge-Inserton Sort variation
void mergeInsertionSort(monster *data, int left, int right, int criteria, sort_results *sortData)
{
    int check = (right - left);

    // Changes end condition to end when there are 25 elements in a sub array
    // Then uses insertion sort to sort those arrays
    // This works because the merge function works 'in-place', 
    // and thus we dont need to merge it all back together
    if(check >= 25)
    {
        int middle = (left + right) / 2;

        mergeInsertionSort(data, left, middle, criteria, sortData);

        mergeInsertionSort(data, middle + 1, right, criteria, sortData);

        merge(data, left, middle, right, criteria, sortData);
    }
    else
    {
        insertionSort(data, left, right + 1, criteria, sortData);
    }
}


int partition(monster *data, int low, int high, int criteria, sort_results *sortData)
{
    // Picks random partition element and swaps into low index
    int i = low + rand() % (high - low + 1);
    swap(&data[low], &data[i]);

    // Stores pivot index
    int lowpos = low;

    // Moves starting position after pivot location
    low++;

    // Runs until low and high counters cross
    while(low <= high)
    {
        // Finds value greater than pivot on left side
        while(low <= high && compareTo(&data[low], &data[lowpos], criteria) <= 0) 
        {
            low++;
            sortData->compares++;
        }

        // Finds value lesser than pivot on right side
        while(high >= low && compareTo(&data[high], &data[lowpos], criteria) > 0)
        {
            high--;
            sortData->compares++;
        }

        // Swaps values on wrong side with each other
        if(low < high)
        {
            swap(&data[low], &data[high]);
            sortData->copies += 3;
        }
    }

    // Swaps pivot to correct location
    swap(&data[lowpos], &data[high]);

    // Returns partition point
    return high;
}

void quickSort(monster *data, int low, int high, int criteria, sort_results *sortData)
{
    if(low < high)
    {
        // Recursively calls parition and quick sort
        int split = partition(data, low, high, criteria, sortData);
        quickSort(data, low, split - 1, criteria, sortData);
        quickSort(data, split + 1, high, criteria, sortData);
    }
}

void selectionWrapper(monster *monsterData, int numMonsters, int criteria, FILE *outfile)
{
            monster *tempData = copyToTempArray(monsterData, numMonsters);
            sort_results *sortData;
            clock_t start_cpu, end_cpu;

            // Intializes sortData
            sortData->compares = 0;
            sortData->copies = 0;


            if(isSorted(tempData, numMonsters, criteria) == 0)
                printf("Array status: not sorted by %s before calling selection sort\n", getCriteriaString(criteria));
            else
                printf("Array status: sorted by %s before calling selection sort\n", getCriteriaString(criteria));

            
            //Starts clock, and calls sort function
            start_cpu = clock();
            if(criteria == 3)                                                   // If criteria is 3, then sort function runs twice
            {
                for(int i = 0; i <= 1; i++)
                {
                    selectionSort(tempData, numMonsters, criteria, sortData);
                }
            }
            else
            {
                selectionSort(tempData, numMonsters, criteria, sortData);
            }
            end_cpu = clock();


            if(isSorted(tempData, numMonsters, criteria) == 0)
                printf("Array status: not sorted by %s after calling selection sort\n", getCriteriaString(criteria));
            else
                printf("Array status: sorted by %s after calling selection sort\n", getCriteriaString(criteria));
            
            
            // Prints relevant data to console and csv file
            print_clocks(end_cpu - start_cpu);
            printf("Total number of comparisons %lli\n", sortData->compares);
            printf("Total number of copy operations %lli\n", sortData->copies);
            fprintf(outfile, "%lli, %lli, %lf, ", sortData->compares, sortData->copies, calculateTime(end_cpu - start_cpu));

            free(tempData);
}

void bubbleWrapper(monster *monsterData, int numMonsters, int criteria, FILE *outfile)
{
            monster *tempData = copyToTempArray(monsterData, numMonsters);
            sort_results *sortData;
            clock_t start_cpu, end_cpu;
            
            // Initializes sortData
            sortData->compares = 0;
            sortData->copies = 0;


            if(isSorted(tempData, numMonsters, criteria) == 0)
                printf("Array status: not sorted by %s before calling bubble sort\n", getCriteriaString(criteria));
            else
                printf("Array status: sorted by %s before calling bubble sort\n", getCriteriaString(criteria));


            // Starts clock, and calls sort function
            start_cpu = clock();
            if(criteria == 3)                                               // If criteria is 3, then sort function runs twice 
            {
                for(int i = 0; i <= 1; i++)
                {
                    bubbleSort(tempData, numMonsters, criteria, sortData);
                }
            }
            else
            {
                bubbleSort(tempData, numMonsters, criteria, sortData);
            }
            end_cpu = clock();


            if(isSorted(tempData, numMonsters, criteria) == 0)
                printf("Array status: not sorted by %s after calling bubble sort\n", getCriteriaString(criteria));
            else
                printf("Array status: sorted by %s after calling bubble sort\n", getCriteriaString(criteria));
            
            
            // Prints relevant data to console and csv file
            print_clocks(end_cpu - start_cpu);
            printf("Total number of comparisons %lli\n", sortData->compares);
            printf("Total number of copy operations %lli\n", sortData->copies);
            fprintf(outfile, "%lli, %lli, %lf, ", sortData->compares, sortData->copies, calculateTime(end_cpu - start_cpu));

            free(tempData);
}

void insertionWrapper(monster *monsterData, int numMonsters, int criteria, FILE *outfile)
{
            monster *tempData = copyToTempArray(monsterData, numMonsters);
            sort_results *sortData;
            clock_t start_cpu, end_cpu;

            // Initializes sortData
            sortData->compares = 0;
            sortData->copies = 0;


            if(isSorted(tempData, numMonsters, criteria) == 0)
                printf("Array status: not sorted by %s before calling insertion sort\n", getCriteriaString(criteria));
            else
                printf("Array status: sorted by %s before calling insertion sort\n", getCriteriaString(criteria));


            // Starts clock, and calls sort function
            start_cpu = clock();
            if(criteria == 3)                                                    // If criteria is 3, then sort function runs twice
            {
                for(int i = 0; i <= 1; i++)
                {
                    insertionSort(tempData, 0, numMonsters, criteria, sortData);
                }
            }
            else
            {
                insertionSort(tempData, 0, numMonsters, criteria, sortData);
            }
            end_cpu = clock();


            if(isSorted(tempData, numMonsters, criteria) == 0)
                printf("Array status: not sorted by %s after calling insertion sort\n", getCriteriaString(criteria));
            else
                printf("Array status: sorted by %s after calling insertion sort\n", getCriteriaString(criteria));
            
            
            // Prints relevant data to console and csv file
            print_clocks(end_cpu - start_cpu);
            printf("Total number of comparisons %lli\n", sortData->compares);
            printf("Total number of copy operations %lli\n", sortData->copies);
            fprintf(outfile, "%lli, %lli, %lf, ", sortData->compares, sortData->copies, calculateTime(end_cpu - start_cpu));

            free(tempData);
}

void mergeWrapper(monster *monsterData, int numMonsters, int criteria, FILE *outfile)
{
            monster *tempData = copyToTempArray(monsterData, numMonsters);
            sort_results *sortData;
            clock_t start_cpu, end_cpu;

            // Initializes sortData
            sortData->compares = 0;
            sortData->copies = 0;
            
            if(isSorted(tempData, numMonsters, criteria) == 0)
                printf("Array status: not sorted by %s before calling merge sort\n", getCriteriaString(criteria));
            else
                printf("Array status: sorted by %s before calling merge sort\n", getCriteriaString(criteria));


            // Starts clock, and calls sort function
            start_cpu = clock();
            if(criteria == 3)                                                    // If criteria is 3, then sort function runs twice
            {
                for(int i = 0; i <= 1; i++)
                {
                    mergeSort(tempData, 0, numMonsters - 1, criteria, sortData);
                }
            }
            else
            {
                mergeSort(tempData, 0, numMonsters - 1, criteria, sortData);
            }
            end_cpu = clock();


            if(isSorted(tempData, numMonsters, criteria) == 0)
                printf("Array status: not sorted by %s after calling merge sort\n", getCriteriaString(criteria));
            else
                printf("Array status: sorted by %s after calling merge sort\n", getCriteriaString(criteria));
            
            
            // Prints relevant data to console and csv file
            print_clocks(end_cpu - start_cpu);
            printf("Total number of comparisons %lli\n", sortData->compares);
            printf("Total number of copy operations %lli\n", sortData->copies);
            fprintf(outfile, "%lli, %lli, %lf, ", sortData->compares, sortData->copies, calculateTime(end_cpu - start_cpu));

            free(tempData);
}

void mergeInsertionWrapper(monster *monsterData, int numMonsters, int criteria, FILE *outfile)
{
            monster *tempData = copyToTempArray(monsterData, numMonsters);
            sort_results *sortData;
            clock_t start_cpu, end_cpu;

            // Intializes sortData
            sortData->compares = 0;
            sortData->copies = 0;
            
            if(isSorted(tempData, numMonsters, criteria) == 0)
                printf("Array status: not sorted by %s before calling merge/insertion sort\n", getCriteriaString(criteria));
            else
                printf("Array status: sorted by %s before calling merge/insertion sort\n", getCriteriaString(criteria));


            // Starts clock, and calls sort function
            start_cpu = clock();
            if(criteria == 3)                                                               // If criteria is 3, then sort function runs twice
            {
                for(int i = 0; i <= 1; i++)
                {
                    mergeInsertionSort(tempData, 0, numMonsters - 1, criteria, sortData);
                }
            }
            else
            {
                mergeInsertionSort(tempData, 0, numMonsters - 1, criteria, sortData);
            }
            end_cpu = clock();


            if(isSorted(tempData, numMonsters, criteria) == 0)
                printf("Array status: not sorted by %s after calling merge/insertion sort\n", getCriteriaString(criteria));
            else
                printf("Array status: sorted by %s after calling merge/insertion sort\n", getCriteriaString(criteria));
            
            
            // Prints relevant data to console and csv file
            print_clocks(end_cpu - start_cpu);
            printf("Total number of comparisons %lli\n", sortData->compares);
            printf("Total number of copy operations %lli\n", sortData->copies);
            fprintf(outfile, "%lli, %lli, %lf, ", sortData->compares, sortData->copies, calculateTime(end_cpu - start_cpu));

            free(tempData);
}

void quickWrapper(monster *monsterData, int numMonsters, int criteria, FILE *outfile)
{
            monster *tempData = copyToTempArray(monsterData, numMonsters);
            sort_results *sortData;
            clock_t start_cpu, end_cpu;
            
            // Intializes sortData
            sortData->compares = 0;
            sortData->copies = 0;

            if(isSorted(tempData, numMonsters, criteria) == 0)
                printf("Array status: not sorted by %s before calling quick sort\n", getCriteriaString(criteria));
            else
                printf("Array status: sorted by %s before calling quick sort\n", getCriteriaString(criteria));
           

            // Starts clock, and calls sort function
            start_cpu = clock();
            if(criteria == 3)                                                    // If criteria is 3, then sort function runs twice
            {
                for(int i = 0; i <= 1; i++)
                {
                    quickSort(tempData, 0, numMonsters - 1, criteria, sortData);
                }
            }
            else
            {
                quickSort(tempData, 0, numMonsters - 1, criteria, sortData);
            }
            end_cpu = clock();


            if(isSorted(tempData, numMonsters, criteria) == 0)
                printf("Array status: not sorted by %s after calling quick sort\n", getCriteriaString(criteria));
            else
                printf("Array status: sorted by %s after calling quick sort\n", getCriteriaString(criteria));
            
            
            // Prints relevant data to console and csv file
            print_clocks(end_cpu - start_cpu);
            printf("Total number of comparisons %lli\n", sortData->compares);
            printf("Total number of copy operations %lli\n", sortData->copies);
            fprintf(outfile, "%lli, %lli, %lf\n", sortData->compares, sortData->copies, calculateTime(end_cpu - start_cpu));

            free(tempData);
}

int main(void)
{
    atexit(report_mem_leak);

    int numFiles = 6;
    char fileNames[][10] = {"10K.txt", "20K.txt", "30K.txt", "40K.txt", "50K.txt", "60K.txt"};
    char outputFiles[][15] = {"criteria_1.csv", "criteria_2.csv", "criteria_3.csv"};
    int numMonsters[] = {10000, 20000, 30000, 40000, 50000, 60000};
    FILE *infile, *outfile;
    monster *monsterData, *sortedData;

    for(int criteria = 1; criteria <= 3; criteria++)
        for(int file = 0; file < numFiles; file++)
        {
            infile = fopen(fileNames[file], "r");
            outfile = fopen(outputFiles[criteria - 1], "a");

            // Initial header for each 3 criteria's output file
            if(file == 0)
            {
                fprintf(outfile, "DataSize, SelectionSortCompare, SelectionSortCopy, SelectionSortTIme, BubbleSortCompare, ");
                fprintf(outfile, "BubbleSortCopy, BubbleSortTime, InsertionSortCompare, InsertionSortCopy, Insert`ionSortTime, ");
                fprintf(outfile, "MergeSortCompare, MergeSortCopy, MergeSortTime, Merge_InsertionSortCompare, Merge_InsertionSortCopy, ");
                fprintf(outfile, "Merge_InsertionSortTime, QuickSortCompare, QuickSortCopy, QuickSortTime\n");
            }

            fprintf(outfile, "%i, ", numMonsters[file]);


            printf("========\n");
            printf("Processing Criteria %i and file %s\n", criteria, fileNames[file]);
            printf("========\n");


            if(infile == NULL)
            {
                printf("Please provide a valid input file.\n");
            }
            else
            {
                monsterData = allocateSpaceForData(numMonsters[file]);

                readFile(infile, monsterData, numMonsters[file]);

                // Calls wrappers for each sort function
                selectionWrapper(monsterData, numMonsters[file], criteria, outfile);
                bubbleWrapper(monsterData, numMonsters[file], criteria, outfile);
                insertionWrapper(monsterData, numMonsters[file], criteria, outfile);
                mergeWrapper(monsterData, numMonsters[file], criteria, outfile);
                mergeInsertionWrapper(monsterData, numMonsters[file], criteria, outfile);
                quickWrapper(monsterData, numMonsters[file], criteria, outfile);

                free(monsterData);


                printf("\n");
            }


            fclose(infile);
            fclose(outfile);
        }

    return 0;
}