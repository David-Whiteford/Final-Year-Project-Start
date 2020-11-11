#ifndef MOVE_LEFT_PLAYER_STATE_H
#define MOVE_LEFT_PLAYER_STATE_H

#include "Player.h"
#include "PlayerState.h"



class PlayerMoveLeftState : public PlayerState {
public:
	virtual PlayerState* handleInput(gpp::Events& input);
	virtual void update(Player&);
	virtual void enter(Player&);
	virtual void exit(Player&);
};
#endif