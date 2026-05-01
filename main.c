// File: main.c
// Author: Danny Holt
// This file contains everything necessary for the "Greasy Cards" game.

#define _POSIX_C_SOURCE 200809L // Might remove
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

// CARD INFO
#define NUM_SUITS 4
#define NUM_RANKS 13
#define DECK_SIZE 52

// Card struct to represent a single card in the deck.
typedef struct {
    int value;
    int suit;
} Card;

// Deck struct to represent the deck of cards for a game.
typedef struct {
    Card cards[DECK_SIZE];
    int topIndex;
    int bottomIndex;
} Deck;

// Hand struct to represent a player's hand of cards.
typedef struct {
    Card handCards[DECK_SIZE];
    int topIndex;
} Hand;

// GameState struct to track state of game.
typedef struct{
    Deck deck;
    Hand playerHands[DECK_SIZE]; // Only indicies 0 to numPlayers-1 will be used.
                                 // Index "n" represents the hand of player "n".
    Card greasyCard;

    pthread_mutex_t deckMutex;
    pthread_mutex_t rngMutex;
    pthread_mutex_t chipBagMutex;

    int numChipsInBag;
    int numPlayers;
    int seed;
} GameState;

typedef struct{
    int numChipsInBag;
} BagOfChips;

// This struct will hold info to be passed to each player thread.
struct thread_args {
    int playerID;
    int numRounds;
    GameState *gameState;
};

// Barriers for synchrinozing player actions
pthread_barrier_t roundEndBarrier;
pthread_barrier_t dealerEndBarrier;
pthread_barrier_t playerCreationBarrier;

// Log file for loggig player actions
FILE *logFile;
pthread_mutex_t logFileMutex;



// writeToLog: Allows a process to write to the shared log file.
void writeToLog(const char *format, ...){
    pthread_mutex_lock(&logFileMutex);

    va_list args;

    // Print to console
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    // Write to log file
    va_start(args, format);
    vfprintf(logFile, format, args);
    va_end(args);

    fflush(logFile);

    pthread_mutex_unlock(&logFileMutex);
}

// initGameState: Initializes the GameState and deck.
//
// *gameState: Pointer to the GameState.
// seed: Seed for RNG.
void initGameState(GameState *gameState, int seed, int numPlayers, int numChipInBag) {

    printf("Initializing GameState...\n");

    // Initialize deck and mutexes, set RNG seed
    initDeck(&gameState->deck);
    pthread_mutex_init(&gameState->deckMutex, NULL);
    pthread_mutex_init(&gameState->rngMutex, NULL);
    pthread_mutex_init(&gameState->chipBagMutex, NULL);
    gameState->seed = seed;
    gameState->numPlayers = numPlayers;
    gameState->numChipsInBag = numChipInBag;

    // Initialize player hands to be empty
    for (int i = 0; i < DECK_SIZE; i++) {
        gameState->playerHands[i] = (Hand){0};
    }
}

// ********** CHIPS FUNCTIONS **********

void initBagOfChips(BagOfChips *bag, int numChipsInBag) {
    bag->numChipsInBag = numChipsInBag;
}

// eatChips: Eats a number of chips out of the current bag, and opens a new one if needed.
//
// *gameState: Pointer to the GameState.
// *bag: Pointer to the current bag of chips.
// playerID: ID of the player eating the chips.
void eatChips(GameState *gameState, BagOfChips *bag, int playerID) {
    
    pthread_mutex_lock(&gameState->rngMutex);
    int chipsToEat = getRandomInt(gameState, 1, 5);
    pthread_mutex_unlock(&gameState->rngMutex);

    pthread_mutex_lock(&gameState->chipBagMutex);
    
    int overflow = chipsToEat - bag->numChipsInBag;
    if (overflow > 0) {
        writeToLog("Player %d wants to eat %d chips, but only %d are left in the bag. They eat the remaining chips and open a new bag.\n", playerID, chipsToEat, bag->numChipsInBag);
        bag->numChipsInBag = gameState->numChipsInBag; // Finish current bag, open new bag
        bag->numChipsInBag -= overflow; // Eat remaining chips from new bag
    }
    else{
        bag->numChipsInBag -= chipsToEat;
    }

    pthread_mutex_unlock(&gameState->chipBagMutex);
}

// ********** DECK AND CARD FUNCTIONS **********

// initDeck: This will initialize the deck of cards for the game.
//
// *deck: Pointer to the deck to be initialized.
void initDeck(Deck *deck) {

    printf("Initializing deck...\n");

    int index = 0;
    for (int suit = 0; suit < NUM_SUITS; suit++) {
        for (int rank = 1; rank <= NUM_RANKS; rank++) {
            deck->cards[index].value = rank;
            deck->cards[index].suit = suit;
            index++;
        }
    }

    deck->topIndex = 0;
    deck->bottomIndex = DECK_SIZE - 1;
}

