#include "Player.h"
#include "IdleState.h"
#include "Events.h"
#include <stdio.h>

Player::Player(const AnimatedSprite& sprite):m_animated_sprite(sprite)
{
	m_state = new IdlePlayerState();
	m_state->enter(*this);
	m_animated_sprite.setScale(0.5f, 0.5f);
}

Player::~Player()
{
}

int Player::getWidth()
{
	return 0;
}

int Player::getHeigth()
{
	return 0;
}

void Player::playerRays()
{
	sf::Vector2f playerCentre = sf::Vector2f(getPosition().x + 4, getPosition().y + 8);
	m_raycastUp.setRayVals(playerCentre, sf::Vector2f(0.0f, -1.0f),5);
	m_raycastDown.setRayVals(playerCentre, sf::Vector2f(0.0f, 1.0f), 5);
	m_raycastLeft.setRayVals(playerCentre, sf::Vector2f(-1.0f, 0.0f), 3.5f);
	m_raycastRigth.setRayVals(playerCentre, sf::Vector2f(1.0f, 0.0f), 3.5f);
	m_rays.push_back(m_raycastUp.getEndPoint());
	m_rays.push_back(m_raycastDown.getEndPoint());
	m_rays.push_back(m_raycastLeft.getEndPoint());
	m_rays.push_back(m_raycastRigth.getEndPoint());
}
void Player::render(sf::RenderWindow& t_window, sf::View t_view)
{
	t_window.draw(m_raycastUp.drawArray());
	t_window.draw(m_raycastDown.drawArray());
	t_window.draw(m_raycastLeft.drawArray());
	t_window.draw(m_raycastRigth.drawArray());
	
	for (int i = 0; i < m_debugRects.size(); i++)
	{
		if (m_collisions.ViewCheck(t_view, m_debugRects[i].getPosition()) && DEBUG >= 1)
		{
			t_window.draw(m_debugRects[i]);
		}
	}
	for (int i = 0; i < m_triggerRects.size(); i++)
	{
		if (m_collisions.ViewCheck(t_view, m_triggerRects[i].getPosition()) && DEBUG >= 1)
		{
			t_window.draw(m_triggerRects[i]);
		}
	}
	if (DEBUG >= 1)
	{
		m_collisions.render(t_window);
	}
}
sf::Vector2f Player::getPosition()
{
	return m_animated_sprite.getPosition();
}
float Player::getCircleRadius()
{
	return 0.0f;
}

void Player::collisionCheck()
{
	for (int i = 0; i < m_debugRects.size(); i++)
	{
		
		if (m_collisions.rayCastToSpriteCol(m_raycastRigth.getEndPoint(), m_debugRects[i].getPosition(), m_debugRects[i].getSize())){
			//std::cout << "Collision";
			m_collisionRight = true;
		}
		else if (m_collisions.rayCastToSpriteCol(m_raycastLeft.getEndPoint(), m_debugRects[i].getPosition(), m_debugRects[i].getSize())){
			//std::cout << "Collision";
			m_collisionLeft = true;
		}
		else if (m_collisions.rayCastToSpriteCol(m_raycastUp.getEndPoint(), m_debugRects[i].getPosition(), m_debugRects[i].getSize())){
			//std::cout << "Collision";
			m_collisionUp = true;
		}
		else if (m_collisions.rayCastToSpriteCol(m_raycastDown.getEndPoint(), m_debugRects[i].getPosition(), m_debugRects[i].getSize())){
			//std::cout << "Collision";
			m_collisionDown = true;
		}
	}

}

void Player::triggerCheck(std::vector<Tiles*>& t_triggerVec)
{
	String triggerString;
	for (int i = 0; i < t_triggerVec.size(); i++)
	{
		float size = t_triggerVec[i]->getSize();
		if (m_collisions.rayCastToSpriteCol(m_raycastRigth.getEndPoint(), t_triggerVec[i]->getPosition(), sf::Vector2f(size, size))) {
			m_inTrigger = true;
			triggerString = t_triggerVec[i]->getTag();
		}
		else if (m_collisions.rayCastToSpriteCol(m_raycastLeft.getEndPoint(), t_triggerVec[i]->getPosition(), sf::Vector2f(size, size))) {
			m_inTrigger = true;
			triggerString = t_triggerVec[i]->getTag();
		}
		else if (m_collisions.rayCastToSpriteCol(m_raycastUp.getEndPoint(), t_triggerVec[i]->getPosition(), sf::Vector2f(size, size))) {
			m_inTrigger = true;
			triggerString = t_triggerVec[i]->getTag();
			
		}
		else if (m_collisions.rayCastToSpriteCol(m_raycastDown.getEndPoint(), t_triggerVec[i]->getPosition(), sf::Vector2f(size, size))) {
			m_inTrigger = true;
			triggerString = t_triggerVec[i]->getTag();
		}
		if (triggerString == "FallToDeath"){
			DEBUG_MSG("Player Falls to Death");
		}
		else if (triggerString == "Spike"){
			DEBUG_MSG("Player takes Damage");
		}
		else if (triggerString == "Health"){
			DEBUG_MSG("Player Gets Health");
		}
	}
}
void Player::setPosition(sf::Vector2f t_position)
{
	m_animated_sprite.setPosition(t_position);
}
float Player::getSpeed()
{
	return m_speed;
}

