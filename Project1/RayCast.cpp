#include "RayCast.h"

Raycast::Raycast()
{
	m_rayLines[0].color = sf::Color::White;
	m_rayLines[1].color = sf::Color::White;
}

Raycast::~Raycast()
{
}

void Raycast::hit(sf::Vector2f t_targetPos, float t_targetSize)
{
}

void Raycast::setRayVals(sf::Vector2f t_startPos, sf::Vector2f t_direction, float t_length)
{
	m_position = t_startPos;
	m_direction = t_direction;
	m_rayLength = t_length;
	m_rayEndPos = m_position + (m_direction * (m_rayLength));
	//std::cout << "Ray End " << m_rayEndPos.x << " , " << m_rayEndPos.y << std::endl;
}

sf::VertexArray Raycast::drawArray()
{
	sf::VertexArray ray(sf::LinesStrip, 2);
	ray[0].position = m_position;
	ray[1].position = m_position + (m_direction * (m_rayLength));
	return ray;
}

void Raycast::addObjectsHit()
{
}

bool Raycast::intersectsRect(sf::Vector2f p1, sf::Vector2f p2)
{
	return false;
}

sf::Vector2f Raycast::getEndPoint()
{
	
	return m_rayEndPos;
}
