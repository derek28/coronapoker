/**************************************************** 
 * Filename: game_state.h
 * Last modified: 2020/04/01
 * Description: Define game_state variable, which is visible to everyone
 *              Thus, hole card of players are not in the game_state
 ***************************************************/
 
#pragma once
#include "card.h"
#include <vector>
 
 //Variable "action" is passed from Player to Game when player makes a move
 struct Action {
     int action = 0; //fold=0, call=1, bet=2
     int amount = 0; 
 };
 
 struct ActionWithID {
     int ID = 0;
     Action player_action;
 };
 
struct LegalActions {
    Action LegalCall;
    Action LegalFold;
    Action LegalRaise;
};

struct ActionHistory {
    std::vector<ActionWithID> preflop;
    std::vector<ActionWithID> flop;
    std::vector<ActionWithID> turn;
    std::vector<ActionWithID> river;
};

 struct GameState {
     
     //Info below are defined when game starts
     int num_player = 0;

     //Info below are updated per game, when game starts
     int starting_stack_size[9] = {};
     int btn_pos = 0; // index of seat (0 to 8 for 9-max)
     int sb_pos = 0;
     int bb_pos = 0;
     int sb_amount = 50;
     int bb_amount = 100;

     //Info below are updated per betting street during the game
     std::vector<Card> community_cards;
     int pot_size = 0;
     //Info below are updated per action during game
     int current_street = 0; //0=preflop, 1=flop, 2=turn 3=river, 4=showdown
     int num_player_in_hand = 0;
     int stack_size[9] = {};
     int next_player_to_act = 0;
     int aggressor = 0;
     int player_status[9] = {}; //0: no player 1: in game 2:folded
     int bet_ring[9] = {}; //amount of money placed on the bet ring (in front of player, no in the pot), they gets collected to pot when the street ends
     int allin_state[9] = {};
     int raise_amount = 0; // raise size - previous raise size
     int bankroll[9] = {};
     struct ActionHistory;
     
     //Info below are updated at the end of the game
     std::vector<Card> player_hole_cards[9];

/*To be added*/ //To add range for each player.
     
     
     
     void print() { //can take parameter and redirect to a file later
         std::cout << "*************game_state***************" << std::endl;
         std::cout << "num_player (on the table):" << num_player << std::endl;
         std::cout << "btn_pos:" << btn_pos << ",sb_pos:" << sb_pos << ",bb_pos:" << bb_pos << std::endl;
         std::cout << "bankroll:";
         for (int i = 0; i < 9; i++)
		     std::cout << bankroll[i] << ",";
         std::cout << std::endl;
         std::cout << "*********" << std::endl;
         std::cout << "Number of players in hand: " << num_player_in_hand << std::endl;
         std::cout << "Stack sizes: ";
         for (int i=0; i < 9; i++)
            std::cout << stack_size[i] << "," ;
         std::cout << std::endl;
         std::cout << "pot_size:" << pot_size << std::endl;
         std::cout << "current_street:" << current_street << std::endl;
         std::cout << "community_cards:";
         for (auto &i: community_cards)
		     std::cout << i << " ";
         std::cout << std::endl;
         std::cout << "aggressor:" << aggressor << std::endl;
         std::cout << "next_player_to_act:" << next_player_to_act << std::endl;
         std::cout << "player_status:";
         for (int i = 0; i < 9; i++)
		     std::cout << player_status[i] << ",";
         std::cout << std::endl;
         std::cout << "bet_amount:";
         for (int i = 0; i < 9; i++)
		     std::cout << bet_ring[i] << ",";
         std::cout << std::endl;
//To be written         //std::cout << ActionHistory ;
         std::cout << "**************************************" << std::endl;
     } 
 };