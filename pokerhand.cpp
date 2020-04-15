/* 
 * pokerhand.cpp
 */

#include "pokerhand.h"
#include <vector>
#include "card.h"
#include <algorithm>

using namespace std;

PokerHand::PokerHand (){ //default constructor
	hand.reserve(7);
}

PokerHand::PokerHand (const int ncard){ //constructor by defining number of random cards, generate a random hand. For example, PokerHand hand1(7);
	int icard=0;
	hand.reserve(7);
	while(icard<ncard){
		int i=std::rand()%4+1;
		int j=std::rand()%13+1;

		if(handmap[i][j]==1) continue; //if the card is already there in the hand, regenerate the card

		Card card(j,i);
		hand.push_back(card); //update the vector of cards
		handmap[i][j]=1; //then update the hand map
		handmap[0][j]+=1;
		handmap[i][0]+=1;
		++icard;
		if(j==1) {handmap[i][j+13]=1;handmap[0][j+13]++;}
	}
	this->UpdateStrength();
}

PokerHand::PokerHand (const char* pokerhand) { //constructor from a string input such as "KH KD TC 3C"
  	int i,j;
   	while (*pokerhand != '\0'){
		if( (*pokerhand)==' ') {pokerhand++; continue;}
	      	switch(*pokerhand){
	       		case 'K' : i=13; break;
	       		case 'Q' : i=12; break;
	       		case 'J' : i=11; break;
	       		case 'T' : i=10; break;
	      		case '9' : i=9 ; break;
		   	    case '8' : i=8 ; break;
			    case '7' : i=7 ; break;
		       	case '6' : i=6 ; break;
		       	case '5' : i=5 ; break;
	     	  	case '4' : i=4 ; break;
	       		case '3' : i=3 ; break;
	       		case '2' : i=2 ; break;
	       		case 'A' : i=1 ; break;
	       		default : std::cout<< "Rank input error!" << std::endl;
	   	   	}
	   		pokerhand++;
	      	switch(*pokerhand){
	       		case 'S' : j=1; break;
	       		case 'H' : j=2; break;
	       		case 'C' : j=3; break;
	       		case 'D' : j=4; break;
	      		default : std::cout<< "Suit input error!" << std::endl;
	      	}
	      	pokerhand++;

		Card card(i,j);
		hand.push_back(card); //update the vector of cards

	      	if(i==1) {handmap[j][14]++; handmap[0][14]++;} //update the handmap
		handmap[j][i]++;
		handmap[0][i]++;
		handmap[j][0]++;
		i=0;
		j=0;
   	}
	this->UpdateStrength();
};


PokerHand::~PokerHand() { 

}

bool operator == (const PokerHand &ph1, const PokerHand &ph2) {
	return (ph1.strength == ph2.strength);
}

bool operator < (const PokerHand &ph1, const PokerHand &ph2) {
	return (ph1.strength < ph2.strength);
}

bool operator > (const PokerHand &ph1, const PokerHand &ph2) {
	return (ph1.strength > ph2.strength);
}

void PokerHand::add(Card card){
	hand.push_back(card);

	int i=card.GetRank();
	int j=card.GetSuit();

	if(handmap[j][i]==1){ std::cout <<"card already exists in this hand!!"; return; }
	if(i==1) {handmap[j][14]++; handmap[0][14]++;} //update the handmap
	handmap[j][i]++;
	handmap[0][i]++;
	handmap[j][0]++;
//	cout << "Add " << card << " success" << endl;
	this->UpdateStrength();
}

void PokerHand::printarray(){
	for (int i=0; i<=4; i++){
		for (int j=0; j<=14; j++)
			std::cout << handmap[i][j] << ' ';
		std::cout<<std::endl;
	}
}

void PokerHand::print(){
	for(auto&i: hand) std::cout << i << " ";
	std::cout<<std::endl;
}

