// File: main.c
// Author: Danny Holt
// This file contains the main function for the "Greasy cards" game.
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>


// playerThread: Logic for each player thread
//
// *arg: Pointer to the player's ID
void *playerThread(void *arg) {
    long playerID = *(int *)arg;
    printf("Now in playerThread\n");
    printf("Player %ld is starting their thread.\n", playerID);

    return NULL;
}

int main() {

    int seed;
    int numPlayers;
    int numChipsInBag;

    printf("Input seed: ");
    scanf("%d", &seed);
    printf("Input number of players: ");
    scanf("%d", &numPlayers);
    printf("Input number of chips in bag: ");
    scanf("%d", &numChipsInBag);

    // Number of rounds = number of players
    int numRounds = numPlayers;

    // Creating a thread for each player
    pthread_t threads[numPlayers];
    int playerIDs[numPlayers];

    for (int t = 0; t < numPlayers; t++){
        playerIDs[t] = t;

        if (pthread_create(&threads[t], NULL, playerThread, (void *)&playerIDs[t]) != 0) {
            printf("Error creating thread for player %d\n", t);
            return 1;
        }
    }

    for (int i = 0; i < numPlayers; i++){
        pthread_join(threads[i], NULL);
    }

    // The parent process should wait until all child processes/players are done before exiting
    // for (int i = 0; i < numPlayers; i++) {
    //     waitpid(pids[i], NULL, 0);
    // }

    // In each round:
    // One player is dealer:
        // Shuffle cards
        // Choose random card (Greasy card)
        // Deal single card to each player
        // Open first bag of potato chips
        // Wait for round to end
    
    // Players eat random number of chips bt [1,5] after playing, before next turn
    // -> Only one player can grab chips at a time
    // When a player finds there's no chips left, they open a new bag.

    // There will be one thread per player

    // Each round (non-dealers)



    printf("All player threads have finished.\n");
    return 0;
}

