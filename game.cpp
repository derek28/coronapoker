/*********************************** * Filename: game.cpp * Last Modified: 2020/04/02
 * **********************************/


#include "game.h"
#include <algorithm> //std::copy for array copying, std::max_element

using namespace std;

Game::Game() {

}

Game::~Game() {

}

int Game::AddPlayer(int seat, int stack_size, int type) {
	if (players[seat].GetStatus() != STATUS_NO_PLAYER) {
		cerr << "Player already sitting at seat #" << seat << endl;
		cerr << "AddPlayer() failure" << endl;
		return -1;
    }	
	players[seat].SetType(type);
    players[seat].SetStatus(1); //#define STATUS_IN_GAME		1
	players[seat].AddToStack(stack_size);
    players[seat].SetID(seat);
	
    //Update game_state_
    game_state_.num_player++;
    game_state_.stack_size[seat] = stack_size;
    game_state_.player_status[seat] = 1; //1=in-game
	return 0;
}

void Game::RemovePlayer(int seat) {
	players[seat].SetStatus(STATUS_NO_PLAYER);	
}

void Game::StartAHand() {

	Card dealt_cards[2];
	deck.shuffle();
    #ifdef DEBUG
        std::cout << "[Debug] Starting a hand" << std::endl;
        std::cout << "[Debug] Deck is:";
        deck.print();
    #endif
	for (int i = 0; i < NUM_OF_PLAYERS; i++) {
		dealt_cards[0] = deck.deal();
		dealt_cards[1] = deck.deal();
		players[i].SetHoleCards(dealt_cards);

        #ifdef DEBUG
        std::cout << "[Debug] Hole card of Player Seat" << i << ":";
        std::cout << dealt_cards[0] << dealt_cards[1] << std::endl;
        #endif
	}
}

void Game::PostBlinds() {
    #ifdef DEBUG
    std::cout << "[Debug] Posting Blinds";
    #endif
    ComputeBlindPos();
    players[sb_pos_].Bet(game_state_.sb_amount);
    players[bb_pos_].Bet(game_state_.bb_amount);
    
    //update game state
    std::copy(game_state_.stack_size, game_state_.stack_size+9, game_state_.starting_stack_size);
    game_state_.bb_pos = bb_pos_;
    game_state_.sb_pos = sb_pos_;
    game_state_.stack_size[game_state_.sb_pos] -= game_state_.sb_amount;
    game_state_.stack_size[game_state_.bb_pos] -= game_state_.bb_amount;
    game_state_.bet_ring[game_state_.sb_pos] = game_state_.sb_amount;
    game_state_.bet_ring[game_state_.bb_pos] = game_state_.bb_amount;
    game_state_.pot_size = 0;
    game_state_.next_player_to_act = FindNextPlayer(bb_pos_);
    game_state_.aggressor = FindNextPlayer(bb_pos_); //last to act is BB
    game_state_.raise_amount = game_state_.bb_amount;
    game_state_.num_player_in_hand = game_state_.num_player;
}

ActionWithID Game::AskPlayerToAct() {

    ActionWithID player_action_with_id;
    player_action_with_id.ID = game_state_.next_player_to_act;
    player_action_with_id.player_action = players[game_state_.next_player_to_act].Act(game_state_);
	#ifdef DEBUG
		std::cout << "[DEBUG] Player " << player_action_with_id.ID << " takes action: " \
                    << player_action_with_id.player_action.action << "; Amount: " \
                    << player_action_with_id.player_action.amount << std::endl;
	#endif
    return player_action_with_id;
}

void Game::PrintGameState() {
    game_state_.print();
}

void Game::ComputeBlindPos() {
    if (game_state_.num_player < 2)
        std:cerr << "Player number is incorrect:" << game_state_.num_player << std::endl;
    

    sb_pos_ = FindNextPlayer(btn_pos_);
    bb_pos_ = FindNextPlayer(sb_pos_);

    if (game_state_.num_player == 2) {
        std::swap(sb_pos_, bb_pos_);
    }
    
    #ifdef DEBUG
    std::cout << "[Debug] sb_pos:" << sb_pos_ << ",bb_pos:" << bb_pos_ << std::endl;
    #endif
}

int Game::FindNextPlayer(int i) {
    i++;
    while ( game_state_.player_status[i%9] != 1)
      i++;
    return i%9;
}

void Game::MoveBtn(){
    btn_pos_ = FindNextPlayer(btn_pos_);
}


bool Game::IsPotUncontested() {
    if (game_state_.num_player_in_hand >= 2) {
        return 0;
    }
    else {
        return 1;
    }
}


bool Game::HasReachShowdown() {
    if (game_state_.current_street == 4)
        return 1;
    else
        return 0;
}

