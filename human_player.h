/*
 * Filename: human_player.h
 * Description: derived class from Player
 */

#include "player.h"
#include "server.h"
#define GUI

class HumanPlayer : public Player
{
public:	
	Action Act(GameState, LegalActions);
	void Notify(GameState);
	void Notify(GameResult);
	void Notify(ActionWithID);
#ifdef GUI
	StreamServer tcpserver;
#endif //endif gui
};
