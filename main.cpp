#include <iostream>
#include "card.h"
#include "deck.h"
#include "pokerhand.h"
#include "game.h"
#include "game_state.h"
#define DEBUG 

using namespace std;

int main()
{
	cout << "Hello, Corona!" << endl;

	// Initialize Game
    Game game;
    game.PrintGameState();
	// Define the 2 players, one AI, the other AI/player	
    game.AddPlayer(0,10000,1);
	game.AddPlayer(1,10000,1);
    game.PrintGameState();
	
    //while (1) {  // while no one is broke
/*To be added*/        // maybe check couple criteria to make sure game can happen
   		// post small/big blinds
        game.PostBlinds();
		// deal cards to players
        game.PrintGameState();

		// player action
		// flop
		// player action
		// turn
		// player action
		// river 
		// player action
		// showdown
		// chip counting

//	}	
	return 0;
}
