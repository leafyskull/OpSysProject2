# OpSysProject2

# To compile and run:  
Enter "gcc main.c -o main -pthread" into your console to compile  
Enter "./main" into your console to run the .exe  
You will be prompted to input the seed, number of players, and the number of chips per bag.  
Output will be printed to console, and written to "gameLog.txt".  


# REPORT ON DESIGN AND IMPLEMENTATION:  

This program uses POSIX threads to simulate the "Greasy Cards" game.  

The program starts by getting input from the user for the seed, number of players, and number of chips per bag.  
A "GameState" is initialized, which will used to handle shared information about the game.  
A thread is created for each player in the game, and is given an ID, the number of rounds to play, and a reference to the GameState.  
Each player then runs their independent thread logic (playerThread).  
In the player thread, each player counts the rounds indiviually.  
First, the dealer performs their actions (shuffling cards, dealining initial cards, drawing greasy card). The non-dealer players wait for the dealer to finish their actions before proceeding.  
While a winner of the round has not been found, the players take turns doing the following:  
Drawing a card, checking if their hand contains a match to the greasy card, either discarding a card or declaring themself a winner, and eating chips.  
If a player finishes the current bag of chips, they will open a new one.  
Each player waits for the preceeding player to finish their turn before playing. The first player to play is the one with an ID that is one greater than the dealer's ID.  
Once a round's winner has been decided, every player thread waits for all other player threads to reach the end of the round logic.  
Once all players have reached this, the dealer signals the end of the round. After this happens, the next round may begin. All hands are cleared, and the process repeats with a new dealer.  

There are several mutexes that exist, to protect different parts of shared data:  
deckMutex: Ensures only one player can access the deck of cards at a time.  
rngMUTEX: Ensures only one player can access random number generation at a time.  
chipBagMutex: Ensures only one player can access the bag of chips at a time.  
winnerMutex: Ensures only one player can check for/declare victory at a time.  
logFileMutex: Ensures only one player can write to the log file at a time.  

There also exist "barriers" to control program flow, which mark things such as:  
Player creation  
Dealer finishing their setup  
Player finishing their turn  
Players reaching the end of the round  
Dealer announcing the end of the round  

Structs are defined for the following data types:  
Card (Card)  
Hand of cards (Hand)  
Deck of cards (Deck)  
Bag of chips (BagOfChips)  
GameState (GameState)  
Thread arguments (ThreadArgs)  



# RESULTS  
Below are the outputs of 5 independent runs, as requested in the assignment description.  
Each run uses 6 players and 30 chips in every bag.  



