#pragma once
#include "GameObject.h"

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "Collider.h"


class Tiles : public GameObject
{
public:
	Tiles(sf::RenderWindow& t_window, sf::Vector2f size, sf::Vector2f pos,sf::Sprite t_sprite,std::string t_tag,int t_numTag) :
		m_window(t_window)
	{
		//set the sprite , position ,tag, size and number tag of tile
		m_sprites = t_sprite;
		m_sprites.setPosition(pos);
		m_tag = t_tag;
		m_size = size;
		m_numTag = t_numTag;
		m_collider = new Collider(m_sprites.getPosition(), m_tag,m_size);
	} 
	virtual ~Tiles()
	{
		delete m_collider;
	}
	//func to ge the tile sprite, set/get position 
	sf::Sprite getSprite() { return m_sprites; }
	void setPosition(sf::Vector2f(t_pos)) { m_sprites.setPosition(t_pos); }
	sf::Vector2f getPosition() { return m_sprites.getPosition(); }
	//func to get tag ,number tag,size and to draw tile
	std::string getTag(){ return m_tag; }
	int getNumTag() { return m_numTag; }
	sf::Vector2f getSize() { return m_size; }
	Collider* &getCollider()
	{
		return m_collider;
	}
	void draw(sf::View t_view) {
		m_window.draw(m_sprites);
		m_collider->draw(m_window, t_view);
	}

	

private:
	//reference to render window
	sf::RenderWindow& m_window;
	//the sprite of the tile,its tag,num tag and size
	sf::Sprite m_sprites;
	std::string m_tag;
	sf::Vector2f m_size;
	Collider* m_collider;
	int m_numTag;
};