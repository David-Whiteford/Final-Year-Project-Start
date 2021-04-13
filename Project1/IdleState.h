#ifndef IDLE_PLAYER_STATE_H
#define IDLE_PLAYER_STATE_H

#include "Player.h"
#include "PlayerState.h"

class IdlePlayerState : public PlayerState {
public:
	//virtual function for derived class to handle player input,entering that state,exiting the state and the update 
	virtual PlayerState* handleInput(gpp::Events& input);
	virtual void update(Player&);
	virtual void enter(Player&);
	virtual void exit(Player&);
};
#endif
