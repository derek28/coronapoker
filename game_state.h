/**************************************************** 
 * Filename: game_state.h
 * Last modified: 2020/04/01
 * Description: Define game_state variable, which is visible to everyone
 *              Thus, hole card of players are not in the game_state
 ***************************************************/
 
#pragma once
#include "card.h"
#include <vector>
#include "misc.h"
 //Variable "action" is passed from Player to Game when player makes a move
 struct Action {
     int action = 0; //fold=0, call=1, bet=2
     int amount = 0; 

     friend std::ostream& operator<<(std::ostream& os, Action action)
    {
        switch(action.action)
        {
            case 0         : os << "folds";    break;
            case 1         :
                if (action.amount == 0) 
                    os << "checks ";
                else
                    os << "calls " << action.amount; 
                break;
            case 2         : 
                os << "raises " << action.amount; 
                break;
            default        : os << "[ERROR] Unknown Action" ;
        }
        return os;
    }

 };

struct ActionWithID {
     int ID = 0;
     Action player_action;

     friend std::ostream& operator<<(std::ostream& os, ActionWithID player_action)
    {
        os << "Player " << player_action.ID << " " << player_action.player_action << std::endl;
        return os;
    }
};
 
struct LegalActions {
    Action LegalCall;
    Action LegalFold;
    Action LegalMinRaise;
    Action LegalMaxRaise;
};
struct LegalActionsSimplify {
    Action legal_action[4];
    
    LegalActionsSimplify(LegalActions la){
        legal_action[0].amount=la.LegalFold.amount;
        legal_action[1].amount=la.LegalCall.amount;
        legal_action[2].amount=la.LegalMinRaise.amount;
        legal_action[3].amount=la.LegalMaxRaise.amount;
    }

};

struct ActionHistory {
    std::vector<ActionWithID> preflop;
    std::vector<ActionWithID> flop;
    std::vector<ActionWithID> turn;
    std::vector<ActionWithID> river;

    friend std::ostream& operator<<(std::ostream& os, ActionHistory ac_his)
    {
        os << "************PREFLOP**********" << std::endl;
        os << ac_his.preflop;
        os << "************FLOP*************" << std::endl;
        os << ac_his.flop;
        os << "************TURN*************" << std::endl;
        os << ac_his.turn;
        os << "************RIVER************" << std::endl;
        os << ac_his.river;
        return os;
    }

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
     int hand_number = 0;
     double nb_of_buyins[9] = {};
     //Info below are updated per betting street during the game
     std::vector<Card> community_cards;
     int pot_size = 0;
     int total_pot_size = 0;
     //Info below are updated per action during game
     int current_street = 0; //0=preflop, 1=flop, 2=turn 3=river, 4=showdown, 5=finished
     int num_player_in_hand = 0;
     int stack_size[9] = {};
     int next_player_to_act = 0;
     int aggressor = 0;
     int player_status[9] = {}; //0: no player 1: in game 2:folded
     int bet_ring[9] = {}; //amount of money placed on the bet ring (in front of player, no in the pot), they gets collected to pot when the street ends
     int allin_state[9] = {};
     int raise_amount = 0; // raise size - previous raise size
     ActionHistory action_history;
     
     //Info below are updated at the end of the game
     std::vector<Card> player_hole_cards[9];

/*To be added*/ //To add range for each player.


     //struct constructor
     GameState(){
         community_cards.reserve(5);
         for (int i=0; i<9; i++)
            player_hole_cards[i].reserve(2);
     }

     void print() { //can take parameter and redirect to a file later
         std::cout << "**************************************" << std::endl;
         std::cout << "Hand#" << hand_number << std::endl;
         //std::cout << "num_player (on the table):" << num_player << std::endl;
         std::cout << "btn_pos:" << btn_pos << ",sb_pos:" << sb_pos << ",bb_pos:" << bb_pos << std::endl;
         std::cout << "nb_of_buyins_at_the_end_of_game:";
         for (int i = 0; i < 9; i++)
		     std::cout << nb_of_buyins[i] << ",";
         std::cout << std::endl;
         std::cout << "pot_size:" << pot_size << std::endl;
         std::cout << "community_cards:" << community_cards << std::endl;
         std::cout << action_history;
         std::cout << "**************************************" << std::endl;
     } 
        
