/*
 * Filename: ehs_player.cpp
 */

#include "ehs_player.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include "misc.h"
#include "strength.h"

using namespace std;

static int RandomAction(float prob) {
	float r = (float)rand() / RAND_MAX;
	cout << "[AI] Thresh prob: " << prob;
	cout << "; Random(): " << r << endl;
	if (r > prob) 
		return 0;
	return 1;
}

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

float **AssignOppRange(GameState game_state, int IsInPosition) {
	int n = GetNumOfActions(game_state, 0);
	float **range = NULL;
	string ac_tree;
	int ac;

	// build action tree
	for (int i = 0; i < n; i++) {
		ac = game_state.action_history.preflop[i].player_action.action;
		ac_tree.push_back(ac == 1 ? '1' : '2');
	}
	cout << ac_tree << endl;

	if (IsInPosition) {	// I moved first
		if (ac_tree.compare("11") == 0) 		// limp - check
			return GetRangeTable(0.0, 1.0);
		if (ac_tree.compare("21") == 0) 		// raise - call	 ... call 2-bet range
			return GetRangeTable(0.2, 0.93);
		if (ac_tree.compare("121") == 0)				// limp - raise - call	... 
			return GetRangeTable(0.6, 1.0);
		if (ac_tree.compare("221") == 0)		// raise - 3bet - call	... 3-bet range
			return GetRangeTable(0.7, 1.0);
		if (ac_tree.compare("2221") == 0)		// raise - 3bet - 4bet - call	... call 4-bet range
			return GetRangeTable(0.8, 0.99);
		if (ac_tree.compare("22221") == 0)		// raise - 3bet - 4bet - 5bet - call	.. 5-bet range
			return GetRangeTable(0.97, 1.0);
		if (ac_tree.compare("1221") == 0)			// limp - raise - 3bet - call		... call 3-bet range
			return GetRangeTable(0.3, 0.99);
		if (ac_tree.compare("12221") == 0)			// limp - raise - 3bet - 4bet - call	... 4-bet range
			return GetRangeTable(0.95, 1.0);
		return GetRangeTable(0.97, 1.0);
	} else {		// Opp moved first
		if (ac_tree.compare("11") == 0) 		// limp - check
			return GetRangeTable(0.0, 1.0);
		if (ac_tree.compare("21") == 0) 		// raise - call	... open 2-bet range
			return GetRangeTable(0.2, 1.0);
		if (ac_tree.compare("221") == 0)		// raise - 3bet - call	... call 3-bet range
			return GetRangeTable(0.3, 0.99);
		if (ac_tree.compare("2221") == 0)		// raise - 3bet - 4bet - call	... 4 bet range
			return GetRangeTable(0.95, 1.0);
		if (ac_tree.compare("22221") == 0)		// raise - 3bet - 4bet - 5bet - call ... call 5 bet range
			return GetRangeTable(0.95, 0.995);
		if (ac_tree.compare("121") == 0)				// limp - raise - call
			return GetRangeTable(0.0, 0.75);
		if (ac_tree.compare("1221") == 0)			// limp - raise - 3bet - call
			return GetRangeTable(0.79, 1.0);
		if (ac_tree.compare("12221") == 0)			// limp - raise - 3bet - 4bet - call
			return GetRangeTable(0.85, 0.99);
		return GetRangeTable(0.97, 1.0);
	}
}

int GetOppAction(GameState game_state, int street) {
	int n = GetNumOfActions(game_state, street) - 1;
	switch (street) {
		case 0:	
				return game_state.action_history.preflop[n].player_action.action;
		case 1:
				return game_state.action_history.flop[n].player_action.action;
		case 2:
				return game_state.action_history.turn[n].player_action.action;
		case 3:
				return game_state.action_history.river[n].player_action.action;
		default:
				return -1;
	}
}

int GetMyAction(GameState game_state, int street) {
	int n = GetNumOfActions(game_state, street) - 2;
	switch (street) {
		case 0:	
				return game_state.action_history.preflop[n].player_action.action;
		case 1:
				return game_state.action_history.flop[n].player_action.action;
		case 2:
				return game_state.action_history.turn[n].player_action.action;
		case 3:
				return game_state.action_history.river[n].player_action.action;
		default:
				return -1;
	}
}