// shuffleDeck: This will shuffle the deck of cards for the game.
//
// *gameState: Pointer to the gameState.
void shuffleDeck(GameState *gameState) {

    printf("Shuffling deck...\n");

    pthread_mutex_lock(&gameState->deckMutex);

    for (int i = 0; i < DECK_SIZE; i++) {
        int j = getRandomInt(gameState, i, DECK_SIZE - 1);
        Card temp = gameState->deck.cards[i];
        gameState->deck.cards[i] = gameState->deck.cards[j];
        gameState->deck.cards[j] = temp;
    }

    gameState->deck.topIndex = 0;

    pthread_mutex_unlock(&gameState->deckMutex);
}

// drawCard: Will draw the top card from the deck
//
// *gameState: Pointer to the gameState.
Card drawCard(GameState *gameState) {
    pthread_mutex_lock(&gameState->deckMutex);

    Card drawnCard = gameState->deck.cards[gameState->deck.topIndex];
    gameState->deck.topIndex = (gameState->deck.topIndex + 1) % DECK_SIZE;

    pthread_mutex_unlock(&gameState->deckMutex);
    return drawnCard;
}

// returnCardToEndOfDeck: Returns a card to the end/bottom of the deck.
// *gameState: Pointer to the gameState.
// card: The card to be returned to the end of the deck.
void returnCardToEndOfDeck(GameState *gameState, Card card) {
    pthread_mutex_lock(&gameState->deckMutex);

    gameState->deck.bottomIndex = (gameState->deck.bottomIndex + 1) % DECK_SIZE;
    gameState->deck.cards[gameState->deck.bottomIndex] = card;

    pthread_mutex_unlock(&gameState->deckMutex);
}

// addCardToHand: Adds a card to a player's hand.
//
// *gameState: Pointer to the GameState
// playerID: ID of the player who is recieving the card
// card: The card to be recieved.
void addCardToHand(GameState *gameState, int playerID, Card card) {
    Hand *hand = &gameState->playerHands[playerID];
    hand->topIndex++;
    hand->handCards[hand->topIndex] = card;
}

// setGreasyCard: Sets the greasy card for the round.
//
// *gameState: Pointer to the GameState
// card: The card to be set as the greasy card.
void setGreasyCard(GameState *gameState, Card card) {
    gameState->greasyCard = card;
}

// areCardsSameValue: Checks if two cards have the same value.
// This will be used for players comparing their hand to the greasy card.
//
// Card1: First card to compare.
// Card2: Second card to compare.
//
// Returns true if both cards have the same value, false otherwise.
bool areCardsSameValue(Card card1, Card card2) {
    return card1.value == card2.value;
}

// ********** RANDOM NUMBER GENERATION **********

// getRandomInt: Returns a random int.
// Used for shuffling deck, discarding cards, and eating chips.
//
// *gameState: Pointer to the game state, used for accessing RNG seed and mutex.
// min: Minimum value for random int (inclusive).
// max: Maximum value for random int (inclusive).
int getRandomInt(GameState *gameState, int min, int max) {
    pthread_mutex_locK(&gameState->rngMutex);

    int randomResult = min + rand_r(&gameState->seed) % (max - min + 1);

    pthread_mutex_unlock(&gameState->rngMutex);

    return randomResult;
}

// ********** PLAYER ACTIONS **********

void doDealerActions(int playerID, GameState *gameState) {
    // If they're the dealer:
        // Shuffle cards
        // Choose random card (Greasy card)
        // Deal single card to each player
        // Open first bag of potato chips
        // Wait for round to end
    
    // Shuffle cards, choose random card as greasy card
    shuffleDeck(gameState);
    Card greasyCard = drawCard(gameState);
    writeToLog("Player %d (dealer) has chosen the greasy card: value %d, suit %d.\n", playerID, greasyCard.value, greasyCard.suit);

    // Deal single card to each player
    for (int i = 0; i < gameState->numPlayers; i++) {
        Card dealtCard = drawCard(gameState);
        addCardToHand(gameState, i, dealtCard);
        writeToLog("Player %d has been dealt card: value %d, suit %d.\n", i, dealtCard.value, dealtCard.suit);
    }

    // Open first bag of potato chips
    // Wait for round to end

}

