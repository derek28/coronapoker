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
int GetOppAction(GameState game_state, int street) {
	switch (street) {
		case 0:
				return game_state.action_history.preflop[0].player_action.action;
		case 1:
				return game_state.action_history.flop[0].player_action.action;
		case 2:
				return game_state.action_history.turn[0].player_action.action;
		case 3:
				return game_state.action_history.river[0].player_action.action;
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

float GetPotOdds(GameState game_state, LegalActions legal_actions) {
	return (float)legal_actions.LegalCall.amount / (float)game_state.total_pot_size;	
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
	int street = game_state.current_street;
	int IsInPosition = (my_id == game_state.sb_pos ? 1 : 0);
	int num_of_actions = GetNumOfActions(game_state, street);

	vector <Card> board = game_state.community_cards;
	vector <Card> my_cards = this->GetHoleCards();

	// preflop strategy
	if (street == 0) {
		IHS = GetImmediateStrength(my_cards, board, NULL);
		cout << "[AI] My IHS = " << IHS << endl;

		if (num_of_actions == 0) {
			// first to act
			cout << "[AI] I am first to act..." << endl;
			if (IHS > 0.6) {
				SetRaise(my_action, 2 * game_state.total_pot_size, legal_actions);
			} else {
				SetCall(my_action, legal_actions);
			}
		}


		// opponent acted once
		if (num_of_actions == 1)  { 
			if (GetOppAction(game_state, street) == 1) { // opp called
				cout << "[AI] OK... you call..." << endl;
				if (IHS > 0.7) {
					SetRaise(my_action, 2 * game_state.total_pot_size, legal_actions);
				} else {
					SetCheck(my_action);
				}	
			} else {					// opp raised
				cout << "[AI] You raised..." << endl;
				if (IHS > 0.9 || IHS < 0.25) {
					SetRaise(my_action, 2 * game_state.total_pot_size, legal_actions);
				} else if (IHS > 0.25) {
					SetCall(my_action, legal_actions);
				} else {
					SetFold(my_action);
				}
			}
		}

		if (num_of_actions >= 2) {
			// opponent raised your limp or reraised you
			cout << "[AI] You raised me!" << endl;
			if (IHS > 0.5) {
				SetCall(my_action, legal_actions);
			} else {
				SetFold(my_action);
			}
		}
	}	

	// flop = 1; turn = 2; river = 3
	if (street > 0 && street <= 3) {
		EHS = GetEffectiveStrength(my_cards, board, NULL);
		cout << "[AI] My EHS = " << EHS << endl;

		if (num_of_actions == 0) {
			// first to act on this street 
			cout << "[AI] I will act first." << endl;
			if (EHS > 0.7) {	// raise half pot
				SetRaise(my_action, 0.5 * game_state.total_pot_size, legal_actions);
			} else {
				SetCheck(my_action);
			}
		} 

		if (num_of_actions == 1) {
			// opponent acted once 
			if (GetOppAction(game_state, street) == 1) {		// check 
				cout << "[AI] OK... you checked..." << endl;
				if (EHS > 0.7) {
					SetRaise(my_action, 0.5 * game_state.total_pot_size, legal_actions);
				} else {
					SetCheck(my_action);
				}
			} else {					// opp raises
				cout << "[AI] You raised..." << endl;
				cout << "[AI] I am given a pot odds of " << GetPotOdds(game_state, legal_actions) << endl;
				if (EHS > 0.85 || EHS < 0.2) {
					SetRaise(my_action, game_state.total_pot_size, legal_actions);
				} else if (EHS > GetPotOdds(game_state, legal_actions)) {
					SetCall(my_action, legal_actions);
				} else {
					SetFold(my_action);	
				}
			}
		}

		if (num_of_actions >= 2) {
			// opponent raised your check or reraised you
			cout << "[AI] You raised me!" << endl;
			cout << "[AI] I am given a pot odds of " << GetPotOdds(game_state, legal_actions) << endl;
			if (EHS > GetPotOdds(game_state, legal_actions)) {
				SetCall(my_action, legal_actions);
			} else {
				SetFold(my_action);
			}
		}

	}

	return my_action;
}