# Run 1: (Seed = 1)
```
Game started with seed 1, 6 players, and 30 chips in bag.
Player 0 is starting their thread.
Player 2 is starting their thread.
Player 3 is starting their thread.
Player 5 is starting their thread.
Player 4 is starting their thread.
Player 1 is starting their thread.

*** New round ***
Player 0 (dealer) shuffling deck...
Player 0 (dealer) has chosen the greasy card: Three of Spades.
Player 1 has been dealt card: Seven of Hearts.
Player 2 has been dealt card: Jack of Clubs.
Player 3 has been dealt card: Five of Spades.
Player 4 has been dealt card: Nine of Diamonds.
Player 5 has been dealt card: Six of Clubs.
Player 0 (dealer) is opening the first bag of chips for this round with 30 chips.
Player 1's turn in round 0.
Player 1 has drawn card: Five of Clubs.
Player 1 discarded card: Five of Clubs
Player 1 eats 3 chips from the bag.
Player 2's turn in round 0.
Player 2 has drawn card: King of Diamonds.
Player 2 discarded card: King of Diamonds
Player 2 eats 3 chips from the bag.
Player 3's turn in round 0.
Player 3 has drawn card: Four of Spades.
Player 3 discarded card: Four of Spades
Player 3 eats 3 chips from the bag.
Player 4's turn in round 0.
Player 4 has drawn card: Jack of Spades.
Player 4 discarded card: Jack of Spades
Player 4 eats 3 chips from the bag.
Player 5's turn in round 0.
Player 5 has drawn card: Seven of Spades.
Player 5 discarded card: Seven of Spades
Player 5 eats 3 chips from the bag.
Player 1's turn in round 0.
Player 1 has drawn card: Two of Hearts.
Player 1 discarded card: Seven of Hearts
Player 1 eats 1 chips from the bag.
Player 2's turn in round 0.
Player 2 has drawn card: Ace of Diamonds.
Player 2 discarded card: Jack of Clubs
Player 2 eats 1 chips from the bag.
Player 3's turn in round 0.
Player 3 has drawn card: Six of Hearts.
Player 3 discarded card: Five of Spades
Player 3 eats 1 chips from the bag.
Player 4's turn in round 0.
Player 4 has drawn card: Seven of Diamonds.
Player 4 discarded card: Nine of Diamonds
Player 4 eats 1 chips from the bag.
Player 5's turn in round 0.
Player 5 has drawn card: Eight of Hearts.
Player 5 discarded card: Six of Clubs
Player 5 eats 1 chips from the bag.
Player 1's turn in round 0.
Player 1 has drawn card: Jack of Diamonds.
Player 1 discarded card: Jack of Diamonds
Player 1 eats 5 chips from the bag.
Player 2's turn in round 0.
Player 2 has drawn card: Queen of Diamonds.
Player 2 discarded card: Queen of Diamonds
Player 2 eats 5 chips from the bag.
Player 3's turn in round 0.
Player 3 has drawn card: King of Spades.
Player 3 discarded card: King of Spades
Player 3 opens a new bag of chips.
Player 3 eats 5 chips from the bag.
Player 4's turn in round 0.
Player 4 has drawn card: Four of Hearts.
Player 4 discarded card: Four of Hearts
Player 4 eats 5 chips from the bag.
Player 5's turn in round 0.
Player 5 has drawn card: Two of Clubs.
Player 5 discarded card: Two of Clubs
Player 5 eats 5 chips from the bag.
Player 1's turn in round 0.
Player 1 has drawn card: Five of Diamonds.
Player 1 discarded card: Two of Hearts
Player 1 eats 4 chips from the bag.
Player 2's turn in round 0.
Player 2 has drawn card: Five of Hearts.
Player 2 discarded card: Ace of Diamonds
Player 2 eats 4 chips from the bag.
Player 3's turn in round 0.
Player 3 has drawn card: King of Hearts.
Player 3 discarded card: Six of Hearts
Player 3 eats 4 chips from the bag.
Player 4's turn in round 0.
Player 4 has drawn card: Three of Hearts.
Player 4 has a card matching the greasy card and wins the round! Their hand:
Card 0: Seven of Diamonds
Card 1: Three of Hearts
Player 4 wants to eat 4 chips, but only 3 are left in the bag. They eat the remaining chips and will open a new bag.
Player 4 opens a new bag of chips.
Player 4 eats 1 chips from the bag.
Player 0 (dealer) is signaling the end of round 0.

*** New round ***
Player 1 (dealer) shuffling deck...
Player 1 (dealer) has chosen the greasy card: Ace of Clubs.
Player 0 has been dealt card: Jack of Clubs.
Player 2 has been dealt card: Eight of Hearts.
Player 3 has been dealt card: Ten of Diamonds.
Player 4 has been dealt card: Six of Hearts.
Player 5 has been dealt card: Seven of Hearts.
Player 1 (dealer) is opening the first bag of chips for this round with 30 chips.
Player 2's turn in round 1.
Player 2 has drawn card: Six of Clubs.
Player 2 discarded card: Eight of Hearts
Player 2 eats 5 chips from the bag.
Player 3's turn in round 1.
Player 3 has drawn card: Ten of Spades.
Player 3 discarded card: Ten of Diamonds
Player 3 eats 5 chips from the bag.
Player 4's turn in round 1.
Player 4 has drawn card: Queen of Hearts.
Player 4 discarded card: Six of Hearts
Player 4 eats 3 chips from the bag.
Player 5's turn in round 1.
Player 5 has drawn card: Two of Spades.
Player 5 discarded card: Seven of Hearts
Player 5 eats 4 chips from the bag.
Player 0's turn in round 1.
Player 0 has drawn card: Jack of Spades.
Player 0 discarded card: Jack of Clubs
Player 0 eats 3 chips from the bag.
Player 2's turn in round 1.
Player 2 has drawn card: Ten of Clubs.
Player 2 discarded card: Ten of Clubs
Player 2 eats 1 chips from the bag.
Player 3's turn in round 1.
Player 3 has drawn card: Five of Spades.
Player 3 discarded card: Five of Spades
Player 3 eats 1 chips from the bag.
Player 4's turn in round 1.
Player 4 has drawn card: Five of Hearts.
Player 4 discarded card: Queen of Hearts
Player 4 eats 1 chips from the bag.
Player 5's turn in round 1.
Player 5 has drawn card: Ten of Hearts.
Player 5 discarded card: Two of Spades
Player 5 eats 5 chips from the bag.
Player 0's turn in round 1.
Player 0 has drawn card: Eight of Diamonds.
Player 0 discarded card: Eight of Diamonds
Player 0 wants to eat 5 chips, but only 2 are left in the bag. They eat the remaining chips and will open a new bag.
Player 0 opens a new bag of chips.
Player 0 eats 3 chips from the bag.
Player 2's turn in round 1.
Player 2 has drawn card: Three of Diamonds.
Player 2 discarded card: Three of Diamonds
Player 2 eats 3 chips from the bag.
Player 3's turn in round 1.
Player 3 has drawn card: Six of Spades.
Player 3 discarded card: Six of Spades
Player 3 eats 3 chips from the bag.
Player 4's turn in round 1.
Player 4 has drawn card: Ace of Diamonds.
Player 4 has a card matching the greasy card and wins the round! Their hand:
Card 0: Five of Hearts
Card 1: Ace of Diamonds
Player 4 eats 1 chips from the bag.
Player 1 (dealer) is signaling the end of round 1.

*** New round ***
Player 2 (dealer) shuffling deck...
Player 2 (dealer) has chosen the greasy card: Four of Clubs.
Player 0 has been dealt card: Eight of Clubs.
Player 1 has been dealt card: Nine of Spades.
Player 3 has been dealt card: King of Clubs.
Player 4 has been dealt card: Six of Clubs.
Player 5 has been dealt card: Jack of Clubs.
Player 2 (dealer) is opening the first bag of chips for this round with 30 chips.
Player 3's turn in round 2.
Player 3 has drawn card: Ace of Diamonds.
Player 3 discarded card: Ace of Diamonds
Player 3 eats 2 chips from the bag.
Player 4's turn in round 2.
Player 4 has drawn card: Five of Hearts.
Player 4 discarded card: Five of Hearts
Player 4 eats 3 chips from the bag.
Player 5's turn in round 2.
Player 5 has drawn card: Four of Hearts.
Player 5 has a card matching the greasy card and wins the round! Their hand:
Card 0: Jack of Clubs
Card 1: Four of Hearts
Player 5 eats 1 chips from the bag.
Player 2 (dealer) is signaling the end of round 2.

*** New round ***
Player 3 (dealer) shuffling deck...
Player 3 (dealer) has chosen the greasy card: Six of Clubs.
Player 0 has been dealt card: Ten of Hearts.
Player 1 has been dealt card: Four of Clubs.
Player 2 has been dealt card: Jack of Spades.
Player 4 has been dealt card: Five of Spades.
Player 5 has been dealt card: Ace of Spades.
Player 3 (dealer) is opening the first bag of chips for this round with 30 chips.
Player 4's turn in round 3.
Player 4 has drawn card: Six of Spades.
Player 4 has a card matching the greasy card and wins the round! Their hand:
Card 0: Five of Spades
Card 1: Six of Spades
Player 4 eats 2 chips from the bag.
Player 3 (dealer) is signaling the end of round 3.

*** New round ***
Player 4 (dealer) shuffling deck...
Player 4 (dealer) has chosen the greasy card: Jack of Diamonds.
Player 0 has been dealt card: King of Clubs.
Player 1 has been dealt card: Six of Spades.
Player 2 has been dealt card: Three of Clubs.
Player 3 has been dealt card: Four of Diamonds.
Player 5 has been dealt card: Ace of Diamonds.
Player 4 (dealer) is opening the first bag of chips for this round with 30 chips.
Player 5's turn in round 4.
Player 5 has drawn card: Four of Hearts.
Player 5 discarded card: Four of Hearts
Player 5 eats 2 chips from the bag.
Player 0's turn in round 4.
Player 0 has drawn card: Ace of Spades.
Player 0 discarded card: Ace of Spades
Player 0 eats 2 chips from the bag.
Player 1's turn in round 4.
Player 1 has drawn card: Four of Clubs.
Player 1 discarded card: Six of Spades
Player 1 eats 1 chips from the bag.
Player 2's turn in round 4.
Player 2 has drawn card: Five of Spades.
Player 2 discarded card: Three of Clubs
Player 2 eats 3 chips from the bag.
Player 3's turn in round 4.
Player 3 has drawn card: Three of Hearts.
Player 3 discarded card: Four of Diamonds
Player 3 eats 4 chips from the bag.
Player 5's turn in round 4.
Player 5 has drawn card: Ten of Spades.
Player 5 discarded card: Ten of Spades
Player 5 eats 2 chips from the bag.
Player 0's turn in round 4.
Player 0 has drawn card: Queen of Hearts.
Player 0 discarded card: Queen of Hearts
Player 0 eats 4 chips from the bag.
Player 1's turn in round 4.
Player 1 has drawn card: Jack of Clubs.
Player 1 has a card matching the greasy card and wins the round! Their hand:
Card 0: Four of Clubs
Card 1: Jack of Clubs
Player 1 eats 1 chips from the bag.
Player 4 (dealer) is signaling the end of round 4.

*** New round ***
Player 5 (dealer) shuffling deck...
Player 5 (dealer) has chosen the greasy card: Jack of Diamonds.
Player 0 has been dealt card: King of Clubs.
Player 1 has been dealt card: Six of Spades.
Player 2 has been dealt card: Three of Clubs.
Player 3 has been dealt card: Four of Diamonds.
Player 4 has been dealt card: Ace of Diamonds.
Player 5 (dealer) is opening the first bag of chips for this round with 30 chips.
Player 0's turn in round 5.
Player 0 has drawn card: Four of Hearts.
Player 0 discarded card: King of Clubs
Player 0 eats 1 chips from the bag.
Player 1's turn in round 5.
Player 1 has drawn card: Ace of Spades.
Player 1 discarded card: Six of Spades
Player 1 eats 4 chips from the bag.
Player 2's turn in round 5.
Player 2 has drawn card: Four of Clubs.
Player 2 discarded card: Three of Clubs
Player 2 eats 4 chips from the bag.
Player 3's turn in round 5.
Player 3 has drawn card: Five of Spades.
Player 3 discarded card: Four of Diamonds
Player 3 eats 1 chips from the bag.
Player 4's turn in round 5.
Player 4 has drawn card: Three of Hearts.
Player 4 discarded card: Ace of Diamonds
Player 4 eats 5 chips from the bag.
Player 0's turn in round 5.
Player 0 has drawn card: Ten of Spades.
Player 0 discarded card: Four of Hearts
Player 0 eats 3 chips from the bag.
Player 1's turn in round 5.
Player 1 has drawn card: Queen of Hearts.
Player 1 discarded card: Ace of Spades
Player 1 eats 1 chips from the bag.
Player 2's turn in round 5.
Player 2 has drawn card: Jack of Clubs.
Player 2 has a card matching the greasy card and wins the round! Their hand:
Card 0: Four of Clubs
Card 1: Jack of Clubs
Player 2 eats 2 chips from the bag.
Player 5 (dealer) is signaling the end of round 5.
All player threads have finished. Game End!
```