// Judge what is the action, fold, check, call or raise
static int AnalyzeAction(struct Action action) {
	if (action.action == 0) 
		return 0;
	if (action.action == 1 && action.amount == 0)
		return 0;
	if (action.action == 1 && action.amount > 0)
		return 1;
	return 2;
}

/*
 * Opp raises : aggro += x;
 * Opp calls your raise: aggro += x/2;
 * Opp checks: aggro = aggro;
 */
float GetAggroFactor(GameState game_state, int id) {	
	float aggro = 0.5;
	struct ActionHistory ah = game_state.action_history;
	int size = 0;
	int i = 0;
	size = ah.preflop.size();
	for (i = 0; i < size; i++) {
		if (ah.preflop[i].ID == id){
			aggro += 0.33 * AnalyzeAction(ah.preflop[i].player_action);
		}
	}

	size = ah.flop.size();
	if (size > 0) {
		aggro = 1;			// reset aggressive factor to 1, starting flop
	}
	for (i = 0; i < size; i++) {
		if (ah.flop[i].ID == id){
			aggro += 0.33 * AnalyzeAction(ah.flop[i].player_action);
		}
	}
	size = ah.turn.size();
	for (i = 0; i < size; i++) {
		if (ah.turn[i].ID == id){
			aggro += 0.5 * AnalyzeAction(ah.turn[i].player_action);
		}
	}
	size = ah.river.size();
	for (i = 0; i < size; i++) {
		if (ah.river[i].ID == id){
			aggro += 0.5 * AnalyzeAction(ah.river[i].player_action);
		}
	}
	return aggro;
}

static float GetSPR(GameState game_state, int id) {
	return (float)game_state.stack_size[id] / (float)game_state.total_pot_size;
}

static void SetCall(Action &act, LegalActions legal_act) {
	act.action = 1;
	act.amount = legal_act.LegalCall.amount;
}

static void SetRaise(Action &act, int amount, LegalActions legal_act) {
	act.action = 2;
	int max = legal_act.LegalMaxRaise.amount;
	act.amount = (amount > max ? max : amount);
}

static void SetFold(Action &act) {
	act.action = 0;
}

static void SetCheck(Action &act) {
	act.action = 1;
	act.amount = 0;
}

float GetPotOdds(GameState game_state, LegalActions legal_actions) {
	return (float)legal_actions.LegalCall.amount / (float)game_state.total_pot_size;	
}

int IsPreflopAggressor(GameState game_state, int id) {

	return 1;
}

/* 
 * Type-A Polarized Action Strategy, call/reraise/fold
 * Categorize a hand to Strong, Medium and Weak
 * Adopt a randomized mixed strategy 
 */ 
Action PolarizedAction(float hs, float th_strong, float th_weak, float p_slow, float p_bluff,
				float r_bet, GameState game_state, LegalActions legal_actions, int type) {
	Action my_action;
	float pot_odds = GetPotOdds(game_state, legal_actions);
	float th_odds = pot_odds / (1.0 + pot_odds);			// required win rate to justify a call

	cout << "OPP SPR = " << GetSPR(game_state, 0) << endl;

	// Decide whether to bet for value	
	if (hs > th_strong) { 
		if (RandomAction(p_slow)) {
			// slow play, check/call
			if (type == 1) {
				SetCall(my_action, legal_actions);	
			} else {
				SetCheck(my_action);
			}
			cout << "[AI] Slow play for balance." << endl;
		} else {
			// bet for value
			SetRaise(my_action, r_bet * game_state.total_pot_size, legal_actions);
			cout << "[AI] Bet for value." << endl;
		}
		return my_action;
	}

	// Decide whether to bluff or not. if Opp SPR too low, then no bluff.
	if (hs < th_weak) {
		if (RandomAction(p_bluff) && GetSPR(game_state, 0) > 0.3) {
			// bluff
			SetRaise(my_action, r_bet * game_state.total_pot_size, legal_actions);
			cout << "[AI] Raise as a bluff." << endl;
			return my_action;
		}
	} 
	
	if (type == 1) { // tpye 1, call/fold
	// medium strength hand
		cout << "[AI] I am given a pot odds of " << pot_odds << endl;
		cout << "[AI] I need " << th_odds * 100 << "\% win rate to call." << endl;
		if (hs > th_odds) {
			SetCall(my_action, legal_actions);
		} else {
			SetFold(my_action);
		}
	} else {	// type == 0, can check
		SetCheck(my_action);
	}
	return my_action;
}

