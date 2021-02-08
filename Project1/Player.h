#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Events.h"
#include "PlayerState.h"
#include "AnimatedSprite.h"
#include "RayCast.h"
#include "Tilemap.h"
#include "Tiles.h"
#include "Collisions.h"

class Player
{
public:
	Player(const AnimatedSprite&);
	~Player();

	int getWidth();
	int getHeigth();
	void playerRays();
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
	void collisionCheck(std::vector<Tiles*> &t_tilemapObstacles,sf::String t_type);
	void init();
	void update();
	void setUpPlayer();
	sf::Vector2f getOrigin();
	void render(sf::RenderWindow& t_window);
	void handleKeyInput(gpp::Events input);
	float getSpeed();
	void setHealthCost(int t_healthCost, bool t_takeDamage);
	void setDebugRects(std::vector<Tiles*>& t_tilemapObstacles);
	bool getColUp() { return m_collisionUp; }
	bool getColDown() { return m_collisionDown; }
	bool getColRight() { return m_collisionRight; }
	bool getColLeft() { return m_collisionLeft; }

private:
	Collisions m_collisions;
	Raycast m_raycastUp, m_raycastDown, m_raycastLeft, m_raycastRigth;
	PlayerState* m_state;
	AnimatedSprite m_animated_sprite;
	sf::Texture texture;
	sf::Sprite m_player;
	float m_speed = 0.5f;
	int m_size = 16;
	int m_playerHealth = 100;
	std::vector<sf::Vector2f> m_rays;
	std::vector<sf::RectangleShape> m_debugRects;
	bool m_collisionRight, m_collisionLeft, m_collisionUp, m_collisionDown;
};
#endif