# Run 2 (Seed = 2)
```
Game started with seed 2, 6 players, and 30 chips in bag.
Player 0 is starting their thread.
Player 4 is starting their thread.
Player 2 is starting their thread.
Player 1 is starting their thread.
Player 5 is starting their thread.
Player 3 is starting their thread.

*** New round ***
Player 0 (dealer) shuffling deck...
Player 0 (dealer) has chosen the greasy card: Three of Spades.
Player 1 has been dealt card: Seven of Hearts.
Player 2 has been dealt card: Jack of Clubs.
Player 3 has been dealt card: Five of Spades.
Player 4 has been dealt card: Nine of Diamonds.
Player 5 has been dealt card: Six of Clubs.
Player 0 (dealer) is opening the first bag of chips for this round with 30 chips.
Player 1's turn in round 0.
Player 1 has drawn card: Five of Clubs.
Player 1 discarded card: Five of Clubs
Player 1 eats 3 chips from the bag.
Player 2's turn in round 0.
Player 2 has drawn card: King of Diamonds.
Player 2 discarded card: King of Diamonds
Player 2 eats 3 chips from the bag.
Player 3's turn in round 0.
Player 3 has drawn card: Four of Spades.
Player 3 discarded card: Four of Spades
Player 3 eats 3 chips from the bag.
Player 4's turn in round 0.
Player 4 has drawn card: Jack of Spades.
Player 4 discarded card: Jack of Spades
Player 4 eats 3 chips from the bag.
Player 5's turn in round 0.
Player 5 has drawn card: Seven of Spades.
Player 5 discarded card: Seven of Spades
Player 5 eats 3 chips from the bag.
Player 1's turn in round 0.
Player 1 has drawn card: Two of Hearts.
Player 1 discarded card: Seven of Hearts
Player 1 eats 1 chips from the bag.
Player 2's turn in round 0.
Player 2 has drawn card: Ace of Diamonds.
Player 2 discarded card: Jack of Clubs
Player 2 eats 1 chips from the bag.
Player 3's turn in round 0.
Player 3 has drawn card: Six of Hearts.
Player 3 discarded card: Five of Spades
Player 3 eats 1 chips from the bag.
Player 4's turn in round 0.
Player 4 has drawn card: Seven of Diamonds.
Player 4 discarded card: Nine of Diamonds
Player 4 eats 1 chips from the bag.
Player 5's turn in round 0.
Player 5 has drawn card: Eight of Hearts.
Player 5 discarded card: Six of Clubs
Player 5 eats 1 chips from the bag.
Player 1's turn in round 0.
Player 1 has drawn card: Jack of Diamonds.
Player 1 discarded card: Jack of Diamonds
Player 1 eats 5 chips from the bag.
Player 2's turn in round 0.
Player 2 has drawn card: Queen of Diamonds.
Player 2 discarded card: Queen of Diamonds
Player 2 eats 5 chips from the bag.
Player 3's turn in round 0.
Player 3 has drawn card: King of Spades.
Player 3 discarded card: King of Spades
Player 3 opens a new bag of chips.
Player 3 eats 5 chips from the bag.
Player 4's turn in round 0.
Player 4 has drawn card: Four of Hearts.
Player 4 discarded card: Four of Hearts
Player 4 eats 5 chips from the bag.
Player 5's turn in round 0.
Player 5 has drawn card: Two of Clubs.
Player 5 discarded card: Two of Clubs
Player 5 eats 5 chips from the bag.
Player 1's turn in round 0.
Player 1 has drawn card: Five of Diamonds.
Player 1 discarded card: Two of Hearts
Player 1 eats 4 chips from the bag.
Player 2's turn in round 0.
Player 2 has drawn card: Five of Hearts.
Player 2 discarded card: Ace of Diamonds
Player 2 eats 4 chips from the bag.
Player 3's turn in round 0.
Player 3 has drawn card: King of Hearts.
Player 3 discarded card: Six of Hearts
Player 3 eats 4 chips from the bag.
Player 4's turn in round 0.
Player 4 has drawn card: Three of Hearts.
Player 4 has a card matching the greasy card and wins the round! Their hand:
Card 0: Seven of Diamonds
Card 1: Three of Hearts
Player 4 wants to eat 4 chips, but only 3 are left in the bag. They eat the remaining chips and will open a new bag.
Player 4 opens a new bag of chips.
Player 4 eats 1 chips from the bag.
Player 0 (dealer) is signaling the end of round 0.

*** New round ***
Player 1 (dealer) shuffling deck...
Player 1 (dealer) has chosen the greasy card: Ace of Clubs.
Player 0 has been dealt card: Jack of Clubs.
Player 2 has been dealt card: Eight of Hearts.
Player 3 has been dealt card: Ten of Diamonds.
Player 4 has been dealt card: Six of Hearts.
Player 5 has been dealt card: Seven of Hearts.
Player 1 (dealer) is opening the first bag of chips for this round with 30 chips.
Player 2's turn in round 1.
Player 2 has drawn card: Six of Clubs.
Player 2 discarded card: Eight of Hearts
Player 2 eats 5 chips from the bag.
Player 3's turn in round 1.
Player 3 has drawn card: Ten of Spades.
Player 3 discarded card: Ten of Diamonds
Player 3 eats 5 chips from the bag.
Player 4's turn in round 1.
Player 4 has drawn card: Queen of Hearts.
Player 4 discarded card: Six of Hearts
Player 4 eats 3 chips from the bag.
Player 5's turn in round 1.
Player 5 has drawn card: Two of Spades.
Player 5 discarded card: Seven of Hearts
Player 5 eats 4 chips from the bag.
Player 0's turn in round 1.
Player 0 has drawn card: Jack of Spades.
Player 0 discarded card: Jack of Clubs
Player 0 eats 3 chips from the bag.
Player 2's turn in round 1.
Player 2 has drawn card: Ten of Clubs.
Player 2 discarded card: Ten of Clubs
Player 2 eats 1 chips from the bag.
Player 3's turn in round 1.
Player 3 has drawn card: Five of Spades.
Player 3 discarded card: Five of Spades
Player 3 eats 1 chips from the bag.
Player 4's turn in round 1.
Player 4 has drawn card: Five of Hearts.
Player 4 discarded card: Queen of Hearts
Player 4 eats 1 chips from the bag.
Player 5's turn in round 1.
Player 5 has drawn card: Ten of Hearts.
Player 5 discarded card: Two of Spades
Player 5 eats 5 chips from the bag.
Player 0's turn in round 1.
Player 0 has drawn card: Eight of Diamonds.
Player 0 discarded card: Eight of Diamonds
Player 0 wants to eat 5 chips, but only 2 are left in the bag. They eat the remaining chips and will open a new bag.
Player 0 opens a new bag of chips.
Player 0 eats 3 chips from the bag.
Player 2's turn in round 1.
Player 2 has drawn card: Three of Diamonds.
Player 2 discarded card: Three of Diamonds
Player 2 eats 3 chips from the bag.
Player 3's turn in round 1.
Player 3 has drawn card: Six of Spades.
Player 3 discarded card: Six of Spades
Player 3 eats 3 chips from the bag.
Player 4's turn in round 1.
Player 4 has drawn card: Ace of Diamonds.
Player 4 has a card matching the greasy card and wins the round! Their hand:
Card 0: Five of Hearts
Card 1: Ace of Diamonds
Player 4 eats 1 chips from the bag.
Player 1 (dealer) is signaling the end of round 1.

*** New round ***
Player 2 (dealer) shuffling deck...
Player 2 (dealer) has chosen the greasy card: Four of Clubs.
Player 0 has been dealt card: Eight of Clubs.
Player 1 has been dealt card: Nine of Spades.
Player 3 has been dealt card: King of Clubs.
Player 4 has been dealt card: Six of Clubs.
Player 5 has been dealt card: Jack of Clubs.
Player 2 (dealer) is opening the first bag of chips for this round with 30 chips.
Player 3's turn in round 2.
Player 3 has drawn card: Ace of Diamonds.
Player 3 discarded card: Ace of Diamonds
Player 3 eats 2 chips from the bag.
Player 4's turn in round 2.
Player 4 has drawn card: Five of Hearts.
Player 4 discarded card: Five of Hearts
Player 4 eats 3 chips from the bag.
Player 5's turn in round 2.
Player 5 has drawn card: Four of Hearts.
Player 5 has a card matching the greasy card and wins the round! Their hand:
Card 0: Jack of Clubs
Card 1: Four of Hearts
Player 5 eats 1 chips from the bag.
Player 2 (dealer) is signaling the end of round 2.

*** New round ***
Player 3 (dealer) shuffling deck...
Player 3 (dealer) has chosen the greasy card: Six of Clubs.
Player 0 has been dealt card: Ten of Hearts.
Player 1 has been dealt card: Four of Clubs.
Player 2 has been dealt card: Jack of Spades.
Player 4 has been dealt card: Five of Spades.
Player 5 has been dealt card: Ace of Spades.
Player 3 (dealer) is opening the first bag of chips for this round with 30 chips.
Player 4's turn in round 3.
Player 4 has drawn card: Six of Spades.
Player 4 has a card matching the greasy card and wins the round! Their hand:
Card 0: Five of Spades
Card 1: Six of Spades
Player 4 eats 2 chips from the bag.
Player 3 (dealer) is signaling the end of round 3.

*** New round ***
Player 4 (dealer) shuffling deck...
Player 4 (dealer) has chosen the greasy card: Jack of Diamonds.
Player 0 has been dealt card: King of Clubs.
Player 1 has been dealt card: Six of Spades.
Player 2 has been dealt card: Three of Clubs.
Player 3 has been dealt card: Four of Diamonds.
Player 5 has been dealt card: Ace of Diamonds.
Player 4 (dealer) is opening the first bag of chips for this round with 30 chips.
Player 5's turn in round 4.
Player 5 has drawn card: Four of Hearts.
Player 5 discarded card: Four of Hearts
Player 5 eats 2 chips from the bag.
Player 0's turn in round 4.
Player 0 has drawn card: Ace of Spades.
Player 0 discarded card: Ace of Spades
Player 0 eats 2 chips from the bag.
Player 1's turn in round 4.
Player 1 has drawn card: Four of Clubs.
Player 1 discarded card: Six of Spades
Player 1 eats 1 chips from the bag.
Player 2's turn in round 4.
Player 2 has drawn card: Five of Spades.
Player 2 discarded card: Three of Clubs
Player 2 eats 3 chips from the bag.
Player 3's turn in round 4.
Player 3 has drawn card: Three of Hearts.
Player 3 discarded card: Four of Diamonds
Player 3 eats 4 chips from the bag.
Player 5's turn in round 4.
Player 5 has drawn card: Ten of Spades.
Player 5 discarded card: Ten of Spades
Player 5 eats 2 chips from the bag.
Player 0's turn in round 4.
Player 0 has drawn card: Queen of Hearts.
Player 0 discarded card: Queen of Hearts
Player 0 eats 4 chips from the bag.
Player 1's turn in round 4.
Player 1 has drawn card: Jack of Clubs.
Player 1 has a card matching the greasy card and wins the round! Their hand:
Card 0: Four of Clubs
Card 1: Jack of Clubs
Player 1 eats 1 chips from the bag.
Player 4 (dealer) is signaling the end of round 4.

*** New round ***
Player 5 (dealer) shuffling deck...
Player 5 (dealer) has chosen the greasy card: Jack of Diamonds.
Player 0 has been dealt card: King of Clubs.
Player 1 has been dealt card: Six of Spades.
Player 2 has been dealt card: Three of Clubs.
Player 3 has been dealt card: Four of Diamonds.
Player 4 has been dealt card: Ace of Diamonds.
Player 5 (dealer) is opening the first bag of chips for this round with 30 chips.
Player 0's turn in round 5.
Player 0 has drawn card: Four of Hearts.
Player 0 discarded card: King of Clubs
Player 0 eats 1 chips from the bag.
Player 1's turn in round 5.
Player 1 has drawn card: Ace of Spades.
Player 1 discarded card: Six of Spades
Player 1 eats 4 chips from the bag.
Player 2's turn in round 5.
Player 2 has drawn card: Four of Clubs.
Player 2 discarded card: Three of Clubs
Player 2 eats 4 chips from the bag.
Player 3's turn in round 5.
Player 3 has drawn card: Five of Spades.
Player 3 discarded card: Four of Diamonds
Player 3 eats 1 chips from the bag.
Player 4's turn in round 5.
Player 4 has drawn card: Three of Hearts.
Player 4 discarded card: Ace of Diamonds
Player 4 eats 5 chips from the bag.
Player 0's turn in round 5.
Player 0 has drawn card: Ten of Spades.
Player 0 discarded card: Four of Hearts
Player 0 eats 3 chips from the bag.
Player 1's turn in round 5.
Player 1 has drawn card: Queen of Hearts.
Player 1 discarded card: Ace of Spades
Player 1 eats 1 chips from the bag.
Player 2's turn in round 5.
Player 2 has drawn card: Jack of Clubs.
Player 2 has a card matching the greasy card and wins the round! Their hand:
Card 0: Four of Clubs
Card 1: Jack of Clubs
Player 2 eats 2 chips from the bag.
Player 5 (dealer) is signaling the end of round 5.
All player threads have finished. Game End!
```


