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
	//functions to return and set the size of player,the sprite,player collision rays,position,animated frames 
	int getWidth();
	int getHeigth();
	void playerRays();
	sf::Vector2f getPosition();
	void setPosition(sf::Vector2f t_position);
	AnimatedSprite& getAnimatedSprite();
	AnimatedSprite& getAnimatedSpriteFrame();
	void setAnimatedSprite(AnimatedSprite&);
	PlayerState* getPlayerState();
	void setPlayerState(PlayerState*);
	float getCircleRadius();
	//collision and trigger check functions
	void collisionCheck(std::vector<Tiles*>& t_tilesVec);
	std::string triggerCheck(std::vector<Tiles*>& t_triggerVec);
	void update();
	sf::Vector2f getOrigin();
	void render(sf::RenderWindow& t_window,sf::View &t_view);
	void handleKeyInput(gpp::Events input);
	float getSpeed();
	//health and debug rectangles
	void setHealthCost(int t_healthCost, bool t_takeDamage);
	//get all collision bools
	bool getColUp() { return m_collisionUp; }
	bool getColDown() { return m_collisionDown; }
	bool getColRight() { return m_collisionRight; }
	bool getColLeft() { return m_collisionLeft; }
	//set all collision bools functions
	void setColUp(bool t_colUp) { m_collisionUp = t_colUp; }
	void setColDown(bool t_colDown) { m_collisionDown = t_colDown; }
	void setColRight(bool t_colRight) { m_collisionRight = t_colRight; }
	void setColLeft(bool t_colLeft) { m_collisionLeft = t_colLeft; }
	void resetCollisions();
	//get and set triggers bool functions
	void setIfInTrigger(bool t_inTrigger) { m_inTrigger = t_inTrigger; }
	bool getIfInTrigger() { return m_inTrigger; }

	//get and set if in death trigger funtion
	void setIfInKilledTrigger(bool t_inDeathTrigger) { m_inDeathTrigger = t_inDeathTrigger; }
	bool getIfInKilledTrigger() { return m_inDeathTrigger; }
private:
	//sets up the collision ,rays,playerstate,animated sprite objects
	Collisions m_collisions;
	Raycast m_raycastUp, m_raycastDown, m_raycastLeft, m_raycastRigth;
	PlayerState* m_state;
	AnimatedSprite m_animated_sprite;
	//player textur and sprite
	sf::Texture texture;
	sf::Sprite m_player;
	//player speed, size,health
	float m_speed = 0.5f;
	int m_size = 16;
	int m_playerHealth = 100; 
	//vector to store rays,debug collsions,and triggers
	std::vector<sf::Vector2f> m_rays;
	//all rays needed
	bool m_collisionRight, m_collisionLeft, m_collisionUp, m_collisionDown;
	//bools to determine if player in trigger,death trigger
	bool m_inTrigger = false;
	bool m_inDeathTrigger = false;
};
#endif