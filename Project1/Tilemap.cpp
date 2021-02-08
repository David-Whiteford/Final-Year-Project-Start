#include "Tilemap.h"

Tilemap::Tilemap()
{
	init();
	LoadMap(lvl1,0,0);
	
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
	if (!m_dunTexture.loadFromFile("IMAGES//Inner.png"))
	{
		// error...
	}
	OverWorldTilesSetUp();
	DungeonTilesSetUp();
}
void Tilemap::DungeonTilesSetUp()
{
	m_floorTile.setTexture(m_dunTexture);
	m_floorTile.setTextureRect(sf::IntRect(0, 16, 16, 16));
	m_wallTile.setTexture(m_dunTexture);
	m_wallTile.setTextureRect(sf::IntRect(94, 112, 16, 16));
	m_skullTile.setTexture(m_texture);
	m_skullTile.setTextureRect(sf::IntRect(448, 32, 16, 16));
	m_chainsTile.setTexture(m_texture);
	m_chainsTile.setTextureRect(sf::IntRect(416, 20, 16, 16));
	m_chestTile.setTexture(m_texture);
	m_chestTile.setTextureRect(sf::IntRect(416, 320, 16, 16));
	m_DoorTile.setTexture(m_texture);
	m_DoorTile.setTextureRect(sf::IntRect(32, 0, 16, 16));

	//trigger
	m_monsterSpawn.setTexture(m_texture);
	m_monsterSpawn.setTextureRect(sf::IntRect(16, 0, 16, 16));

}
void Tilemap::OverWorldTilesSetUp()
{
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
void Tilemap::LoadMap(int arr[m_mapWidth][m_mapHeigth], int t_mapWidth, int t_mapHeight)
{
	for (int row = 0; row < m_tileMapSize; row++)
	{
		for (int col = 0; col < m_tileMapSize; col++)
		{
			tilemap[row][col] = arr[row][col];
		}
	}
}

void Tilemap::DrawOverWorld(sf::View t_view)
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
void Tilemap::DrawDungeon(sf::View t_view)
{
	for (int i = 0; i < m_dunTileVec.size(); ++i)
	{
		if (t_view.getCenter().x + t_view.getSize().x / 2 > m_dunTileVec[i]->getSprite().getPosition().x
			&& t_view.getCenter().x - t_view.getSize().x / 2 < m_dunTileVec[i]->getSprite().getPosition().x
			&& t_view.getCenter().y + t_view.getSize().y / 2 > m_dunTileVec[i]->getSprite().getPosition().y
			&& t_view.getCenter().y - t_view.getSize().y / 2 < m_dunTileVec[i]->getSprite().getPosition().y)
		{
			m_dunTileVec[i]->draw();
		}
	}
}

void Tilemap::setMap(sf::RenderWindow& t_window)
{
	int type = 0;
	int x = 0;
	int y = 0;
	for (int i = 0; i < m_tileVecValues.size(); i++)
	{
		type = m_tileVecValues[i];	
		switch (type)
		{
			case 1:
					m_tileVec.push_back(new Tiles(t_window, m_tileSize,
						sf::Vector2f(x * m_tileSize, y * m_tileSize), m_grassTile, "Grass"));
				break;
			case 2:
					m_tileVec.push_back(new Tiles(t_window, m_tileSize,
						sf::Vector2f(x * m_tileSize, y * m_tileSize), m_dirtTile, "Grass"));
				break;
			case 3:
				m_tileVec.push_back(new Tiles(t_window, m_tileSize,
					sf::Vector2f(x * m_tileSize, y * m_tileSize), m_waterTile, "Obstacle"));
				
				break;
			case 4:
				m_tileVec.push_back(new Tiles(t_window, m_tileSize,
					sf::Vector2f(x * m_tileSize, y * m_tileSize), m_cliffFront, "Obstacle"));
				break;
			case 5:
				m_tileVec.push_back(new Tiles(t_window, m_tileSize,
					sf::Vector2f(x * m_tileSize, y * m_tileSize), m_cliffRightCornerTile, "Obstacle"));
				break;
			case 6:
				m_tileVec.push_back(new Tiles(t_window, m_tileSize,
					sf::Vector2f(x * m_tileSize, y * m_tileSize),m_cliffLeftCornerTile, "Obstacle"));
				break;
			case 7:
				m_tileVec.push_back(new Tiles(t_window, m_tileSize,
					sf::Vector2f(x * m_tileSize, y * m_tileSize), m_cliffDown, "Cliff Grass"));
				break;
			case 8:
				m_tileVec.push_back(new Tiles(t_window, m_tileSize,
					sf::Vector2f(x * m_tileSize, y * m_tileSize), m_cliffRightTile, "Cliff Grass"));
				break;
			case 9:
				m_tileVec.push_back(new Tiles(t_window, m_tileSize,
					sf::Vector2f(x * m_tileSize, y * m_tileSize), m_cliffLeftTile,"Cliff Grass"));
				break;
			case 10:
				m_tileVec.push_back(new Tiles(t_window, m_tileSize,
					sf::Vector2f(x * m_tileSize, y * m_tileSize), m_caveTile, "Cave"));
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
std::vector<Tiles*> Tilemap::getDunObstaclesVec()
{
	std::vector<Tiles*> obstacles;
	for (int i = 0; i < m_dunTileVec.size(); i++)
	{
		if (m_dunTileVec[i]->getTag() == "Obstacle")
		{
			obstacles.push_back(m_dunTileVec[i]);
		}
	}
	return obstacles;
}
std::vector<Tiles*> Tilemap::getOverWorldObstaclesVec()
{
	std::vector<Tiles*> obstacles;
	for (int i = 0; i < m_tileVec.size(); i++)
	{
		if (m_tileVec[i]->getTag() == "Obstacle")
		{
			obstacles.push_back(m_tileVec[i]);
		}
	}
	return obstacles;
}

std::vector<Tiles*> Tilemap::getCavesVec()
{
	std::vector<Tiles*> caves;
	for (int i = 0; i < m_tileVec.size(); i++)
	{
		if (m_tileVec[i]->getTag() == "Cave")
		{
			caves.push_back(m_tileVec[i]);
		}
	}
	return caves;
}

void Tilemap::Dun(std::vector<int> &t_dunVec, sf::RenderWindow& t_window, int t_mapWidth, int t_mapHeight)
{
	int type = 0;
	int x = 0;
	int y = 0;	
	for(int i = 0; i < t_dunVec.size();i++)
	{
		type = t_dunVec[i];
		switch (type)
		{
			case 0:
				m_dunTileVec.push_back(new Tiles(t_window, m_tileSize,
					sf::Vector2f(x * m_tileSize, y * m_tileSize), m_wallTile, "Obstacle"));
				break;
			case 1:
				m_dunTileVec.push_back(new Tiles(t_window, m_tileSize,
					sf::Vector2f(x * m_tileSize, y * m_tileSize), m_floorTile, "Floor"));
				break;
			case 2:
				m_dunTileVec.push_back(new Tiles(t_window, m_tileSize,
					sf::Vector2f(x * m_tileSize, y * m_tileSize), m_floorTile, "Floor"));
				break;
			case 3:
				m_dunTileVec.push_back(new Tiles(t_window, m_tileSize,
					sf::Vector2f(x * m_tileSize, y * m_tileSize), m_floorTile, "Floor"));
				m_dunTileVec.push_back(new Tiles(t_window, m_tileSize,
					sf::Vector2f(x * m_tileSize, y * m_tileSize), m_DoorTile, "Door"));
				break;
			case 4:
				m_dunTileVec.push_back(new Tiles(t_window, m_tileSize,
					sf::Vector2f(x * m_tileSize, y * m_tileSize), m_dirtTile, "Dirt"));
				break;
			case 5:
				m_dunTileVec.push_back(new Tiles(t_window, m_tileSize,
					sf::Vector2f(x * m_tileSize, y * m_tileSize), m_dirtTile, "Dirt"));
				break;
			case 6:
				m_dunTileVec.push_back(new Tiles(t_window, m_tileSize,
					sf::Vector2f(x * m_tileSize, y * m_tileSize), m_dirtTile, "Dirt"));
				break;
			case 7:
				m_dunTileVec.push_back(new Tiles(t_window, m_tileSize,
					sf::Vector2f(x * m_tileSize, y * m_tileSize), m_floorTile, "Floor"));
				m_dunTileVec.push_back(new Tiles(t_window, m_tileSize,
					sf::Vector2f(x * m_tileSize, y * m_tileSize), m_chestTile, "Obstacle"));
				break;
			case 8:
				m_dunTileVec.push_back(new Tiles(t_window, m_tileSize,
					sf::Vector2f(x * m_tileSize, y * m_tileSize), m_floorTile, "Floor"));
				m_dunTileVec.push_back(new Tiles(t_window, m_tileSize,
					sf::Vector2f(x * m_tileSize, y * m_tileSize), m_skullTile, "Obstacle"));
				break;
			case 'C':
				m_dunTileVec.push_back(new Tiles(t_window, m_tileSize,
					sf::Vector2f(x * m_tileSize, y * m_tileSize), m_floorTile, "Floor"));
				m_dunTileVec.push_back(new Tiles(t_window, m_tileSize,
					sf::Vector2f(x * m_tileSize, y * m_tileSize), m_monsterSpawn, "Obstacle"));
				break;
			case 'B':
				m_dunTileVec.push_back(new Tiles(t_window, m_tileSize,
					sf::Vector2f(x * m_tileSize, y * m_tileSize), m_floorTile, "Floor"));
				m_dunTileVec.push_back(new Tiles(t_window, m_tileSize,
					sf::Vector2f(x * m_tileSize, y * m_tileSize), m_chainsTile, "Chains"));
				break;
			default:
				break;
		}

		x++;
		if (x == t_mapWidth)
		{
			x = 0;
			y++;
		}
		
	}
}