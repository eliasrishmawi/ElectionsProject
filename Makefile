CC = gcc
CFLAGS = -Wall -g

cs240Elections: main.c elections.c district.c station.c party.c candidate.c voter.c elections.h
	$(CC) $(CFLAGS) main.c elections.c district.c station.c party.c candidate.c voter.c -o cs240Elections -lm

.PHONY: clean

clean:
	rm -f cs240Elections