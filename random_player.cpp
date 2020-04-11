
/**************************************************** 
 * Filename: random_player.cpp
 * Last modified: 2020/04/01
 * Description: Blabla
 ***************************************************/



#pragma once

#include "player.h"

Action RandomPlayer::Act(GameState) {
    Action action_to_return;
	int rand_num = rand() % 100;
	if ( rand_num < 80) {
		action_to_return.action = 1 ; //1 = call
		action_to_return.amount = *std::max_element(game_state.bet_ring,game_state.bet_ring+9) \
										- game_state.bet_ring[player_id_] ;	
	} 
	else if (rand_num < 95) {
		action_to_return.action = 2 ; //2 = raise
		action_to_return.amount = *std::max_element(game_state.bet_ring,game_state.bet_ring+9) \
										+ game_state.raise_amount;
	} 
	else {
		action_to_return.action = 2 ;
		action_to_return.amount = 5 * ( *std::max_element(game_state.bet_ring,game_state.bet_ring+9) \
										+ game_state.raise_amount );
	}


	return action_to_return;
}