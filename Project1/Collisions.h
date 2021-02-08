#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

class Collisions
{
public:
	Collisions();
	~Collisions();
	bool pointCircleCol(sf::Vector2f t_point, sf::Vector2f t_circle, int t_radius);
	bool circleToCircleCol(sf::Vector2f t_circle1, sf::Vector2f t_circle2, int t_circleOneRadius, int t_circleTwoRadius);
	bool rayCastToSpriteCol(sf::Vector2f t_rayEnd, sf::Vector2f t_spritePos,float t_width,float t_height);
	void render(sf::RenderWindow& t_window);
private:
	bool m_colision = false;
	sf::CircleShape testCircle;
	sf::CircleShape testCircle2;
};