# Run 3: (Seed = 3)
```
Game started with seed 3, 6 players, and 30 chips in bag.
Player 0 is starting their thread.
Player 4 is starting their thread.
Player 5 is starting their thread.
Player 1 is starting their thread.
Player 2 is starting their thread.
Player 3 is starting their thread.

*** New round ***
Player 0 (dealer) shuffling deck...
Player 0 (dealer) has chosen the greasy card: Three of Spades.
Player 1 has been dealt card: Seven of Hearts.
Player 2 has been dealt card: Jack of Clubs.
Player 3 has been dealt card: Five of Spades.
Player 4 has been dealt card: Nine of Diamonds.
Player 5 has been dealt card: Six of Clubs.
Player 0 (dealer) is opening the first bag of chips for this round with 30 chips.
Player 1's turn in round 0.
Player 1 has drawn card: Five of Clubs.
Player 1 discarded card: Five of Clubs
Player 1 eats 3 chips from the bag.
Player 2's turn in round 0.
Player 2 has drawn card: King of Diamonds.
Player 2 discarded card: King of Diamonds
Player 2 eats 3 chips from the bag.
Player 3's turn in round 0.
Player 3 has drawn card: Four of Spades.
Player 3 discarded card: Four of Spades
Player 3 eats 3 chips from the bag.
Player 4's turn in round 0.
Player 4 has drawn card: Jack of Spades.
Player 4 discarded card: Jack of Spades
Player 4 eats 3 chips from the bag.
Player 5's turn in round 0.
Player 5 has drawn card: Seven of Spades.
Player 5 discarded card: Seven of Spades
Player 5 eats 3 chips from the bag.
Player 1's turn in round 0.
Player 1 has drawn card: Two of Hearts.
Player 1 discarded card: Seven of Hearts
Player 1 eats 1 chips from the bag.
Player 2's turn in round 0.
Player 2 has drawn card: Ace of Diamonds.
Player 2 discarded card: Jack of Clubs
Player 2 eats 1 chips from the bag.
Player 3's turn in round 0.
Player 3 has drawn card: Six of Hearts.
Player 3 discarded card: Five of Spades
Player 3 eats 1 chips from the bag.
Player 4's turn in round 0.
Player 4 has drawn card: Seven of Diamonds.
Player 4 discarded card: Nine of Diamonds
Player 4 eats 1 chips from the bag.
Player 5's turn in round 0.
Player 5 has drawn card: Eight of Hearts.
Player 5 discarded card: Six of Clubs
Player 5 eats 1 chips from the bag.
Player 1's turn in round 0.
Player 1 has drawn card: Jack of Diamonds.
Player 1 discarded card: Jack of Diamonds
Player 1 eats 5 chips from the bag.
Player 2's turn in round 0.
Player 2 has drawn card: Queen of Diamonds.
Player 2 discarded card: Queen of Diamonds
Player 2 eats 5 chips from the bag.
Player 3's turn in round 0.
Player 3 has drawn card: King of Spades.
Player 3 discarded card: King of Spades
Player 3 opens a new bag of chips.
Player 3 eats 5 chips from the bag.
Player 4's turn in round 0.
Player 4 has drawn card: Four of Hearts.
Player 4 discarded card: Four of Hearts
Player 4 eats 5 chips from the bag.
Player 5's turn in round 0.
Player 5 has drawn card: Two of Clubs.
Player 5 discarded card: Two of Clubs
Player 5 eats 5 chips from the bag.
Player 1's turn in round 0.
Player 1 has drawn card: Five of Diamonds.
Player 1 discarded card: Two of Hearts
Player 1 eats 4 chips from the bag.
Player 2's turn in round 0.
Player 2 has drawn card: Five of Hearts.
Player 2 discarded card: Ace of Diamonds
Player 2 eats 4 chips from the bag.
Player 3's turn in round 0.
Player 3 has drawn card: King of Hearts.
Player 3 discarded card: Six of Hearts
Player 3 eats 4 chips from the bag.
Player 4's turn in round 0.
Player 4 has drawn card: Three of Hearts.
Player 4 has a card matching the greasy card and wins the round! Their hand:
Card 0: Seven of Diamonds
Card 1: Three of Hearts
Player 4 wants to eat 4 chips, but only 3 are left in the bag. They eat the remaining chips and will open a new bag.
Player 4 opens a new bag of chips.
Player 4 eats 1 chips from the bag.
Player 0 (dealer) is signaling the end of round 0.

*** New round ***
Player 1 (dealer) shuffling deck...
Player 1 (dealer) has chosen the greasy card: Ace of Clubs.
Player 0 has been dealt card: Jack of Clubs.
Player 2 has been dealt card: Eight of Hearts.
Player 3 has been dealt card: Ten of Diamonds.
Player 4 has been dealt card: Six of Hearts.
Player 5 has been dealt card: Seven of Hearts.
Player 1 (dealer) is opening the first bag of chips for this round with 30 chips.
Player 2's turn in round 1.
Player 2 has drawn card: Six of Clubs.
Player 2 discarded card: Eight of Hearts
Player 2 eats 5 chips from the bag.
Player 3's turn in round 1.
Player 3 has drawn card: Ten of Spades.
Player 3 discarded card: Ten of Diamonds
Player 3 eats 5 chips from the bag.
Player 4's turn in round 1.
Player 4 has drawn card: Queen of Hearts.
Player 4 discarded card: Six of Hearts
Player 4 eats 3 chips from the bag.
Player 5's turn in round 1.
Player 5 has drawn card: Two of Spades.
Player 5 discarded card: Seven of Hearts
Player 5 eats 4 chips from the bag.
Player 0's turn in round 1.
Player 0 has drawn card: Jack of Spades.
Player 0 discarded card: Jack of Clubs
Player 0 eats 3 chips from the bag.
Player 2's turn in round 1.
Player 2 has drawn card: Ten of Clubs.
Player 2 discarded card: Ten of Clubs
Player 2 eats 1 chips from the bag.
Player 3's turn in round 1.
Player 3 has drawn card: Five of Spades.
Player 3 discarded card: Five of Spades
Player 3 eats 1 chips from the bag.
Player 4's turn in round 1.
Player 4 has drawn card: Five of Hearts.
Player 4 discarded card: Queen of Hearts
Player 4 eats 1 chips from the bag.
Player 5's turn in round 1.
Player 5 has drawn card: Ten of Hearts.
Player 5 discarded card: Two of Spades
Player 5 eats 5 chips from the bag.
Player 0's turn in round 1.
Player 0 has drawn card: Eight of Diamonds.
Player 0 discarded card: Eight of Diamonds
Player 0 wants to eat 5 chips, but only 2 are left in the bag. They eat the remaining chips and will open a new bag.
Player 0 opens a new bag of chips.
Player 0 eats 3 chips from the bag.
Player 2's turn in round 1.
Player 2 has drawn card: Three of Diamonds.
Player 2 discarded card: Three of Diamonds
Player 2 eats 3 chips from the bag.
Player 3's turn in round 1.
Player 3 has drawn card: Six of Spades.
Player 3 discarded card: Six of Spades
Player 3 eats 3 chips from the bag.
Player 4's turn in round 1.
Player 4 has drawn card: Ace of Diamonds.
Player 4 has a card matching the greasy card and wins the round! Their hand:
Card 0: Five of Hearts
Card 1: Ace of Diamonds
Player 4 eats 1 chips from the bag.
Player 1 (dealer) is signaling the end of round 1.

*** New round ***
Player 2 (dealer) shuffling deck...
Player 2 (dealer) has chosen the greasy card: Four of Clubs.
Player 0 has been dealt card: Eight of Clubs.
Player 1 has been dealt card: Nine of Spades.
Player 3 has been dealt card: King of Clubs.
Player 4 has been dealt card: Six of Clubs.
Player 5 has been dealt card: Jack of Clubs.
Player 2 (dealer) is opening the first bag of chips for this round with 30 chips.
Player 3's turn in round 2.
Player 3 has drawn card: Ace of Diamonds.
Player 3 discarded card: Ace of Diamonds
Player 3 eats 2 chips from the bag.
Player 4's turn in round 2.
Player 4 has drawn card: Five of Hearts.
Player 4 discarded card: Five of Hearts
Player 4 eats 3 chips from the bag.
Player 5's turn in round 2.
Player 5 has drawn card: Four of Hearts.
Player 5 has a card matching the greasy card and wins the round! Their hand:
Card 0: Jack of Clubs
Card 1: Four of Hearts
Player 5 eats 1 chips from the bag.
Player 2 (dealer) is signaling the end of round 2.

*** New round ***
Player 3 (dealer) shuffling deck...
Player 3 (dealer) has chosen the greasy card: Six of Clubs.
Player 0 has been dealt card: Ten of Hearts.
Player 1 has been dealt card: Four of Clubs.
Player 2 has been dealt card: Jack of Spades.
Player 4 has been dealt card: Five of Spades.
Player 5 has been dealt card: Ace of Spades.
Player 3 (dealer) is opening the first bag of chips for this round with 30 chips.
Player 4's turn in round 3.
Player 4 has drawn card: Six of Spades.
Player 4 has a card matching the greasy card and wins the round! Their hand:
Card 0: Five of Spades
Card 1: Six of Spades
Player 4 eats 2 chips from the bag.
Player 3 (dealer) is signaling the end of round 3.

*** New round ***
Player 4 (dealer) shuffling deck...
Player 4 (dealer) has chosen the greasy card: Jack of Diamonds.
Player 0 has been dealt card: King of Clubs.
Player 1 has been dealt card: Six of Spades.
Player 2 has been dealt card: Three of Clubs.
Player 3 has been dealt card: Four of Diamonds.
Player 5 has been dealt card: Ace of Diamonds.
Player 4 (dealer) is opening the first bag of chips for this round with 30 chips.
Player 5's turn in round 4.
Player 5 has drawn card: Four of Hearts.
Player 5 discarded card: Four of Hearts
Player 5 eats 2 chips from the bag.
Player 0's turn in round 4.
Player 0 has drawn card: Ace of Spades.
Player 0 discarded card: Ace of Spades
Player 0 eats 2 chips from the bag.
Player 1's turn in round 4.
Player 1 has drawn card: Four of Clubs.
Player 1 discarded card: Six of Spades
Player 1 eats 1 chips from the bag.
Player 2's turn in round 4.
Player 2 has drawn card: Five of Spades.
Player 2 discarded card: Three of Clubs
Player 2 eats 3 chips from the bag.
Player 3's turn in round 4.
Player 3 has drawn card: Three of Hearts.
Player 3 discarded card: Four of Diamonds
Player 3 eats 4 chips from the bag.
Player 5's turn in round 4.
Player 5 has drawn card: Ten of Spades.
Player 5 discarded card: Ten of Spades
Player 5 eats 2 chips from the bag.
Player 0's turn in round 4.
Player 0 has drawn card: Queen of Hearts.
Player 0 discarded card: Queen of Hearts
Player 0 eats 4 chips from the bag.
Player 1's turn in round 4.
Player 1 has drawn card: Jack of Clubs.
Player 1 has a card matching the greasy card and wins the round! Their hand:
Card 0: Four of Clubs
Card 1: Jack of Clubs
Player 1 eats 1 chips from the bag.
Player 4 (dealer) is signaling the end of round 4.

*** New round ***
Player 5 (dealer) shuffling deck...
Player 5 (dealer) has chosen the greasy card: Jack of Diamonds.
Player 0 has been dealt card: King of Clubs.
Player 1 has been dealt card: Six of Spades.
Player 2 has been dealt card: Three of Clubs.
Player 3 has been dealt card: Four of Diamonds.
Player 4 has been dealt card: Ace of Diamonds.
Player 5 (dealer) is opening the first bag of chips for this round with 30 chips.
Player 0's turn in round 5.
Player 0 has drawn card: Four of Hearts.
Player 0 discarded card: King of Clubs
Player 0 eats 1 chips from the bag.
Player 1's turn in round 5.
Player 1 has drawn card: Ace of Spades.
Player 1 discarded card: Six of Spades
Player 1 eats 4 chips from the bag.
Player 2's turn in round 5.
Player 2 has drawn card: Four of Clubs.
Player 2 discarded card: Three of Clubs
Player 2 eats 4 chips from the bag.
Player 3's turn in round 5.
Player 3 has drawn card: Five of Spades.
Player 3 discarded card: Four of Diamonds
Player 3 eats 1 chips from the bag.
Player 4's turn in round 5.
Player 4 has drawn card: Three of Hearts.
Player 4 discarded card: Ace of Diamonds
Player 4 eats 5 chips from the bag.
Player 0's turn in round 5.
Player 0 has drawn card: Ten of Spades.
Player 0 discarded card: Four of Hearts
Player 0 eats 3 chips from the bag.
Player 1's turn in round 5.
Player 1 has drawn card: Queen of Hearts.
Player 1 discarded card: Ace of Spades
Player 1 eats 1 chips from the bag.
Player 2's turn in round 5.
Player 2 has drawn card: Jack of Clubs.
Player 2 has a card matching the greasy card and wins the round! Their hand:
Card 0: Four of Clubs
Card 1: Jack of Clubs
Player 2 eats 2 chips from the bag.
Player 5 (dealer) is signaling the end of round 5.
All player threads have finished. Game End!
```


