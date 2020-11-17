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
	if (!m_texture.loadFromFile("IMAGES//Overworld.png"))
	{
		// error...
	}
	m_grassTile.setTexture(m_texture);
	m_grassTile.setTextureRect(sf::IntRect(0, 0, 16, 16));

	m_waterTile.setTexture(m_texture);
	m_waterTile.setTextureRect(sf::IntRect(304, 112, 16, 16));

	m_dirtTile.setTexture(m_texture);
	m_dirtTile.setTextureRect(sf::IntRect(16, 64, 16, 16));

	m_cliffLeftCornerTile.setTexture(m_texture);
	m_cliffLeftCornerTile.setTextureRect(sf::IntRect(64, 176, 16, 16));

	m_cliffRightCornerTile.setTexture(m_texture);
	m_cliffRightCornerTile.setTextureRect(sf::IntRect(96, 176, 16, 16));

	m_cliffLeftTile.setTexture(m_texture);
	m_cliffLeftTile.setTextureRect(sf::IntRect(64, 160, 16, 16));

	m_cliffRightTile.setTexture(m_texture);
	m_cliffRightTile.setTextureRect(sf::IntRect(96, 160, 16, 16));

	m_cliffDown.setTexture(m_texture);
	m_cliffDown.setTextureRect(sf::IntRect(80, 176, 16, 16));
	
	m_cliffFront.setTexture(m_texture);
	m_cliffFront.setTextureRect(sf::IntRect(64, 208, 16, 16));

	m_caveTile.setTexture(m_texture);
	m_caveTile.setTextureRect(sf::IntRect(112, 192, 16, 16));
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

void Tilemap::DrawMap(sf::View t_view)
{
	for (int i = 0; i < m_tileVec.size(); ++i)
	{
		if (t_view.getCenter().x + t_view.getSize().x / 2 > m_tileVec[i]->getSprite().getPosition().x 
			&& t_view.getCenter().x - t_view.getSize().x / 2 < m_tileVec[i]->getSprite().getPosition().x
			&& t_view.getCenter().y + t_view.getSize().y / 2 > m_tileVec[i]->getSprite().getPosition().y
			&& t_view.getCenter().y - t_view.getSize().y / 2 < m_tileVec[i]->getSprite().getPosition().y)
		{
			m_tileVec[i]->draw();
		}
	}
}
void Tilemap::setMap(sf::RenderWindow& t_window)
{
	int type = 0;
	int x = 0;
	int y = 0;
	for (int row = 0; row < m_tileMapSize; row++)
	{
		for (int col = 0; col < m_tileMapSize; col++)
		{
			type = tilemap[row][col];
			switch (type)
			{
			case 1:
				m_tileVec.push_back(new Tiles(t_window, m_tileSize, 
					sf::Vector2f(x * m_tileSize, y * m_tileSize),m_grassTile));
				
				break;
			case 2:
				m_tileVec.push_back(new Tiles(t_window, m_tileSize,
					sf::Vector2f(x * m_tileSize, y * m_tileSize), m_dirtTile));
				break;
			case 3:
				m_tileVec.push_back(new Tiles(t_window, m_tileSize,
					sf::Vector2f(x * m_tileSize, y * m_tileSize), m_waterTile));
				break;
			case 4:
				m_tileVec.push_back(new Tiles(t_window, m_tileSize,
					sf::Vector2f(x * m_tileSize, y * m_tileSize), m_cliffFront));
				break;
			case 5:
				m_tileVec.push_back(new Tiles(t_window, m_tileSize,
					sf::Vector2f(x * m_tileSize, y * m_tileSize), m_cliffRightCornerTile));
				break;
			case 6:
				m_tileVec.push_back(new Tiles(t_window, m_tileSize,
					sf::Vector2f(x * m_tileSize, y * m_tileSize),m_cliffLeftCornerTile));
				break;
			case 7:
				m_tileVec.push_back(new Tiles(t_window, m_tileSize,
					sf::Vector2f(x * m_tileSize, y * m_tileSize), m_cliffDown));
				break;
			case 8:
				m_tileVec.push_back(new Tiles(t_window, m_tileSize,
					sf::Vector2f(x * m_tileSize, y * m_tileSize), m_cliffRightTile));
				break;
			case 9:
				m_tileVec.push_back(new Tiles(t_window, m_tileSize,
					sf::Vector2f(x * m_tileSize, y * m_tileSize), m_cliffLeftTile));
				break;
			case 10:
				m_tileVec.push_back(new Tiles(t_window, m_tileSize,
					sf::Vector2f(x * m_tileSize, y * m_tileSize), m_caveTile));
				break;
			
		
				break;
			default:
				break;
			}
			
			x++;
			if (x == m_mapWidth)
			{
				x = 0;
				y++;
			}
		}
	}
}
