#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Events.h"
#include "PlayerState.h"
#include "AnimatedSprite.h"

class Player
{
public:
	Player(const AnimatedSprite&);
	~Player();

	int getWidth();
	int getHeigth();
	
	sf::Vector2f getPosition();
	void setPosition(sf::Vector2f t_position);
	void playerMovement(double dt);
	AnimatedSprite& getAnimatedSprite();
	AnimatedSprite& getAnimatedSpriteFrame();
	void setAnimatedSprite(AnimatedSprite&);
	PlayerState* getPlayerState();
	void setPlayerState(PlayerState*);
	float getCircleRadius();
	void BoundryControl(sf::VideoMode desktop);
	void init();
	void update();
	void setUpPlayer();
	sf::Vector2f getOrigin();
	void render(sf::RenderWindow& t_window);
	void handleKeyInput(gpp::Events input);
	float getSpeed();

private:
	PlayerState* m_state;
	AnimatedSprite m_animated_sprite;
	sf::Texture texture;
	sf::Sprite m_player;
	float m_speed = 0.5f;
	int m_size = 16;
};
#endif