# Run 4: (Seed = 4)
```
Game started with seed 4, 6 players, and 30 chips in bag.
Player 0 is starting their thread.
Player 4 is starting their thread.
Player 2 is starting their thread.
Player 5 is starting their thread.
Player 1 is starting their thread.
Player 3 is starting their thread.

*** New round ***
Player 0 (dealer) shuffling deck...
Player 0 (dealer) has chosen the greasy card: Three of Spades.
Player 1 has been dealt card: Seven of Hearts.
Player 2 has been dealt card: Jack of Clubs.
Player 3 has been dealt card: Five of Spades.
Player 4 has been dealt card: Nine of Diamonds.
Player 5 has been dealt card: Six of Clubs.
Player 0 (dealer) is opening the first bag of chips for this round with 30 chips.
Player 1's turn in round 0.
Player 1 has drawn card: Five of Clubs.
Player 1 discarded card: Five of Clubs
Player 1 eats 3 chips from the bag.
Player 2's turn in round 0.
Player 2 has drawn card: King of Diamonds.
Player 2 discarded card: King of Diamonds
Player 2 eats 3 chips from the bag.
Player 3's turn in round 0.
Player 3 has drawn card: Four of Spades.
Player 3 discarded card: Four of Spades
Player 3 eats 3 chips from the bag.
Player 4's turn in round 0.
Player 4 has drawn card: Jack of Spades.
Player 4 discarded card: Jack of Spades
Player 4 eats 3 chips from the bag.
Player 5's turn in round 0.
Player 5 has drawn card: Seven of Spades.
Player 5 discarded card: Seven of Spades
Player 5 eats 3 chips from the bag.
Player 1's turn in round 0.
Player 1 has drawn card: Two of Hearts.
Player 1 discarded card: Seven of Hearts
Player 1 eats 1 chips from the bag.
Player 2's turn in round 0.
Player 2 has drawn card: Ace of Diamonds.
Player 2 discarded card: Jack of Clubs
Player 2 eats 1 chips from the bag.
Player 3's turn in round 0.
Player 3 has drawn card: Six of Hearts.
Player 3 discarded card: Five of Spades
Player 3 eats 1 chips from the bag.
Player 4's turn in round 0.
Player 4 has drawn card: Seven of Diamonds.
Player 4 discarded card: Nine of Diamonds
Player 4 eats 1 chips from the bag.
Player 5's turn in round 0.
Player 5 has drawn card: Eight of Hearts.
Player 5 discarded card: Six of Clubs
Player 5 eats 1 chips from the bag.
Player 1's turn in round 0.
Player 1 has drawn card: Jack of Diamonds.
Player 1 discarded card: Jack of Diamonds
Player 1 eats 5 chips from the bag.
Player 2's turn in round 0.
Player 2 has drawn card: Queen of Diamonds.
Player 2 discarded card: Queen of Diamonds
Player 2 eats 5 chips from the bag.
Player 3's turn in round 0.
Player 3 has drawn card: King of Spades.
Player 3 discarded card: King of Spades
Player 3 opens a new bag of chips.
Player 3 eats 5 chips from the bag.
Player 4's turn in round 0.
Player 4 has drawn card: Four of Hearts.
Player 4 discarded card: Four of Hearts
Player 4 eats 5 chips from the bag.
Player 5's turn in round 0.
Player 5 has drawn card: Two of Clubs.
Player 5 discarded card: Two of Clubs
Player 5 eats 5 chips from the bag.
Player 1's turn in round 0.
Player 1 has drawn card: Five of Diamonds.
Player 1 discarded card: Two of Hearts
Player 1 eats 4 chips from the bag.
Player 2's turn in round 0.
Player 2 has drawn card: Five of Hearts.
Player 2 discarded card: Ace of Diamonds
Player 2 eats 4 chips from the bag.
Player 3's turn in round 0.
Player 3 has drawn card: King of Hearts.
Player 3 discarded card: Six of Hearts
Player 3 eats 4 chips from the bag.
Player 4's turn in round 0.
Player 4 has drawn card: Three of Hearts.
Player 4 has a card matching the greasy card and wins the round! Their hand:
Card 0: Seven of Diamonds
Card 1: Three of Hearts
Player 4 wants to eat 4 chips, but only 3 are left in the bag. They eat the remaining chips and will open a new bag.
Player 4 opens a new bag of chips.
Player 4 eats 1 chips from the bag.
Player 0 (dealer) is signaling the end of round 0.

*** New round ***
Player 1 (dealer) shuffling deck...
Player 1 (dealer) has chosen the greasy card: Ace of Clubs.
Player 0 has been dealt card: Jack of Clubs.
Player 2 has been dealt card: Eight of Hearts.
Player 3 has been dealt card: Ten of Diamonds.
Player 4 has been dealt card: Six of Hearts.
Player 5 has been dealt card: Seven of Hearts.
Player 1 (dealer) is opening the first bag of chips for this round with 30 chips.
Player 2's turn in round 1.
Player 2 has drawn card: Six of Clubs.
Player 2 discarded card: Eight of Hearts
Player 2 eats 5 chips from the bag.
Player 3's turn in round 1.
Player 3 has drawn card: Ten of Spades.
Player 3 discarded card: Ten of Diamonds
Player 3 eats 5 chips from the bag.
Player 4's turn in round 1.
Player 4 has drawn card: Queen of Hearts.
Player 4 discarded card: Six of Hearts
Player 4 eats 3 chips from the bag.
Player 5's turn in round 1.
Player 5 has drawn card: Two of Spades.
Player 5 discarded card: Seven of Hearts
Player 5 eats 4 chips from the bag.
Player 0's turn in round 1.
Player 0 has drawn card: Jack of Spades.
Player 0 discarded card: Jack of Clubs
Player 0 eats 3 chips from the bag.
Player 2's turn in round 1.
Player 2 has drawn card: Ten of Clubs.
Player 2 discarded card: Ten of Clubs
Player 2 eats 1 chips from the bag.
Player 3's turn in round 1.
Player 3 has drawn card: Five of Spades.
Player 3 discarded card: Five of Spades
Player 3 eats 1 chips from the bag.
Player 4's turn in round 1.
Player 4 has drawn card: Five of Hearts.
Player 4 discarded card: Queen of Hearts
Player 4 eats 1 chips from the bag.
Player 5's turn in round 1.
Player 5 has drawn card: Ten of Hearts.
Player 5 discarded card: Two of Spades
Player 5 eats 5 chips from the bag.
Player 0's turn in round 1.
Player 0 has drawn card: Eight of Diamonds.
Player 0 discarded card: Eight of Diamonds
Player 0 wants to eat 5 chips, but only 2 are left in the bag. They eat the remaining chips and will open a new bag.
Player 0 opens a new bag of chips.
Player 0 eats 3 chips from the bag.
Player 2's turn in round 1.
Player 2 has drawn card: Three of Diamonds.
Player 2 discarded card: Three of Diamonds
Player 2 eats 3 chips from the bag.
Player 3's turn in round 1.
Player 3 has drawn card: Six of Spades.
Player 3 discarded card: Six of Spades
Player 3 eats 3 chips from the bag.
Player 4's turn in round 1.
Player 4 has drawn card: Ace of Diamonds.
Player 4 has a card matching the greasy card and wins the round! Their hand:
Card 0: Five of Hearts
Card 1: Ace of Diamonds
Player 4 eats 1 chips from the bag.
Player 1 (dealer) is signaling the end of round 1.

*** New round ***
Player 2 (dealer) shuffling deck...
Player 2 (dealer) has chosen the greasy card: Four of Clubs.
Player 0 has been dealt card: Eight of Clubs.
Player 1 has been dealt card: Nine of Spades.
Player 3 has been dealt card: King of Clubs.
Player 4 has been dealt card: Six of Clubs.
Player 5 has been dealt card: Jack of Clubs.
Player 2 (dealer) is opening the first bag of chips for this round with 30 chips.
Player 3's turn in round 2.
Player 3 has drawn card: Ace of Diamonds.
Player 3 discarded card: Ace of Diamonds
Player 3 eats 2 chips from the bag.
Player 4's turn in round 2.
Player 4 has drawn card: Five of Hearts.
Player 4 discarded card: Five of Hearts
Player 4 eats 3 chips from the bag.
Player 5's turn in round 2.
Player 5 has drawn card: Four of Hearts.
Player 5 has a card matching the greasy card and wins the round! Their hand:
Card 0: Jack of Clubs
Card 1: Four of Hearts
Player 5 eats 1 chips from the bag.
Player 2 (dealer) is signaling the end of round 2.

*** New round ***
Player 3 (dealer) shuffling deck...
Player 3 (dealer) has chosen the greasy card: Six of Clubs.
Player 0 has been dealt card: Ten of Hearts.
Player 1 has been dealt card: Four of Clubs.
Player 2 has been dealt card: Jack of Spades.
Player 4 has been dealt card: Five of Spades.
Player 5 has been dealt card: Ace of Spades.
Player 3 (dealer) is opening the first bag of chips for this round with 30 chips.
Player 4's turn in round 3.
Player 4 has drawn card: Six of Spades.
Player 4 has a card matching the greasy card and wins the round! Their hand:
Card 0: Five of Spades
Card 1: Six of Spades
Player 4 eats 2 chips from the bag.
Player 3 (dealer) is signaling the end of round 3.

*** New round ***
Player 4 (dealer) shuffling deck...
Player 4 (dealer) has chosen the greasy card: Jack of Diamonds.
Player 0 has been dealt card: King of Clubs.
Player 1 has been dealt card: Six of Spades.
Player 2 has been dealt card: Three of Clubs.
Player 3 has been dealt card: Four of Diamonds.
Player 5 has been dealt card: Ace of Diamonds.
Player 4 (dealer) is opening the first bag of chips for this round with 30 chips.
Player 5's turn in round 4.
Player 5 has drawn card: Four of Hearts.
Player 5 discarded card: Four of Hearts
Player 5 eats 2 chips from the bag.
Player 0's turn in round 4.
Player 0 has drawn card: Ace of Spades.
Player 0 discarded card: Ace of Spades
Player 0 eats 2 chips from the bag.
Player 1's turn in round 4.
Player 1 has drawn card: Four of Clubs.
Player 1 discarded card: Six of Spades
Player 1 eats 1 chips from the bag.
Player 2's turn in round 4.
Player 2 has drawn card: Five of Spades.
Player 2 discarded card: Three of Clubs
Player 2 eats 3 chips from the bag.
Player 3's turn in round 4.
Player 3 has drawn card: Three of Hearts.
Player 3 discarded card: Four of Diamonds
Player 3 eats 4 chips from the bag.
Player 5's turn in round 4.
Player 5 has drawn card: Ten of Spades.
Player 5 discarded card: Ten of Spades
Player 5 eats 2 chips from the bag.
Player 0's turn in round 4.
Player 0 has drawn card: Queen of Hearts.
Player 0 discarded card: Queen of Hearts
Player 0 eats 4 chips from the bag.
Player 1's turn in round 4.
Player 1 has drawn card: Jack of Clubs.
Player 1 has a card matching the greasy card and wins the round! Their hand:
Card 0: Four of Clubs
Card 1: Jack of Clubs
Player 1 eats 1 chips from the bag.
Player 4 (dealer) is signaling the end of round 4.

*** New round ***
Player 5 (dealer) shuffling deck...
Player 5 (dealer) has chosen the greasy card: Jack of Diamonds.
Player 0 has been dealt card: King of Clubs.
Player 1 has been dealt card: Six of Spades.
Player 2 has been dealt card: Three of Clubs.
Player 3 has been dealt card: Four of Diamonds.
Player 4 has been dealt card: Ace of Diamonds.
Player 5 (dealer) is opening the first bag of chips for this round with 30 chips.
Player 0's turn in round 5.
Player 0 has drawn card: Four of Hearts.
Player 0 discarded card: King of Clubs
Player 0 eats 1 chips from the bag.
Player 1's turn in round 5.
Player 1 has drawn card: Ace of Spades.
Player 1 discarded card: Six of Spades
Player 1 eats 4 chips from the bag.
Player 2's turn in round 5.
Player 2 has drawn card: Four of Clubs.
Player 2 discarded card: Three of Clubs
Player 2 eats 4 chips from the bag.
Player 3's turn in round 5.
Player 3 has drawn card: Five of Spades.
Player 3 discarded card: Four of Diamonds
Player 3 eats 1 chips from the bag.
Player 4's turn in round 5.
Player 4 has drawn card: Three of Hearts.
Player 4 discarded card: Ace of Diamonds
Player 4 eats 5 chips from the bag.
Player 0's turn in round 5.
Player 0 has drawn card: Ten of Spades.
Player 0 discarded card: Four of Hearts
Player 0 eats 3 chips from the bag.
Player 1's turn in round 5.
Player 1 has drawn card: Queen of Hearts.
Player 1 discarded card: Ace of Spades
Player 1 eats 1 chips from the bag.
Player 2's turn in round 5.
Player 2 has drawn card: Jack of Clubs.
Player 2 has a card matching the greasy card and wins the round! Their hand:
Card 0: Four of Clubs
Card 1: Jack of Clubs
Player 2 eats 2 chips from the bag.
Player 5 (dealer) is signaling the end of round 5.
All player threads have finished. Game End!
```


