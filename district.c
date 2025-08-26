#include "elections.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int d_count = 0;
int f1 = 0;
// event D
int create_district(int did, int seats){
    if(d_count >= DistrictSize) return 1;
    Districts[d_count].did = did;
    Districts[d_count].seats = seats;
    Districts[d_count].allotted = 0;
    Districts[d_count].blanks = 0;
    Districts[d_count].voids = 0;
    Districts[d_count].stations = NULL;
    Districts[d_count].candidates = NULL;
    d_count++;

    return 0;
}
void printAllDistricts(int did, int seats)
{
    printf("D %d %d\n", did, seats);
    printf("  Districts = ");
    for(int i = 0; i < d_count; i++)
    {
        if(i != 0) printf(", ");
        printf("%d", Districts[i].did);
    }
    printf("\nDONE\n");
}


//event M
void count_votes(int did){
    int i = 0;
    while(Districts[i].did != did){
        i++;
    }
    int partyVotes[PartySize] = {0}; // we know that the party_id is from 0 - 4
    struct candidate *currentC = Districts[i].candidates;
    while(currentC != NULL)
    {
        partyVotes[currentC->pid] += currentC->votes;
        currentC = currentC->next;
    }
    int totalVotes = 0;
    for(int j = 0; j < PartySize; j++)
    {
        totalVotes += partyVotes[j];
    }
    double EklogikoMetro = (double)(totalVotes)/Districts[i].seats;
    int partySeats[PartySize] = {0};
    for(int k = 0; k < PartySize; k++)
    {
        partySeats[k] = (int)floor(partyVotes[k]/EklogikoMetro);
    }

    // printing
    printf("M %d\n", did);
    printf(" Seats =\n");
    // done printing headers


    int partyElected[PartySize] = {0};
    currentC = Districts[i].candidates;
    // iteration for every candidate and select the candidates of every party with the highest votes (for every party select onlt partsSeats[i] seats)
    while(currentC != NULL)
    {
        if(partyElected[currentC->pid] < partySeats[currentC->pid]){
            currentC->elected = 1;

            insertToParty(currentC);
            Parties[currentC->pid].nelected++;
            Districts[i].allotted++;
            partyElected[currentC->pid]++;

            //printing 
            printf("  %4d %4d %4d\n", currentC->cid, currentC->pid, currentC->votes);
        }
        currentC = currentC->next;
    }
    printf("DONE\n");
}

void insertToParty(struct candidate *copy){
    struct candidate *newC = (struct candidate *)malloc(sizeof(struct candidate));
            newC->cid = copy->cid;
            newC->pid = copy->pid;
            newC->votes = copy->votes;
            newC->elected = copy->elected; // 1
            newC->prev = NULL;
    int pid = newC->pid;
    struct candidate *currentC = Parties[pid].elected;
    struct candidate *prvC = NULL;
    if(currentC == NULL)
    {
        Parties[pid].elected = newC;
        newC->next = NULL;
    }else{
        while (currentC != NULL && currentC->votes >= newC->votes){
            prvC = currentC;
            currentC = currentC->next;
        }

        if (prvC == NULL) {
            newC->next = Parties[pid].elected;
            Parties[pid].elected = newC;
        } else {
            prvC->next = newC;
            newC->next = currentC;
        }
    }
    
}

//event I
void print_district(int did){
    int i = 0;
    while(Districts[i].did != did){
        i++;
    }
    int seats = Districts[i].seats;
    int blanks = Districts[i].blanks;
    int voids = Districts[i].voids;
    printf("I %d\n", did);
    printf(" Seats = %d\n", seats);
    printf(" Blanks = %d\n", blanks);
    printf(" Voids = %d\n", voids);
    printf(" Candidates =\n");
    struct candidate *currentC = Districts[i].candidates;
    while(currentC != NULL){
        printf("  %4d %4d %4d\n", currentC->cid, currentC->pid, currentC->votes);
        currentC = currentC->next;
    }
    printf(" Stations = ");
    struct station *currentS = Districts[i].stations;
    while(currentS != NULL){
        printf("%d", currentS->sid);
        if(currentS->next != NULL){
            printf(", ");
        }else{
            printf("\n");
        }
        currentS = currentS->next;
    }
    printf("DONE\n");
}