EhsPlayer::EhsPlayer() {
	int i, j;
	ifstream fin;
	string buff;
	vector <string> splitStrings;

	cout << "Reading weights files..." << endl;
	// Read neural network weight files
	fin = open_file("nn_weights/weight1.txt");
	for (j = 0; j < IN_WIDTH + 1; j++) {
		getline(fin, buff);
		splitStrings = split(buff, ' ');
		for (i = 0; i < L1_WIDTH; i++) {
			M[j][i] = stof(splitStrings[i]);
		}
	}
	fin.close();
	
	fin = open_file("nn_weights/weight2.txt");
	for (j = 0; j < L1_WIDTH + 1; j++) {
		getline(fin, buff);
		splitStrings = split(buff, ' ');
		for (i = 0; i < L2_WIDTH; i++) {
			N[j][i] = stof(splitStrings[i]);
		}
	}
	fin.close();

	fin = open_file("nn_weights/weight3.txt");
	for (i = 0; i < L2_WIDTH + 1; i++) {
		getline(fin, buff);
		O[i] = stof(buff);
	}
	fin.close();
	
	for (i = 0; i < L2_WIDTH + 1; i++) {
		cout << O[i] << " ";
	}
	cout << endl;
	for (i = 0; i < L2_WIDTH; i++) {
		cout << N[L1_WIDTH][i] << " ";
	}
	cout << endl; 
}

// Use neural network weight matrix to predict effective hand strength
float EhsPlayer::PredictEHS(vector <Card> hand, vector <Card> board, float **range) {
	double nn_in[IN_WIDTH + 1] = {0.0};
	double nn_m[L1_WIDTH + 1] = {0.0};
	double nn_n[L2_WIDTH + 1] = {0.0};
	double nn_o = 0.0;

	// convert hand to neural network input format	
	nn_in[conv(hand[0])] = 1;
	nn_in[conv(hand[1])] = 1;
	nn_in[conv(board[0]) + 52] = 1;
	nn_in[conv(board[1]) + 52] = 1;
	nn_in[conv(board[2]) + 52] = 1;

	nn_in[IN_WIDTH] = 1.0;		// input bias term

	int i, j, k;

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 13; j++) {
			cout << nn_in[i * 13 + j] << " ";
		}
		cout << endl;
	}

	cout << "L1 results:" << endl;
	for (i = 0; i < L1_WIDTH; i++) {
		for (j = 0; j < IN_WIDTH + 1; j++) {
			nn_m[i] += nn_in[j] * M[j][i];
		}
		nn_m[i] = relu(nn_m[i]);	// activation function
		cout << nn_m[i] << " ";
	}
	nn_m[L1_WIDTH] = 1.0;	// bias term always = 1
	cout << nn_m[L1_WIDTH] << endl;

	cout << "L2 results:" << endl;
	for (i = 0; i < L2_WIDTH; i++) {
		for (j = 0; j < L1_WIDTH + 1; j++) {
			nn_n[i] += nn_m[j] * N[j][i];
		}
		nn_n[i] = relu(nn_n[i]);
		cout << nn_n[i] << " ";
	}
	nn_n[L2_WIDTH] = 1.0; // bias term always = 1
	cout << nn_n[L2_WIDTH] << endl;

	// model output
	for (i = 0; i < L2_WIDTH + 1; i++) {
		nn_o += nn_n[i] * O[i];	
	}

	return nn_o;
}

