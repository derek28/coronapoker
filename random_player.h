
/**************************************************** 
 * Filename: random_player.h
 * Last modified: 2020/04/01
 * Description: Blabla
 ***************************************************/



#pragma once

#include "player.h"

class RandomPlayer: public Player 
{
    public: 
    	Action Act(GameState, LegalActions);
};