# Run 5: (Seed = 5)
```
Game started with seed 5, 6 players, and 30 chips in bag.
Player 0 is starting their thread.
Player 1 is starting their thread.
Player 4 is starting their thread.
Player 5 is starting their thread.
Player 2 is starting their thread.
Player 3 is starting their thread.

*** New round ***
Player 0 (dealer) shuffling deck...
Player 0 (dealer) has chosen the greasy card: Three of Spades.
Player 1 has been dealt card: Seven of Hearts.
Player 2 has been dealt card: Jack of Clubs.
Player 3 has been dealt card: Five of Spades.
Player 4 has been dealt card: Nine of Diamonds.
Player 5 has been dealt card: Six of Clubs.
Player 0 (dealer) is opening the first bag of chips for this round with 30 chips.
Player 1's turn in round 0.
Player 1 has drawn card: Five of Clubs.
Player 1 discarded card: Five of Clubs
Player 1 eats 3 chips from the bag.
Player 2's turn in round 0.
Player 2 has drawn card: King of Diamonds.
Player 2 discarded card: King of Diamonds
Player 2 eats 3 chips from the bag.
Player 3's turn in round 0.
Player 3 has drawn card: Four of Spades.
Player 3 discarded card: Four of Spades
Player 3 eats 3 chips from the bag.
Player 4's turn in round 0.
Player 4 has drawn card: Jack of Spades.
Player 4 discarded card: Jack of Spades
Player 4 eats 3 chips from the bag.
Player 5's turn in round 0.
Player 5 has drawn card: Seven of Spades.
Player 5 discarded card: Seven of Spades
Player 5 eats 3 chips from the bag.
Player 1's turn in round 0.
Player 1 has drawn card: Two of Hearts.
Player 1 discarded card: Seven of Hearts
Player 1 eats 1 chips from the bag.
Player 2's turn in round 0.
Player 2 has drawn card: Ace of Diamonds.
Player 2 discarded card: Jack of Clubs
Player 2 eats 1 chips from the bag.
Player 3's turn in round 0.
Player 3 has drawn card: Six of Hearts.
Player 3 discarded card: Five of Spades
Player 3 eats 1 chips from the bag.
Player 4's turn in round 0.
Player 4 has drawn card: Seven of Diamonds.
Player 4 discarded card: Nine of Diamonds
Player 4 eats 1 chips from the bag.
Player 5's turn in round 0.
Player 5 has drawn card: Eight of Hearts.
Player 5 discarded card: Six of Clubs
Player 5 eats 1 chips from the bag.
Player 1's turn in round 0.
Player 1 has drawn card: Jack of Diamonds.
Player 1 discarded card: Jack of Diamonds
Player 1 eats 5 chips from the bag.
Player 2's turn in round 0.
Player 2 has drawn card: Queen of Diamonds.
Player 2 discarded card: Queen of Diamonds
Player 2 eats 5 chips from the bag.
Player 3's turn in round 0.
Player 3 has drawn card: King of Spades.
Player 3 discarded card: King of Spades
Player 3 opens a new bag of chips.
Player 3 eats 5 chips from the bag.
Player 4's turn in round 0.
Player 4 has drawn card: Four of Hearts.
Player 4 discarded card: Four of Hearts
Player 4 eats 5 chips from the bag.
Player 5's turn in round 0.
Player 5 has drawn card: Two of Clubs.
Player 5 discarded card: Two of Clubs
Player 5 eats 5 chips from the bag.
Player 1's turn in round 0.
Player 1 has drawn card: Five of Diamonds.
Player 1 discarded card: Two of Hearts
Player 1 eats 4 chips from the bag.
Player 2's turn in round 0.
Player 2 has drawn card: Five of Hearts.
Player 2 discarded card: Ace of Diamonds
Player 2 eats 4 chips from the bag.
Player 3's turn in round 0.
Player 3 has drawn card: King of Hearts.
Player 3 discarded card: Six of Hearts
Player 3 eats 4 chips from the bag.
Player 4's turn in round 0.
Player 4 has drawn card: Three of Hearts.
Player 4 has a card matching the greasy card and wins the round! Their hand:
Card 0: Seven of Diamonds
Card 1: Three of Hearts
Player 4 wants to eat 4 chips, but only 3 are left in the bag. They eat the remaining chips and will open a new bag.
Player 4 opens a new bag of chips.
Player 4 eats 1 chips from the bag.
Player 0 (dealer) is signaling the end of round 0.

*** New round ***
Player 1 (dealer) shuffling deck...
Player 1 (dealer) has chosen the greasy card: Ace of Clubs.
Player 0 has been dealt card: Jack of Clubs.
Player 2 has been dealt card: Eight of Hearts.
Player 3 has been dealt card: Ten of Diamonds.
Player 4 has been dealt card: Six of Hearts.
Player 5 has been dealt card: Seven of Hearts.
Player 1 (dealer) is opening the first bag of chips for this round with 30 chips.
Player 2's turn in round 1.
Player 2 has drawn card: Six of Clubs.
Player 2 discarded card: Eight of Hearts
Player 2 eats 5 chips from the bag.
Player 3's turn in round 1.
Player 3 has drawn card: Ten of Spades.
Player 3 discarded card: Ten of Diamonds
Player 3 eats 5 chips from the bag.
Player 4's turn in round 1.
Player 4 has drawn card: Queen of Hearts.
Player 4 discarded card: Six of Hearts
Player 4 eats 3 chips from the bag.
Player 5's turn in round 1.
Player 5 has drawn card: Two of Spades.
Player 5 discarded card: Seven of Hearts
Player 5 eats 4 chips from the bag.
Player 0's turn in round 1.
Player 0 has drawn card: Jack of Spades.
Player 0 discarded card: Jack of Clubs
Player 0 eats 3 chips from the bag.
Player 2's turn in round 1.
Player 2 has drawn card: Ten of Clubs.
Player 2 discarded card: Ten of Clubs
Player 2 eats 1 chips from the bag.
Player 3's turn in round 1.
Player 3 has drawn card: Five of Spades.
Player 3 discarded card: Five of Spades
Player 3 eats 1 chips from the bag.
Player 4's turn in round 1.
Player 4 has drawn card: Five of Hearts.
Player 4 discarded card: Queen of Hearts
Player 4 eats 1 chips from the bag.
Player 5's turn in round 1.
Player 5 has drawn card: Ten of Hearts.
Player 5 discarded card: Two of Spades
Player 5 eats 5 chips from the bag.
Player 0's turn in round 1.
Player 0 has drawn card: Eight of Diamonds.
Player 0 discarded card: Eight of Diamonds
Player 0 wants to eat 5 chips, but only 2 are left in the bag. They eat the remaining chips and will open a new bag.
Player 0 opens a new bag of chips.
Player 0 eats 3 chips from the bag.
Player 2's turn in round 1.
Player 2 has drawn card: Three of Diamonds.
Player 2 discarded card: Three of Diamonds
Player 2 eats 3 chips from the bag.
Player 3's turn in round 1.
Player 3 has drawn card: Six of Spades.
Player 3 discarded card: Six of Spades
Player 3 eats 3 chips from the bag.
Player 4's turn in round 1.
Player 4 has drawn card: Ace of Diamonds.
Player 4 has a card matching the greasy card and wins the round! Their hand:
Card 0: Five of Hearts
Card 1: Ace of Diamonds
Player 4 eats 1 chips from the bag.
Player 1 (dealer) is signaling the end of round 1.

*** New round ***
Player 2 (dealer) shuffling deck...
Player 2 (dealer) has chosen the greasy card: Four of Clubs.
Player 0 has been dealt card: Eight of Clubs.
Player 1 has been dealt card: Nine of Spades.
Player 3 has been dealt card: King of Clubs.
Player 4 has been dealt card: Six of Clubs.
Player 5 has been dealt card: Jack of Clubs.
Player 2 (dealer) is opening the first bag of chips for this round with 30 chips.
Player 3's turn in round 2.
Player 3 has drawn card: Ace of Diamonds.
Player 3 discarded card: Ace of Diamonds
Player 3 eats 2 chips from the bag.
Player 4's turn in round 2.
Player 4 has drawn card: Five of Hearts.
Player 4 discarded card: Five of Hearts
Player 4 eats 3 chips from the bag.
Player 5's turn in round 2.
Player 5 has drawn card: Four of Hearts.
Player 5 has a card matching the greasy card and wins the round! Their hand:
Card 0: Jack of Clubs
Card 1: Four of Hearts
Player 5 eats 1 chips from the bag.
Player 2 (dealer) is signaling the end of round 2.

*** New round ***
Player 3 (dealer) shuffling deck...
Player 3 (dealer) has chosen the greasy card: Six of Clubs.
Player 0 has been dealt card: Ten of Hearts.
Player 1 has been dealt card: Four of Clubs.
Player 2 has been dealt card: Jack of Spades.
Player 4 has been dealt card: Five of Spades.
Player 5 has been dealt card: Ace of Spades.
Player 3 (dealer) is opening the first bag of chips for this round with 30 chips.
Player 4's turn in round 3.
Player 4 has drawn card: Six of Spades.
Player 4 has a card matching the greasy card and wins the round! Their hand:
Card 0: Five of Spades
Card 1: Six of Spades
Player 4 eats 2 chips from the bag.
Player 3 (dealer) is signaling the end of round 3.

*** New round ***
Player 4 (dealer) shuffling deck...
Player 4 (dealer) has chosen the greasy card: Jack of Diamonds.
Player 0 has been dealt card: King of Clubs.
Player 1 has been dealt card: Six of Spades.
Player 2 has been dealt card: Three of Clubs.
Player 3 has been dealt card: Four of Diamonds.
Player 5 has been dealt card: Ace of Diamonds.
Player 4 (dealer) is opening the first bag of chips for this round with 30 chips.
Player 5's turn in round 4.
Player 5 has drawn card: Four of Hearts.
Player 5 discarded card: Four of Hearts
Player 5 eats 2 chips from the bag.
Player 0's turn in round 4.
Player 0 has drawn card: Ace of Spades.
Player 0 discarded card: Ace of Spades
Player 0 eats 2 chips from the bag.
Player 1's turn in round 4.
Player 1 has drawn card: Four of Clubs.
Player 1 discarded card: Six of Spades
Player 1 eats 1 chips from the bag.
Player 2's turn in round 4.
Player 2 has drawn card: Five of Spades.
Player 2 discarded card: Three of Clubs
Player 2 eats 3 chips from the bag.
Player 3's turn in round 4.
Player 3 has drawn card: Three of Hearts.
Player 3 discarded card: Four of Diamonds
Player 3 eats 4 chips from the bag.
Player 5's turn in round 4.
Player 5 has drawn card: Ten of Spades.
Player 5 discarded card: Ten of Spades
Player 5 eats 2 chips from the bag.
Player 0's turn in round 4.
Player 0 has drawn card: Queen of Hearts.
Player 0 discarded card: Queen of Hearts
Player 0 eats 4 chips from the bag.
Player 1's turn in round 4.
Player 1 has drawn card: Jack of Clubs.
Player 1 has a card matching the greasy card and wins the round! Their hand:
Card 0: Four of Clubs
Card 1: Jack of Clubs
Player 1 eats 1 chips from the bag.
Player 4 (dealer) is signaling the end of round 4.

*** New round ***
Player 5 (dealer) shuffling deck...
Player 5 (dealer) has chosen the greasy card: Jack of Diamonds.
Player 0 has been dealt card: King of Clubs.
Player 1 has been dealt card: Six of Spades.
Player 2 has been dealt card: Three of Clubs.
Player 3 has been dealt card: Four of Diamonds.
Player 4 has been dealt card: Ace of Diamonds.
Player 5 (dealer) is opening the first bag of chips for this round with 30 chips.
Player 0's turn in round 5.
Player 0 has drawn card: Four of Hearts.
Player 0 discarded card: King of Clubs
Player 0 eats 1 chips from the bag.
Player 1's turn in round 5.
Player 1 has drawn card: Ace of Spades.
Player 1 discarded card: Six of Spades
Player 1 eats 4 chips from the bag.
Player 2's turn in round 5.
Player 2 has drawn card: Four of Clubs.
Player 2 discarded card: Three of Clubs
Player 2 eats 4 chips from the bag.
Player 3's turn in round 5.
Player 3 has drawn card: Five of Spades.
Player 3 discarded card: Four of Diamonds
Player 3 eats 1 chips from the bag.
Player 4's turn in round 5.
Player 4 has drawn card: Three of Hearts.
Player 4 discarded card: Ace of Diamonds
Player 4 eats 5 chips from the bag.
Player 0's turn in round 5.
Player 0 has drawn card: Ten of Spades.
Player 0 discarded card: Four of Hearts
Player 0 eats 3 chips from the bag.
Player 1's turn in round 5.
Player 1 has drawn card: Queen of Hearts.
Player 1 discarded card: Ace of Spades
Player 1 eats 1 chips from the bag.
Player 2's turn in round 5.
Player 2 has drawn card: Jack of Clubs.
Player 2 has a card matching the greasy card and wins the round! Their hand:
Card 0: Four of Clubs
Card 1: Jack of Clubs
Player 2 eats 2 chips from the bag.
Player 5 (dealer) is signaling the end of round 5.
All player threads have finished. Game End!
```