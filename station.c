#include "elections.h"
#include <stdio.h>
#include <stdlib.h>

// event S
int create_station(int sid, int did){
    int i = 0;
    while(i < DistrictSize && Districts[i].did != did)
    {
        i++;
    }
    if(i == DistrictSize) return 1; // there is no district with is did
    struct station *new;
    new = (struct station*)malloc(sizeof(struct station));
    new->sid = sid;
    new->registered = 0;
    struct voter *sentinel = (struct voter *)malloc(sizeof(struct voter));
    sentinel->vid = -1;
    sentinel->voted = 0;
    sentinel->next = NULL;
    new->vsentinel = sentinel;
    new->voters = new->vsentinel;
    new->next = Districts[i].stations;
    Districts[i].stations = new;
    return 0;
}

void printAllStations(int sid, int did)
{
    int i;
    for(i = 0; i < DistrictSize; i++)
    {
        if(Districts[i].did == did) break;
    }
    printf("S %d %d\n", sid, did);
    printf("  Stations = ");
    struct station *currentS = Districts[i].stations;
    while(currentS != NULL)
    {
        printf("%d, ", currentS->sid);
        currentS = currentS->next;
    }
    printf("\nDONE\n");
}

// event E
void delete_empty_stations(void){
    // and printint it
    printf("E\n");
    struct station *prv = NULL;
    struct station *current = NULL;
    for(int i = 0; i < DistrictSize; i++)
    {
        prv = NULL;
        current = Districts[i].stations;
        while(current!= NULL)
        {
            if(current->registered <= 0)
            {
                printf("  %4d %4d\n", current->sid, Districts[i].did);
                struct station *tmp = current->next;
                if(prv == NULL)
                {
                    Districts[i].stations = tmp;
                }else{
                    prv->next = tmp;
                }
                free(current);
                current = tmp;
            }
            else{
                prv = current;
                current = current->next;
            }
        }
    }

    printf("DONE\n");

}

// event J
void print_station(int sid, int did){
    int i = 0;
    while(Districts[i].did != did){
        i++;
    }
    struct station *currentS = Districts[i].stations;
    while(currentS != NULL && currentS->sid != sid){
        currentS = currentS->next;
    }
    if(currentS == NULL)return;
    printf("J %d\n", sid);
    printf(" Registered = %d\n", currentS->registered);
    printf(" Voters =\n");
    struct voter *currentV = currentS->voters;
    while(currentV->vid != -1){
        printf("  %4d %4d\n", currentV->vid, currentV->voted);
        currentV = currentV->next;
    }
    printf("DONE\n");
}