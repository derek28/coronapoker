/*********************************** * Filename: game.cpp * Last Modified: 2020/04/02
 * **********************************/


#include "game.h"
#include <algorithm> //std::copy for array copying, std::max_element

using namespace std;

Game::Game() {
    // // Create shm;
    // const key_t key = 23323;
    // // shmget returns an identifier in shmid 
    // int shmid = shmget(key,1024,0666|IPC_CREAT); 
    // if (shmid ==-1) {
    //  //Error
    //     std::cerr << "something wrong when attach to shared memory ID " << key << std::endl; 
    // }
    // else {
    //     std::cout << "shmid " <<shmid << "is created" << std::endl;
    // }
    // // shmat to attach to shared memory 
    // game_state_shm = (int*)shmat(shmid,NULL,0);
    // *game_state_shm = 2;
    // std::cout << "*game_state_shm is " << *game_state_shm << std::endl;
    // // destroy the shared memory 
    // shmctl(shmid,IPC_RMID,NULL); 
}

Game::~Game() {
    // //detach shared memory
    // shmdt(game_state_shm);
}

int Game::AddPlayer(int seat, int stack_size, int type) {
	if (players[seat]->GetStatus() != STATUS_NO_PLAYER) {
		cerr << "Player already sitting at seat #" << seat << endl;
		cerr << "AddPlayer() failure" << endl;
		return -1;
    }	
	players[seat]->SetType(type);
    players[seat]->SetStatus(1); //#define STATUS_IN_GAME		1
	players[seat]->AddToStack(stack_size);
    players[seat]->SetID(seat);
	
    //Update game_state_
    game_state_.num_player++;
    game_state_.stack_size[seat] = stack_size;
    game_state_.starting_stack_size[seat] = stack_size;
    game_state_.nb_of_buyins[seat] = 0;
    game_state_.player_status[seat] = 1; //1=in-game
	return 0;
}


int Game::AddPlayer(int seat, int stack_size, Player* player) {
    players[seat] = player;
    game_state_.num_player++;
    game_state_.stack_size[seat] = stack_size;
    game_state_.starting_stack_size[seat] = stack_size;
    game_state_.nb_of_buyins[seat] = 0;
    game_state_.player_status[seat] = 1; //1=in-game
    players[seat]->SetID(seat);

    players[seat]->SetStatus(1); //#define STATUS_IN_GAME		1
}

void Game::RemovePlayer(int seat) {
	players[seat]->SetStatus(STATUS_NO_PLAYER);	
}

void Game::ShuffleAndDeal() {

	vector<Card> dealt_cards;
    dealt_cards.reserve(2);
    deck.Regenerate();
	deck.Shuffle();

//    #ifdef DEBUG
//        std::cout << "[Debug] Starting a hand" << std::endl;
//        std::cout << "[Debug] Deck is:";
//        deck.print();
//    #endif
	for (int i = 0; i < NUM_OF_PLAYERS; i++) {
        dealt_cards.clear();
		dealt_cards.push_back(deck.Deal());
		dealt_cards.push_back(deck.Deal());
		players[i]->SetHoleCards(dealt_cards[0]);
        players[i]->SetHoleCards(dealt_cards[1]);
        game_state_.player_hole_cards[i].push_back(dealt_cards[0]);
        game_state_.player_hole_cards[i].push_back(dealt_cards[1]);
        
        #ifdef DEBUG
        std::cout << "[Debug] Hole card of Player Seat" << i << ":";
        std::cout << dealt_cards[0] << dealt_cards[1] << std::endl;
        #endif
	}
}

