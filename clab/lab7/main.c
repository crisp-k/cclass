#include<stdio.h>
#include <stdlib.h>
#include "leak_detector_c.h"

struct node {
     int data;
     struct node  *next;
};

struct node* insert(struct node* list,int d );
struct node* del(struct node* list,int d );
void print( struct node *list);
void freeList(struct node* list);
void copy ( struct node *q, struct node **s );

int main( ) {

    atexit(report_mem_leak);

    int number = 0, choice=0;
    struct node *pList=NULL;
    struct node *nList = NULL;

    // Let the user add values until they enter -1.
    while(choice!= 4)
    {
        // Get the operation.
        printf("Do you want to (1)insert, (2)delete, (3)Copy (4)quit.\n");
        scanf("%d", &choice);

        printf("Your choice is %d\n", choice);

        // Execute the operation.
        if (choice == 1)
        {
           // Get the number.
            printf("Enter the value to insert\n");
            scanf("%d", &number);
            pList = insert(pList, number);
                    // Look at the list.
            printf("Items in linked list: ");
            print(pList);
            printf("\n");
        }
        else if (choice == 2)
        {    // Get the number.
            printf("Enter the value to delete.\n");
            scanf("%d", &number);
            pList = del(pList, number);
                    // Look at the list.
            printf("Items in linked list: ");
            print(pList);
            printf("\n");
        }
        else if (choice == 3)
        {
            if (nList)
                freeList(nList);

            copy(pList, &nList); //passing reference of nList as it is not returning anything
                    // Look at the list.
            printf("Items in NEW linked list: ");
            print(nList);
            printf("\n");
        }
        else
        {
              break;
        }
    }

    freeList(nList);
    freeList(pList);
    printf("\nBye..\n");
    return 0;
}

struct node* insert(struct node* list,int d )
{
    // base case
    if(list == NULL || d < list->data)
    {
        struct node* pNew = (struct node*) malloc(sizeof(struct node));
        pNew->data = d;
        pNew->next = list;
        return pNew;
    }

    // recursive case
    list->next = insert(list->next, d);
    return list;
}

void print(struct node *list)
{
    if(list == NULL)
        return;

    if(list != NULL)
    {
        printf("%d ", list->data);
        print(list->next);
    }
}

struct node* del(struct node* list,int d )
{
    // base case 1: not in list
    if(list == NULL)
        return NULL;
    
    // base case 2: found item
    if(list->data == d)
    {
        struct node* rest = list->next;
        free(list);
        return rest;
    }

    // recursive case
    list->next = del(list->next, d);
    return list;
}

void freeList(struct node* list)
{
    // base case
    if(list == NULL)
        return;

    // recursive
    if(list->next != NULL)
    {
        freeList(list->next);
    }

    free(list);
}

// copy one linked list into another
void copy ( struct node *q, struct node **s )
{
    if(q != NULL)
    {
        *s = insert(*s, q->data);
        copy(q->next, s);
    }
}