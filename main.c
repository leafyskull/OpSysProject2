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



// ********** STRUCT DEFINITIONS **********

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

typedef struct{
    int numChipsInBag;
} BagOfChips;

// GameState struct to track state of game.
typedef struct{
    Deck deck;
    Hand playerHands[DECK_SIZE]; // Only indicies 0 to numPlayers-1 will be used.
                                 // Index "n" represents the hand of player "n".
    Card greasyCard;

    pthread_mutex_t deckMutex;
    pthread_mutex_t rngMutex;
    pthread_mutex_t chipBagMutex;
    pthread_mutex_t winnerMutex;

    int numChipsInBag;
    int numPlayers;
    int seed;

    BagOfChips chipBag;

    bool hasRoundWinner;
} GameState;

// This struct will hold info to be passed to each player thread.
typedef struct {
    int playerID;
    int numRounds;
    GameState *gameState;
} threadArgs;

// ********** FUNCTION PROTOTYPES **********

// Logging
void writeToLog(const char *format, ...);

// Game state
void initGameState(GameState *gameState, int seed, int numPlayers, int numChipsInBag);

// Chips
void initBagOfChips(GameState *gameState, int numChipsInBag);
void eatChips(GameState *gameState, BagOfChips *bag, int playerID);

// Deck & cards
void initDeck(Deck *deck);
void shuffleDeck(GameState *gameState);
Card drawCard(GameState *gameState);
void returnCardToEndOfDeck(GameState *gameState, Card card);
void addCardToHand(GameState *gameState, int playerID, Card card);
void setGreasyCard(GameState *gameState, Card card);
bool areCardsSameValue(Card card1, Card card2);
void clearHands(GameState *gameState);

// Random
int getRandomInt(GameState *gameState, int min, int max);

// Player actions
void doDealerActions(int playerID, GameState *gameState);
void doNonDealerActions(int playerID, GameState *gameState);

// Thread
void *playerThread(void *newThreadArgs);


// ********** GLOBAL BARRIERS & LOG FILE STUFF **********
pthread_barrier_t roundEndBarrier;
pthread_barrier_t dealerEndBarrier;
pthread_barrier_t playerCreationBarrier;

// Log file for logging player actions
FILE *logFile;
pthread_mutex_t logFileMutex;



// ********** LOGGING FUNCTION **********

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



// ********** GAMESTATE FUNCTIONS **********

// initGameState: Initializes the GameState and deck.
//
// *gameState: Pointer to the GameState.
// seed: Seed for RNG.
void initGameState(GameState *gameState, int seed, int numPlayers, int numChipsInBag) {

    printf("Initializing GameState...\n");

    // Initialize deck and mutexes, set RNG seed
    initDeck(&gameState->deck);
    pthread_mutex_init(&gameState->deckMutex, NULL);
    pthread_mutex_init(&gameState->rngMutex, NULL);
    pthread_mutex_init(&gameState->chipBagMutex, NULL);
    pthread_mutex_init(&gameState->winnerMutex, NULL);
    gameState->seed = seed;
    gameState->numPlayers = numPlayers;

    gameState->numChipsInBag = numChipsInBag;

    // Initialize player hands to be empty
    for (int i = 0; i < DECK_SIZE; i++) {
        gameState->playerHands[i] = (Hand){0};
    }

    gameState->hasRoundWinner = false;
}

// hasRoundWinner: Checks if there is already a winner for the round.
//
// *gameState: Pointer to the GameState.
bool hasRoundWinner(GameState *gameState) {
    pthread_mutex_lock(&gameState->winnerMutex);
    bool result = gameState->hasRoundWinner;
    pthread_mutex_unlock(&gameState->winnerMutex);
    return result;
}

// setRoundWinner: Sets that there is a winner for the round.
//
// *gameState: Pointer to the GameState.
void setRoundWinner(GameState *gameState) {
    pthread_mutex_lock(&gameState->winnerMutex);
    gameState->hasRoundWinner = true;
    pthread_mutex_unlock(&gameState->winnerMutex);
}

// clearRoundWinner: Clears the round winner for the next round.
//
// *gameState: Pointer to the GameState.
void clearRoundWinner(GameState *gameState) {
    pthread_mutex_lock(&gameState->winnerMutex);
    gameState->hasRoundWinner = false;
    pthread_mutex_unlock(&gameState->winnerMutex);
}



