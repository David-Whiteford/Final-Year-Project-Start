#include "Events.h"
#include "PLayerMoveUp.h"

#include "IdleState.h"
#include "PLayerMoveLeft.h"
#include "PlayerMoveRight.h"
#include "PLayerMoveDown.h"

PlayerState* PlayerMoveUpState::handleInput(gpp::Events& input)
{
	//handles the input thats and event passed into the function checks the new input
	if (input.getCurrent() == gpp::Events::Event::IDLE)
	{
		//only for the debug output message below
		if (DEBUG == 2)
		{
			DEBUG_MSG("PlayerMoveUpState -> IdlePlayerState ");
		}
		//return the new state
		return new IdlePlayerState();
	}
	//check all inputs and change state depending on the state
	else if (input.getCurrent() == gpp::Events::Event::PLAYERMOVELEFT)
	{
		if (DEBUG == 2)
		{
			DEBUG_MSG("PlayerMoveUpState -> PlayerMoveLeft");
		}
		return new PlayerMoveLeftState();
	}
	else if (input.getCurrent() == gpp::Events::Event::PLAYERMOVERIGTH)
	{
		if (DEBUG == 2)
		{
			DEBUG_MSG("PlayerMoveUpState -> PlayerMoveRigth");
		}
		return new PlayerMoveRigthState();
	}
	else if (input.getCurrent() == gpp::Events::Event::PLAYERMOVEDOWN)
	{
		if (DEBUG == 2)
		{
			DEBUG_MSG("PlayerMoveUpState -> PlayerMoveDown");
		}
		return new PlayerMoveDownState();
	}
	return nullptr;
}
void PlayerMoveUpState::update(Player& player)
{
	//check collision up is false then move up
	if (player.getColUp() == false) {
		sf::Vector2f pos = player.getPosition();
		pos.y -= 0.1f;

		player.setPosition(pos);
	}
	//set all other collision directions to false
	player.setColRight(false);
	player.setColLeft(false);
	player.setColDown(false);
}
void PlayerMoveUpState::enter(Player& player)
{
	//message for entering state
	if (DEBUG == 2)
	{
		DEBUG_MSG("Entering MoveUpState");
	}
	//clear frames and add new frames for the sprite and each renctangle of the sprite
	player.getAnimatedSprite().clearFrames();
	player.getAnimatedSprite().addFrame(sf::IntRect(0, 64, 16, 32));
	player.getAnimatedSprite().addFrame(sf::IntRect(16, 64, 16, 32));
	player.getAnimatedSprite().addFrame(sf::IntRect(32, 64, 16, 32));
	player.getAnimatedSprite().addFrame(sf::IntRect(48, 64, 16, 32));

	player.getAnimatedSprite().setTime(seconds(0.1f));
}
void PlayerMoveUpState::exit(Player& player)
{
	//message for exiting state
	if (DEBUG == 2)
	{
		DEBUG_MSG("Exiting MoveRightPlayerState");
	}
}




