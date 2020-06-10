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
float GetImmediateStrength(vector <Card> hand, vector <Card> board, float **range) {
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
	int num_of_cards = board.size();

	if (hand.size() != 2) {
		cerr << "Hand size must be 2!" << endl;
		return -1;
	}

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
	vector <int> index;
	index.resize(2 + num_of_cards);
	index[0] = hand[0].GetRank() - 1 + (hand[0].GetSuit() - 1) * 13;
	index[1] = hand[1].GetRank() - 1 + (hand[1].GetSuit() - 1) * 13;

	for (i = 0; i < num_of_cards; i++) {
		index[i + 2] = board[i].GetRank() - 1 + (board[i].GetSuit() - 1) * 13;
	}

	for (i = 0; i < num_of_cards + 2; i++) {
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
	} */

	// Create 2 pokerhand for comparison
	PokerHand poker1;
	PokerHand poker2; 
	
	poker1.add(hand[0]);
	poker1.add(hand[1]);
	for (i = 0; i < num_of_cards; i++) {
		poker1.add(board[i]);
		poker2.add(board[i]);
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
				cerr << " i = " << i << "; j = " << j << endl;
				cerr << "combos[i][j] = " << combos[i][j] << endl;
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
float GetHandEquity(vector <Card> a, vector <Card> b, vector <Card> board) {
	float eq1, eq2;
	float wins = 0.0;	
	float ties = 0.0;
	float loses = 0.0;
	int count = 0;				// total count
	
	int num_of_cards = board.size();

	Deck deck;
	deck.RemoveCard(a[0]);
	deck.RemoveCard(a[1]);
	deck.RemoveCard(b[0]);
	deck.RemoveCard(b[1]);

	if (num_of_cards < 0 || num_of_cards > 4) {
		cerr << "GetHandEquity(): Invalid input." << endl;
	}

	for (int i = 0; i < num_of_cards; i++) {
		deck.RemoveCard(board[i]);
	}

	PokerHand ph1, ph2;

	ph1.add(a[0]);
	ph1.add(a[1]);
	ph2.add(b[0]);
	ph2.add(b[1]);

	for (int i = 0; i < num_of_cards; i++) {
		ph1.add(board[i]);
		ph2.add(board[i]);
	}

//	deck.print();
	for (int i = 0; i < 100000; i++) {
		Deck dk = deck;
		PokerHand ph1_sim = ph1;
		PokerHand ph2_sim = ph2;

		dk.Shuffle();
		// deal the common cards to river
		for (int j = num_of_cards; j < 5; j++) {
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

float GetEffectiveStrength(vector <Card> hand, vector <Card> board, float **range) {	
	int i, j;
	float strength = 0.0;
	vector <Card> all_cards;
	Deck deck;
	int num_of_cards = board.size();
	int cards_to_deal = 5 - num_of_cards;
	all_cards.resize(5);

	// remove the dead cards
	deck.RemoveCard(hand[0]);
	deck.RemoveCard(hand[1]);
	for (i = 0; i < num_of_cards; i++) {
		deck.RemoveCard(board[i]);
		all_cards[i] = board[i];
	}

	// enumerate the remaining community cards 	
	if (cards_to_deal == 0) {
		return GetImmediateStrength(hand, all_cards, range);
	}
	if (cards_to_deal == 1) {
		// deal the river card
		for (i = 0; i < 46; i++) {	// 52 - 2 - 4 = 46 , total - player's cards - com cards
			all_cards[4] = deck.Deal();
		//	cout << all_cards[4] << " ";
			float t = GetImmediateStrength(hand, all_cards, range);
			strength += t;
		//	cout << t << endl;
		}
		cout << endl;
		return strength / 46.0;		// average 
	}
	if (cards_to_deal == 2) {
		for (i = 0; i < 47; i++) {	// 52 - 2 - 3 = 47, total - player's cards - com cards
			// deal turn card
			all_cards[3] = deck.Deal();
			//cout << "i:" << i << " " << all_cards[3] << endl;
			Deck deck_river = deck;
			for (j = 0; j < 46 - i; j++) {
				// deal river card
				all_cards[4] = deck_river.Deal();
			//cout << "j:" << j << " " << all_cards[4] << endl;
				strength += GetImmediateStrength(hand, all_cards, range);
			}
		}
		return strength / (47 * 46 * 0.5);
	}	

	cerr << "Too few community cards! Can't get effective handstrength." << endl;
	return -1;
}

float GetPreflopStrength(vector <Card> hand) {
	float strength = 0.0;
	int ra = 0, rb = 0;	// rank a, rank b
	
	if (hand.size() != 2) {
		cerr << "Invalid <Card> size" << endl;
		return 0.0;
	}

	ra = (14 - hand[0].GetRank()) % 13;
	rb = (14 - hand[1].GetRank()) % 13;		// A -> 0, K -> 1, Q -> 2 ...
	
	if (hand[0].GetSuit() == hand[1].GetSuit()) {		// suited
		strength = (ra < rb ? preflop_strength[ra][rb] : preflop_strength[rb][ra]);
	} else {		// offsuit
		strength = (ra < rb ? preflop_strength[rb][ra] : preflop_strength[ra][rb]);
	}

	return strength / 200.0;
}


// return 52 x 52 range table, from p_low% to p_high%
float **GetRangeTable(float p_low, float p_high) {
	float **combo = NULL;
	combo = new float*[52];
	for (int i = 0; i < 52; i++) {
		combo[i] = new float[52];
	}

	float range[13][13];
	int th_low = p_low * 200;
	int th_hi = p_high * 200;

	for (int i = 0; i < 13; i++) {
		for (int j = 0; j < 13; j++) {
			if (preflop_strength[i][j] > th_hi) {
				range[i][j] = 0;
			} else if (preflop_strength[i][j] < th_low) {
				range[i][j] = 0; 
			} else {
				range[i][j] = 1;
			}
		}
	}

	// print range table
	for (int i = 0; i < 13; i++) {
		for (int j = 0; j < 13; j++) {
			cout << (range[i][j] == 1) << " ";
		}
		cout << endl;
	}

	// Initialize range table to 0
	for (int i = 0; i < 52; i++) {
		for (int j = 0; j < 52; j++) {
			combo[i][j] = 0.0;
		}
	}

	// Suited, 4 combos each
	for (int i = 0; i < 13; i++) {
		for (int j = i + 1; j < 13; j++) {
			if (i == 0) {	// Ace
				combo[0][13 - j] = range[i][j];
				combo[13][26 - j] = range[i][j];
				combo[26][39 - j] = range[i][j];
				combo[39][52 - j] = range[i][j];
			} else {	// 2 - K
				combo[13 - j][13 - i] = range[i][j];
				combo[26 - j][26 - i] = range[i][j];
				combo[39 - j][39 - i] = range[i][j];
				combo[52 - j][52 - i] = range[i][j];
			}
		}
	}

	// Offsuit, 12 combos each
	for (int i = 1; i < 13; i++) {
		for (int j = 0; j < i - 1; j++) {
			int k = (j == 0 ? 13 : j);		// A
			combo[13 - i][26 - k] = range[i][j];
			combo[13 - i][39 - k] = range[i][j];
			combo[13 - i][52 - k] = range[i][j];

			combo[26 - i][39 - k] = range[i][j];
			combo[26 - i][52 - k] = range[i][j];
			combo[13 - k][26 - i] = range[i][j];

			combo[39 - i][52 - k] = range[i][j];
			combo[13 - k][39 - i] = range[i][j];
			combo[26 - k][39 - i] = range[i][j];

			combo[13 - k][52 - i] = range[i][j];
			combo[26 - k][52 - i] = range[i][j];
			combo[39 - k][52 - i] = range[i][j];
		}
	}

	// pocket pair, 6 combos each
	for (int i = 0; i < 13; i++) {
		int k = (i == 0 ? 13 : i);
		combo[13 - k][26 - k] = range[i][i];
		combo[13 - k][39 - k] = range[i][i];
		combo[13 - k][52 - k] = range[i][i];
		combo[26 - k][39 - k] = range[i][i];
		combo[26 - k][52 - k] = range[i][i];
		combo[39 - k][52 - k] = range[i][i];
	}

	return combo;
}


// free the allocated memory
void DeleteRangeTable(float **range) {
	for (int i = 0; i < 52; i++) {
		delete[] range[i];
	}
	delete[] range;
	cout << "Range table memory freed." << endl;
}
