# Coronapoker 

Texas Hold'em AI created during the great COVID-19 epidemic, by Bill and Kai.


Step 1: Create your own class, derived from base class Player. 
        See RandomPlayer for example.

Step 2: Setup main() like follow:
        //Setup game here
        // Define the 2 players, one AI, the other AI/player	
        Player* player1 = new RandomPlayer();
        Player* player2 = new RandomPlayer();
        game.AddPlayer(0,10000, player1);
        game.AddPlayer(1,10000, player2);
        int number_of_hands = 10000;

Step 3: Compile and run ./xxx >log 2>&1



Known limitation:
        Only headsup is functional. For multi-way to work, need to have better pot size raise computation and side pot functionality.
        Each hand must start with 100bb.