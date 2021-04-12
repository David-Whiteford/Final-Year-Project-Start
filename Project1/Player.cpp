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
	//set up the rays for the player collisions
	sf::Vector2f playerCentre = sf::Vector2f(getPosition().x + 4, getPosition().y + 8);
	m_raycastUp.setRayVals(playerCentre, sf::Vector2f(0.0f, -1.0f),5);
	m_raycastDown.setRayVals(playerCentre, sf::Vector2f(0.0f, 1.0f), 5);
	m_raycastLeft.setRayVals(playerCentre, sf::Vector2f(-1.0f, 0.0f), 3.5f);
	m_raycastRigth.setRayVals(playerCentre, sf::Vector2f(1.0f, 0.0f), 3.5f);
	//store these in the rays vector
	m_rays.push_back(m_raycastUp.getEndPoint());
	m_rays.push_back(m_raycastDown.getEndPoint());
	m_rays.push_back(m_raycastLeft.getEndPoint());
	m_rays.push_back(m_raycastRigth.getEndPoint());
}
void Player::render(sf::RenderWindow& t_window, sf::View t_view)
{
	//draw all rays that the player needs
	t_window.draw(m_raycastUp.drawArray());
	t_window.draw(m_raycastDown.drawArray());
	t_window.draw(m_raycastLeft.drawArray());
	t_window.draw(m_raycastRigth.drawArray());
	//loop and draw all collision rectangles within player view stored in collisions vector
	for (int i = 0; i < m_debugRects.size(); i++)
	{
		if (m_collisions.ViewCheck(t_view, m_debugRects[i].getPosition()) && DEBUG >= 1)
		{
			t_window.draw(m_debugRects[i]);
		}
	}
	//loop and draw all trigger rectangles within player view stored in m_triggerRects vector
	for (int i = 0; i < m_triggerRects.size(); i++)
	{
		if (m_collisions.ViewCheck(t_view, m_triggerRects[i].getPosition()) && DEBUG >= 1)
		{
			t_window.draw(m_triggerRects[i]);
		}
	}
}
sf::Vector2f Player::getPosition()
{
	//return position of player
	return m_animated_sprite.getPosition();
}
float Player::getCircleRadius()
{
	return 0.0f;
}

void Player::collisionCheck()
{
	//loop through all collision rectangles
	for (int i = 0; i < m_debugRects.size(); i++)
	{
		//check if the ray right has collision and if so set its bool to true
		if (m_collisions.rayCastToSpriteCol(m_raycastRigth.getEndPoint(), m_debugRects[i].getPosition(), m_debugRects[i].getSize())){
			m_collisionRight = true;
		}
		//check if the ray left has collision and if so set its bool to true
		else if (m_collisions.rayCastToSpriteCol(m_raycastLeft.getEndPoint(), m_debugRects[i].getPosition(), m_debugRects[i].getSize())){
			m_collisionLeft = true;
		}
		//check if the ray up has collision and if so set its bool to true
		else if (m_collisions.rayCastToSpriteCol(m_raycastUp.getEndPoint(), m_debugRects[i].getPosition(), m_debugRects[i].getSize())){
			m_collisionUp = true;
		}
		//check if the ray down has collision and if so set its bool to true
		else if (m_collisions.rayCastToSpriteCol(m_raycastDown.getEndPoint(), m_debugRects[i].getPosition(), m_debugRects[i].getSize())){
			m_collisionDown = true;
		}
	}

}

