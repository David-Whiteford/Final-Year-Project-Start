#include "Events.h"
#include "PlayerMoveRight.h"


#include "IdleState.h"
#include "PLayerMoveLeft.h"
#include "PLayerMoveUp.h"
#include "PlayerMoveDown.h"

PlayerState* PlayerMoveRigthState::handleInput(gpp::Events& input)
{
	if (input.getCurrent() == gpp::Events::Event::IDLE)
	{
		if (DEBUG == 2)
		{
			DEBUG_MSG("PlayerMoveRigthState -> IdlePlayerState ");
		}
		return new IdlePlayerState();
	}
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
	if (player.getColRight() == false) {
		m_player.setPosition(sf::Vector2f(1000, 200));
		sf::Vector2f pos = player.getPosition();
		pos.x += 0.1f;

		player.setPosition(pos);
	}
	player.setColUp(false);
	player.setColLeft(false);
	player.setColDown(false);
}
void PlayerMoveRigthState::enter(Player& player)
{
	if (DEBUG == 2)
	{
		DEBUG_MSG("Entering MoveRigthState");
	}
	player.getAnimatedSprite().clearFrames();

	player.getAnimatedSprite().addFrame(sf::IntRect(0, 32, 16, 32));
	player.getAnimatedSprite().addFrame(sf::IntRect(16, 32, 16, 32));
	player.getAnimatedSprite().addFrame(sf::IntRect(32, 32, 16, 32));
	player.getAnimatedSprite().addFrame(sf::IntRect(48, 32, 16, 32));

	player.getAnimatedSprite().setTime(seconds(0.1f));
}
void PlayerMoveRigthState::exit(Player& player)
{
	if (DEBUG == 2)
	{
		DEBUG_MSG("Exiting MoveRightPlayerState");
	}
}