void Game::PostBlinds() {
    ComputeBlindPos();
    std::cout << "************************************************************" << std::endl;
    std::cout << "[INFO] Hand# " << game_state_.hand_number << std::endl;
    std::cout << "[INFO] Player " << game_state_.sb_pos << " posts SB:" << game_state_.sb_amount << std::endl;
    std::cout << "[INFO] Player " << game_state_.bb_pos << " posts BB:" << game_state_.bb_amount << std::endl;
//No longer needs    players[game_state_.sb_pos]->Bet(game_state_.sb_amount);
//No longer needs    players[game_state_.bb_pos]->Bet(game_state_.bb_amount);
    
    //update game state
    game_state_.total_pot_size += (game_state_.sb_amount + game_state_.bb_amount);
    game_state_.stack_size[game_state_.sb_pos] -= game_state_.sb_amount;
    game_state_.stack_size[game_state_.bb_pos] -= game_state_.bb_amount;
    game_state_.bet_ring[game_state_.sb_pos] = game_state_.sb_amount;
    game_state_.bet_ring[game_state_.bb_pos] = game_state_.bb_amount;
    game_state_.next_player_to_act = FindNextPlayer(game_state_.bb_pos);
    game_state_.aggressor = FindNextPlayer(game_state_.bb_pos); //last to act is BB
    game_state_.raise_amount = game_state_.bb_amount;
}

ActionWithID Game::AskPlayerToAct(LegalActions legal_actions) {

    #ifdef DEBUG
    std::cout << "[DEBUG] Asking player " << game_state_.next_player_to_act << " to act." \
                << "(call:" << legal_actions.LegalCall.amount << "," \
                << "min-raise:" << legal_actions.LegalMinRaise.amount << ")" << std::endl;
    #endif
    ActionWithID player_action_with_id;
    player_action_with_id.ID = game_state_.next_player_to_act;
    player_action_with_id.player_action = players[game_state_.next_player_to_act]->Act(game_state_, legal_actions);

	std::cout << "[INFO] Player " << player_action_with_id.ID << " " \
                    << player_action_with_id.player_action << std::endl;
    return player_action_with_id;
}

void Game::PrintGameState() {
    game_state_.print();
}

void Game::PrintGameStateDebug() {
    game_state_.printdebug();
}

void Game::ComputeBlindPos() {
    if (game_state_.num_player < 2)
        std:cerr << "Player number is incorrect:" << game_state_.num_player << std::endl;

    game_state_.sb_pos = FindNextPlayer(game_state_.btn_pos);
    game_state_.bb_pos = FindNextPlayer(game_state_.sb_pos);

    if (game_state_.num_player == 2) {
        std::swap(game_state_.sb_pos, game_state_.bb_pos);
    }
}

int Game::FindNextPlayer(int i) {
    i++;
    while ( game_state_.player_status[i%9] != 1 && game_state_.player_status[i%9] != 2/*|| IsPlayerAllIn(i) */ ) {
        i++;
    }
    return i%9;
}

bool Game::IsPlayerAllIn(int i) {
    return  ( game_state_.stack_size[i] == 0 ) && ( game_state_.player_status[i] == 1 );
}

void Game::MoveBtn(){
    game_state_.btn_pos = FindNextPlayer(game_state_.btn_pos);
}


bool Game::IsPotUncontested() {
    if (game_state_.num_player_in_hand >= 2) {
        return 0;
    }
    else {
        return 1;
    }
}


bool Game::HasReachShowdown() {
    if (game_state_.current_street == 4)
        return 1;
    else
        return 0;
}

vector<int> Game::GetWinner(){
    vector<int> winner;

    if(game_state_.num_player_in_hand == 1) {
        for (int i = 0 ; i < 9 ; i++){
            if (game_state_.player_status[i] == 1) { // 1: in game, 2: folded
                winner.push_back(i);
            }
        }
    }
    else{
        vector<PokerHand> poker_hands;
        vector<int> showdown_player_index; 
        for (int i = 0 ; i < 9 ; i++) {
            if(game_state_.player_status[i] == 1) { //1: in game
                PokerHand temp;
                temp.add( players[i]->GetHoleCards().at(0) );
                temp.add( players[i]->GetHoleCards().at(1) );

                std::cout << "Player " << i << " shows " << players[i]->GetHoleCards().at(0) << players[i]->GetHoleCards().at(1) << std::endl;
                temp.add( game_state_.community_cards[0]);
                temp.add( game_state_.community_cards[1]);
                temp.add( game_state_.community_cards[2]);
                temp.add( game_state_.community_cards[3]);
                temp.add( game_state_.community_cards[4]);
                poker_hands.push_back(temp);
                showdown_player_index.push_back(i);
            }
        }

        for (int i = 0 ; i < poker_hands.size() ; i++ ) {
            if (winner.size() == 0) {
                winner.push_back(showdown_player_index[i]);
            }
            else if (poker_hands[i] > poker_hands[winner[0]] ) {
                winner.clear();
                winner.push_back(showdown_player_index[i]);
            } 
            else if (poker_hands[i] == poker_hands[winner[0]] ) {
                winner.push_back(showdown_player_index[i]);
            }
        }

    }
    #ifdef DEBUG
        std::cout<< "[DEBUG] Winner: " << winner << std::endl;
    #endif
    return winner;
}