// ********** CHIPS FUNCTIONS **********

void initBagOfChips(GameState *gameState, int numChipsInBag) {
    pthread_mutex_lock(&gameState->chipBagMutex);
    gameState->chipBag.numChipsInBag = numChipsInBag;
    pthread_mutex_unlock(&gameState->chipBagMutex);
}

// eatChips: Eats a number of chips out of the current bag, and opens a new one if needed.
//
// *gameState: Pointer to the GameState.
// *bag: Pointer to the current bag of chips.
// playerID: ID of the player eating the chips.
void eatChips(GameState *gameState, BagOfChips *bag, int playerID) {
    
    int chipsToEat = getRandomInt(gameState, 1, 5);

    pthread_mutex_lock(&gameState->chipBagMutex);
    
    while (chipsToEat > 0){
        if (bag->numChipsInBag == 0)
        {
            writeToLog("Player %d opens a new bag of chips.\n", playerID);
            bag->numChipsInBag = gameState->numChipsInBag;
        }

        if (chipsToEat <= bag->numChipsInBag)
        {
            writeToLog("Player %d eats %d chips from the bag.\n", playerID, chipsToEat);
            bag->numChipsInBag -= chipsToEat;
            chipsToEat = 0;
        }
        else
        {
            writeToLog("Player %d wants to eat %d chips, but only %d are left in the bag. They eat the remaining chips and open a new bag.\n", playerID, chipsToEat, bag->numChipsInBag);
            chipsToEat -= bag->numChipsInBag;
            bag->numChipsInBag = 0;
        }
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
    hand->handCards[hand->topIndex] = card;
    hand->topIndex++;
}

void clearHands(GameState *gameState) {
    for (int i = 0; i < gameState->numPlayers; i++) {
        gameState->playerHands[i].topIndex = 0;
    }
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
    pthread_mutex_lock(&gameState->rngMutex);

    int randomResult = min + rand_r(&gameState->seed) % (max - min + 1);

    pthread_mutex_unlock(&gameState->rngMutex);

    return randomResult;
}



// ********** PLAYER ACTIONS **********

// doDealerActions: Performs the actions for the dealer of a round.
//
// playerID: ID of the player who is the dealer.
// *gameState: Pointer to the game state.
void doDealerActions(int playerID, GameState *gameState) {

    // Clear player hands for next round
    clearHands(gameState);

    // Shuffle cards, choose random card as greasy card
    initDeck(&gameState->deck); 
    shuffleDeck(gameState);
    Card greasyCard = drawCard(gameState);
    setGreasyCard(gameState, greasyCard);
    writeToLog("Player %d (dealer) has chosen the greasy card: value %d, suit %d.\n", playerID, greasyCard.value, greasyCard.suit);

    // Deal single card to each non-dealer player
    for (int i = 0; i < gameState->numPlayers; i++) {
        if (i != playerID) {
            Card dealtCard = drawCard(gameState);
            addCardToHand(gameState, i, dealtCard);
            writeToLog("Player %d has been dealt card: value %d, suit %d.\n", i, dealtCard.value, dealtCard.suit);
        }
    }

    // Open first bag of potato chips
    writeToLog("Player %d (dealer) is opening the first bag of chips for this round with %d chips.\n", playerID, gameState->numChipsInBag);
    initBagOfChips(gameState, gameState->numChipsInBag);
}

// doNonDealerActions: Performs the actions for the non-dealers of a round.
//
// playerID: ID of the player who is a non-dealer.
// *gameState: Pointer to the game state.
void doNonDealerActions(int playerID, GameState *gameState) {

    // Draw card, add to hand
    Card drawnCard = drawCard(gameState);
    addCardToHand(gameState, playerID, drawnCard);
    writeToLog("Player %d has drawn card: value %d, suit %d.\n", playerID, drawnCard.value, drawnCard.suit);

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
        setRoundWinner(gameState);
        writeToLog("Player %d has a card matching the greasy card and wins the round! Their hand:\n", playerID);
        for (int i = 0; i < hand->topIndex; i++) {
            writeToLog("Card %d: value %d, suit %d\n", i, hand->handCards[i].value, hand->handCards[i].suit);
        }
    }
    // If they don't have a card matching the greasy card, they discard a random card by
    // placing it at the end of the deck.
    else {
        int cardToDiscardIndex = getRandomInt(gameState, 0, hand->topIndex - 1);
        Card cardToDiscard = hand->handCards[cardToDiscardIndex];

        for (int i = cardToDiscardIndex; i < hand->topIndex - 1; i++) {
            hand->handCards[i] = hand->handCards[i + 1];
        }
        
        hand->topIndex--;
        returnCardToEndOfDeck(gameState, cardToDiscard);
        printf("Player %d discarded card with value %d and suite %d", playerID, cardToDiscard.value, cardToDiscard.suit);
    }

    // Eat a random number of chips
    eatChips(gameState, &gameState->chipBag, playerID);
}

