#include "Collisions.h"

Collisions::Collisions()
{
}

Collisions::~Collisions()
{
}

bool Collisions::pointCircleCol(sf::Vector2f t_point, sf::Vector2f t_circle, int t_radius)
{
	//check if a point is in a circle and return true if so and false if not
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

bool Collisions::rayCastToSpriteCol(sf::Vector2f t_rayEnd, sf::Vector2f t_spritePos, sf::Vector2f t_size)
{
	//check if a ray end point is in a sprite and return true if so and false if not
	if (t_rayEnd.x < t_spritePos.x + t_size.x
		&& t_rayEnd.x  > t_spritePos.x
		&& t_rayEnd.y < t_spritePos.y + t_size.y
		&& t_rayEnd.y > t_spritePos.y)
	{
		return true;
	}
	return false;
}
bool Collisions::ViewCheck(sf::View& t_view, sf::Vector2f t_pos)
{
	//check if the position pass in is within the players view and return true if so and false if not
	t_view.getViewport().left;
	int x1 = t_view.getCenter().x - t_view.getSize().x / 2;
	int x2 = t_view.getCenter().x + t_view.getSize().x / 2;
	int y1 = t_view.getCenter().y - t_view.getSize().y / 2;
	int y2 = t_view.getCenter().y + t_view.getSize().y / 2;
	if (t_pos.x > x1 && t_pos.x< x2 && t_pos.y > y1&& t_pos.y < y2)
	{
		return true;
	}
	return false;
}


