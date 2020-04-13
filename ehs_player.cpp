/*
 * Filename: ehs_player.cpp
 */

#include "ehs_player.h"
#include <iostream>
#include <string>
#include <vector>
#include "misc.h"
#include "strength.h"

using namespace std;

int GetNumOfActions(GameState game_state, int street) {
	switch (street) {
		case 0:
				return game_state.action_history.preflop.size();
		case 1:	
				return game_state.action_history.flop.size();
		case 2:	
				return game_state.action_history.turn.size();
		case 3:	
				return game_state.action_history.river.size();
		default: 
				return -1;
	}
}

Action EhsPlayer::Act(GameState game_state, LegalActions legal_actions) {
	Action action_to_return;
	
	std::cout << "[AI] My cards are:" << GetHoleCards() << std::endl;
	std::cout << "[AI] I am at seat " << GetID() << std::endl;
//	std::cout << "Min raise:" << legal_actions.LegalMinRaise.amount << " ";
//	std::cout << "Max raise:" << legal_actions.LegalMaxRaise.amount;
	std::cout << std::endl;

	float IHS, EHS;
	int my_id = this->GetID();
	int IsInPosition = (my_id == game_state.sb_pos ? 1 : 0);
	vector <Card> board = game_state.community_cards;
	vector <Card> my_cards = this->GetHoleCards();
	// preflop strategy
	if (game_state.current_street == 0) {
		IHS = GetImmediateStrength(my_cards, board, NULL);
		if (GetNumOfActions(game_state, 0) == 0) {
			// first to act
			cout << "[AI] I am first to act..." << endl;
			if (IHS > 0.6) {
				action_to_return.action = 2;		// raise 
				action_to_return.amount = 2 * game_state.total_pot_size;
			} else {
				action_to_return.action = 1;		// call
			}
		}

		if (GetNumOfActions(game_state, 0) == 1) {
			if (game_state.action_history.preflop[0].player_action.action == 1)  { // opp calls
				cout << "[AI] OK... you call..." << endl;
				if (IHS > 0.7) {
					action_to_return.action = 2;		// raise 
					action_to_return.amount = 2 * game_state.total_pot_size;
				}
				action_to_return.action = 1;		// call
			} else {					// opp raises
				cout << "[AI] You raised..." << endl;
				if (IHS > 0.8) {
					action_to_return.action = 2;		// raise 
					action_to_return.amount = 2 * game_state.total_pot_size;
				} else {
					action_to_return.action = 1;		// call
				}
			}
		}

		if (GetNumOfActions(game_state, 0) >= 2) {
			cout << "[AI] You raised me!" << endl;
			if (IHS > 0.8) {
				action_to_return.action = 1;	// call
			} else {
				action_to_return.action = 0;	// fold
			}
		}
		return action_to_return;
	}	

	// flop
	if (game_state.current_street == 1) {
		action_to_return.action = 1;	
	}

	// turn
	if (game_state.current_street == 2) {
		action_to_return.action = 1;	
	}

	// river
	if (game_state.current_street == 3) {
		action_to_return.action = 1;	
	}

	return action_to_return;
}
