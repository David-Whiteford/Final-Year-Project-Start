#include "Events.h"
#include "PlayerMoveRight.h"


#include "IdleState.h"
#include "PLayerMoveLeft.h"
#include "PLayerMoveUp.h"
#include "PlayerMoveDown.h"

PlayerState* PlayerMoveRigthState::handleInput(gpp::Events& input)
{
	//handles the input thats and event passed into the function checks the new input
	if (input.getCurrent() == gpp::Events::Event::IDLE)
	{
		//only for the debug output message below
		if (DEBUG == 2)
		{
			DEBUG_MSG("PlayerMoveRigthState -> IdlePlayerState ");
		}
		//return the new state
		return new IdlePlayerState();
	}
	//check all inputs and change state depending on the state
	else if (input.getCurrent() == gpp::Events::Event::PLAYERMOVELEFT)
	{
		if (DEBUG == 2)
		{
			DEBUG_MSG("PlayerMoveRigthState -> PlayerMoveLeftState ");
		}
		return new PlayerMoveLeftState();
	}
	else if (input.getCurrent() == gpp::Events::Event::PLAYERMOVEUP)
	{
		if (DEBUG == 2)
		{
			DEBUG_MSG("PlayerMoveRigthState -> PlayerMoveUpState");
		}
		return new PlayerMoveUpState();
	}
	else if (input.getCurrent() == gpp::Events::Event::PLAYERMOVEDOWN)
	{
		if (DEBUG == 2)
		{
			DEBUG_MSG("PlayerMoveRigthState -> PlayerMoveDownState");
		}
		return new PlayerMoveDownState();
	}

	return nullptr;
}
void PlayerMoveRigthState::update(Player& player)
{
	//check collision right is false then move right
	if (player.getColRight() == false) {
		m_player.setPosition(sf::Vector2f(1000, 200));
		sf::Vector2f pos = player.getPosition();
		pos.x += 0.1f;

		player.setPosition(pos);
	}
	//set all other collision directions to false
	player.setColUp(false);
	player.setColLeft(false);
	player.setColDown(false);
}
void PlayerMoveRigthState::enter(Player& player)
{
	//message for entering state
	if (DEBUG == 2)
	{
		DEBUG_MSG("Entering MoveRigthState");
	}
	//clear frames and add new frames for the sprite and each renctangle of the sprite
	player.getAnimatedSprite().clearFrames();
	player.getAnimatedSprite().addFrame(sf::IntRect(0, 32, 16, 32));
	player.getAnimatedSprite().addFrame(sf::IntRect(16, 32, 16, 32));
	player.getAnimatedSprite().addFrame(sf::IntRect(32, 32, 16, 32));
	player.getAnimatedSprite().addFrame(sf::IntRect(48, 32, 16, 32));

	player.getAnimatedSprite().setTime(seconds(0.1f));
}
void PlayerMoveRigthState::exit(Player& player)
{
	//message for exiting state
	if (DEBUG == 2)
	{
		DEBUG_MSG("Exiting MoveRightPlayerState");
	}
}