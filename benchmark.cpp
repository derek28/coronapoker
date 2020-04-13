#include <iostream>
#include "card.h"
#include "deck.h"
#include "pokerhand.h"
#include "game.h"
#include "game_state.h"
#include "random_player.h"
using namespace std;

int main()
{
	cout << "Hello, Corona!" << endl;
	std::srand((unsigned) time(0));
	// Instantiate Game
    Game game;


	//Setup game here
	// Define the 2 players, one AI, the other AI/player	
	Player* player1 = new RandomPlayer();
	Player* player2 = new RandomPlayer();
	game.AddPlayer(0,10000, player1);
	game.AddPlayer(1,10000, player2);
	int number_of_hands = 50000;


	#ifdef DEBUG
		game.PrintGameState();
	#endif
	clock_t begin= clock();
    for (int ihand = 0 ; ihand < number_of_hands ; ihand++ ) {
		game.ResetGameState();
		game.PostBlinds();
        game.ShuffleAndDeal();
        
		while (1) { //it breaks when a hand finishes
			//Ask player (pointed by nextplayertoact) to act
			LegalActions legal_ac = game.GetAllLegalActions();
			ActionWithID ac = game.AskPlayerToAct(legal_ac);
			ac = game.VerifyAction(ac, legal_ac);
			game.UpdateGameState(ac);
			#ifdef DEBUG
				game.PrintGameStateDebug();
			#endif
			//Check if there is only 1 player left
			if (game.IsPotUncontested() ){
				std::cout << "[INFO] reach end of game: pot uncontested" << std::endl;
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
				std::cout << "[INFO] reach end of game: showdown" << std::endl;
			    vector<int> winners = game.GetWinner();
				game.PayWinner(winners);
				break;
			}
		}
		//game.PrintGameState();
		game.CleanCommunityCard();
		game.RemovePlayerCard();
		game.MoveBtn();

		
		//std::cin.ignore();
	}

	clock_t end = clock();
	double elapsed_secs= double(end-begin) / CLOCKS_PER_SEC;
	std::cout << number_of_hands << " hand evaluation done in " << elapsed_secs << " seconds" << std::endl;	
	return 0;
}
