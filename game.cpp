/*********************************** * Filename: game.cpp * Last Modified: 2020/04/02
 * **********************************/


#include "game.h"

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
    std::cout << "[Debug] Deck is:";
	deck.print();
    #endif
	for (int i = 0; i < NUM_OF_PLAYERS; i++) {
		dealt_cards[0] = deck.deal();
		dealt_cards[1] = deck.deal();
		players[i].SetHoleCards(dealt_cards);
	}
}

void Game::PostBlinds() {
    #ifdef DEBUG
    std::cout << "[Debug] Posting Blinds";
    #endif
    ComputeBlindPos();
    players[sb_pos_].Bet(100);
    players[bb_pos_].Bet(200);
    
    //update game state
    game_state_.bet_amount[sb_pos_] = 100;
    game_state_.bet_amount[bb_pos_] = 200;
}

int Game::PlayerAction(int seat) {
	int ac;
   	ac = players[seat].Action();

	return ac;
}

int Game::Flop() {



}

int Game::Turn() {


}

int Game::River() {


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