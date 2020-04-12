/****************************
 * Filename: player.cpp
 * Last modified: 2020/04/02
 * Description: implementaion of class player methods
 * *****************************/

#include <iostream>
#include <string>
#include <vector>
#include "player.h"
#include <algorithm>

using namespace std;

Player::Player() {
	stack_size_ = 0;
	player_type_ = PLAYER_BOT;
	status_ = STATUS_NO_PLAYER;
	name_ = "Noname";
}

Player::Player(int player_type, string name) {
	stack_size_ = 0;
	if (player_type < 0 || player_type > 2) {
		cerr << "Player type error!" << endl;
		cerr << "Set to PLAYER_BOT" << endl;
		player_type_ = PLAYER_BOT;
	} else {
		player_type_ = player_type;
	}
	status_ = STATUS_NO_PLAYER;
	name_ = name;
}

Player::~Player() {}


Action Player::Act(GameState game_state, LegalActions legal_actions){
    Action action_to_return;

//  Always fold
//	action_to_return.action = 0 ; //0 = fold 
//	action_to_return.amount = 0 ;

//  Always call
//	action_to_return.action = 1 ; //1 = call
//	action_to_return.amount = legal_actions.LegalCall.amount;

	return action_to_return;
}

int Player::Bet(int bet_amount) {
	if (status_ != STATUS_IN_GAME) {
		cerr << "Cannot bet if not in game!" << endl;	
		return -1;
	}
	
	if (bet_amount <= 0) {
		cerr << "Invalid bet_amount!" << endl;
		return -2;
	}

	if (stack_size_ < bet_amount) {
		cerr << "Not enough chip for this bet!" << endl;
		return -3;
	}
	
	stack_size_ -= bet_amount;
	return 0;
}

int Player::Call(int call_amount) {
	this->Bet(call_amount);
}

int Player::Fold() {
	if (status_ = STATUS_IN_GAME) {
		status_ = STATUS_FOLDED;
		return 0;
	} else {
		cerr << "Player cannot fold if not playing!" << endl;
		return -1;
	}
}

int Player::GetStatus() {
	return status_;
}

void Player::SetStatus(int status) {
	if (status < 0 || status > 3) {
		cerr << "SetStatus() failure: invalid status!" << endl;	
		return;
	}
	status_ = status;
}

void Player::SetType(int type) {
    if (0){
        //maybe should look in a dictionary for available types
    }
    player_type_ = type;
}

int Player::AddToStack(int add_amount) {
	if (add_amount < 0) {
		cerr << "AddToStack(): Invalid add_amount < 0" << endl;
		return -1;
	}
	stack_size_ += add_amount;
	return 0;
}

void Player::ResetHoleCards() {
	hole_cards.clear();
}

void Player::SetHoleCards(const Card & cards) {
	hole_cards.push_back(cards);
}

vector<Card> Player::GetHoleCards() {
	return hole_cards;
}

void Player::SetID(int id) {
	player_id_ = id;
}

int Player::GetID() {
	return player_id_;
}