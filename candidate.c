#include "elections.h"
#include <stdio.h>
#include <stdlib.h>

// event C
int register_candidate(int cid, int did, int pid){
    int i = 0;
    while(i < DistrictSize && Districts[i].did != did)
    {
        i++;
    }
    if(i == DistrictSize) return 1; // there is no district with this did
    int j = 0;
    while(j < PartySize && Parties[j].pid != pid)
    {
        j++;
    }
    if(j == PartySize) return 1; // there is no party with this pid
    struct candidate *new;
    new = (struct candidate *)malloc(sizeof(struct candidate));
    if(cid == 0 || cid == 1) return 1; // cant cid be neither 0, 1
    new->cid = cid;
    new->pid = pid;
    new->elected = 0;
    new->votes = 0;
    new->next = Districts[i].candidates;
    if(Districts[i].candidates == NULL)
    {
        new->prev = NULL;
    }else{
        new->prev = Districts[i].candidates->prev; // it will be always NULL
        Districts[i].candidates->prev = new;

    }
    if(new->prev != NULL){ // this is just in theory, it will always be false
        new->prev->next = new;
    }
    Districts[i].candidates = new; // the new head
    return 0;
}

void printAllCandidates(int cid, int did, int pid)
{
    int i;
    for(i = 0; i < DistrictSize; i++)
    {
        if(Districts[i].did == did) break;
    }
    printf("C %d %d %d\n", cid, did, pid);
    printf("  Candidates = ");
    struct candidate *currentC = Districts[i].candidates;
    while(currentC != NULL)
    {
        printf("%d, ", currentC->cid);
        currentC = currentC->next;
    }
    printf("\nDONE\n");
}