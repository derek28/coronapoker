/* 
 * Filename: ehs_player.h
 * Description: derived class from Player
 */

#include "player.h"

#define IN_WIDTH	104
#define	L1_WIDTH	104
#define L2_WIDTH	104

class EhsPlayer : public Player
{
public:	
	EhsPlayer();
	Action Act(GameState, LegalActions);	
	float PredictEHS(vector <Card>, vector <Card>, float **);
private:
	double M[IN_WIDTH + 1][L1_WIDTH]; 
	double N[L1_WIDTH + 1][L2_WIDTH];
	double O[L2_WIDTH + 1];
};
