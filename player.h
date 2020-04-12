/**************************************************** 
 * Filename: player.h
 * Last modified: 2020/04/01
 * Description: Blabla
 ***************************************************/

#pragma once

#include <vector>
#include <string>
#include "card.h"
#include "game_state.h"

#define STATUS_NO_PLAYER	0
#define STATUS_IN_GAME		1
#define STATUS_FOLDED		2
#define STATUS_SIT_OUT		3
#define STATUS_ALL_IN       4

#define PLAYER_HUMAN		0
#define PLAYER_BOT			1

using namespace std;

class Player {
public:
	Player();//default name, stack_size and default type(human)
	Player(int player_type, string name);//default name, stack_size
  	~Player();

	virtual Action Act(GameState, LegalActions);
	
  	int Bet(int bet_amount);
  	int Call(int call_amount);
  	int Fold();
  	int AddToStack(int add_amount);

   	int GetType();
	void SetType(int type);

  	int GetStatus();
	void SetStatus(int status);
	
	void SetID(int id);
	int GetID();

  	void ResetHoleCards(); //throw away hole card
	void SetHoleCards(const Card & cards);	
	vector<Card> GetHoleCards();
  
private:
  	vector<Card> hole_cards;
/*consider remove*/  	int stack_size_;   //Reason: read from game_state instead
  	int player_type_; 	//various types of AI, or human(0)
  	int status_;		// See #define at top //read from game_state instead 
  	string name_; 		//player name, may need to be unique 
    int player_id_; //need this variable to know who i am in the game state
};