void Game::PayWinner(vector<int> winners){
    
    for(auto const& i: winners) {
        std::cout << "[INFO] Player " << i << " wins " << game_state_.pot_size/winners.size() << std::endl ;
        game_state_.stack_size[i] += game_state_.pot_size/winners.size();
    }


    // Compute bankroll, always rebuy/adjust to 100BB
    for (int iplayer = 0 ; iplayer < 9 ; iplayer++ ) {
        double temp = (double) ( game_state_.stack_size[iplayer] - game_state_.starting_stack_size[iplayer] ) / ( 100 * game_state_.bb_amount) ;
        game_state_.nb_of_buyins[iplayer] += temp;
    }

}

bool Game::IsEndOfStreet() {
    return game_state_.next_player_to_act == game_state_.aggressor ;
}

void Game::CollectMoneyFromBetRing() {

    for (int i = 0 ; i < 9 ; i++ ) {
        if (game_state_.bet_ring[i] == 0)
            continue;
        game_state_.pot_size += game_state_.bet_ring[i];
        game_state_.bet_ring[i] = 0;
    }

    #ifdef DEBUG
    std::cout << "[INFO] Dealer collects money from bet ring and put into pot" << std::endl ;
    #endif
    if (game_state_.pot_size != game_state_.total_pot_size)
        std::cerr << "[ERROR] Pot size: " << game_state_.pot_size  \
                    << " total pot size: " << game_state_.total_pot_size << std::endl;
    std::cout << "[INFO] Pot size is: " << game_state_.pot_size << std::endl;
}


void Game::SetupNextStreet() {

    game_state_.current_street += 1;
    
    int cards_to_deal = 0 ;

    switch (game_state_.current_street) {
        case 1:
            cards_to_deal = 3;
            break;

        case 2:
        case 3:
            cards_to_deal = 1;
            break;
        case 4:
            cards_to_deal = 0;
            break;
        default:
            std::cerr << "[Error] game_state_.current_street is out of bound: " << game_state_.current_street << std::endl;
    }

    #ifdef DEBUG
        std::cout << "[DEBUG] Dealer deals next street: " << game_state_.current_street << std::endl;
    #endif

    for (int i = 0 ; i < cards_to_deal ; i++ ) {
        game_state_.community_cards.push_back(deck.Deal());
    }

    std::cout << "[INFO] ************" << static_cast<StreetName> (game_state_.current_street) <<"************ ";
    for (auto const& card : game_state_.community_cards)
        std::cout << card << ' ';
    std::cout << std::endl;


    if (game_state_.num_player == 2) 
        game_state_.next_player_to_act = game_state_.bb_pos;
    else
        game_state_.next_player_to_act = FindNextPlayer(game_state_.bb_pos);
    game_state_.aggressor = FindNextPlayer(game_state_.btn_pos); 
    game_state_.raise_amount = game_state_.bb_amount;


    //check if all players are all in, if yes, then deal all remaining streets
    if (game_state_.current_street != 4 ) {
        if (HasNoMoreActions())
            SetupNextStreet();
    }
}


