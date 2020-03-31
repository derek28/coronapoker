#include "card.h"

Card::Card() { //default constructor, generate a random card
	_suit=std::rand()%4+1;
	_rank=std::rand()%13+1;
}

Card::Card(int rank,int suit){ //constructor by number
	_suit=suit;
	_rank=rank;
}

Card::Card(const Card & c){ //Copy constructor 
	_suit=c.GetSuit();
	_rank=c.GetRank();
	_type=c.GetType();
}

Card::~Card(){}

int Card::GetSuit() const {
	return _suit;
}

int Card::GetRank() const {
	return _rank;
}

int Card::GetType() const{
	return _type;
}


void Card::SetType(int type){
	_type=type;
}



std::ostream& operator<<(std::ostream& outstream, Card c){

switch(c.GetRank()){
       		case 1 : outstream<<"A"; break;
       		case 2 : outstream<<"2"; break;
       		case 3 : outstream<<"3"; break;
       		case 4 : outstream<<"4"; break;
       		case 5 : outstream<<"5"; break;
       		case 6 : outstream<<"6"; break;
       		case 7 : outstream<<"7"; break;
       		case 8 : outstream<<"8"; break;
       		case 9 : outstream<<"9"; break;
       		case 10 : outstream<<"T"; break;
       		case 11 : outstream<<"J"; break;
       		case 12 : outstream<<"Q"; break;
		case 13 : outstream<<"K"; break;
		case 14 : outstream<<"A"; break;
       		default : std::cout<< "Rank out of bound!" << std::endl;
}
	
switch(c.GetSuit()){
       		case 1 : outstream<<"s"; break;
       		case 2 : outstream<<"h"; break;
       		case 3 : outstream<<"c"; break;
       		case 4 : outstream<<"d"; break;
       		default : std::cout<< "Suit out of bound!" << std::endl;
}



}
