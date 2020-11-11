#ifndef MOVE_UP_PLAYER_STATE_H
#define MOVE_UP_PLAYER_STATE_H

#include "Player.h"
#include "PlayerState.h"

class PlayerMoveUpState : public PlayerState {
public:
	virtual PlayerState* handleInput(gpp::Events& input);
	virtual void update(Player&);
	virtual void enter(Player&);
	virtual void exit(Player&);
};
#endif