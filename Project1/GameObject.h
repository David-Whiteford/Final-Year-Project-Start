#pragma once
#include <iostream>
#include<SFML/Graphics.hpp>

const int ENEMY_TAG = 0, WALL_TAG = 1;

class GameObject
{
public:
	GameObject() { static int _id; id = _id++; }

	virtual ~GameObject() {}

	void setTag(sf::String t_tag) { m_tag = t_tag; }
	sf::String getTag() { return m_tag; }

	sf::Vector2f position;
	float size;

	int id;


private:
	sf::String m_tag;
};