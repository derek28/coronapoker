/*
 * Filename: human_player.h
 * Description: derived class from Player
 */

#include "player.h"

class HumanPlayer : public Player
{
public:	
	Action Act(GameState, LegalActions);	
};
