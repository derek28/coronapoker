#pragma once
#include <iostream>


class Card {
public:
	Card(); //default constructor, generate a random card
	Card(const Card &c); //copy constructor
	Card(int rank, int suit); //construct by giving a rank then suit; Rank: {1:A 2:2 ...10:T 11:J 12:Q 13:K} Suit: {1:S 2:H 3:C 4:D} for example Card acard(1,4) for Ace of Diamond
	~Card(); //destructor

	int GetSuit() const;
	int GetRank() const;

	void SetType(int); //0=uninitialized; 1=private card; 2=board;
	int GetType() const;

	bool operator==(const Card &c) const{  //defines == operator for class Card
		return ( c.GetSuit()== _suit && c.GetRank()== _rank); // return true if the suit and rank matches
	}

private:
	int  _rank, _suit;
	int _type=0; //0=uninitialized; 1=private card; 2=board;
};

std::ostream& operator<<(std::ostream& outstream, Card c); //output overload

