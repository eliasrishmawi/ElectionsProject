#include "elections.h"
#include <stdio.h>
#include <stdlib.h>

// event A
void announce_elections(void){
    for(int i = 0; i< DistrictSize; i++)
    {
        Districts[i].did = -1;
        Districts[i].seats = -1;
        Districts[i].allotted = -1;
        Districts[i].blanks = -1;
        Districts[i].voids = -1;
        Districts[i].stations = NULL;
        Districts[i].candidates = NULL;
    }
    int p_size = 5;
    for(int i = 0; i < p_size; i++)
    {
        Parties[i].pid = -1;
        Parties[i].nelected = -1;
        Parties[i].elected = NULL;
    }
    Parliament.members = NULL;
}

void print_unfilledSeats();
//event G
void form_government(void){
    // find party ID with the most total elected candidates (nelected)
    int totalElected = 0;
    int mostElectedPid = -1;
    for(int i = 0; i < PartySize; i++){
        if(Parties[i].nelected > totalElected){
            mostElectedPid = i;
            totalElected = Parties[i].nelected;
        }
    }
    for(int j = 0; j < DistrictSize; j++){
        int leftover = Districts[j].seats - Districts[j].allotted;
        struct candidate *currentC = Districts[j].candidates;
        while(leftover > 0 && currentC != NULL && currentC->elected != 1){
            if(currentC->pid == mostElectedPid){
                currentC->elected = 1;
                insertToParty(currentC);
                Parties[mostElectedPid].nelected++;
                totalElected = Parties[mostElectedPid].nelected;
                Districts[j].allotted++;
                leftover--;
            }
            currentC = currentC->next;
        }
        currentC = Districts[j].candidates;
        if(leftover > 0){
            while(currentC != NULL && leftover > 0){
                if(currentC->elected != 1){
                    currentC->elected = 1;
                    insertToParty(currentC);
                    Parties[currentC->pid].nelected++;
                    Districts[j].allotted++;
                    leftover--;
                }
                currentC = currentC->next;
            }
        }
    }
    //printing
    print_unfilledSeats();

}

void print_unfilledSeats(){
    printf("G\n");
    printf(" Seats =\n");
    for(int i = 0; i < DistrictSize; i++){
        struct candidate *currentC = Districts[i].candidates;
        while(currentC != NULL){
            if(currentC->elected != 1)
            {
                printf("  %4d %4d %4d\n", Districts[i].did, currentC->cid, currentC->votes);
            }
            currentC = currentC->next;
        }
    }
    printf("DONE\n");
}

struct candidate *copyList(struct candidate *C);
struct candidate *sort_merge(struct candidate *C1, struct candidate *C2);
void print_members();
// event N
void form_parliament(void){
    struct candidate *copy0 = copyList(Parties[0].elected);
    struct candidate *copy1 = copyList(Parties[1].elected);
    struct candidate *copy2 = copyList(Parties[2].elected);
    struct candidate *copy3 = copyList(Parties[3].elected);
    struct candidate *copy4 = copyList(Parties[4].elected);

    struct candidate *merge01 = sort_merge(copy0, copy1);
    struct candidate *merge23 = sort_merge(copy2, copy3);
    struct candidate *merge234 = sort_merge(merge23, copy4);
    struct candidate *merge01234 = sort_merge(merge234, merge01);

    Parliament.members = merge01234;

    print_members();

}

struct candidate *copyList(struct candidate *C){
    if(C == NULL) return NULL;
    struct candidate *new = (struct candidate *)malloc(sizeof(struct candidate));
    new->cid = C->cid;
    new->votes = C->votes;
    new->elected = C->elected;
    new->pid = C->pid;
    new->prev = C->prev; // NULL
    new->next = copyList(C->next);
    return new;
}

struct candidate *sort_merge(struct candidate *C1, struct candidate *C2){
    if(C1 == NULL) return C2;
    if(C2 == NULL) return C1;

    if(C1->votes >= C2->votes){
        C1->next = sort_merge(C1->next, C2);
        return C1;
    }else{
        C2->next = sort_merge(C1, C2->next);
        return C2;
    }

}

void print_members(){
    printf("N\n");
    printf(" Members =\n");
    int count = 0;
    struct candidate *currentC = Parliament.members;
    while(currentC != NULL)
    {
        printf("  %4d %4d %4d\n", currentC->cid, currentC->pid, currentC->votes);
        currentC = currentC->next;
        count++;
    }
    printf("Number of Parliament members = %d\n", count);
    printf("DONE\n");
}

// event L
void print_parliament(void){
    printf("L\n");
    printf(" Members =\n");
    int count = 0;
    struct candidate *currentM = Parliament.members;
    while(currentM != NULL){
        printf("  %4d %4d %4d\n", currentM->cid, currentM->pid, currentM->votes);
        currentM = currentM->next;
        count++;
    }
    printf("Number of Parliament members = %d\n", count);
    printf("DONE\n");
}
