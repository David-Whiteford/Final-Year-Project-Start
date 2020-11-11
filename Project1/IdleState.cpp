#include "Events.h"
#include "IdleState.h"

#include "PlayerMoveRight.h"
#include "PLayerMoveLeft.h"
#include "PLayerMoveUp.h"
#include "PlayerMoveDown.h"

PlayerState* IdlePlayerState::handleInput(gpp::Events& input)
{
	if (input.getCurrent() == gpp::Events::Event::PLAYERMOVERIGTH)
	{
		DEBUG_MSG("IdlePlayerState -> PlayerMoveRigthState");
		return new PlayerMoveRigthState();
	}
	else if (input.getCurrent() == gpp::Events::Event::PLAYERMOVELEFT)
	{
		DEBUG_MSG("IdlePlayerState -> PlayerMoveLeftState");
		return new PlayerMoveLeftState();
	}
	else if (input.getCurrent() == gpp::Events::Event::PLAYERMOVEUP)
	{
		DEBUG_MSG("IdlePlayerState -> PlayerMoveUpState");
		return new PlayerMoveUpState();
	}
	else if (input.getCurrent() == gpp::Events::Event::PLAYERMOVEDOWN)
	{
		DEBUG_MSG("IdlePlayerState -> PlayerMoveDownState");
		return new PlayerMoveDownState();
	}
	return nullptr;
}
void IdlePlayerState::update(Player& player) {}
void IdlePlayerState::enter(Player& player)
{
	DEBUG_MSG("Entering IdlePlayerState");
	player.getAnimatedSprite().clearFrames();

	player.getAnimatedSprite().addFrame(sf::IntRect(80, 0, 16, 32));
	player.getAnimatedSprite().addFrame(sf::IntRect(96, 0, 16, 32));
	player.getAnimatedSprite().addFrame(sf::IntRect(112, 0, 16, 32));

	player.getAnimatedSprite().setTime(seconds(0.2f));
}
void IdlePlayerState::exit(Player& player)
{
	DEBUG_MSG("Exiting IdlePlayerState");

}