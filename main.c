// File: main.c
// Author: Danny Holt
// This file contains the main function for the "Greasy cards" game.

#define _POSIX_C_SOURCE 200809L // Might remove
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>


// This struct will hold info to be passed to each player thread.
struct thread_args {
    int playerID;
    int numRounds;
};


// playerThread: Logic for each player thread
//
// *arg: Pointer to the player's ID
void *playerThread(void *newThreadArgs) {

    struct thread_args *newThreadData = (struct thread_args *)newThreadArgs;
    int playerID = newThreadData->playerID;
    int numRounds = newThreadData->numRounds;

    printf("Now in playerThread\n");
    printf("Player %d is starting their thread.\n", playerID);

    // What does each player do?
    // If they're the dealer:
        // Shuffle cards
        // Choose random card (Greasy card)
        // Deal single card to each player
        // Open first bag of potato chips
        // Wait for round to end

    // If they're not the dealer:
        // Draw card from deack and compare to "greasy card"
        // If no cards in hand are greasy card (match value), discard one card at random (place at end of deck).
        // If there is a match, player shows hand, declares self as winner, and round ends.

    // At the end of round "m", player "m" (dealer) will signal for next round to start.
    // -> game ends when all [numPlayers] rounds are completed

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

    pthread_t threads[numPlayers]; // Holds threads
    int playerIDs[numPlayers]; // Stores playerIDs (might remove later)

    // Create a thread for each player, and let their logic play out
    for (int t = 0; t < numPlayers; t++){
        playerIDs[t] = t;

        struct thread_args newThread = { .playerID = t, .numRounds = numRounds };

        if (pthread_create(&threads[t], NULL, playerThread, (void *)&newThread) != 0) {
            printf("Error creating thread for player %d\n", t);
            return 1;
        }
    }

    // Wait for all of the player threads to finish
    for (int i = 0; i < numPlayers; i++){
        pthread_join(threads[i], NULL);
    }


    
    // Players eat random number of chips bt [1,5] after playing, before next turn
    // -> Only one player can grab chips at a time
    // When a player finds there's no chips left, they open a new bag.

    // Each round (non-dealers):



    printf("All player threads have finished.\n");
    return 0;
}

