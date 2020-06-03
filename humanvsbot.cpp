/*
 *	humanvsbot.cpp
 */

#include <iostream>
#include <cstring>
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

int str2int(char *str) {
	int len = strlen(str);
	int r = 0;
	for (int i = 0; i < len; i++) {
		r = r * 10 + (str[i] - '0');
	}
	return r;
}

int main(int argc, char *argv[])
{
	int nhands;
	cout << "Hello, Corona!" << endl;

	if (argc != 2) {
		cout << "Usage: ./humanvsbot [NUM of Hands]" << endl;
		return -1;
	}

	nhands = str2int(argv[1]);	
	cout << "Starting game... Total number of hands: " << nhands << endl;
		
	std::srand((unsigned) time(0));
	// Instantiate Game
    Game game;
	//Setup game here
	// Define the 2 players.
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
