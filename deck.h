#pragma once

#include<iostream>
#include "card.h"
#include<vector>

using namespace std;

class Deck {
public:
	Deck(); //default constructor
<<<<<<< .merge_file_a18136
	void Shuffle();
	void Print();
	Card Deal();
	void Regenerate();
=======
	void shuffle();
	void print();
	Card deal();
	void RemoveCard(Card c);
>>>>>>> .merge_file_a19484
private:
	std::vector<Card> thedeck;

};
