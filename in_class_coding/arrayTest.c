#include <stdio.h>
#define SIZE 5

int main(void)
{
    int A[SIZE] = {10, 20, 30, 40, 50};

    for(int i = 0; i < SIZE; i++)
        printf("%p ", &A[i]);

    int *p = A;

    for(int i = 0; i < SIZE; i++)
        printf("%d ", p[i]);
    /*
    printf("\n %p", p);
    printf("\n %d", *p);

    p++;

    printf("\n %p", p);
    printf("\n %d", *p);
 */   
    return 0;
}