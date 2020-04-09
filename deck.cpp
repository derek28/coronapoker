/* 
 * deck.cpp
 */

#include "deck.h"
#include <algorithm> // std::random_shuffle
#include <vector>

using namespace std;


Deck::Deck() { //default initializer
	thedeck.reserve(52);
	for (int i = 1; i < 5; i++)
		for (int j = 1; j < 14; j++) {
			Card temp(j, i);
			thedeck.push_back(temp);
		}
}

void Deck::Print() {
	for (auto &i: thedeck)
		cout << i << " ";

	cout << endl;
}

Card Deck::Deal() {
	Card temp = thedeck.back();
	thedeck.erase(thedeck.end()); //remove the end of the card.
	return temp;
}

void Deck::Shuffle() {
	std::random_shuffle (thedeck.begin(), thedeck.end());
}

void Deck::Regenerate() {
	thedeck.clear();
	for (int i = 1; i < 5; i++)
		for (int j = 1; j < 14; j++) {
			Card temp(j, i);
			thedeck.push_back(temp);
		}	
}
void Deck::RemoveCard(Card c) {
	for (int i = 0; i < thedeck.size(); i++) {
		if (thedeck[i] == c) {
			thedeck.erase(thedeck.begin() + i);
			return;
		}
	}
	cerr << "Deck: card removal failure, no such card found." << endl;
}


