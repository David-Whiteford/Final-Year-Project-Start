#include "Collisions.h"

Collisions::Collisions()

{
	testCircle.setRadius(0.5f);
	testCircle.setFillColor(sf::Color::White);
	testCircle2.setRadius(0.5f);
	testCircle2.setFillColor(sf::Color::Green);
	
}

Collisions::~Collisions()
{
}

bool Collisions::pointCircleCol(sf::Vector2f t_point, sf::Vector2f t_circle, int t_radius)
{
	if (t_point.x < t_circle.x + t_radius * 2
		&& t_point.x  > t_circle.x
		&& t_point.y < t_circle.y + t_radius * 2
		&& t_point.y > t_circle.y)
	{
		return true;
	}
	return false;
	
}


bool Collisions::circleToCircleCol(sf::Vector2f t_circle1, sf::Vector2f t_circle2, int t_circleOneRadius, int t_circleTwoRadius)
{
	return false;
}

bool Collisions::rayCastToSpriteCol(sf::Vector2f t_rayEnd, sf::Vector2f t_spritePos, float t_width, float t_height)
{
	testCircle.setPosition(t_spritePos.x, t_spritePos.y);
	testCircle2.setPosition(t_spritePos.x + t_width, t_spritePos.y + t_height);

	if (t_rayEnd.x < t_spritePos.x + t_width
		&& t_rayEnd.x  > t_spritePos.x
		&& t_rayEnd.y < t_spritePos.y + t_height
		&& t_rayEnd.y > t_spritePos.y)
	{
		return true;
	}
	return false;
	

}

void Collisions::render(sf::RenderWindow& t_window)
{
	t_window.draw(testCircle);
	t_window.draw(testCircle2);
}
