// File: main.c
// Author: Danny Holt
// This file contains the main function for the "Greasy cards" game.

#include <stdio.h>
#include <pthread.h>



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
    pid_t pids[numPlayers];

    for (int i = 0; i < numPlayers; i++){
        pid_t pid = fork();

        if (pid < 0) {
            printf("Error creating player %d\n", i);
            exit(1);
        }
        // This is a new player/child process
        if (pid == 0) {
            printf("Player %d created\n", i);

            // TODO: Handle player actions

            exit(0); // Exit child process once it's done
        }
    }

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





    return 0;
}