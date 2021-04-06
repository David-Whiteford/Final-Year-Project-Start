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
	bool rayCastToSpriteCol(sf::Vector2f t_rayEnd, sf::Vector2f t_spritePos,sf::Vector2f t_size);
	void render(sf::RenderWindow& t_window);
	bool ViewCheck(sf::View t_view,sf::Vector2f t_pos);
private:
	bool m_colision = false;
	sf::CircleShape testCircle;
	sf::CircleShape testCircle2;
};