void PokerHand::UpdateStrength() {
	std::fill(strength.begin(),strength.end(),0);
	do {
		if( this->_is_SF()) { continue;}
		if( this->_is_quad()) {continue;}
		if( this->_is_FH()) {continue;}
		if( this->_is_flush()) {continue;}
		if( this->_is_straight()) { continue;}
		if( this->_is_three_of_a_kind()){continue;}
		if( this->_is_two_pairs()){continue;}
		if( this->_is_one_pair()) {continue;} 
		this->_high_cards();
	} while (0);
}

std::vector<int> PokerHand::GetStrength(){
	return strength;
}


bool PokerHand::ifWinnerHandContainsHoleCards(){
	std::vector<Card> WinnerHand=this->GetWinnerHand();

/*
	std::cout<<"Winner Hand is:";
	for (auto& card: WinnerHand) std::cout << card << ' ';
	std::cout <<std::endl;
*/	
	for( auto& card: hand){
		//std:: cout << "card.GetType(): " << card.GetType() <<std::endl;
		if(card.GetType()!=1) continue; //continue to next loop if the card is not a private card	
		if( std::find(WinnerHand.begin(), WinnerHand.end(),card) ==  WinnerHand.end() ) return 0;
	}


	//std::cout<<"Return 1!"<<std::endl;
	return 1;
}


std::vector<Card> PokerHand::GetWinnerHand(){
	std::vector<Card> WinnerHand;
	if(strength[0]<7) { std::cout<< "GetWinnerHand is not supported for this type of hand" << std::endl; return{}; }
	if(strength[0]==8){
		for(int suit=1; suit<=4; suit++){ //go through the suits one-by-one
			if(handmap[suit][0]<5) continue; //if there is less than 5 cards for a suit, skip this suit
			for(int rank=10; rank>=1; rank--){ //if more than 5 cards for a suit, check downwards from T for a SF
				if(handmap[suit][rank]==1 && handmap[suit][rank+1]==1 &&handmap[suit][rank+2]==1 &&handmap[suit][rank+3]==1 && handmap[suit][rank+4]==1){
					WinnerHand.push_back(Card(rank+4,suit));
					WinnerHand.push_back(Card(rank+3,suit));
					WinnerHand.push_back(Card(rank+2,suit));
					WinnerHand.push_back(Card(rank+1,suit));
					WinnerHand.push_back(Card(rank,suit));
					return WinnerHand;
				}
			}
		}
	}
	if(strength[0]==7){
		for(int rank=14; rank>1; rank--){
			if(handmap[0][rank]==4){
				WinnerHand.push_back(Card(rank,1));
				WinnerHand.push_back(Card(rank,2));
				WinnerHand.push_back(Card(rank,3));
				WinnerHand.push_back(Card(rank,4));			
				for(int rank2=14; rank2>1; rank2--) //rank2 is an "iterator" scanning for the strength
	      	        		if(handmap[0][rank2]!=0 && rank2!=rank){
						for(int i=1; i<5;i++){
							if(handmap[i][rank2]==1) WinnerHand.push_back(Card(rank2,i));
						}
						return WinnerHand;
					}
			}
		} 
	}
	return {};
}







bool PokerHand::_is_SF(){
	for(int suit=1; suit<=4; suit++){ //go through the suits one-by-one
		if(handmap[suit][0]<5) continue; //if there is less than 5 cards for a suit, skip this suit
		for(int rank=10; rank>=1; rank--){ //if more than 5 cards for a suit, check downwards from T for a SF
			if(handmap[suit][rank]==1 && handmap[suit][rank+1]==1 &&handmap[suit][rank+2]==1 &&handmap[suit][rank+3]==1 && handmap[suit][rank+4]==1){
				strength[1]=rank+4;
				strength[0]=8; //8 for SF
				return 1;
			}
		}
	}
	return 0;
}


bool PokerHand::_is_quad(){ //this is very straight too; check downwards from Ace for Quads
	for(int rank=14; rank>1; rank--){
		if(handmap[0][rank]==4){
			strength[1]=rank;
			for(int rank2=14; rank2>1; rank2--) //rank2 is an "iterator" scanning for the strength
      	        		if(handmap[0][rank2]!=0 && rank2!=strength[1]){
					strength[2]=rank2;
					break;
				}
			strength[0]=7; //7 for Quad
    	        	return 1;
		}
	} 
 	return 0;
}

