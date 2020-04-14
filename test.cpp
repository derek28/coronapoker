#include<iostream>
#include<string>
#include"pokerhand.h"
#include<ctime>
#include<cstdlib>
#include"card.h"
#include"deck.h"
#include"strength.h"
#include"human_player.h"

using namespace std;

void testPokerhandComparison();

void testHandStrength(); 

void testHumanPlayer();

int main(){
    std::srand(std::time(0));
//	testHandStrength();
	testPokerhandComparison();
    return 0;
}

void testHumanPlayer() {
	HumanPlayer bill;
	Action act;
	GameState state;
	LegalActions legal_actions;
	act = bill.Act(state, legal_actions);
	if (act.action == 0) {
		cout << "You folded." << endl;
	} else if (act.action == 1) {
		cout << "You call/checked." << endl;
	} else if (act.action == 2) {
		cout << "You raised " << act.amount << endl;
	} else { 
		cout << "ERROR! act.action = " << act.action << endl;
	}
}

void testHandStrength() {
	vector <Card> my_hand;
	vector <Card> opp_hand;
	vector <Card> board;

	opp_hand.push_back(Card(9, CLUB));
	opp_hand.push_back(Card(9, DIAMOND));

	my_hand.push_back(Card(2, CLUB));
	my_hand.push_back(Card(2, HEART));

	board.push_back(Card(7, SPADE));
	board.push_back(Card(8, HEART));
	board.push_back(Card(2, SPADE));
	//board.push_back(Card(13, HEART));
	//board.push_back(Card(1, DIAMOND));

	cout << "My hand: " << my_hand[0] << " " << my_hand[1] << endl;
	cout << "Community cards: ";
	for (int i = 0; i < board.size(); i++) {
		cout << board[i] << " ";
	}
	cout << endl;
	float IHS = GetImmediateStrength(my_hand, board, NULL);
	cout << "Immediate hand strength = " << IHS << endl;
	float EHS = GetEffectiveStrength(my_hand, board, NULL);
	cout << "Effective hand strength = " << EHS << endl;
//	float equity = GetHandEquity(hd, op, com, num_of_cards);
//	cout << "Hand equity against " << op[0] << " " << op[1] << " is " 
//			<< equity << endl;
}

void testPokerhandComparison() {
	std::cout << "*****Test: Pokerhand Strength Comparison*****" << std::endl;
//	PokerHand ph1("2H 2D 4H 5C AD 7C 8H");
//	PokerHand ph2("2S 2C 4H 6H AD 7H 8H");

//	PokerHand ph1("6D AS JD TC KH TS 3D");
//	PokerHand ph2("AD 5H JD TC KH TS 3D");

//	PokerHand ph1("JH 9D JD JC 9C AD QS");
//	PokerHand ph2("JH 9D JD JC 9C 4C AS");

	PokerHand ph1;
	PokerHand ph2;
	ph1.add( Card(1,4) );
	ph1.add( Card(12,1) );
	ph1.add( Card(11,2) );
	ph1.add( Card(9,4) );
	ph1.add( Card(11,4) );
	ph1.add( Card(11,3) );
	ph1.add( Card(9,3) );

	ph2.add( Card(4,3) );
	ph2.add( Card(1,1) );
	ph2.add( Card(11,2) );
	ph2.add( Card(9,4) );
	ph2.add( Card(11,4) );
	ph2.add( Card(11,3) );
	ph2.add( Card(9,3) );

	ph1.print();
	ph2.print();

    ph1.printarray();
	ph2.printarray();

	std::cout << "ph1 strength:" << ph1.GetStrength() << std::endl;
	std::cout << "ph2 strength:" << ph2.GetStrength() << std::endl;

	std::cout << "ph1 < ph2 : " << ( ph1 < ph2 ) << std::endl;
	std::cout << "ph1 = ph2 : " << ( ph1 == ph2 ) << std::endl;
	std::cout << "ph1 > ph2 : " << ( ph1 > ph2 ) << std::endl;
}

