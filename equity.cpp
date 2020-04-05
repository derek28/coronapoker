/****************************************
 * Filename: equity.cpp
 * Author: Kai
 * Description: Implement functions for calculating equity of one or more players in game
 * Last modified: 2020/04/04
 * ***************************************/

#include <iostream>
#include <vector>
#include "card.h"
#include "deck.h"
#include "pokerhand.h"

/* Calculate equity of 2 players. */
float calc_equity(Card *a, Card *b, Card *com, int nComCards) {
	float eq1, eq2;
	int wc1 = 0, wc2 = 0;	// win count
	int dc = 0;				// draw count
	int count = 0;			// total count
	
	Deck deck;
	deck.RemoveCard(a[0]);
	deck.RemoveCard(a[1]);
	deck.RemoveCard(b[0]);
	deck.RemoveCard(b[1]);

	if (nComCards < 0 || nComCards > 4) {
		cerr << "calc_equity(): Invalid input." << endl;
	}

	for (int i = 0; i < nComCards; i++) {
		deck.RemoveCard(com[i]);
	}

	PokerHand ph1, ph2;

	ph1.add(a[0]);
	ph1.add(a[1]);
	ph2.add(b[0]);
	ph2.add(b[1]);

	for (int i = 0; i < nComCards; i++) {
		ph1.add(com[i]);
		ph2.add(com[i]);
	}

//	deck.print();
	for (int i = 0; i < 100000; i++) {
		Deck dk = deck;
		PokerHand ph1_sim = ph1;
		PokerHand ph2_sim = ph2;

		dk.shuffle();
		// deal the common cards to river
		for (int j = nComCards; j < 5; j++) {
			Card temp = dk.deal();
			ph1_sim.add(temp);
			ph2_sim.add(temp);
		}

	//	ph1_sim.print();
	//	ph2_sim.print();	

		if (ph1_sim > ph2_sim) {
			wc1++;
	//		cout << "hand 1 wins" << endl;
		} else if (ph1_sim == ph2_sim) {
			dc++;
	//		cout << "draw" << endl;
		} else {
			wc2++;
	//		cout << "hand 2 wins" << endl;
		}
		count++;
	}

	eq1 = ((float)wc1 + (float)dc / 2) / (float)count;
	return eq1;

}
