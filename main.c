/*
 * ============================================
 * file: main.c
 * @Author Elias Rishmawi
 * @Version 24/10/2023
 *
 *
 * @brief Main function
 *        for CS240 Project Phase 1,
 *        Winter Semester 2024-2025
 * @see   Compile using supplied Makefile by running: make
 * ============================================
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "elections.h"

/* Maximum input line size */
#define MAX_LINE 1024

/* 
 * Uncomment the following line to
 * enable debugging prints
 * or comment to disable it
 */
/* #define DEBUG */
//#ifdef DEBUG
//#define DPRINT(...) fprintf(stderr, __VA_ARGS__);
//#else
//#define DPRINT(...)
//#endif /* DEBUG */

struct district Districts[56];
struct party Parties[5];
struct parliament Parliament;

void destroy_structures(void)
{
	for(int i = 0; i < DistrictSize; i++){
		struct candidate *currentC = Districts[i].candidates;
		struct candidate *tmpC;
		while(currentC != NULL){
			tmpC = currentC->next;
			free(currentC);
			currentC = tmpC;
		}

		struct station *currentS = Districts[i].stations;
		while(currentS != NULL){
			struct station *tmpS = currentS->next;
			struct voter *currentV = currentS->voters;
			while(currentV != NULL){
				struct voter *tmpV = currentV->next;
				free(currentV);
				currentV = tmpV;
			}
			free(currentS);
			currentS = tmpS;
		}
	}

	for(int i = 0; i < PartySize; i++){
		struct candidate *currentC = Parties[i].elected;
		while(currentC != NULL){
			struct candidate *tmpC = currentC->next;
			free(currentC);
			currentC = tmpC;
		}
	}

	struct candidate *currentM = Parliament.members;
	while(currentM != NULL){
		struct candidate *tmpM = currentM->next;
		free(currentM);
		currentM = tmpM;
	}

}

int main(int argc, char *argv[])
{
	FILE *event_file;
	char line_buffer[MAX_LINE];

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <input_file>\n",
				argv[0]);
		exit(EXIT_FAILURE);
	}

	event_file = fopen(argv[1], "r");
	if (!event_file) {
		perror("fopen error for event file open");
		exit(EXIT_FAILURE);
	}

	while (fgets(line_buffer, MAX_LINE, event_file)) {
		char *trimmed_line;
		char event;
		int did, sid, vid, cid, pid, seats;
		int ret = 0;
		/*
		 * First trim any whitespace
		 * leading the line.
		 */
		trimmed_line = line_buffer;
		while (trimmed_line && isspace(*trimmed_line))
			trimmed_line++;
		if (!trimmed_line)
			continue;
		/* 
		 * Find the event,
		 * or comment starting with #
		 */
		if (sscanf(trimmed_line, "%c", &event) != 1) {
			fprintf(stderr, "Could not parse event type out of input line:\n\t%s",
					trimmed_line);
			fclose(event_file);
			exit(EXIT_FAILURE);
		}

		switch (event) {
			/* Comment, ignore this line */
			case '#':
				break;
			case 'A':
				announce_elections();
				printf("DONE\n\n");
				break;
			case 'D':
				if (sscanf(trimmed_line, "D %d %d", &did, &seats) != 2) {
					fprintf(stderr, "Event D parsing error\n");
					ret = 1;
					break;
				}
				ret = create_district(did, seats);
				printAllDistricts(did, seats);
				break;
			case 'S':
				if (sscanf(trimmed_line, "S %d %d", &sid, &did) != 2) {
					fprintf(stderr, "Event S parsing error\n");
					ret = 1;
					break;
				}
				ret = create_station(sid, did);
				printAllStations(sid, did);
				break;
			case 'P':
				if (sscanf(trimmed_line, "P %d", &pid) != 1) {
					fprintf(stderr, "Event P parsing error\n");
					ret = 1;
					break;
				}
				create_party(pid);
				printAllParties(pid);
				break;
			case 'C':
				if (sscanf(trimmed_line, "C %d %d %d", &cid, &did, &pid) != 3) {
					fprintf(stderr, "Event C parsing error\n");
					ret = 1;
					break;
				}
				ret = register_candidate(cid, did, pid);
				printAllCandidates(cid, did, pid);
				break;
			case 'R':
				if (sscanf(trimmed_line, "R %d %d %d", &vid, &did, &sid) != 3) {
					fprintf(stderr, "Event R parsing error\n");
					ret = 1;
					break;
				}
				ret = register_voter(vid, did, sid);
				printAllVoters(vid, did, sid);
				break;
			case 'U':
				if (sscanf(trimmed_line, "U %d", &vid) != 1) {
					fprintf(stderr, "Event U parsing error\n");
					ret = 1;
					break;
				}
				ret = unregister_voter(vid);
				break;
			case 'E':
				delete_empty_stations();
				break;
			case 'V':
				if (sscanf(trimmed_line, "V %d %d %d", &vid, &sid, &cid) != 3) {
					fprintf(stderr, "Event V parsing error\n");
					ret = 1;
					break;
				}
				ret = vote(vid, sid, cid);
				break;
			case 'M':
				if (sscanf(trimmed_line, "M %d", &did) != 1) {
					fprintf(stderr, "Event M parsing error\n");
					ret = 1;
					break;
				}
				count_votes(did);
				break;
			case 'G':
				form_government();
				break;
			case 'N':
				form_parliament();
				break;
			case 'I':
				if (sscanf(trimmed_line, "I %d", &did) != 1) {
					fprintf(stderr, "Event I parsing error\n");
					ret = 1;
					break;
				}
				print_district(did);
				break;
			case 'J':
				if (sscanf(trimmed_line, "J %d %d", &sid, &did) != 2) {
					fprintf(stderr, "Event J parsing error\n");
					ret = 1;
					break;
				}
				print_station(sid, did);
				break;
			case 'K':
				if (sscanf(trimmed_line, "K %d", &pid) != 1) {
					fprintf(stderr, "Event I parsing error\n");
					ret = 1;
					break;
				}
				print_party(pid);
				break;
			case 'L':
				print_parliament();
				break;
			default:
				fprintf(stderr, "WARNING: Unrecognized event %c. Continuing...\n",
						event);
				ret = 1;
				break;
		}
		fprintf(stderr, "Event %c %s.\n", event,
				ret ? "failed" : "succeeded");
	}
	fclose(event_file);
	destroy_structures();
	return 0;
}


