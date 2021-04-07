#include "Events.h"
#include "PlayerMoveDown.h"

#include "IdleState.h"
#include "PlayerMoveRight.h"
#include "PLayerMoveLeft.h"
#include "PLayerMoveUp.h"

PlayerState* PlayerMoveDownState::handleInput(gpp::Events& input)
{
	if (input.getCurrent() == gpp::Events::Event::IDLE)
	{
		if (DEBUG == 2)
		{
			DEBUG_MSG("PlayerMoveDownState -> IdlePlayerState ");
		}
		return new IdlePlayerState();
	}
	else if (input.getCurrent() == gpp::Events::Event::PLAYERMOVELEFT)
	{
		if (DEBUG == 2)
		{
			DEBUG_MSG("PlayerMoveDownState -> PlayerMoveLeft");
		}
		return new PlayerMoveLeftState();
	}
	else if (input.getCurrent() == gpp::Events::Event::PLAYERMOVERIGTH)
	{
		if (DEBUG == 2)
		{
			DEBUG_MSG("PlayerMoveDownState -> PlayerMoveRigth");
		}
		return new PlayerMoveRigthState();
	}
	else if (input.getCurrent() == gpp::Events::Event::PLAYERMOVEUP)
	{
		if (DEBUG == 2)
		{
			DEBUG_MSG("PlayerMoveDownState -> PlayerMoveUp");
		}
		return new PlayerMoveUpState();
	}
	return nullptr;
}
void PlayerMoveDownState::update(Player& player) 
{
	if (player.getColDown() == false) {
		sf::Vector2f pos = player.getPosition();
		pos.y += 0.1f;

		player.setPosition(pos);
	}
	player.setColUp(false);
	player.setColRight(false);
	player.setColLeft(false);

}
void PlayerMoveDownState::enter(Player& player)

{

	if (DEBUG == 2)
	{
		DEBUG_MSG("Entering MoveDownState");
	}
	player.getAnimatedSprite().clearFrames();

	player.getAnimatedSprite().addFrame(sf::IntRect(0, 0, 16, 32));
	player.getAnimatedSprite().addFrame(sf::IntRect(16, 0, 16, 32));
	player.getAnimatedSprite().addFrame(sf::IntRect(32, 0, 16, 32));
	player.getAnimatedSprite().addFrame(sf::IntRect(48, 0, 16, 32));

	player.getAnimatedSprite().setTime(seconds(0.1f));
}
void PlayerMoveDownState::exit(Player& player)
{
	if (DEBUG == 2)
	{
		DEBUG_MSG("Exiting MoveRightPlayerState");
	}
}
