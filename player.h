/**************************************************** 
 * Filename: player.h
 * Last modified: 2020/04/01
 * Description: Blabla
 ***************************************************/

#pragma once

#include <vector>
#include <string>
#include "card.h"

#define STATUS_NO_PLAYER	0
#define STATUS_IN_GAME		1
#define STATUS_FOLDED		2
#define STATUS_SIT_OUT		3

#define PLAYER_HUMAN		0
#define PLAYER_BOT			1

using namespace std;

class Player {
public:
	Player();//default name, stack_size and default type(human)
	Player(int player_type, string name);//default name, stack_size
  	~Player();

	int Action();
	
  	int Bet(int bet_amount);
  	int Call(int call_amount);
  	int Fold();
  	int AddToStack(int add_amount);

   	int GetType();
	void SetType(int type);

  	int GetStatus();
	void SetStatus(int status);
  
  	//void ResetHoleCards(); //throw away hole card
	void SetHoleCards(Card *cards);	
  
private:
  	Card hole_cards[2];
  	int stack_size_;
  	int player_type_; 	//various types of AI, or human(0)
  	int status_;		// See #define at top 
  	string name_; 		//player name, may need to be unique 
};