Action EhsPlayer::Act(GameState game_state, LegalActions legal_actions) {
	Action my_action;
	
	std::cout << "[AI] My cards are:" << GetHoleCards() << std::endl;
	std::cout << "[AI] I am at seat " << GetID() << std::endl;
//	std::cout << "Min raise:" << legal_actions.LegalMinRaise.amount << " ";
//	std::cout << "Max raise:" << legal_actions.LegalMaxRaise.amount;
	std::cout << std::endl;

	float IHS, EHS, PHS, UHS, NN_EHS;
	float pot_odds;
	int my_id = this->GetID();
	int opp_id = ~my_id;
	int street = game_state.current_street;
	int IsInPosition = (my_id == game_state.sb_pos ? 1 : 0);
	int num_of_actions = GetNumOfActions(game_state, street);

	vector <Card> board = game_state.community_cards;
	vector <Card> my_cards = this->GetHoleCards();

	float opp_agg = 0.0;
	float my_agg = 0.0;
	float **opp_range;

	// preflop strategy
	if (street == 0) {
		IHS = GetPreflopStrength(my_cards);
		cout << "[AI] My IHS = " << IHS << endl;
		opp_agg = GetAggroFactor(game_state, 0);
		cout << "[AI] OPP Aggression Factor = " << opp_agg << endl;
		UHS = pow(IHS, opp_agg);
		cout << "[AI] My UHS = " << UHS << endl;

		if (num_of_actions == 0) {
			// first to act
			cout << "[AI] I am first to act..." << endl;
			my_action = PolarizedAction(UHS, 0.6, 0.6, UHS - 0.6, UHS, 1.66667, game_state, legal_actions, 1);
		}

		// opponent acted once
		if (num_of_actions == 1) { 
			if (GetOppAction(game_state, street) == 1) { // opp called
				cout << "[AI] OK... you call..." << endl;
				my_action = PolarizedAction(UHS, 0.7, 0.6, UHS - 0.6, UHS, 1.5, game_state, legal_actions, 0);
			} else {					// opp raised
				cout << "[AI] You raised..." << endl;
				my_action = PolarizedAction(UHS, 0.81, 0.81, UHS - 0.7, UHS - 0.5, 2.2, game_state, legal_actions, 1);
			}
		}

		if (num_of_actions >= 2) {
			// opponent raised your limp or reraised you
			cout << "[AI] You raised me!" << endl;
			if (GetMyAction(game_state, street) == 1) {	// I checked. Check raise with strongest hands, bluff < 20%
				my_action = PolarizedAction(UHS, 0.85, 0.8, UHS - 0.7, UHS - 0.5, 1.75, game_state, legal_actions, 1);
			} else {	// I am reraised. Reraise with the strongest hand, slowplay <20%, bluff < 30%
				my_action = PolarizedAction(UHS, 0.95, 0.9, UHS - 0.7, UHS - 0.7, 1.75, game_state, legal_actions, 1);
			}
		}
	}	

	// flop = 1; turn = 2; river = 3
	if (street > 0 && street <= 3) {
		opp_range = AssignOppRange(game_state, IsInPosition);	// estimate opponent range
		IHS = GetImmediateStrength(my_cards, board, opp_range);
		EHS = GetEffectiveStrength(my_cards, board, NULL);
		NN_EHS = PredictEHS(my_cards, board, NULL);
		PHS = EHS - IHS;
		opp_agg = GetAggroFactor(game_state, 0);
		my_agg = GetAggroFactor(game_state, my_id);
		UHS = pow(EHS, opp_agg);
		cout << "[AI] My IHS = " << IHS;
		cout << ", EHS = " << EHS;
		cout << ", NN_EHS = " << NN_EHS;
		cout << ", PHS = " << PHS << endl;
		cout << "[AI] OPP Aggression Factor = " << opp_agg << endl;
		cout << "[AI] My Aggression Factor = " << my_agg << endl;
		cout << "[AI] My UHS = " << UHS << endl;

		if (num_of_actions == 0) {
			// first to act on this street 
			cout << "[AI] I will act first." << endl;	
			// the lower the hand potential, the higher frequency of betting; the higher the potential, the more chance of bluffing
			if (street == 1) { // flop
				my_action = PolarizedAction(UHS, 0.6, 0.5, UHS - 0.4 + 3 * PHS, UHS - 0.1 + 2 * PHS, 0.5, game_state, legal_actions, 0);
			} else if (street == 2) {	// turn
				my_action = PolarizedAction(UHS, 0.6, 0.4, UHS - 0.4 + 2 * PHS, UHS - 0.1 + 2 * PHS, 0.5, game_state, legal_actions, 0);
			} else {	// river
				my_action = PolarizedAction(UHS, 0.6, 0.3, UHS - 0.7, 0.3 - UHS, 0.6666667, game_state, legal_actions, 0);
			}
		} 

		if (num_of_actions == 1) {
			// opponent acted once 
			if (GetOppAction(game_state, street) == 1) {		// opp checks 
				cout << "[AI] OK... you checked..." << endl;
				if (street == 1) {	// flop, 
					my_action = PolarizedAction(UHS, 0.6, 0.5, UHS - 0.4 + 3 * PHS, UHS + 2 * PHS, 0.5, game_state, legal_actions, 0);
				} else if (street == 2) { // turn, slow play strong hands > 10%, bluff = 2 * PHS
					my_action = PolarizedAction(UHS, 0.6, 0.4, UHS - 0.4 + 2 * PHS, UHS - 0.1 + 2 * PHS, 0.5, game_state, legal_actions, 0);
				} else {		// river, always bet with strongest hand, bluff < 30%
					my_action = PolarizedAction(UHS, 0.6, 0.3, 0, 0.3 - UHS, 0.6666667, game_state, legal_actions, 0); 
				}
			} else {					// opp raises
				cout << "[AI] You raised..." << endl;
				if (street == 1) {	// flop 
					my_action = PolarizedAction(UHS, 0.7, 0.5, UHS - 0.5 + 3 * PHS, UHS - 0.3 + PHS, 1.5, game_state, legal_actions, 1);
				} else if (street <= 2) {	// turn, slow play strong hands > 10%, bluff = PHS
					my_action = PolarizedAction(UHS, 0.65, 0.4, UHS - 0.5 + 2 * PHS, UHS - 0.2 + PHS, 1.5, game_state, legal_actions, 1); 
				} else {	// river, always raise with strongest hand, bluff < 20%
					my_action = PolarizedAction(UHS, 0.6, 0.3, 0, 0.2 - UHS, 1.5, game_state, legal_actions, 1); // river
				}
			}
		}

		if (num_of_actions >= 2) {
			// opponent raised your check or reraised you
			cout << "[AI] You raised me!" << endl;
			if (GetMyAction(game_state, street) == 1) {	// I checked. Check raise some% with strongest hands, bluff = 3 * PHS
				my_action = PolarizedAction(UHS, 0.6, 0.3, UHS - 0.4 + 3 * PHS, UHS - 0.3 + PHS, 1.5, game_state, legal_actions, 1);
			} else {	// I got reraised
				if (street == 1) {	// flop 
					my_action = PolarizedAction(UHS, 0.6, 0.4, UHS - 0.4 + 3 * PHS, UHS - 0.4 + PHS, 1.5, game_state, legal_actions, 1);
				} else if (street == 2) {	// flop or turn, slow play strong hands > 0%, bluff = PHS 
					my_action = PolarizedAction(UHS, 0.6, 0.4, UHS - 0.4 + 3 * PHS, UHS - 0.35 + PHS, 1.5, game_state, legal_actions, 1); // flop, turn
				} else {		// river, always raise with strongest hand, bluff = 5%
					my_action = PolarizedAction(UHS, 0.6, 0.3, 0, 0, 1.5, game_state, legal_actions, 1);	// flop, turn
				}	
			}
		}
		DeleteRangeTable(opp_range);
	}

	return my_action;
}
