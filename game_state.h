/**************************************************** 
 * Filename: game_state.h
 * Last modified: 2020/04/01
 * Description: Define game_state variable, which is visible to everyone
 *              Thus, hole card of players are not in the game_state
 ***************************************************/
 
 #pragma once
#include "card.h"
#include<vector>
 
 //Variable "action" is passed from Player to Game when player makes a move
 struct Action {
     int action = 0; //fold=0, call=1, bet=2
     int amount = 0; 
 };
 
 struct ActionWithID {
     int ID = 0;
     struct Action;
 };
 
struct ActionHistory {
    vector<ActionWithID> preflop;
    vector<ActionWithID> flop;
    vector<ActionWithID> turn;
    vector<ActionWithID> river;
};

 struct GameState {
     
     //Info below are defined when game starts
     int num_player = 0;

     //Info below are updated per game
     int stack_size[9] = {};

     int btn_pos_ = 0; // index of seat (0 to 8 for 9-max)
     
     //Info below are updated per action during game
     int current_street = 0; //0=preflop, 1=flop, 2=turn 3=river
     int pot_size = 0;
     vector<Card> community_cards;
     int player_status[9] = {}; //0: no player 1: in game 2:folded
     int bet_amount[9] = {}; //amount of money in front of player, they gets collected to pot when the street ends
     struct ActionHistory;
     
/*To be added*/ //To add range for each player.
     
     
     
     void print(){ //can take parameter and redirect to a file later
         std::cout << "*************game_state***************" << std::endl;
         std::cout << "num_player:" << num_player << std::endl;
         std::cout << "btn_pos_:" << btn_pos_ << std::endl;
         std::cout << "stack_size:";
         for (int i = 0; i < 9; i++)
		     std::cout << stack_size[i] << ",";
         std::cout << std::endl;
         std::cout << "pot_size:" << pot_size << std::endl;
         std::cout << "current_street:" << current_street << std::endl;
         std::cout << "community_cards:";
         for (auto &i: community_cards)
		     cout << i << " ";
         std::cout << std::endl;
         std::cout << "player_status:";
         for (int i = 0; i < 9; i++)
		     std::cout << player_status[i] << ",";
         std::cout << std::endl;
         std::cout << "bet_amount:";
         for (int i = 0; i < 9; i++)
		     std::cout << bet_amount[i] << ",";
         std::cout << std::endl;
//To be written         //std::cout << ActionHistory ;
         std::cout << "**************************************" << std::endl;
     } 
 };