void Game::UpdateGameState(ActionWithID ac) {

    switch (ac.player_action.action ) {
        case 0:
            game_state_.num_player_in_hand--;
            game_state_.player_status[ac.ID] = 2;
            break;
        case 1:
            game_state_.stack_size[ac.ID] -= ac.player_action.amount;
            game_state_.bet_ring[ac.ID] += ac.player_action.amount;
            game_state_.total_pot_size += ac.player_action.amount;
            break;
        case 2:
            game_state_.aggressor = ac.ID;
            game_state_.raise_amount = ac.player_action.amount - *std::max_element(game_state_.bet_ring,game_state_.bet_ring+9);
            game_state_.stack_size[ac.ID] -= (ac.player_action.amount - game_state_.bet_ring[ac.ID] ) ;
            game_state_.total_pot_size += (ac.player_action.amount - game_state_.bet_ring[ac.ID] );
            game_state_.bet_ring[ac.ID] = ac.player_action.amount;
            break;
        default:
            std::cerr << "[ERROR] Unknown player action " << ac.player_action.action << " by " << ac.ID << std::endl;
            std::cerr << "default to fold" << std::endl;
            game_state_.num_player_in_hand--;
            game_state_.player_status[ac.ID] = 2;
    }
    //Update game history game_state_.ActionHistory
    switch (game_state_.current_street) {
        case 0:
            game_state_.action_history.preflop.push_back(ac);
            break;
        case 1:
            game_state_.action_history.flop.push_back(ac);
            break;
        case 2:
            game_state_.action_history.turn.push_back(ac);
            break;
        case 3:
            game_state_.action_history.river.push_back(ac);
            break;
        
    }
    
    
    game_state_.next_player_to_act = FindNextPlayer(game_state_.next_player_to_act);

    #ifdef DEBUG
        PrintGameStateDebug();
    #endif
    //Check if there is only 1 player left
    if (IsPotUncontested() ){
        std::cout << "[INFO] reach end of game: pot uncontested" << std::endl;
        vector<int> winners = GetWinner();
        CollectMoneyFromBetRing();
        PayWinner(winners);
        game_state_.current_street = 5;
        return;				
    }
    //If end of street is reached
    if (IsEndOfStreet()) {
        CollectMoneyFromBetRing();
        SetupNextStreet();
    }

    //End of game condition: we reach showdown
    if (HasReachShowdown()) {
        std::cout << "[INFO] reach end of game: showdown" << std::endl;
        vector<int> winners = GetWinner();
        PayWinner(winners);
        game_state_.current_street = 5;
        return;
    }

}

void Game::RemovePlayerCard() {
    for (int i = 0 ; i < game_state_.num_player; i++ ){
        players[i]->ResetHoleCards();
        game_state_.player_hole_cards[i].clear();
    }
}

void Game::CleanCommunityCard() {
    game_state_.community_cards.clear();
}

void Game::ResetGameState() {

	CleanCommunityCard();
	RemovePlayerCard();
    game_state_.hand_number++;
    game_state_.current_street = 0;
    game_state_.pot_size = 0;
    game_state_.total_pot_size = 0;
    game_state_.num_player_in_hand = game_state_.num_player;

    game_state_.action_history.preflop.clear();
    game_state_.action_history.flop.clear();
    game_state_.action_history.turn.clear();
    game_state_.action_history.river.clear();
    //std::copy(game_state_.stack_size, game_state_.stack_size+9, game_state_.starting_stack_size);
        
    for (int iplayer = 0 ; iplayer < 9 ; iplayer++ ) {
        if (iplayer == 0 or iplayer == 1 )
            game_state_.player_status[iplayer] = 1;
    }
    // Always reset stacksize
    std::copy(game_state_.starting_stack_size, game_state_.starting_stack_size+9, game_state_.stack_size); 
}