void test4(){ //test of deck generation, shuffle and deal.
	std::cout << "*****Test4: Deck generation and shuffling*****" <<std::endl;
	Deck deck;
	std::cout << "deck before shuffling:" << std::endl; 
	deck.Print();
	deck.Shuffle();
	std::cout << "deck after shuffling:" << std::endl;
	deck.Print();
	std::cout << "dealing a card:" << std::endl;
	std::cout << "Card being dealt is: " << deck.Deal() << std::endl;
	deck.Print();
}

void test5(){ //generate and print PokerHand
	std::cout << "*****Test5: PokerHand generation and display*****" <<std::endl;
	PokerHand hand1("2H 3H 4H 5H 6D 7H 8H");
	std::cout << "Hand: \"2H 3H 4H 5H 6D 7H 8H\"" <<std::endl;
	hand1.printarray();
	PokerHand hand2("KS AS TS QS JD 9S 8S");
	std::cout << "Hand: \"KS AS TS QS JD 9S 8S\"" <<std::endl;
	hand2.printarray();	
}

void test6(){ //poker hand type evaluation benchmark
	std::cout << "*****Test6: Hand Frequency calculator and benchmark*****" <<std::endl;
	long ihand=0; 
	long nhand=10000000;
	vector<long> count{0,0,0,0,0,0,0,0,0};

	clock_t begin= clock();
	while(ihand<nhand){
		PokerHand hand(7);
		if( hand._is_SF()) { count[0]++; ihand++; continue;}
		if( hand._is_quad()) {count[1]++; ihand++; continue;}
		if( hand._is_FH()) {count[2]++;  ihand++;continue;}
		if( hand._is_flush()) {count[3]++;  ihand++;continue;}
		if( hand._is_straight()) {count[4]++; ihand++; continue;}
		if( hand._is_three_of_a_kind()){ count[5]++; ihand++; continue;}
		if( hand._is_two_pairs()){ count[6]++;ihand++;  continue;}
		if( hand._is_one_pair()) {count[7]++;ihand++; continue;} 
		count[8]++;ihand++; //the hands reaching this point are high-card
		
	}
	clock_t end = clock();
	double elapsed_secs= double(end-begin) / CLOCKS_PER_SEC;
	std::cout << nhand << " hand evaluation done in " << elapsed_secs << " seconds" << std::endl;
	std::cout << "Frequency table:" <<std::endl;
	std::cout << "SF (Expected: 0.0311%):      " << (double)count[0]/nhand << std::endl; 
	std::cout << "quad (Expected: 0.168%):     " << (double)count[1]/nhand << std::endl; 
	std::cout << "FH (Expected: 2.60%):        " << (double)count[2]/nhand << std::endl; 
	std::cout << "flush (Expected: 3.03%):     " << (double)count[3]/nhand << std::endl; 
	std::cout << "straight (Expected: 4.62%):  " << (double)count[4]/nhand << std::endl; 
	std::cout << "trip (Expected: 4.83%):      " << (double)count[5]/nhand << std::endl; 
	std::cout << "two-pair (Expected: 23.5%):  " << (double)count[6]/nhand << std::endl; 
	std::cout << "pair (Expected: 43.8%):      " << (double)count[7]/nhand << std::endl; 
	std::cout << "high-card(Expected: 17.4%):  " << (double)count[8]/nhand << std::endl; 
}



void test7(){
	std::cout << "*****Test7: Hand Strength Evaludate Function and return*****" <<std::endl;
	PokerHand hand(7);
	std::cout << "Hand map is:" << std::endl;
	hand.printarray();
	std::cout << "Hand is:" <<std::endl;
	hand.print();
	vector <int> strength=hand.GetStrength();
	std::cout << "Hand strength is:";
	for (auto& i : strength) std::cout << i << ' ';
	std::cout << std::endl;
}


