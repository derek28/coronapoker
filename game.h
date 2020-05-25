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
#include "pokerhand.h"
#include <cmath>
#include <utility>
#include <iostream>
#include "misc.h"

#include <sys/ipc.h>
#include <sys/shm.h> 

#define NUM_OF_PLAYERS	2

class Game {
public:
	Game();
	~Game();
    int AddPlayer(int seat, int stack_size, int type);
    int AddPlayer(int seat, int stack_size, Player* player);
    void RemovePlayer(int seat);


    void ShuffleAndDeal(); 							//Set everyplayer to be in hand by populate deque, push UTG first 
	void PostBlinds();							// Set Small Blinds / Bid Blinds
    void ComputeBlindPos();
    
    void MoveBtn();
    int FindNextPlayer(int i);                  // Input seat index i (range 0 to 8), return index of next seated player
    ActionWithID AskPlayerToAct(LegalActions ac);   // All the players make action until IsEndOfStreet(). 
    vector<int> GetWinner(); 					// Check hand strength of players and return position ID of winner(s)
    void PayWinner(vector<int> winners);        // Move money from pot to winner(s)'s stack

    bool IsCurrentHandFinished();
    bool IsPotUncontested();
    bool IsPlayerAllIn(int i);
    bool HasReachShowdown();
    bool HasNoMoreActions();
    bool IsEndOfStreet(); 						//check if this betting round is done. maybe this function is not necessary;
    void CollectMoneyFromBetRing();             //Collect money from bet ring and put into middle
    bool IsBetInvalid(); //if betsize is invalid (too small), throw warning, make player fold
    //tentatively, by default, there is iterator it_end which points to SB for postflop or UTG for preflop. When it==it_end, action closes
    //However, when someone bets, assign it_end = it
    // when someone folds, mydeque.erase (it); //what happens to itself?? Idk..
    void SetupNextStreet();
    void RemovePlayerCard();
    void CleanCommunityCard();
    ActionWithID VerifyAction(ActionWithID ac, LegalActions legal_action);
    LegalActions GetAllLegalActions();
    void UpdateGameState(ActionWithID ac);
    void ResetGameState();
    void PrintGameState();  //may consider add a parameter to redirect output, append mode
    void PrintGameStateDebug();
    void PrintResult();
    void Start();
    void SetNumOfHands(int i);
private:
    //int big_blind_ = 100;
    //int small_blind_ = 50;
    //int btn_pos_ = 0; //obsolete
    //int sb_pos_ = 0; //obsolete
    //int bb_pos_ = 0; //obsolete
	//int pot_size_ = 0;
	//int aggressor_;	 // When someone bets/raises, he opens action. All other ppl needs to call/fold/all-in in order to go to next round.
                         // This counter points to the player who is the aggressor
	Player *players[NUM_OF_PLAYERS] ; //the index is thus player ID 
	Deck deck;
    GameState game_state_ ;
    // int *game_state_shm ;
    int num_of_hands_to_run_;
};
