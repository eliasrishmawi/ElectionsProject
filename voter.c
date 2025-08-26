#include "elections.h"
#include <stdio.h>
#include <stdlib.h>

// event R
int register_voter(int vid, int did, int sid){
    int i = 0;
    while(i < DistrictSize && Districts[i].did != did)
    {
        i++;
    }
    if(i == DistrictSize) return 1; // there is no district with this did
    struct station *current_s = Districts[i].stations;
    while(current_s != NULL && sid != current_s->sid)
    {
        current_s = current_s->next;
    }
    if(current_s == NULL) return 1; // there is no station with sid
    struct voter *new = (struct voter *)malloc(sizeof(struct voter));
    current_s->registered++;
    new->vid = vid;
    new->voted = 0;
    new->next = current_s->voters;
    current_s->voters = new;
    return 0;
}


//event U
int unregister_voter(int vid){
    struct voter *prv = NULL;
    struct voter *current = NULL;
    struct station *currentStation = NULL;
    int i;
    for(i = 0; i < DistrictSize; i++){
        currentStation = Districts[i].stations;
        while(currentStation != NULL)
        {
            prv = NULL;
            current = currentStation->voters;
            while(current != NULL)
            {
                if(current->vid == vid) break; // we found the node
                else{
                    prv = current;
                    current = current->next;
                }
            }
            if(current!= NULL && current->vid == vid) break;
            currentStation = currentStation->next;
        }
        if(current != NULL && current->vid == vid) break;
    }
    if(current == NULL) return 1;
    if(current->vid != vid) return 1;
    
    currentStation->registered--;

    if(prv == NULL)
    {
        currentStation->voters = current->next;
        free(current);
    }
    else{
        prv->next = current->next;
        free(current);
    }
    // printing it
    printf("U %d\n", vid);
    printf("  %d %d\n", Districts[i].did, currentStation->sid);
    printf("  Voters = ");
    struct voter *currentV = currentStation->voters;
    while(currentV->vid != -1){
        printf("%d, ", currentV->vid);
        currentV = currentV->next;
    }
    printf("\nDONE\n");

    return 0;

}

void printAllVoters(int vid, int did, int sid)
{
    int i;
    for(i = 0; i < DistrictSize; i++)
    {
        if(Districts[i].did == did) break;
    }
    printf("R %d %d %d\n", vid, did, sid);
    printf("  Voters = ");
    struct station *currentS = Districts[i].stations;
    while(currentS != NULL && (currentS->sid != sid))
    {
        currentS = currentS->next;
    }
    struct voter *currentV = currentS->voters;
        while(currentV->vid != -1){
            printf("%d, ", currentV->vid);
            currentV = currentV->next;
        }
    printf("\nDONE\n");
}


void printAllVotes(int index, int vid, int sid, int cid);
//event V
int vote(int vid, int sid, int cid){
    struct station *currentS = NULL;
    int i;
    for(i = 0; i < DistrictSize; i++)
    {
        currentS = Districts[i].stations;
        while(currentS != NULL){
            if(currentS->sid == sid) break;
            currentS = currentS->next;
        }
        if(currentS != NULL && currentS->sid == sid) break;
    }
    if(currentS == NULL){
        printf("\ndidnt find station with sid = %d\n", sid);
        return 1; // didnt find station with sid
    } 
    if(currentS->sid != sid) return 1;
    struct voter *currentV = currentS->voters;
    while(currentV->vid != -1 && currentV->vid != vid)
    {
        currentV = currentV->next;
    }
    if(currentV->vid == -1){
        printf("\ndidnt find voter with vid = %d\n", vid);
        return 1; // didnt find vid
    } 
    currentV->voted = 1;

    if(cid == 0)
    {
        Districts[i].blanks++;
    }
    if(cid == 1)
    {
        Districts[i].voids++;
    }
    struct candidate *currentC = Districts[i].candidates;
if(cid != 0 && cid != 1)
{
    while(currentC != NULL){
        if(currentC->cid == cid) break;
        currentC = currentC->next;
    }
    if(currentC == NULL) return 1; // didnt find candidate with cid in district i

    currentC->votes++;

    while(currentC->prev!=NULL && currentC->prev->votes < currentC->votes){
        struct candidate *prevC = currentC->prev;
        if(Districts[i].candidates == prevC)
        {
            Districts[i].candidates = currentC;
        }
        if(prevC->prev != NULL)
        {
            prevC->prev->next = currentC;
        }
        currentC->prev = prevC->prev;
        prevC->next  = currentC->next;
        if(currentC->next != NULL)
        {
            currentC->next->prev = prevC;
        }
        currentC->next = prevC;
        prevC->prev = currentC;

    }
}

    //print V
    printAllVotes(i, vid, sid, cid);
    return 0;
    
}

void printAllVotes(int index, int vid, int sid, int cid){
    printf("V %d %d %d\n", vid, sid, cid);
    printf("  District = %d\n", Districts[index].did);
    printf("  candidate votes = ");
    struct candidate *tmp = Districts[index].candidates;
    while(tmp != NULL){
        printf("(%d, %d), ", tmp->cid, tmp->votes);
        tmp = tmp->next;
    }
    printf("\n  Blanks = %d\n", Districts[index].blanks);
    printf("  Voids  = %d\n", Districts[index].voids);
    printf("DONE\n");
}