std::string Player::triggerCheck(std::vector<Tiles*>& t_triggerVec)
{
	//set up string and loop through trigger rectangles
	std::string triggerString;
	for (int i = 0; i < t_triggerVec.size(); i++)
	{
		//set the size of the rectangle
		sf::Vector2f size = t_triggerVec[i]->getSize();
		//check if the ray right is in trigger and if so set its bool to true and set the string to the tag of that trigger
		if (m_collisions.rayCastToSpriteCol(m_raycastRigth.getEndPoint(), t_triggerVec[i]->getPosition(), size)) {
			m_inTrigger = true;
			triggerString = t_triggerVec[i]->getTag();
		}
		//check if the ray left is in trigger and if so set its bool to true and set the string to the tag of that trigger
		else if (m_collisions.rayCastToSpriteCol(m_raycastLeft.getEndPoint(), t_triggerVec[i]->getPosition(), size)) {
			m_inTrigger = true;
			triggerString = t_triggerVec[i]->getTag();
		}
		//check if the ray up is in trigger and if so set its bool to true and set the string to the tag of that trigger
		else if (m_collisions.rayCastToSpriteCol(m_raycastUp.getEndPoint(), t_triggerVec[i]->getPosition(), size)) {
			m_inTrigger = true;
			triggerString = t_triggerVec[i]->getTag();
		}
		//check if the ray down is in trigger and if so set its bool to true and set the string to the tag of that trigger
		else if (m_collisions.rayCastToSpriteCol(m_raycastDown.getEndPoint(), t_triggerVec[i]->getPosition(), size)) {
			m_inTrigger = true;
			triggerString = t_triggerVec[i]->getTag();
		}
		//check the string type
		if (triggerString == "FallToDeath"){
			//if this type then display message
			DEBUG_MSG("Player Falls to Death");
		}
		else if (triggerString == "Spike"){
			//if this type then display message
			DEBUG_MSG("Player takes Damage");
		}
		else if (triggerString == "Health"){
			//if this type then display message
			DEBUG_MSG("Player Gets Health");
		}
	}
	return triggerString;
}
void Player::setPosition(sf::Vector2f t_position)
{
	//set position of player sprite
	m_animated_sprite.setPosition(t_position);
}
float Player::getSpeed()
{
	//get the speed of the player
	return m_speed;
}

void Player::setHealthCost(int t_healthCost, bool t_takeDamage)
{
	//set the health cost of the player if it takes damage
	if (t_takeDamage)
	{
		m_playerHealth -= t_healthCost;
	}
	//the player gets health back if not taking damage and is not full health
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
	//set the triggers for exits,death and other types and all colliders
	setObstacles(t_tilemapObstacles);
	setOtherTriggers(t_tilemapObstacles);
	setUniqueObstacles(t_tilemapObstacles);
}

void Player::setUniqueObstacles(std::vector<Tiles*>& t_tilemapObstacles)
{
	//loop through obstacles vector
	for (int i = 0; i < t_tilemapObstacles.size(); i++)
	{
		//create a local rectangle and set up
		sf::RectangleShape rect;
		rect.setOutlineColor(sf::Color::White);
		rect.setOutlineThickness(0.1f);
		rect.setFillColor(sf::Color::Transparent);
		rect.setPosition(t_tilemapObstacles[i]->getPosition().x, t_tilemapObstacles[i]->getPosition().y);
		//check the tag of the collider and if one of the beow tags then set the size to be same size as tile
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
		else if (t_tilemapObstacles[i]->getTag() == "Coffin")
		{
			rect.setSize(sf::Vector2f(16, 32));
		}
		//then push these into debug rects
		m_debugRects.push_back(rect);
	}
}

void Player::setObstacles(std::vector<Tiles*>& t_tilemapObstacles)
{
	//loop through obstacles vector
	for (int i = 0; i < t_tilemapObstacles.size(); i++)
	{
		//create a local rectangle and set up
		sf::RectangleShape rect;
		if (t_tilemapObstacles[i]->getTag() == "Obstacle") {
			//if the type is obstacle then set beow values for size , position and others
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
			//if the type is cave or exit then set beow values for size , position and others
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
	//loop through obstacles vector
	for (int i = 0; i < t_tilemapObstacles.size(); i++)
	{
		//create a local rectangle and set size,color,position etc
		sf::RectangleShape rect;
		rect.setOutlineColor(sf::Color::Green);
		rect.setOutlineThickness(0.1f);
		rect.setFillColor(sf::Color::Transparent);
		rect.setPosition(t_tilemapObstacles[i]->getPosition().x, t_tilemapObstacles[i]->getPosition().y);
		//check the tag of the trigger and if one of the below tags then set the size to be same size as tile
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
		//then push these into trigger vector
		m_triggerRects.push_back(rect);
	}
}

void Player::resetCollisions()
{
	//reset all bools for collisions
	m_collisionDown , m_collisionLeft , m_collisionRight , m_collisionUp = false;
}

void Player::update()
{
	//update for the player sprite
	m_animated_sprite.update();
	m_state->update(*this);
}
sf::Vector2f Player::getOrigin()
{
	return m_player.getOrigin();
}

void Player::handleKeyInput(gpp::Events input)
{
	//set up a state 
	PlayerState* state = m_state->handleInput(input);
	//changes the state eg exits state and enters new state
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
	//returns the frame for the animated sprite
	int frame = m_animated_sprite.getCurrentFrame();
	m_animated_sprite.setTextureRect(m_animated_sprite.getFrame(frame));
	return m_animated_sprite;
}

void Player::setAnimatedSprite(AnimatedSprite& animated_sprite) 
{
	//sets the animated sprite
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