/**************************************************** 
 * Filename: player.h
 * Last modified: 2020/04/01
 * Description: Blabla
 ***************************************************/

#pragma once
#include <vector>
#include <string>
#include "card.h"

class Player {
public:
  Player();//default name, stack_size and default type(human)
  Player(int player_type);//default name, stack_size
  
  void MakeABet(int bet_amount);
  void MakeACall();
  void MakeAFold();
  
  void AddToStack(int won_amount);
  void ResetHoleCard(); //throw away hole card
  
private:
  list<Card> hole_cards;
  int stack_size_;
  int player_type_; //various types of AI, or human(0)
  string name_; //player name, may need to be unique 
};