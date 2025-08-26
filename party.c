#include "elections.h"
#include <stdio.h>

int p_count = 0;
// event p
void create_party(int pid){
    Parties[p_count].pid = pid;
    Parties[p_count].elected = NULL;
    Parties[p_count].nelected = 0;
    p_count++;
}

void printAllParties(int pid)
{
    printf("P %d\n", pid);
    printf("  Parties = ");
    for(int i = 0; i < p_count; i++)
    {
        printf("%d, ", Parties[i].pid);
    }
    printf("\nDONE\n");
}

// event K
void print_party(int pid){
    //no need for check but just to make sure
    int i = 0;
    while(Parties[i].pid != pid){
        i++;
    }
    printf("K %d\n", pid);
    printf(" Elected =\n");\
    struct candidate *currentC = Parties[i].elected;
    while(currentC != NULL){
        printf("  %4d %4d\n", currentC->cid, currentC->votes);

        currentC = currentC->next;
    }
    printf("DONE\n");
}