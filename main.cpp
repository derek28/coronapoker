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
	std::srand((unsigned) time(0));
	// Initialize Game
    Game game;
	// Define the 2 players, one AI, the other AI/player	
    game.AddPlayer(0,10000,1);
	game.AddPlayer(1,10000,1);
	
    for (int ihand = 0 ; ihand < 10 ; ihand++ ) {  //run 10 hands
/*To be added*/        // maybe check couple criteria to make sure game can happen
		game.PostBlinds();
		//game.PrintGameState();
        game.StartAHand();
        //game.PrintGameState();
        
		while (1) { //it breaks when a hand finishes
			//Find next player to act

			//Ask player (pointed by nextplayertoact) to act
			ActionWithID ac = game.AskPlayerToAct();
			game.UpdateGameState(ac);
			game.PrintGameState();

			//Check if there is only 1 player left
			if (game.IsPotUncontested() ){
				#ifdef DEBUG
					std::cout << "[DEBUG] reach end of game: pot uncontested" << std::endl;
			    #endif
				vector<int> winners = game.GetWinner();
				game.CollectMoneyFromBetRing();
				game.PayWinner(winners);
				break;				
			}
			//If end of street is reached
			if (game.IsEndOfStreet()) {
			    game.CollectMoneyFromBetRing();
				game.SetupNextStreet();
			}

			//End of game condition: we reach showdown
            if (game.HasReachShowdown()) {
				#ifdef DEBUG
					std::cout << "[DEBUG] reach end of game: showdown" << std::endl;
				#endif
			    vector<int> winners = game.GetWinner();
				game.PayWinner(winners);
				break;
			}
		}
		game.PrintGameState();
		game.CleanCommunityCard();
		game.RemovePlayerCard();
		game.MoveBtn();
		std::cin.ignore();
	}	
	return 0;
}
