#pragma once

#include<iostream>
#include "card.h"
#include<vector>

using namespace std;

class Deck {
public:
	Deck(); //default constructor
	void Shuffle();
	void Print();
	Card Deal();
	void Regenerate();
private:
	std::vector<Card> thedeck;

};
