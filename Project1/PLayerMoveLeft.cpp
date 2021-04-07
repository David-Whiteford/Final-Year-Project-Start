#include "Events.h"
#include "PlayerMoveLeft.h"


#include "IdleState.h"
#include "PlayerMoveRight.h"
#include "PLayerMoveUp.h"
#include "PlayerMoveDown.h"

PlayerState* PlayerMoveLeftState::handleInput(gpp::Events& input)
{
	if (input.getCurrent() == gpp::Events::Event::IDLE)
	{
		if (DEBUG == 2)
		{
			DEBUG_MSG("PlayerMoveLeftState -> IdlePlayerState ");
		}
		return new IdlePlayerState();
	}
	else if (input.getCurrent() == gpp::Events::Event::PLAYERMOVERIGTH)
	{
		if (DEBUG == 2)
		{
			DEBUG_MSG("PlayerMoveLeftState -> PlayerMoveRightState ");
		}
		return new PlayerMoveRigthState();
	}
	else if (input.getCurrent() == gpp::Events::Event::PLAYERMOVEUP)
	{
		if (DEBUG == 2)
		{
			DEBUG_MSG("PlayerMoveLeftState -> PlayerMoveUpState");
		}
		return new PlayerMoveUpState();
	}
	else if (input.getCurrent() == gpp::Events::Event::PLAYERMOVEDOWN)
	{
		if (DEBUG == 2)
		{
			DEBUG_MSG("PlayerMoveLeftState -> PlayerMoveDownState");
		}
		return new PlayerMoveDownState();
	}
	return nullptr;
}
void PlayerMoveLeftState::update(Player& player) 
{
	if (player.getColLeft() == false) {
		sf::Vector2f pos = player.getPosition();
		pos.x -= 0.1f;

		player.setPosition(pos);
	}
	player.setColUp(false);
	player.setColRight(false);
	player.setColDown(false);
}
void PlayerMoveLeftState::enter(Player& player)
{
	if (DEBUG == 2)
	{
		DEBUG_MSG("Entering MoveLeftState");
	}
	player.getAnimatedSprite().clearFrames();

	player.getAnimatedSprite().addFrame(sf::IntRect(0, 96, 16, 32));
	player.getAnimatedSprite().addFrame(sf::IntRect(16, 96, 16, 32));
	player.getAnimatedSprite().addFrame(sf::IntRect(32, 96, 16, 32));
	player.getAnimatedSprite().addFrame(sf::IntRect(48, 96, 16, 32));

	player.getAnimatedSprite().setTime(seconds(0.1f));
}
void PlayerMoveLeftState::exit(Player& player)
{
	if (DEBUG == 2)
	{
		DEBUG_MSG("Exiting MoveRightPlayerState");
	}
}