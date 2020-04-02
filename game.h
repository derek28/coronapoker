/**************************************************** 
 * Filename: game.h
 * Last modified: 2020/04/01
 * Description: Blabla
 * Note: needs to consider cases where there are 2 winners(split)
 ***************************************************/

#pragma once
#include <vector>
#include "player.h"
#include "deck.h"

class Game{
public:
    void AddPlayer(Player player, int seat); //Add player to game
    void RemovePlayer(Player player, int seat); //Remove player to game
    void StartAHand(); //Set everyplayer to be in hand by populate deque, push UTG first 
    void MoveBetsToPot(); //put money from bet area to pot area
    void DealNextStreet(); //collect bets and deal cards
    vector<int> GetWinner(); //check hand strength of players and return position ID of winner(s)
    bool IsEndOfStreet(); //check if this betting round is done. maybe this function is not necessary;
    //tentatively, by default, there is iterator it_end which points to SB for postflop or UTG for preflop. When it==it_end, action closes
    //However, when someone bets, assign it_end = it
    // when someone folds, mydeque.erase (it); //what happens to itself?? Idk..
private:
    int big_blind_ = 100;
    int small_blind_ = 50;
    int button_position_ = 0;
    std::deque<Player> players_in_hand;
    Player* it; //iterator that navigate through deque 
    std::vector<Player> players_in_game;
    Dictionary<int, Player> players_at_table = new Dictionary<int, Player>(); //int is seat position
    
}