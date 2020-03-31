#include<iostream>
#include "card.h"
#include<vector>

using namespace std;

class Deck {
public:
	Deck(); //default constructor
	void shuffle();
	void print();
	Card deal();
private:
	std::vector<Card> thedeck;

};
