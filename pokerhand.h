#pragma once
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include "card.h"

class PokerHand {
public:
	PokerHand(); //default constructor

	PokerHand (const char* pokerhand) ; //constructor from char input: for example: PokerHand hand1("2H 3H 4H 5H 6D 7H 8H");

	PokerHand (const int ); //constructor by defining number of random cards, generate a random hand. For example, PokerHand hand1(7);

	~PokerHand (); //destructor

	void add (const Card & ); //add a card to the card map
	
	void print();
	void printarray();

	//std::vector<Card> GetMadeHand(); //return a 5-card list which is the final made hand

	std::vector<int> GetStrength();
	//check if hand is SF, return a vector of length 6 signifying [hand type,kicker1,kicker2,kicker3,kicker4,kicker5]
	//strength 8=SF; 7=Quad; 6=Full House; 5=Flush 4=Straight 3=trip 2=twopairs 1=onepair 0=highcard

	std::vector<Card> GetWinnerHand(); //only working for Strength=8 and Strength 7; must be run after Strength is computed.
	bool ifWinnerHandContainsHoleCards(); //check if the Winner Hand contains the private cards

	bool _is_SF(); 
	bool _is_quad(); 
	bool _is_FH();
	bool _is_flush();
	bool _is_straight();
	bool _is_three_of_a_kind();
	bool _is_two_pairs();
	bool _is_one_pair();
	void _high_cards();

private:
	int handmap[5][15]= {{0}}; //an 2D array initialized to zero which is used to do hand strength computation
	std::vector <Card> hand; // a vector of cards contained
	std::vector <int> strength={0,0,0,0,0,0}; //a vector indicating the show-down value of the hand
};