void Player::setHealthCost(int t_healthCost, bool t_takeDamage)
{
	if (t_takeDamage)
	{
		m_playerHealth -= t_healthCost;
	}
	else if (t_takeDamage == false && m_playerHealth < 100)
	{
		m_playerHealth += t_healthCost;
		if (m_playerHealth >= 100)
		{
			m_playerHealth = 100;
		}
	}
	
}

void Player::setDebugRects(std::vector<Tiles*>& t_tilemapObstacles)
{
	setObstacles(t_tilemapObstacles);
	setOtherTriggers(t_tilemapObstacles);
	setUniqueObstacles(t_tilemapObstacles);
}

void Player::setUniqueObstacles(std::vector<Tiles*>& t_tilemapObstacles)
{
	for (int i = 0; i < t_tilemapObstacles.size(); i++)
	{
		sf::RectangleShape rect;
		rect.setOutlineColor(sf::Color::White);
		rect.setOutlineThickness(0.1f);
		rect.setFillColor(sf::Color::Transparent);
		rect.setPosition(t_tilemapObstacles[i]->getPosition().x, t_tilemapObstacles[i]->getPosition().y);
		if (t_tilemapObstacles[i]->getTag() == "Bed") 
		{
			rect.setSize(sf::Vector2f(16, 45));
		}
		else if (t_tilemapObstacles[i]->getTag() == "Statue")	
		{
			rect.setSize(sf::Vector2f(32, 45));
		}
		else if (t_tilemapObstacles[i]->getTag() == "WorshipStone")
		{
			rect.setSize(sf::Vector2f(32, 28));
		}
		else if (t_tilemapObstacles[i]->getTag() == "Table")
		{
			rect.setSize(sf::Vector2f(48, 44));
		}
		else if (t_tilemapObstacles[i]->getTag() == "BookCase")
		{
			rect.setSize(sf::Vector2f(45, 28));
		}
		m_debugRects.push_back(rect);
	}
}

void Player::setObstacles(std::vector<Tiles*>& t_tilemapObstacles)
{
	for (int i = 0; i < t_tilemapObstacles.size(); i++)
	{
		sf::RectangleShape rect;
		if (t_tilemapObstacles[i]->getTag() == "Obstacle") {

			sf::RectangleShape rect;
			rect.setSize(sf::Vector2f(16, 16));
			rect.setOutlineColor(sf::Color::White);
			rect.setOutlineThickness(0.1f);
			rect.setFillColor(sf::Color::Transparent);
			rect.setPosition(t_tilemapObstacles[i]->getPosition().x, t_tilemapObstacles[i]->getPosition().y);
			m_debugRects.push_back(rect);
		}
		else if (t_tilemapObstacles[i]->getTag() == "Cave"
			|| t_tilemapObstacles[i]->getTag() == "Exits")
		{
			sf::RectangleShape rect;
			rect.setSize(sf::Vector2f(16, 16));
			rect.setOutlineColor(sf::Color::Green);
			rect.setOutlineThickness(0.1f);
			rect.setFillColor(sf::Color::Transparent);
			rect.setPosition(t_tilemapObstacles[i]->getPosition().x, t_tilemapObstacles[i]->getPosition().y);
			m_triggerRects.push_back(rect);
		}
	}
}

void Player::setOtherTriggers(std::vector<Tiles*>& t_tilemapObstacles)
{
	for (int i = 0; i < t_tilemapObstacles.size(); i++)
	{
		sf::RectangleShape rect;
		rect.setOutlineColor(sf::Color::Green);
		rect.setOutlineThickness(0.1f);
		rect.setFillColor(sf::Color::Transparent);
		rect.setPosition(t_tilemapObstacles[i]->getPosition().x, t_tilemapObstacles[i]->getPosition().y);
		if (t_tilemapObstacles[i]->getTag() == "FallToDeath")
		{
			rect.setSize(sf::Vector2f(48, 48));
		}
		else if (t_tilemapObstacles[i]->getTag() == "Spike")
		{
			rect.setSize(sf::Vector2f(16, 16));
		}
		else if (t_tilemapObstacles[i]->getTag() == "Health")
		{
			rect.setSize(sf::Vector2f(16, 16));
		}
		m_triggerRects.push_back(rect);
	}
}

void Player::resetCollisions()
{
	m_collisionDown , m_collisionLeft , m_collisionRight , m_collisionUp = false;
}

void Player::update()
{
	m_animated_sprite.update();
	m_state->update(*this);
}
sf::Vector2f Player::getOrigin()
{
	return m_player.getOrigin();
}

void Player::handleKeyInput(gpp::Events input)
{
	PlayerState* state = m_state->handleInput(input);
	
	if (state != NULL) {
		m_state->exit(*this);
		delete m_state;
		m_state = state;
		m_state->enter(*this);
		m_state->update(*this);
		
	}
}
AnimatedSprite& Player::getAnimatedSprite() {
	return m_animated_sprite;
}

AnimatedSprite& Player::getAnimatedSpriteFrame() {
	int frame = m_animated_sprite.getCurrentFrame();
	m_animated_sprite.setTextureRect(m_animated_sprite.getFrame(frame));
	return m_animated_sprite;
}

void Player::setAnimatedSprite(AnimatedSprite& animated_sprite) 
{
	this->m_animated_sprite = animated_sprite;
}

PlayerState* Player::getPlayerState() 
{ 
	return this->m_state; 
}

void Player::setPlayerState(PlayerState* state) 
{ 
	this->m_state = state; 
}