vector<int> Game::GetWinner(){
    vector<int> winner;
    if(game_state_.num_player_in_hand == 1){
        for (int i = 0 ; i < 9 ; i++){
            if (game_state_.player_status[i] == 1) { // 1: in game, 2: folded
                winner.push_back(i);
            }
        }
    }
    else{
        std::cerr << "[ERROR] GetWinner for showdown is not available";
        /*winner.push_back(0);
        int winning_strength = 0;
        for (int i = 0 ; i < 9 ; i++) {
            if(game_state_.player_status[i] == 1) { //1: in game
                PokerHand current_hand; 
                current_hand.add( game_state_.player_hole_cards[i][0]);
                current_hand.add( game_state_.player_hole_cards[i][1]);
                current_hand.add( game_state_.community_cards[0]);
                current_hand.add( game_state_.community_cards[1]);
                current_hand.add( game_state_.community_cards[2]);
                current_hand.add( game_state_.community_cards[3]);
                current_hand.add( game_state_.community_cards[4]);

                vector<int> current_strength = current_hand.GetStrength();

                #ifdef DEBUG
                std::cout<< "[DEBUG] Player" << i << ":" ;
                current_hand.print();
                #endif
                int current_hand_strength = 0;
                for (int i = 0 ; i < 6 ; i++) {
                    current_hand_strength += pow(15,5-i) *current_strength[i];
                }

                if(winning_strength < current_hand_strength) {
                    winner.clear();
                    winner.push_back(i);
                    winning_strength=current_hand_strength;
                }
                if(winning_strength == current_hand_strength) {
                    winner.push_back(i);
                }
            } 
        }*/ 
    }
    #ifdef DEBUG
        std::cout<< "[DEBUG] Found winner: " << winner[0] << ". Nb of winners: " << winner.size() << std::endl ;
    #endif
    return winner;
}


void Game::PayWinner(vector<int> winners){
    
    for(auto const& i: winners) {
        #ifdef DEBUG
        std::cout << "[DEBUG] Paying Player " << i << std::endl ;
        #endif
        game_state_.stack_size[i] += game_state_.pot_size/winners.size();
    }

}

bool Game::IsEndOfStreet() {
    return game_state_.next_player_to_act == game_state_.aggressor ;
}

void Game::CollectMoneyFromBetRing() {
    #ifdef DEBUG
    std::cout << "[DEBUG] Dealer collects money from bet ring and put into pot" << std::endl ;
    #endif

    for (int i = 0 ; i < 9 ; i++ ) {
        if (game_state_.bet_ring[i] == 0)
            continue;
        game_state_.pot_size += game_state_.bet_ring[i];
        game_state_.bet_ring[i] = 0;
    }
    game_state_.raise_amount = 0;
}


void Game::SetupNextStreet() {

    game_state_.current_street += 1;
    
    int cards_to_deal = 0 ;
    switch (game_state_.current_street) {
        case 1:
            cards_to_deal = 3;
            break;

        case 2:
        case 3:
            cards_to_deal = 1;
            break;
        case 4:
            cards_to_deal = 0;
            break;
        default:
            std::cerr << "[Error] Not supposed to be here" << std::endl;
    }

    for (int i = 0 ; i < cards_to_deal ; i++ ) {
        game_state_.community_cards.push_back(deck.deal());
    }


    game_state_.next_player_to_act = FindNextPlayer(bb_pos_);
    aggressor_ = FindNextPlayer(bb_pos_); 
}


void Game::UpdateGameState(ActionWithID ac) {

    //Correct ac if it is invalid
    int biggest_bet_amount = *std::max_element(game_state_.bet_ring,game_state_.bet_ring+9);
    if (ac.player_action.action == 1 ) {
        if (ac.player_action.amount !=  biggest_bet_amount) {
            std::cerr << "[WARNING] call amount is invalid: " << ac.player_action.amount  \
                      << "Should be: " <<  biggest_bet_amount << std::endl;
            ac.player_action.amount = 0;
            ac.player_action.action = 0;
        }
    }
    else if ( ac.player_action.action == 2) {
        if (ac.player_action.amount < biggest_bet_amount + game_state_.raise_amount)
            std::cerr << "[WARNING] raise amount is invalid: " << ac.player_action.amount  \
                      << "Should be at least: " <<  biggest_bet_amount + game_state_.raise_amount << std::endl;            
    }

    switch (ac.player_action.action ) {
    case 0:
        game_state_.num_player_in_hand--;
        game_state_.player_status[ac.ID] = 0;
        break;
    case 1:
        game_state_.stack_size[ac.ID] -= ac.player_action.amount;
        game_state_.bet_ring[ac.ID] += ac.player_action.amount;
        break;
    case 2:
        std::cerr << "[ERROR] Process 'Raise action' is not supported yet" << std::endl;
        break;
    default:
        std::cerr << "[ERROR] Unknown player action " << ac.player_action.action << " by " << ac.ID << std::endl;
        std::cerr << "default to fold" << std::endl;
        game_state_.num_player_in_hand--;
        game_state_.player_status[ac.ID] = 0;
    }

    //steps that are performed for all player actions
    game_state_.next_player_to_act = FindNextPlayer(game_state_.next_player_to_act);
/*To be added*/    //Update game history game_state_.ActionHistory

}
