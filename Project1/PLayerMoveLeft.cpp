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
		DEBUG_MSG("PlayerMoveLeftState -> IdlePlayerState ");
		return new IdlePlayerState();
	}
	else if (input.getCurrent() == gpp::Events::Event::PLAYERMOVERIGTH)
	{
		DEBUG_MSG("PlayerMoveLeftState -> PlayerMoveRightState ");
		return new PlayerMoveRigthState();
	}
	else if (input.getCurrent() == gpp::Events::Event::PLAYERMOVEUP)
	{
		DEBUG_MSG("PlayerMoveLeftState -> PlayerMoveUpState");
		return new PlayerMoveUpState();
	}
	else if (input.getCurrent() == gpp::Events::Event::PLAYERMOVEDOWN)
	{
		DEBUG_MSG("PlayerMoveLeftState -> PlayerMoveDownState");
		return new PlayerMoveDownState();
	}
	return nullptr;
}
void PlayerMoveLeftState::update(Player& player) {}
void PlayerMoveLeftState::enter(Player& player)
{
	DEBUG_MSG("Entering MoveLeftState");
	player.getAnimatedSprite().clearFrames();

	player.getAnimatedSprite().addFrame(sf::IntRect(0, 96, 16, 32));
	player.getAnimatedSprite().addFrame(sf::IntRect(16, 96, 16, 32));
	player.getAnimatedSprite().addFrame(sf::IntRect(32, 96, 16, 32));
	player.getAnimatedSprite().addFrame(sf::IntRect(48, 96, 16, 32));

	player.getAnimatedSprite().setTime(seconds(0.1f));
}
void PlayerMoveLeftState::exit(Player& player)
{
	DEBUG_MSG("Exiting MoveRightPlayerState");
}