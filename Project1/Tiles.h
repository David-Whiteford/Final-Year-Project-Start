#pragma once
#include "GameObject.h"

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

class Tiles : public GameObject
{
public:
	Tiles(sf::RenderWindow& t_window, float size, sf::Vector2f pos,sf::Sprite t_sprite) :
		m_window(t_window)
	{
		m_sprites = t_sprite;
		m_sprites.setPosition(pos);
		m_sprites.setOrigin(sf::Vector2f(size / 2, size / 2));
		myGameObject = dynamic_cast<GameObject*>(this);
		myGameObject->setTag(WALL_TAG);

	} // end wall
	virtual ~Tiles() {}
	sf::Sprite getSprite() { return m_sprites; }
	void setPosition(sf::Vector2f(t_pos)) { m_sprites.setPosition(t_pos); }
	//void setShape(sf::RectangleShape shapeParam) { m_wall = shapeParam; }
	void draw() { m_window.draw(m_sprites); }
	GameObject* myGameObject;

private:

	sf::RenderWindow& m_window;
	sf::Sprite m_sprites;
	
};