ActionWithID Game::VerifyAction(ActionWithID ac, LegalActions legal_actions) {
    if ( ac.player_action.action == 1 ) {
        if (ac.player_action.amount != legal_actions.LegalCall.amount ) {
                std::cerr << "[WARNING] call amount is invalid: " << ac.player_action.amount  \
                        << " ,should be : " <<  legal_actions.LegalCall.amount \
                        << " .Default to legal call/check size" << std::endl;
                ac.player_action.amount = legal_actions.LegalCall.amount;
            }

    } else if ( ac.player_action.action == 2) {
        if ( ac.player_action.amount < legal_actions.LegalMinRaise.amount ) {
            std::cerr << "[WARNING] raise amount is invalid: " << ac.player_action.amount  \
                      << " ,should be at least: " <<  legal_actions.LegalMinRaise.amount \
                      << " .Default to fold" << std::endl; 
            ac.player_action.amount = 0;
            ac.player_action.action = 0;
        }

        if ( ac.player_action.amount > legal_actions.LegalMaxRaise.amount ) {
            std::cerr << "[WARNING] raise amount is invalid: " << ac.player_action.amount  \
                      << " ,should be at most: " <<  legal_actions.LegalMaxRaise.amount  \
                      << " .Default to fold" << std::endl; 
            ac.player_action.amount = 0;
            ac.player_action.action = 0;
        }

    }
    return ac;
}

LegalActions Game::GetAllLegalActions() {
    //Correct ac if it is invalid
    int biggest_bet_amount = *std::max_element( game_state_.bet_ring, game_state_.bet_ring+9 );
    LegalActions legal_actions;
    legal_actions.LegalFold.action = 0;
    legal_actions.LegalFold.amount = 0;

    legal_actions.LegalCall.action = 1;
    legal_actions.LegalCall.amount = std::min ( biggest_bet_amount \
                                    - game_state_.bet_ring[game_state_.next_player_to_act] \
                                    , game_state_.stack_size[game_state_.next_player_to_act] );

    legal_actions.LegalMinRaise.action = 2;
    legal_actions.LegalMinRaise.amount = biggest_bet_amount + game_state_.raise_amount;

    legal_actions.LegalMaxRaise.action = 2;
    legal_actions.LegalMaxRaise.amount = game_state_.stack_size[game_state_.next_player_to_act] ;

    if ( biggest_bet_amount >= game_state_.stack_size[game_state_.next_player_to_act] 
                                + game_state_.bet_ring[game_state_.next_player_to_act] ) {
        legal_actions.LegalMinRaise.amount = -1 ;
        legal_actions.LegalMaxRaise.amount = -1 ;
    }
        
    if ( legal_actions.LegalMinRaise.amount > legal_actions.LegalMaxRaise.amount ) {
        legal_actions.LegalMinRaise.amount = game_state_.stack_size[game_state_.next_player_to_act] ;
        legal_actions.LegalMaxRaise.amount = game_state_.stack_size[game_state_.next_player_to_act] ;      
    }

    return legal_actions;
}


bool Game::HasNoMoreActions() {
    int actionable_player= game_state_.num_player_in_hand;
    for (int i = 0 ; i < 9 ; i++ ) {
        if (IsPlayerAllIn(i) )
            actionable_player--;
    }
    if ( actionable_player >=0 && actionable_player < 2)
        return 1;
    else
        return 0;
}


void Game::PrintResult() {
    std::cout << "After " << game_state_.hand_number << " hands, " << std::endl;
    std::cout << "Player 1 perf: " << game_state_.nb_of_buyins[0] * 100000 / game_state_.hand_number << "mbb/hand" << std::endl;
    std::cout << "Player 2 perf: " << game_state_.nb_of_buyins[1] * 100000 / game_state_.hand_number << "mbb/hand" << std::endl;
}

bool Game::IsCurrentHandFinished() {
    return (game_state_.current_street == 5); // 3=river 4=showdown 5=finished
}

void Game::SetNumOfHands(int nhands) {
    num_of_hands_to_run_ = nhands;
}

void Game::Start() {
    for (int ihand = 0 ; ihand < num_of_hands_to_run_ ; ihand++ ) {
		ResetGameState();
		PostBlinds();
        ShuffleAndDeal();
        
		while ( !IsCurrentHandFinished() ) { //it breaks when a hand finishes
			//Ask player (pointed by nextplayertoact) to act
			LegalActions legal_ac = GetAllLegalActions();
			ActionWithID ac = AskPlayerToAct(legal_ac);
			ac = VerifyAction(ac, legal_ac);
			UpdateGameState(ac);
		}
		//game.PrintGameState();
		MoveBtn();
	}

}