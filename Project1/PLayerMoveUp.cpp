#include "Events.h"
#include "PLayerMoveUp.h"

#include "IdleState.h"
#include "PLayerMoveLeft.h"
#include "PlayerMoveRight.h"
#include "PLayerMoveDown.h"

PlayerState* PlayerMoveUpState::handleInput(gpp::Events& input)
{
	if (input.getCurrent() == gpp::Events::Event::IDLE)
	{
		DEBUG_MSG("PlayerMoveUpState -> IdlePlayerState ");
		return new IdlePlayerState();
	}
	else if (input.getCurrent() == gpp::Events::Event::PLAYERMOVELEFT)
	{
		DEBUG_MSG("PlayerMoveUpState -> PlayerMoveLeft");
		return new PlayerMoveLeftState();
	}
	else if (input.getCurrent() == gpp::Events::Event::PLAYERMOVERIGTH)
	{
		DEBUG_MSG("PlayerMoveUpState -> PlayerMoveRigth");
		return new PlayerMoveRigthState();
	}
	else if (input.getCurrent() == gpp::Events::Event::PLAYERMOVEDOWN)
	{
		DEBUG_MSG("PlayerMoveUpState -> PlayerMoveDown");
		return new PlayerMoveDownState();
	}
	return nullptr;
}
void PlayerMoveUpState::update(Player& player)
{
	if (player.getColUp() == false) {
		sf::Vector2f pos = player.getPosition();
		pos.y -= 0.03f;

		player.setPosition(pos);
	}
}
void PlayerMoveUpState::enter(Player& player)
{
	DEBUG_MSG("Entering MoveUpState");
	player.getAnimatedSprite().clearFrames();

	player.getAnimatedSprite().addFrame(sf::IntRect(0, 64, 16, 32));
	player.getAnimatedSprite().addFrame(sf::IntRect(16, 64, 16, 32));
	player.getAnimatedSprite().addFrame(sf::IntRect(32, 64, 16, 32));
	player.getAnimatedSprite().addFrame(sf::IntRect(48, 64, 16, 32));

	player.getAnimatedSprite().setTime(seconds(0.1f));
}
void PlayerMoveUpState::exit(Player& player)
{
	DEBUG_MSG("Exiting MoveRightPlayerState");
	
}




