#include "Events.h"
#include "IdleState.h"
#include "PlayerMoveRight.h"
#include "PLayerMoveLeft.h"
#include "PLayerMoveUp.h"
#include "PlayerMoveDown.h"

PlayerState* IdlePlayerState::handleInput(gpp::Events& input)
{
	//handles the input thats and event passed into the function checks the new input
	if (input.getCurrent() == gpp::Events::Event::PLAYERMOVERIGTH)
	{
		//only for the debug output message below
		if (DEBUG == 2)
		{
			DEBUG_MSG("IdlePlayerState -> PlayerMoveRigthState");
		}
		//return the new state
		return new PlayerMoveRigthState();
	}
	//check all inputs and change state depending on the state
	else if (input.getCurrent() == gpp::Events::Event::PLAYERMOVELEFT)
	{
		if (DEBUG == 2)
		{
			DEBUG_MSG("IdlePlayerState -> PlayerMoveLeftState");
		}
		return new PlayerMoveLeftState();
	}
	else if (input.getCurrent() == gpp::Events::Event::PLAYERMOVEUP)
	{
		if (DEBUG == 2)
		{
			DEBUG_MSG("IdlePlayerState -> PlayerMoveUpState");
		}
		return new PlayerMoveUpState();
	}
	else if (input.getCurrent() == gpp::Events::Event::PLAYERMOVEDOWN)
	{
		if (DEBUG == 2)
		{
			DEBUG_MSG("IdlePlayerState -> PlayerMoveDownState");
		}
		return new PlayerMoveDownState();
	}
	return nullptr;
}
void IdlePlayerState::update(Player& player) {}
void IdlePlayerState::enter(Player& player)
{
	//message for entering state
	if (DEBUG == 2)
	{
		DEBUG_MSG("Entering IdlePlayerState");
	}
	//clear frames and add new frames for the sprite and each renctangle of the sprite
	player.getAnimatedSprite().clearFrames();
	player.getAnimatedSprite().addFrame(sf::IntRect(80, 0, 16, 32));
	player.getAnimatedSprite().addFrame(sf::IntRect(96, 0, 16, 32));
	player.getAnimatedSprite().addFrame(sf::IntRect(112, 0, 16, 32));

	player.getAnimatedSprite().setTime(seconds(0.2f));
}
void IdlePlayerState::exit(Player& player)
{
	//message for exiting state
	if (DEBUG == 2)
	{
		DEBUG_MSG("Exiting IdlePlayerState");
	}

}