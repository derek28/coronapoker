/* 
 * strength.cpp
 */

#include <iostream>
#include "card.h"
#include "pokerhand.h"
#include "strength.h"
#include "deck.h"

/*
 * input argument:
 *	@ph player's poker hand, Card[2]
 *	@com community card, Card[nComCards]
 *	@nComCards, # of community cards, int
 *	@range opponent's range, float[52][52] , if range == NULL, then range = all
 * output: 
 *	immediate hand rank (IHR), IHR = (wins + (ties/2))/(wins + ties + losses)
 */
float GetHandStrength(Card *ph, Card *com, int nComCards, float **range) {
	/*********************************************************
	 *    As 2s 3s ... Ah 2h ... Ac 2c ... Ad 2d .. Kd
	 * As 0  1  1  ... 1  1  ... 1  1  ... 1  1  .. 1
	 * 2s 1  0  1  ... 1  1  ... 1  1  ... 1  1  .. 1
	 * ... ........................................ 1
	 * ... ........................................ 1
	 * ... ........................................ 1
	 * Qd  .......................................0 1
	 * Kd  .......................................1 0
	 *******************************************************/
	float combos[52][52];
	int i, j;
	float wins = 0.0, ties = 0.0, loses = 0.0;

/*	cout << "Input Information:" << endl;
	cout << "My Hand: " << ph[0] << " " << ph[1] << endl;
	cout << "Board: ";
	for (i = 0; i < nComCards; i++) {
		cout << com[i] << " ";
	}
	cout << endl;*/

	if (range != NULL) {
		// copy from range[52][52], do not modify range[][]
		for (i = 0; i < 52; i++) {
			for (j = 0; j < 52; j++) {
				combos[i][j] = range[i][j];
			}
		}
	} else {
		// assume 100% range, diagonal elements should be 0.0
		for (i = 0; i < 52; i++) {
			for (j = 0; j < 52; j++) {
				combos[i][j] = 1.0;
			}
		}
		for (i = 0; i < 52; i++) {
			combos[i][i] = 0.0;
		}
	}

	// combo removal given input hands
	int *index = new int[2 + nComCards];	
	index[0] = ph[0].GetRank() - 1 + (ph[0].GetSuit() - 1) * 13;
	index[1] = ph[1].GetRank() - 1 + (ph[1].GetSuit() - 1) * 13;

	for (i = 0; i < nComCards; i++) {
		index[i + 2] = com[i].GetRank() - 1 + (com[i].GetSuit() - 1) * 13;
	}

	for (i = 0; i < nComCards + 2; i++) {
		for (j = 0; j < 52; j++) {
			combos[index[i]][j] = 0.0;
			combos[j][index[i]] = 0.0;
		}
	}

	/*print out the range table
	for (i = 0; i < 52; i++) {
		if (i % 13 == 0) {
			cout << endl;
		}
		for (j = 0; j < 52; j++) {
			if (j % 13 == 0) {
				cout << " ";
			}
			cout << combos[i][j] << " ";
		}
		cout << endl;
	}	*///////////////////////////

	// Create 2 pokerhand for comparison
	PokerHand poker1;
	PokerHand poker2; 
	
	poker1.add(ph[0]);
	poker1.add(ph[1]);
	for (i = 0; i < nComCards; i++) {
		poker1.add(com[i]);
		poker2.add(com[i]);
	}

	//poker1.print();
	//poker2.print();

	// enumerate opponent's combos, # of combos < 1326;
	//PokerHand opp_hand;
	for (i = 0; i < 52; i++) {
		for (j = i; j < 52; j++) {
			PokerHand opp_hand = poker2;
			if (combos[i][j] < 0.0 || combos[i][j] > 1.0) {
				cerr << "Invalid combo weight!" << endl;
				continue;
			}
			if (combos[i][j] != 0) {
				opp_hand.add(Card(i % 13 + 1, i / 13 + 1));		// (rank, suit)
				opp_hand.add(Card(j % 13 + 1, j / 13 + 1));	
				if (poker1 > opp_hand) {
					wins += combos[i][j];
				} else if (poker1 == opp_hand) {
					ties += combos[i][j];
				} else {
					loses += combos[i][j];
				}
			}	
			
		}
	}

	float result = (wins + ties / 2) / (wins + ties + loses);
	return result;
}

/* 
 * Calculate equity of 2 players. *
 * Input Arguments:
 * @a: hand a, type Card[2]
 * @b: hand b, type Card[2]
 * @com: community cards, type Card*
 * #nComCards: # of community cards, int
 */
float GetHandEquity(Card *a, Card *b, Card *com, int nComCards) {
	float eq1, eq2;
	float wins = 0.0;	
	float ties = 0.0;
	float loses = 0.0;
	int count = 0;				// total count
	
	Deck deck;
	deck.RemoveCard(a[0]);
	deck.RemoveCard(a[1]);
	deck.RemoveCard(b[0]);
	deck.RemoveCard(b[1]);

	if (nComCards < 0 || nComCards > 4) {
		cerr << "GetHandEquity(): Invalid input." << endl;
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

		dk.Shuffle();
		// deal the common cards to river
		for (int j = nComCards; j < 5; j++) {
			Card temp = dk.Deal();
			ph1_sim.add(temp);
			ph2_sim.add(temp);
		}

	//	ph1_sim.print();
	//	ph2_sim.print();	

		if (ph1_sim > ph2_sim) {
			wins += 1.0;
	//		cout << "hand 1 wins" << endl;
		} else if (ph1_sim == ph2_sim) {
			ties += 1.0;
	//		cout << "draw" << endl;
		} else {
			loses += 1.0;
	//		cout << "hand 2 wins" << endl;
		}
		count++;
	}

	return ((wins + ties / 2) / (float)count);
}

float GetEffectiveStrength(Card *ph, Card *com, int nComCards, float **range) {	
	int i, j;
	float strength = 0.0;
	int cards_to_deal = 5 - nComCards;
	Card all_cards[5];
	Deck deck;

	// remove the dead cards
	deck.RemoveCard(ph[0]);
	deck.RemoveCard(ph[1]);
	for (i = 0; i < nComCards; i++) {
		deck.RemoveCard(com[i]);
		all_cards[i] = com[i];
	}

	// enumerate the remaining community cards 	
	if (cards_to_deal == 0) {
		return GetHandStrength(ph, all_cards, 5, range);
	}
	if (cards_to_deal == 1) {
		// deal the river card
		for (i = 0; i < 46; i++) {	// 52 - 2 - 4 = 46 , total - player's cards - com cards
			all_cards[4] = deck.deal();
		//	cout << all_cards[4] << " ";
			float t = GetHandStrength(ph, all_cards, 5, range);
			strength += t;
		//	cout << t << endl;
		}
		cout << endl;
		return strength / 46.0;		// average 
	}
	if (cards_to_deal == 2) {
		for (i = 0; i < 47; i++) {	// 52 - 2 - 3 = 47, total - player's cards - com cards
			// deal turn card
			all_cards[3] = deck.deal();
			//cout << "i:" << i << " " << all_cards[3] << endl;
			Deck deck_river = deck;
			for (j = 0; j < 46 - i; j++) {
				// deal river card
				all_cards[4] = deck_river.deal();
			//cout << "j:" << j << " " << all_cards[4] << endl;
				strength += GetHandStrength(ph, all_cards, 5, range);
			}
		}
		return strength / (47 * 46 * 0.5);
	}	

	cerr << "Too few community cards! Can't get effective handstrength." << endl;
	return -1;
}
