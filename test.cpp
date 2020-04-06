#include<iostream>
#include<string>
#include"pokerhand.h"
#include<ctime>
#include<cstdlib>
#include"card.h"
#include"deck.h"


void testPokerhandComparison();

using namespace std;
int main(){
    std::srand(std::time(0));
    testPokerhandComparison();
    return 0;
}

void testPokerhandComparison() {
	std::cout << "*****Test: Pokerhand Strength Comparison*****" << std::endl;
	PokerHand ph1("2H 2D 4H 5C AD 7C 8H");
	PokerHand ph2("2S 2C 4H 6H AD 7H 8H");

	std::cout << "ph1 < ph2 : " << ( ph1 < ph2 ) << std::endl;
	std::cout << "ph1 = ph2 : " << ( ph1 == ph2 ) << std::endl;
	std::cout << "ph1 > ph2 : " << ( ph1 > ph2 ) << std::endl;
}

void test4(){ //test of deck generation, shuffle and deal.
	cout << "*****Test4: Deck generation and shuffling*****" <<endl;
	Deck deck;
	std::cout << "deck before shuffling:" << std::endl; 
	deck.print();
	deck.shuffle();
	std::cout << "deck after shuffling:" << std::endl;
	deck.print();
	std::cout << "dealing a card:" << std::endl;
	std::cout << "Card being dealt is: " << deck.deal() << std::endl;
	deck.print();
}

void test5(){ //generate and print PokerHand
	cout << "*****Test5: PokerHand generation and display*****" <<endl;
	PokerHand hand1("2H 3H 4H 5H 6D 7H 8H");
	std::cout << "Hand: \"2H 3H 4H 5H 6D 7H 8H\"" <<std::endl;
	hand1.printarray();
	PokerHand hand2("KS AS TS QS JD 9S 8S");
	std::cout << "Hand: \"KS AS TS QS JD 9S 8S\"" <<std::endl;
	hand2.printarray();	
}

void test6(){ //poker hand type evaluation benchmark
	cout << "*****Test6: Hand Frequency calculator and benchmark*****" <<endl;
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
	cout << nhand << " hand evaluation done in " << elapsed_secs << " seconds" << endl;
	cout << "Frequency table:" <<std::endl;
	cout << "SF (Expected: 0.0311%):      " << (double)count[0]/nhand << endl; 
	cout << "quad (Expected: 0.168%):     " << (double)count[1]/nhand << endl; 
	cout << "FH (Expected: 2.60%):        " << (double)count[2]/nhand << endl; 
	cout << "flush (Expected: 3.03%):     " << (double)count[3]/nhand << endl; 
	cout << "straight (Expected: 4.62%):  " << (double)count[4]/nhand << endl; 
	cout << "trip (Expected: 4.83%):      " << (double)count[5]/nhand << endl; 
	cout << "two-pair (Expected: 23.5%):  " << (double)count[6]/nhand << endl; 
	cout << "pair (Expected: 43.8%):      " << (double)count[7]/nhand << endl; 
	cout << "high-card(Expected: 17.4%):  " << (double)count[8]/nhand << endl; 
}



void test7(){
	cout << "*****Test7: Hand Strength Evaludate Function and return*****" <<endl;
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
	cout << "*****Test8: Use the hand strength function for 10M hands*****" <<endl;	
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
	cout << nhand << " hand evaluation done in " << elapsed_secs << " seconds" << endl;
	cout << "Frequency table:" <<std::endl;
	cout << "SF (Expected: 0.0311%):      " << (double)count[0]/nhand << endl; 
	cout << "quad (Expected: 0.168%):     " << (double)count[1]/nhand << endl; 
	cout << "FH (Expected: 2.60%):        " << (double)count[2]/nhand << endl; 
	cout << "flush (Expected: 3.03%):     " << (double)count[3]/nhand << endl; 
	cout << "straight (Expected: 4.62%):  " << (double)count[4]/nhand << endl; 
	cout << "trip (Expected: 4.83%):      " << (double)count[5]/nhand << endl; 
	cout << "two-pair (Expected: 23.5%):  " << (double)count[6]/nhand << endl; 
	cout << "pair (Expected: 43.8%):      " << (double)count[7]/nhand << endl; 
	cout << "high-card(Expected: 17.4%):  " << (double)count[8]/nhand << endl; 
}



void test9(){
	cout << "*****Test9: Initialize a deck, deal then evaluate for 10M hands*****" <<endl;	
	long ihand=0; 
	long nhand=10000000;
	vector<long> count{0,0,0,0,0,0,0,0,0};

	clock_t begin= clock();
	while(ihand<nhand){
		Deck theDeck;
		PokerHand theHand;
		theDeck.shuffle();
		for (int i=0; i<7;i++) theHand.add(theDeck.deal());
		vector <int> strength=theHand.GetStrength();
		count[strength[0]]++; 
		ihand++;
	}
	clock_t end = clock();


	std::reverse(count.begin(),count.end());
	double elapsed_secs= double(end-begin) / CLOCKS_PER_SEC;
	cout << nhand << " hand evaluation done in " << elapsed_secs << " seconds" << endl;
	cout << "Frequency table:" <<std::endl;
	cout << "SF (Expected: 0.0311%):      " << (double)count[0]/nhand << endl; 
	cout << "quad (Expected: 0.168%):     " << (double)count[1]/nhand << endl; 
	cout << "FH (Expected: 2.60%):        " << (double)count[2]/nhand << endl; 
	cout << "flush (Expected: 3.03%):     " << (double)count[3]/nhand << endl; 
	cout << "straight (Expected: 4.62%):  " << (double)count[4]/nhand << endl; 
	cout << "trip (Expected: 4.83%):      " << (double)count[5]/nhand << endl; 
	cout << "two-pair (Expected: 23.5%):  " << (double)count[6]/nhand << endl; 
	cout << "pair (Expected: 43.8%):      " << (double)count[7]/nhand << endl; 
	cout << "high-card(Expected: 17.4%):  " << (double)count[8]/nhand << endl; 
}

void test10(){
	cout << "*****Test10: Initialize a deck, deal 5 players then evaluate for 10M hands*****" <<endl;	
	long ihand=0; 
	long nhand=10000000;
	vector<long> count{0,0,0,0,0,0,0,0,0};

	clock_t begin= clock();
	while(ihand<nhand){
		Deck theDeck;
		PokerHand theHand;
		theDeck.shuffle();
		
		for(int j=0;j<5;j++){
			for (int i=0; i<7;i++) theHand.add(theDeck.deal());
			vector <int> strength=theHand.GetStrength();
			count[strength[0]]++; 
			ihand++;
		}
	}
	clock_t end = clock();


	std::reverse(count.begin(),count.end());
	double elapsed_secs= double(end-begin) / CLOCKS_PER_SEC;
	cout << nhand << " hand evaluation done in " << elapsed_secs << " seconds" << endl;
}