void test8(){
	std::cout << "*****Test8: Use the hand strength function for 10M hands*****" <<std::endl;	
	long ihand=0; 
	long nhand=10000000;
	vector<long> count{0,0,0,0,0,0,0,0,0};

	clock_t begin= clock();
	while(ihand<nhand){
		PokerHand hand(7);
		vector <int> strength=hand.GetStrength();
		count[strength[0]]++; 
		ihand++;
	}
	clock_t end = clock();


	std::reverse(count.begin(),count.end());
	double elapsed_secs= double(end-begin) / CLOCKS_PER_SEC;
	std::cout << nhand << " hand evaluation done in " << elapsed_secs << " seconds" << std::endl;
	std::cout << "Frequency table:" <<std::endl;
	std::cout << "SF (Expected: 0.0311%):      " << (double)count[0]/nhand << std::endl; 
	std::cout << "quad (Expected: 0.168%):     " << (double)count[1]/nhand << std::endl; 
	std::cout << "FH (Expected: 2.60%):        " << (double)count[2]/nhand << std::endl; 
	std::cout << "flush (Expected: 3.03%):     " << (double)count[3]/nhand << std::endl; 
	std::cout << "straight (Expected: 4.62%):  " << (double)count[4]/nhand << std::endl; 
	std::cout << "trip (Expected: 4.83%):      " << (double)count[5]/nhand << std::endl; 
	std::cout << "two-pair (Expected: 23.5%):  " << (double)count[6]/nhand << std::endl; 
	std::cout << "pair (Expected: 43.8%):      " << (double)count[7]/nhand << std::endl; 
	std::cout << "high-card(Expected: 17.4%):  " << (double)count[8]/nhand << std::endl; 
}



void test9(){
	std::cout << "*****Test9: Initialize a deck, deal then evaluate for 10M hands*****" <<std::endl;	
	long ihand=0; 
	long nhand=10000000;
	vector<long> count{0,0,0,0,0,0,0,0,0};

	clock_t begin= clock();
	while(ihand<nhand){
		Deck theDeck;
		PokerHand theHand;
		theDeck.Shuffle();
		for (int i=0; i<7;i++) theHand.add(theDeck.Deal());
		vector <int> strength=theHand.GetStrength();
		count[strength[0]]++; 
		ihand++;
	}
	clock_t end = clock();


	std::reverse(count.begin(),count.end());
	double elapsed_secs= double(end-begin) / CLOCKS_PER_SEC;
	std::cout << nhand << " hand evaluation done in " << elapsed_secs << " seconds" << std::endl;
	std::cout << "Frequency table:" <<std::endl;
	std::cout << "SF (Expected: 0.0311%):      " << (double)count[0]/nhand << std::endl; 
	std::cout << "quad (Expected: 0.168%):     " << (double)count[1]/nhand << std::endl; 
	std::cout << "FH (Expected: 2.60%):        " << (double)count[2]/nhand << std::endl; 
	std::cout << "flush (Expected: 3.03%):     " << (double)count[3]/nhand << std::endl; 
	std::cout << "straight (Expected: 4.62%):  " << (double)count[4]/nhand << std::endl; 
	std::cout << "trip (Expected: 4.83%):      " << (double)count[5]/nhand << std::endl; 
	std::cout << "two-pair (Expected: 23.5%):  " << (double)count[6]/nhand << std::endl; 
	std::cout << "pair (Expected: 43.8%):      " << (double)count[7]/nhand << std::endl; 
	std::cout << "high-card(Expected: 17.4%):  " << (double)count[8]/nhand << std::endl; 
}

void test10(){
	std::cout << "*****Test10: Initialize a deck, deal 5 players then evaluate for 10M hands*****" <<std::endl;	
	long ihand=0; 
	long nhand=10000000;
	vector<long> count{0,0,0,0,0,0,0,0,0};

	clock_t begin= clock();
	while(ihand<nhand){
		Deck theDeck;
		PokerHand theHand;
		theDeck.Shuffle();
		
		for(int j=0;j<5;j++){
			for (int i=0; i<7;i++) theHand.add(theDeck.Deal());
			vector <int> strength=theHand.GetStrength();
			count[strength[0]]++; 
			ihand++;
		}
	}
	clock_t end = clock();


	std::reverse(count.begin(),count.end());
	double elapsed_secs= double(end-begin) / CLOCKS_PER_SEC;
	std::cout << nhand << " hand evaluation done in " << elapsed_secs << " seconds" << std::endl;
}
