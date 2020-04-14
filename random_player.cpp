
/**************************************************** 
 * Filename: random_player.cpp
 * Last modified: 2020/04/01
 * Description: Blabla
 ***************************************************/

#include "random_player.h"

Action RandomPlayer::Act(GameState game_state, LegalActions legal_actions) {
    Action action_to_return;

	int rand_num = rand() % 100;
	
	int action = 0; // 0 fold 1 check/call 2 min-raise 3 1/2 pot raise/bet

	//20% fold/check  
	//30% check/call  
	//20% min-raise (if possible, otherwise call) 
	//30% 1/2 pot raise (if possible, otherwise call)
	if ( rand_num < 20) {
		action = 0;
		if (legal_actions.LegalCall.amount == 0) //if call is free, then call
			action = 1;
	}
	else if (rand_num < 50) {
		action = 1;
	} 
	else if (rand_num < 70) {
		action = 2;
		if (legal_actions.LegalMinRaise.amount == -1)
			action = 1;
	} 
	else {
		action = 3;
		if (legal_actions.LegalMinRaise.amount == -1)
			action = 1;
	}

	switch (action)
	{
	case 0:
		action_to_return.action = 0;
		action_to_return.amount = 0;
		break;

	case 1:
		action_to_return.action = 1;
		action_to_return.amount = legal_actions.LegalCall.amount;
		break;

	case 2:
		action_to_return.action = 2;
		action_to_return.amount = legal_actions.LegalMinRaise.amount;
		break;

	case 3:
		action_to_return.action = 2;
		action_to_return.amount = std::min( game_state.stack_size[GetID()], \
							legal_actions.LegalCall.amount + game_state.bet_ring[GetID()] \
							+(game_state.total_pot_size + legal_actions.LegalCall.amount)/2 );
		break;

	default:
		break;
	}




	return action_to_return;
}