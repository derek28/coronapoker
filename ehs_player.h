/*
 * Filename: ehs_player.h
 * Description: derived class from Player
 */

#include "player.h"

class EhsPlayer : public Player
{
public:	
	Action Act(GameState, LegalActions);	
};
