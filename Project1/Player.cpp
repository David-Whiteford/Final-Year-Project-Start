#include "Player.h"
#include "IdleState.h"
#include "Events.h"
#include <stdio.h>

Player::Player(const AnimatedSprite& sprite):m_animated_sprite(sprite)
{
	m_state = new IdlePlayerState();
	m_state->enter(*this);
	//m_animated_sprite.setScale(3.0f, 3.0f);
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

sf::Vector2f Player::getPosition()
{
	return m_animated_sprite.getPosition();
}
float Player::getCircleRadius()
{
	return 0.0f;
}
void Player::BoundryControl(sf::VideoMode desktop)
{
	
}
void Player::setPosition(sf::Vector2f t_position)
{
	m_animated_sprite.setPosition(t_position);
}

void Player::playerMovement(double dt)
{


}
float Player::getSpeed()
{
	return m_speed;
}
void Player::init()
{
	setUpPlayer();
}
void Player::update()
{
	m_animated_sprite.update();
	m_state->update(*this);
}

void Player::setUpPlayer()
{

	if (!texture.loadFromFile("IMAGES//Player.png"))
	{
		// error...
	}
	m_player.setTexture(texture);
	m_player.setPosition(sf::Vector2f(10.f, 500.f));
	m_player.setTextureRect(sf::IntRect(80,0,16,32));
	//m_player.setOrigin(m_player.getPosition().x + m_size, m_player.getPosition().y + m_size);
	
}
sf::Vector2f Player::getOrigin()
{
	return m_player.getOrigin();
}

void Player::render(sf::RenderWindow& t_window)
{
	t_window.draw(m_player);
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