     void printdebug() { //can take parameter and redirect to a file later
         std::cout << "*************game_state***************" << std::endl;
         std::cout << "num_player (on the table):" << num_player << std::endl;
         std::cout << "btn_pos:" << btn_pos << ",sb_pos:" << sb_pos << ",bb_pos:" << bb_pos << std::endl;
         std::cout << "nb_of_buyins:";
         for (int i = 0; i < 9; i++)
		     std::cout << nb_of_buyins[i] << ",";
         std::cout << std::endl;
         std::cout << "*********" << std::endl;
         std::cout << "Number of players in hand: " << num_player_in_hand << std::endl;
         std::cout << "Starting stack sizes: " ;
         for (int i=0; i < 9; i++)
            std::cout << starting_stack_size[i] << "," ;
         std::cout << std::endl;         
         std::cout << "Stack sizes: ";
         for (int i=0; i < 9; i++)
            std::cout << stack_size[i] << "," ;
         std::cout << std::endl;
         std::cout << "pot_size:" << pot_size << std::endl;
         std::cout << "total_pot_size:" << total_pot_size << std::endl;
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

 struct GameStateNoVector {
     
     //Info below are defined when game starts
     int num_player = 0;

     //Info below are updated per game, when game starts
     int starting_stack_size[9] = {};
     int btn_pos = 0; // index of seat (0 to 8 for 9-max)
     int sb_pos = 0;
     int bb_pos = 0;
     int sb_amount = 50;
     int bb_amount = 100;
     int hand_number = 0;
     double nb_of_buyins[9] = {};
     //Info below are updated per betting street during the game
     Card community_cards[5];
     int community_size=0;
     int pot_size = 0;
     int total_pot_size = 0;
     //Info below are updated per action during game
     int current_street = 0; //0=preflop, 1=flop, 2=turn 3=river, 4=showdown, 5=finished
     int num_player_in_hand = 0;
     int stack_size[9] = {};
     int next_player_to_act = 0;
     int aggressor = 0;
     int player_status[9] = {}; //0: no player 1: in game 2:folded
     int bet_ring[9] = {}; //amount of money placed on the bet ring (in front of player, no in the pot), they gets collected to pot when the street ends
     int allin_state[9] = {};
     int raise_amount = 0; // raise size - previous raise size
     //ActionHistory action_history;
     
     //Info below are updated at the end of the game
     Card player_hole_cards[9][2];

/*To be added*/ //To add range for each player.


     //struct copy from GameState constructor
     GameStateNoVector(GameState gamestate){
        num_player = gamestate.num_player;
        btn_pos = gamestate.btn_pos;
        sb_pos = gamestate.sb_pos;
        bb_pos = gamestate.bb_pos;
        sb_amount = gamestate.sb_amount;
        bb_amount = gamestate.bb_amount;
        hand_number = gamestate.hand_number;
        pot_size = gamestate.pot_size;
        total_pot_size = gamestate.total_pot_size;
        current_street = gamestate.current_street;
        num_player_in_hand = gamestate.num_player_in_hand;
        next_player_to_act = gamestate.next_player_to_act;
        aggressor = gamestate.aggressor;
        raise_amount = gamestate.raise_amount;
        for (int i=0; i<2; i++){
            player_hole_cards[i][0]=gamestate.player_hole_cards[i].at(0);
            player_hole_cards[i][1]=gamestate.player_hole_cards[i].at(1);
            starting_stack_size[i]=gamestate.starting_stack_size[i];
            nb_of_buyins[i]=gamestate.nb_of_buyins[i];
            stack_size[i]=gamestate.stack_size[i];
            player_status[i]=gamestate.player_status[i];
            bet_ring[i]=gamestate.bet_ring[i];
            allin_state[i]=gamestate.allin_state[i];
        }
        community_size = gamestate.community_cards.size();
        for (int i=0; i<community_size; i++){
            community_cards[i]=gamestate.community_cards[i];
        }
     }
 };