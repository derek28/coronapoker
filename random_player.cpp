
/**************************************************** 
 * Filename: random_player.h
 * Last modified: 2020/04/01
 * Description: Blabla
 ***************************************************/



#pragma once

#include "player.h"

class random_player: public player {
    public: 
    	Action Act(GameState);
}

Action random_player::Act(GameState) {
    Action action_to_return;

//  Always fold
//	action_to_return.action = 0 ; //0 = fold 
//	action_to_return.amount = 0 ;

//  Always call
//	action_to_return.action = 1 ; //1 = call
//	action_to_return.amount = *std::max_element(game_state.bet_ring,game_state.bet_ring+9) \
									- game_state.bet_ring[player_id_]  ;

//  Always raise
	action_to_return.action = 2 ; //2 = raise
	action_to_return.amount = *std::max_element(game_state.bet_ring,game_state.bet_ring+9) \
									+ game_state.raise_amount;
	return action_to_return;
}