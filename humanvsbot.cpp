#include <iostream>
#include "card.h"
#include "deck.h"
#include "pokerhand.h"
#include "game.h"
#include "game_state.h"
#include "random_player.h"
#include "human_player.h"
#include "ehs_player.h"
#include "server.h"

using namespace std;

int main()
{
	cout << "Hello, Corona!" << endl;
	std::srand((unsigned) time(0));
	// Instantiate Game
    Game game;
	//Setup game here
	// Define the 2 players.
	int nhands=5;	
	Player* player1 = new HumanPlayer();
	Player* player2 = new EhsPlayer();
	game.AddPlayer(0,10000, player1);
	game.AddPlayer(1,10000, player2);
	game.SetNumOfHands(nhands);
	#ifdef DEBUG
		game.PrintGameState();
	#endif

	clock_t begin= clock();
	game.Start();
	clock_t end = clock();
	double elapsed_secs= double(end-begin) / CLOCKS_PER_SEC;
	std::cout << nhands << " hand evaluation done in " << elapsed_secs << " seconds" << std::endl;	
	game.PrintResult();
	return 0;
}
