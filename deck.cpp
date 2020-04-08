/* 
 * deck.cpp
 */

#include "deck.h"
#include <algorithm> // std::random_shuffle
#include <vector>

using namespace std;


Deck::Deck() { //default initializer
	for (int i = 1; i < 5; i++)
		for (int j = 1; j < 14; j++) {
			Card temp(j, i);
			thedeck.push_back(temp);
		}
}

void Deck::print() {
	for (auto &i: thedeck)
		cout << i << " ";

	cout << endl;
}

Card Deck::deal() {
	Card temp = thedeck.back();
	thedeck.erase(thedeck.end()); //remove the end of the card.
	return temp;
}

void Deck::shuffle() {
	std::random_shuffle (thedeck.begin(), thedeck.end());
}

void Deck::RemoveCard(Card c) {
	for (int i = 0; i < thedeck.size(); i++) {
		if (thedeck[i] == c) {
			thedeck.erase(thedeck.begin() + i);
			break;
		}
	}
}


