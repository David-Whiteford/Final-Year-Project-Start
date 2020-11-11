#include "Tilemap.h"

Tilemap::Tilemap()
{
	init();
	LoadMap(lvl1);
	
}

Tilemap::~Tilemap()
{
}

void Tilemap::init()
{
	if (!m_grass.loadFromFile("IMAGES//Overworld.png"))
	{
		// error...
	}
	m_grassTile.setTexture(m_grass);
	m_grassTile.setTextureRect(sf::IntRect(0, 0, 16, 16));
	m_grassTile.setScale(sf::Vector2f(3.0f, 3.0f));

	if (!m_water.loadFromFile("IMAGES//Overworld.png"))
	{
		// error...
	}
	m_waterTile.setTexture(m_water);
	m_waterTile.setTextureRect(sf::IntRect(64, 128, 16, 16));
	m_waterTile.setScale(sf::Vector2f(3.0f, 3.0f));

	if (!m_dirt.loadFromFile("IMAGES//Overworld.png"))
	{
		// error...
	}
	m_dirtTile.setTexture(m_dirt);
	m_dirtTile.setTextureRect(sf::IntRect(24, 224, 16, 16));
	m_dirtTile.setScale(sf::Vector2f(3.0f, 3.0f));
}

void Tilemap::LoadMap(int arr[30][30])
{
	for (int row = 0; row < m_tileMapSize; row++)
	{
		for (int col = 0; col < m_tileMapSize; col++)
		{
			tilemap[row][col] = arr[row][col];
		}
	}
}

void Tilemap::DrawMap(sf::RenderWindow& t_window)
{
	int type = 0;

	for (int row = 0; row < m_tileMapSize; row++)
	{
		for (int col = 0; col < m_tileMapSize; col++)
		{
			type = tilemap[row][col];
			switch (type)
			{
			case 0:
			
				break;
			case 1:
			
				break;
			case 2:
			
				break;
			default:
				break;
			}
		}
	}
}
