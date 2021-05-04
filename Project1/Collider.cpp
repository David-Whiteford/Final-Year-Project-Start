#include "Collider.h"

Collider::Collider(sf::Vector2f t_pos, std::string t_tag, sf::Vector2f t_size)
{
	m_pos = t_pos;
	m_tag = t_tag;
	m_size = t_size;
	setUpBoxCollider();
}

Collider::~Collider()
{
}

void Collider::setUpBoxCollider()
{
	//rectangle and set up size,color etc
	m_boxCollider.setOutlineColor(sf::Color::White);
	m_boxCollider.setOutlineThickness(0.1f);
	m_boxCollider.setFillColor(sf::Color::Transparent);
	m_boxCollider.setPosition(m_pos);
	m_boxCollider.setSize(m_size);
}

void Collider::draw(sf::RenderWindow& t_window, sf::View& t_view)
{
	/*if (m_collisions.ViewCheck(t_view, m_boxCollider.getPosition()))
	{
		t_window.draw(m_boxCollider);
	}*/
}

