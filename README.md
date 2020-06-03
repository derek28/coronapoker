# Coronapoker 

Highly efficient Texas Hold'em simulation framework, with AI created during the great COVID-19 epidemic, by JWang925 and Kai.

## Player with AI:

Step 1: Run ./humanvsbot <numberofhands> \
Step 2: Run ./PokerGUI and click "file"-> "connect" to connect to human player server, if your game has a human player.




## Create your own AI:
Step 1: Create your own player class, derived from base class Player. See RandomPlayer for example. \
Step 2: Setup your game, see humanvsbot.cpp for an example. \
Step 3: Run ./humanvsbot <numberofhands>  \
Step 4 (optional): Run ./PokerGUI and click "file"-> "connect" to connect to human player server, if your game has a human player.


## Known limitation:
Only headsup is functional. For multi-way to work, need to have better pot size raise computation and side pot functionality.
Each hand must start with 100bb.
