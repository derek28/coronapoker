/*********************************** * Filename: game.cpp * Last Modified: 2020/04/02
 * **********************************/

#include <iostream>
#include "game.h"

using namespace std;

Game::Game() {
	it = 0;
}

Game::~Game() {

}

int Game::AddPlayer(int seat, int stack_size, int type) {
	if (players[seat].GetStatus != STATUS_NO_PLAYER) {
		cerr << "Player already sitting at seat #" << seat << endl;
		cerr << "AddPlayer() failure" << endl;
		return -1;
    }	
	players[seat].SetType(type);
	players[seat].AddToStack(stack_size);
	
	return 0;
}

void Game::RemovePlayer(int seat) {
	players[seat].SetStatus(STATUS_NO_PLAYER);	
}

void Game::StartAHand() {
	Card dealt_cards[2];
	deck.shuffle();
	for (int i = 0; i < NUM_OF_PLAYERS; i++) {
		dealt_cards[0] = deck.deal();
		dealt_cards[1] = deck.deal();
		players[i].SetHoleCards(dealt_cards);
	}
}

void Game::SetBlinds() {

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
