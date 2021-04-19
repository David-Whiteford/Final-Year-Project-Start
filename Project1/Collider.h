#ifndef COLLIDER_H
#define COLLIDER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Collisions.h"

class Collider
{
public:
	Collider(sf::Vector2f t_pos, std::string t_tag,sf::Vector2f t_size);
	~Collider();
	void setUpBoxCollider();
	void draw(sf::RenderWindow& t_window, sf::View t_view);
	sf::Vector2f getPosition()
	{
		//return collider position
		return m_pos;
	}
	sf::Vector2f getSize()
	{
		//return collider size
		return m_size;
	}
private:
	//values nneded to set up the collider,e.g the type,size position and tag
	sf::RectangleShape m_boxCollider;
	sf::Vector2f m_pos = sf::Vector2f(0.0f, 0.0f);
	sf::Vector2f m_size = sf::Vector2f(0.0f, 0.0f);
	std::string m_tag = "";
	Collisions m_collisions;
};

#endif