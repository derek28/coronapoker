/**************************************************** 
 * Filename: game.h
 * Last modified: 2020/04/01
 * Description: Blabla
 * Note: needs to consider cases where there are 2 winners(split)
 ***************************************************/

#pragma once

#include "player.h"
#include "deck.h"
#include "game_state.h"
#include <utility>
#include <iostream>

#define DEBUG 

#define NUM_OF_PLAYERS	2

class Game {
public:
	Game();
	~Game();
    int AddPlayer(int seat, int stack_size, int type);
    void RemovePlayer(int seat);
    void StartAHand(); 							//Set everyplayer to be in hand by populate deque, push UTG first 
	void PostBlinds();							// Set Small Blinds / Bid Blinds
    void ComputeBlindPos();
    int FindNextPlayer(int i);                  // Input seat index i (range 0 to 8), return index of next seated player
    int PlayerAction(int seat);					// All the players make action until IsEndOfStreet(). 
	int Flop();									// return 1 if need to proceed 
	int Turn();									// return 1 if need to proceed 
	int River();								// return 1 if need to showdown
    vector<int> GetWinner(); 					//check hand strength of players and return position ID of winner(s)
    bool IsEndOfStreet(); 						//check if this betting round is done. maybe this function is not necessary;
    bool IsBetInvalid(); //if betsize is invalid (too small), throw warning, make player fold
    //tentatively, by default, there is iterator it_end which points to SB for postflop or UTG for preflop. When it==it_end, action closes
    //However, when someone bets, assign it_end = it
    // when someone folds, mydeque.erase (it); //what happens to itself?? Idk..
    void PrintGameState();  //may consider add a parameter to redirect output, append mode
private:
    int big_blind_ = 100;
    int small_blind_ = 50;
    int btn_pos_ = 0;
    int sb_pos_ = 0;
    int bb_pos_ = 0;
	int pot_size_ = 0;
	int it;							// # of player in action 
	Player players[NUM_OF_PLAYERS] ; //the index is thus player ID 
	Deck deck;
    GameState game_state_ ;
};
