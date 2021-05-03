#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

class Collisions
{
public:
	Collisions();
	~Collisions();
	//set up all functions for collisions
	bool pointCircleCol(sf::Vector2f t_point, sf::Vector2f t_circle, int t_radius);
	bool circleToCircleCol(sf::Vector2f t_circle1, sf::Vector2f t_circle2, int t_circleOneRadius, int t_circleTwoRadius);
	bool rayCastToSpriteCol(sf::Vector2f t_rayEnd, sf::Vector2f t_spritePos,sf::Vector2f t_size);
	bool ViewCheck(sf::View &t_view,sf::Vector2f t_pos);
private:
	bool m_colision = false;

};