void doNonDealerActions(int playerID, GameState *gameState) {
    // If they're not the dealer:
        // Draw card from deck
        // See if any cards in hand match value of greasy card.
        // If no cards in hand are greasy card (match value), discard one card at random (place at end of deck).
        // If there is a match, player shows hand, declares self as winner, and round ends.

    // Draw card, add to hand
    Card drawnCard = drawCard(gameState);
    addCardToHand(gameState, playerID, drawnCard);


    // See if any cards in hand match greasy card
    Hand *hand = &gameState->playerHands[playerID];
    bool hasGreasyCard = false;
    for (int i = 0; i < hand->topIndex; i++) {
        if (areCardsSameValue(hand->handCards[i], gameState->greasyCard)) {
            hasGreasyCard = true;
            break;
        }
    }

    // If they have a card matching the greasy card, they show their hand,
    // declare themself as a winner, and the round ends.
    if (hasGreasyCard) {
        writeToLog("Player %d has a card matching the greasy card and wins the round! Their hand:\n", playerID);
        for (int i = 0; i < hand->topIndex; i++) {
            writeToLog("Card %d: value %d, suit %d\n", i, hand->handCards[i].value, hand->handCards[i].suit);
        }
    }
    // If they don't have a card matching the greasy card, they discard a random card by
    // placing it at the end of the deck.
    else {

    }
}

// playerThread: Logic for each player thread
//
// *arg: Pointer to the player's ID
void *playerThread(void *newThreadArgs) {

    struct thread_args *newThreadData = (struct thread_args *)newThreadArgs;
    int playerID = newThreadData->playerID;
    int numRounds = newThreadData->numRounds;

    writeToLog("Player %d is starting their thread.\n", playerID);

    // Wait for all players to be created before starting the game.
    pthread_barrier_wait(&playerCreationBarrier);


    // At the end of round "m", player "m" (dealer) will signal for next round to start.
    // -> game ends when all [numPlayers] rounds are completed


    for (int round = 0; round < numRounds; round++){

        // DEALER ACTIONS
        if (playerID == round){
            // DO DEALER SETUP
            writeToLog("Player %d is the dealer for round %d.\n", playerID, round);
            doDealerActions(playerID, newThreadData->gameState);
        }

        // Everyone waits for the dealer to finish their setup for the round.
        pthread_barrier_wait(&dealerEndBarrier);

        // NON DEALER ACTIONS
        if (playerID != round){
            // DO NON-DEALER ACTIONS
            writeToLog("Player %d is a non-dealer for round %d.\n", playerID, round);
        }

        // END OF ROUND: Everyone waits for everyone to finish round.
        pthread_barrier_wait(&roundEndBarrier); 
    }
    


    return NULL;
}


// ********** MAIN FUNCTION **********

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

    // Validate input
    if (seed == NULL || numPlayers <= 0 || numChipsInBag <= 0) {
        printf("Invalid input. Please enter valid seed, number of players, and number of chips in bag.\n");
        return 1;
    }
    
    // Initialize GameState
    GameState gameState;
    initGameState(&gameState, seed, numPlayers, numChipsInBag);

    // Initialize log file
    logFile = fopen("gameLog.txt", "w");
    if (logFile == NULL) {
        printf("Error opening log file.\n");
        return 1;
    }
    pthread_mutex_init(&logFileMutex, NULL);
    writeToLog("Game started with seed %d, %d players, and %d chips in bag.\n", seed, numPlayers, numChipsInBag);

    // Number of rounds = number of players
    int numRounds = numPlayers;

    pthread_t threads[numPlayers]; // Holds threads
    int playerIDs[numPlayers]; // Stores playerIDs (might remove later)
    struct thread_args threadArgs[numPlayers]; // Struct to hold arguments for each thread (playerID, numRounds)

    // Initialize thread barriers for:
       // Dealer turn end
       // Round end
       // Player creation/joining game
    pthread_barrier_init(&roundEndBarrier, NULL, numPlayers);
    pthread_barrier_init(&dealerEndBarrier, NULL, numPlayers);
    pthread_barrier_init(&playerCreationBarrier, NULL, numPlayers);

    // Create a thread for each player, and let their logic play out
    for (int t = 0; t < numPlayers; t++){
        playerIDs[t] = t;

        printf("Creating thread for player %d.\n", t);
        threadArgs[t].playerID = t;
        threadArgs[t].numRounds = numRounds;
        threadArgs[t].gameState = &gameState;

        // Create thread for player
        // NOTE TO SELF: &threads[t]: address of thread for player t.
        // NOTE TO SELF: NULL: default thread attributes.
        // NOTE TO SELF: playerThread: function to run in thread.
        // NOTE TO SELF: (void *)&newThread: argument to pass to playerThread.
        if (pthread_create(&threads[t], NULL, playerThread, (void *)&threadArgs[t]) != 0) {
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



    printf("All player threads have finished.\n");
    return 0;
}