bool PokerHand::_is_FH(){ //check for trips and then check for pair
	for(int rank=14; rank>1; rank--){
		if(handmap[0][rank]==3) {
			strength[1]=rank;
			for(int rank2=14; rank2>=2; rank2--){ //then look for the pair
				if(rank2==strength[1]) continue;	//well we have to skip the rank which has the trip
				if(handmap[0][rank2]>=2) {
					strength[2]=rank2;
					strength[0]=6; //6 for FH
					return 1;
				}   
			}
		}
	}
	return 0;
}


bool PokerHand::_is_flush(){
	int suit;
	auto it=strength.begin();
	it++;
	for(suit=1; suit<=4; suit++) //go through the suits and look if the suit has 5 cards, if so, go ahead and assume it is not possible to have 2 Flushes in a hand. (need to check that the pokerhand size <10)
		if(handmap[suit][0]>=5) break;
	if (suit==5) { return 0;} //return 0 when no flush is found
	for (int High_card=14; High_card>=2; High_card--){ //now we have high cards, count downwards
		if(handmap[suit][High_card]==1 && it!=strength.end()){ //stop when the vector <strength> is fully filled
			(*it) =High_card;
			it++;
		}
	}
	strength[0]=5; //5 for Flush
	return 1;
}


bool PokerHand::_is_straight(){ //this is straight forward: check the numbers
	for(int rank=1; rank<=10; rank++){
		if(handmap[0][rank] != 0 && handmap[0][rank+1] != 0 && handmap[0][rank+2] != 0 && handmap[0][rank+3] !=0 && handmap[0][rank+4] !=0){
			strength[1]=rank+4;
			strength[0]=4; //4 for straight
			return 1;
		}
	}
	return 0;
}


bool PokerHand::_is_three_of_a_kind(){ //check for trips
	auto it=strength.begin();
	it++;
	for(int rank=14; rank>=2; rank--){ //find the three_of_a_kind
		if(handmap[0][rank]==3) {
			(*it)=rank;it++;
			for(int rank2=14; rank2>1; rank2--){ //collect the strengths
				if(handmap[0][rank2]==1 && rank2!=rank && it!=strength.end()) {(*it)=rank2; it++;}
			}
		strength[0]=3; //3 for trip
		return 1;
		}
	}
	return 0;
}

bool PokerHand::_is_two_pairs(){
	auto it=strength.begin();
	it++;
	for(int rank=14; rank>=2; rank--){ //find 2 pairs.
		if(handmap[0][rank]==2) { //find first pair
			(*it)=rank;it++;
			for(int rank2=rank-1; rank2>=2; rank2--){
				if(handmap[0][rank2]==2){//find second pair
					(*it)=rank2; it++;
					for(int rank3=14; rank3>=2; rank3--){ //find the strength
						if(handmap[0][rank3]>0 && rank3 !=rank2 && rank3!=rank){
							(*it)=rank3;
							strength[0]=2; //2 for two pairs 
							return 1;
						}
					} 
				}
			}
		}
	}
    return 0;
}

bool PokerHand::_is_one_pair(){
	auto it=strength.begin();
	it++;
	for(int rank=14; rank>=2; rank--){ //Starting from Ace(14), looking for a pair one-by-one downwards
		if(handmap[0][rank]==2) { //found first pair
			(*it)=rank;it++; //document it!
			for(int rank2=14; rank2>=2; rank2--){ //now start to record the strengths, again downward from Ace (14).
				if(handmap[0][rank2]==1 && it!= (strength.end()-1)  ){ //need strength.end()-1 here because we only need 3 kickers for 1 pair hand, thus last entry of vector is unpopulated
					(*it)=rank2; it++; 
				}
			}
		strength[0]=1; //1 for one pair;
		return 1;
		}
	}
	return 0;
}

void PokerHand::_high_cards(){
	auto it=strength.begin();
	it++;
	for(int rank=14; rank>=2; rank--){
		if(handmap[0][rank]==1 && it!=strength.end()) {  (*it)=rank;it++; }
	}
}


