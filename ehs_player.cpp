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

void SetCall(Action &act, LegalActions legal_act) {
	act.action = 1;
	act.amount = legal_act.LegalCall.amount;
}

void SetRaise(Action &act, int amount, LegalActions legal_act) {
	act.action = 2;
	act.amount = amount;
}

void SetFold(Action &act) {
	act.action = 0;
}

void SetCheck(Action &act) {
	act.action = 1;
	act.amount = 0;
}

Action EhsPlayer::Act(GameState game_state, LegalActions legal_actions) {
	Action my_action;
	
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
				SetRaise(my_action, 2 * game_state.total_pot_size, legal_actions);
			} else {
				SetCall(my_action, legal_actions);
			}
		}

		if (GetNumOfActions(game_state, 0) == 1) {
			if (game_state.action_history.preflop[0].player_action.action == 1)  { // opp calls
				cout << "[AI] OK... you call..." << endl;
				if (IHS > 0.7) {
					SetRaise(my_action, 2 * game_state.total_pot_size, legal_actions);
				} else {
					SetCheck(my_action);
				}
			} else {					// opp raises
				cout << "[AI] You raised..." << endl;
				if (IHS > 0.8) {
					SetRaise(my_action, 2 * game_state.total_pot_size, legal_actions);
				} else {
					SetCall(my_action, legal_actions);
				}
			}
		}

		if (GetNumOfActions(game_state, 0) >= 2) {
			cout << "[AI] You raised me!" << endl;
			if (IHS > 0.8) {
				SetCall(my_action, legal_actions);
			} else {
				SetFold(my_action);
			}
		}
	}	

	// flop
	if (game_state.current_street == 1) {
		IHS = GetImmediateStrength(my_cards, board, NULL);
		SetCall(my_action, legal_actions);	
	}

	// turn
	if (game_state.current_street == 2) {
		SetCall(my_action, legal_actions);	
	}

	// river
	if (game_state.current_street == 3) {
		SetCall(my_action, legal_actions);	
	}

	return my_action;
}
