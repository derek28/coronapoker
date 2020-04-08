/* 
 * strength.cpp
 */

#include <iostream>
#include "card.h"
#include "pokerhand.h"
#include "strength.h"

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
	float wins, ties, loses;

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
	int *index = new int(2 + nComCards);	
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

	// print out the range table
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
	}

	// Create 2 pokerhand for comparison
	PokerHand poker1, poker2; 
	poker1.add(ph[0]);
	poker1.add(ph[1]);
	for (i = 0; i < nComCards; i++) {
		poker1.add(com[i]);
		poker2.add(com[i]);
	}

	poker1.print();
	poker2.print();

	// enumerate opponent's combos, # of combos < 1326;
	for (i = 0; i < 52; i++) {
		for (j = i; j < 52; j++) {
			PokerHand op_hd = poker2;
			if (combos[i][j] < 0.0 || combos[i][j] > 1.0) {
				cerr << "Invalid combo weight!" << endl;
				continue;
			}
			if (combos[i][j] != 0) {
				op_hd.add(Card(i % 13 + 1, i / 13 + 1));		// (rank, suit)
				op_hd.add(Card(j % 13 + 1, j / 13 + 1));	
				if (poker1 > op_hd) {
					wins += combos[i][j];
				} else if (poker1 == op_hd) {
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