// playerThread: Logic for each player thread
//
// *arg: Pointer to the player's ID
void *playerThread(void *newThreadArgs) {

    threadArgs *newThreadData = (threadArgs *)newThreadArgs;
    int playerID = newThreadData->playerID;
    int numRounds = newThreadData->numRounds;

    writeToLog("Player %d is starting their thread.\n", playerID);

    // Wait for all players to be created before starting the game.
    pthread_barrier_wait(&playerCreationBarrier);

    for (int round = 0; round < numRounds; round++){
        
        // DEALER ACTIONS: Once at start of round
        if (playerID == round){
            clearRoundWinner(newThreadData->gameState);
            writeToLog("Player %d is the dealer for round %d.\n", playerID, round);
            doDealerActions(playerID, newThreadData->gameState);
        }

        // Everyone waits for the dealer to finish their setup for the round.
        pthread_barrier_wait(&dealerEndBarrier);

        // NON DEALER ACTIONS: Repeat until winner is found for round.
        // NOTE: Ensuring that players play in numerical order, starting with ID after
        // dealer's value, and looping back around.
        while (!hasRoundWinner(newThreadData->gameState)) {
            for (int offset = 1; offset < newThreadData->gameState->numPlayers; offset++) {

                int currentPlayer = (round + offset) % newThreadData->gameState->numPlayers;

                // Only the current player takes a turn
                if (!hasRoundWinner(newThreadData->gameState) && playerID == currentPlayer) {
                    writeToLog("Player %d's turn in round %d.\n", playerID, round);
                    doNonDealerActions(playerID, newThreadData->gameState);
                }

                // Everyone waits so turns happen one at a time
                pthread_barrier_wait(&roundEndBarrier);
            }

            // if (playerID != round){
            //     writeToLog("Player %d's turn in round %d.\n", playerID, round);
            //     doNonDealerActions(playerID, newThreadData->gameState);
            // }

            // pthread_barrier_wait(&roundEndBarrier);
        }

        // END OF ROUND: Everyone waits for everyone to finish round,
        // and dealer signals end of round.
        pthread_barrier_wait(&roundEndBarrier);
        if (playerID == round){
            writeToLog("Player %d (dealer) is signaling the end of round %d.\n", playerID, round);
        }
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
    if (numPlayers <= 0 || numChipsInBag <= 0) {
        printf("Invalid input. Please enter valid number of players and number of chips in bag.\n");
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
    threadArgs threadArgs[numPlayers]; // Struct to hold arguments for each thread

    // Initialize thread barriers for:
       // Dealer turn end
       // Round end
       // Player creation/joining game
    pthread_barrier_init(&roundEndBarrier, NULL, numPlayers);
    pthread_barrier_init(&dealerEndBarrier, NULL, numPlayers);
    pthread_barrier_init(&playerCreationBarrier, NULL, numPlayers);

    // Create a thread for each player, and let their logic play out
    for (int t = 0; t < numPlayers; t++){

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

    printf("All player threads have finished. Game End!\n");

    // Destroy barriers and mutexes, close log file
    pthread_barrier_destroy(&roundEndBarrier);
    pthread_barrier_destroy(&dealerEndBarrier);
    pthread_barrier_destroy(&playerCreationBarrier);

    pthread_mutex_destroy(&gameState.deckMutex);
    pthread_mutex_destroy(&gameState.rngMutex);
    pthread_mutex_destroy(&gameState.chipBagMutex);
    pthread_mutex_destroy(&logFileMutex);

    fclose(logFile);

    return 0;
}

