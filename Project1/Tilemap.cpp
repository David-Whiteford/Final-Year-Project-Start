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
	m_stoneFloorTile.setTexture(m_texture);
	m_stoneFloorTile.setTextureRect(sf::IntRect(80, 16, 16, 16));
	m_wallTile.setTexture(m_texture);
	m_wallTile.setTextureRect(sf::IntRect(64, 48, 16, 16));

	//Decor Tiles
	m_chainsTile.setTexture(m_texture);
	m_chainsTile.setTextureRect(sf::IntRect(64, 16, 16, 16));
	m_chestTile.setTexture(m_texture);
	m_chestTile.setTextureRect(sf::IntRect(416, 320, 16, 16));
	m_DoorTile.setTexture(m_texture);
	m_DoorTile.setTextureRect(sf::IntRect(32, 0, 16, 16));
	m_Door2Tile.setTexture(m_texture);
	m_Door2Tile.setTextureRect(sf::IntRect(48, 0, 16, 16));
	m_plantTile.setTexture(m_texture);
	m_plantTile.setTextureRect(sf::IntRect(64, 0, 16, 16));
	m_potionTile.setTexture(m_texture);
	m_potionTile.setTextureRect(sf::IntRect(64, 32, 16, 16));
	m_moneyTile.setTexture(m_texture);
	m_moneyTile.setTextureRect(sf::IntRect(80, 32, 16, 16));
	m_chairTileF.setTexture(m_dunTexture);
	m_chairTileF.setTextureRect(sf::IntRect(256, 63, 16, 16));
	m_torchTile.setTexture(m_texture);
	m_torchTile.setTextureRect(sf::IntRect(80, 48, 16, 16));
	m_prisonTile.setTexture(m_texture);
	m_prisonTile.setTextureRect(sf::IntRect(128, 0, 16, 16));
	m_pictureTile.setTexture(m_texture);
	m_pictureTile.setTextureRect(sf::IntRect(64, 64, 16, 16));
	m_coffinTile.setTexture(m_texture);
	m_coffinTile.setTextureRect(sf::IntRect(112, 0, 16, 32));
	m_bookCaseTile.setTexture(m_dunTexture);
	m_bookCaseTile.setTextureRect(sf::IntRect(49, 192, 45, 28));
	m_tableSprite.setTexture(m_dunTexture);
	m_tableSprite.setTextureRect(sf::IntRect(332, 89, 48, 44));
	

	//trigger
	m_skullTile.setTexture(m_texture);
	m_skullTile.setTextureRect(sf::IntRect(48, 64, 16, 16));
	m_spawnPoint.setTexture(m_texture);
	m_spawnPoint.setTextureRect(sf::IntRect(80, 0, 16, 16));
	m_spikeTrap.setTexture(m_texture);
	m_spikeTrap.setTextureRect(sf::IntRect(81, 64, 16, 16));
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
	for (int i = 0; i < m_dunDecorTileVec.size(); ++i)
	{
		if (t_view.getCenter().x + t_view.getSize().x / 2 > m_dunDecorTileVec[i]->getSprite().getPosition().x
			&& t_view.getCenter().x - t_view.getSize().x / 2 < m_dunDecorTileVec[i]->getSprite().getPosition().x
			&& t_view.getCenter().y + t_view.getSize().y / 2 > m_dunDecorTileVec[i]->getSprite().getPosition().y
			&& t_view.getCenter().y - t_view.getSize().y / 2 < m_dunDecorTileVec[i]->getSprite().getPosition().y)
		{
			m_dunDecorTileVec[i]->draw();
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

void Tilemap::Dun(std::vector<char> &t_dunVec, sf::RenderWindow& t_window, int t_mapWidth, int t_mapHeight)
{
	int type = 0;
	int x = 0;
	int y = 0;	
	for(int i = 0; i < t_dunVec.size();i++)
	{
		type = t_dunVec[i];
		char newType = type;
		switch (newType)
		{
			case '0':
				m_dunTileVec.push_back(new Tiles(t_window, m_tileSize,
					sf::Vector2f(x * m_tileSize, y * m_tileSize), m_wallTile, "Obstacle"));
				break;
			case '1':
				m_dunTileVec.push_back(new Tiles(t_window, m_tileSize,
					sf::Vector2f(x * m_tileSize, y * m_tileSize), m_floorTile, "Floor"));
				break;
			case '3':
				m_dunTileVec.push_back(new Tiles(t_window, m_tileSize,
					sf::Vector2f(x * m_tileSize, y * m_tileSize), m_floorTile, "Floor"));
				m_dunTileVec.push_back(new Tiles(t_window, m_tileSize,
					sf::Vector2f(x * m_tileSize, y * m_tileSize), m_DoorTile, "Door"));
				break;
			case '4':
				m_dunTileVec.push_back(new Tiles(t_window, m_tileSize,
					sf::Vector2f(x * m_tileSize, y * m_tileSize), m_floorTile, "Floor"));
				m_dunTileVec.push_back(new Tiles(t_window, m_tileSize,
					sf::Vector2f(x * m_tileSize, y * m_tileSize), m_Door2Tile, "Door2"));
				break;
			case 'S':
				m_dunTileVec.push_back(new Tiles(t_window, m_tileSize,
					sf::Vector2f(x * m_tileSize, y * m_tileSize), m_stoneFloorTile, "Floor"));
				break;
			case 'L':
				m_dunTileVec.push_back(new Tiles(t_window, m_tileSize,
					sf::Vector2f(x * m_tileSize, y * m_tileSize), m_spikeTrap, "Decor"));
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
void Tilemap::DunDecor(std::vector<char>& t_dunDecorVec, sf::RenderWindow& t_window, int t_mapWidth, int t_mapHeight)
{
	int type = 0;
	int x = 0;
	int y = 0;
	for (int i = 0; i < t_dunDecorVec.size(); i++)
	{
		type = t_dunDecorVec[i];

		switch (type)
		{
		case '2':
			m_dunDecorTileVec.push_back(new Tiles(t_window, m_tileSize,
				sf::Vector2f(x * m_tileSize, y * m_tileSize), m_plantTile, "Obstacle"));
			break;
		case '5':
			m_dunDecorTileVec.push_back(new Tiles(t_window, m_tileSize,
				sf::Vector2f(x * m_tileSize, y * m_tileSize), m_potionTile, "Obstacle"));
			break;
		case '6':
			m_dunDecorTileVec.push_back(new Tiles(t_window, m_tileSize,
				sf::Vector2f(x * m_tileSize, y * m_tileSize), m_chainsTile, "Decor"));
			break;
		case '7':
			m_dunDecorTileVec.push_back(new Tiles(t_window, m_tileSize,
				sf::Vector2f(x * m_tileSize, y * m_tileSize), m_chestTile, "Obstacle"));
			break;
		case '8':
			m_dunDecorTileVec.push_back(new Tiles(t_window, m_tileSize,
				sf::Vector2f(x * m_tileSize, y * m_tileSize), m_skullTile, "Obstacle"));
			m_enemySpawn.push_back(sf::Vector2f(x * m_tileSize, y * m_tileSize));
			break;
		case 'C':
			m_dunDecorTileVec.push_back(new Tiles(t_window, m_tileSize,
				sf::Vector2f(x * m_tileSize, y * m_tileSize), m_skullTile, "Obstacle"));
			break;
		case '10':
			m_dunDecorTileVec.push_back(new Tiles(t_window, m_tileSize,
				sf::Vector2f(x * m_tileSize, y * m_tileSize), m_chainsTile, "Chains"));
			break;
		case '9':
			m_dunDecorTileVec.push_back(new Tiles(t_window, m_tileSize,
				sf::Vector2f(x * m_tileSize, y * m_tileSize), m_spawnPoint, "Spawn"));
			m_exits.push_back(sf::Vector2f(x * m_tileSize, y * m_tileSize));
			break;
		case 'M':
			m_dunDecorTileVec.push_back(new Tiles(t_window, m_tileSize,
				sf::Vector2f(x * m_tileSize, y * m_tileSize), m_moneyTile, "Obstacle"));
			break;
		case 'T':
			m_dunDecorTileVec.push_back(new Tiles(t_window, m_tileSize,
				sf::Vector2f(x * m_tileSize, y * m_tileSize), m_torchTile, "Decor"));
			break;
		case 'P':
			m_dunDecorTileVec.push_back(new Tiles(t_window, m_tileSize,
				sf::Vector2f(x * m_tileSize, y * m_tileSize), m_pictureTile, "Decor"));
			break;
		case 'V':
			m_dunDecorTileVec.push_back(new Tiles(t_window, m_tileSize,
				sf::Vector2f(x * m_tileSize, y * m_tileSize), m_prisonTile, "Obstacle"));
			break;
		case 'O':
			m_dunDecorTileVec.push_back(new Tiles(t_window, m_tileSize,
				sf::Vector2f(x * m_tileSize, y * m_tileSize), m_chairTileF, "Decor"));
			break;
		case 'J':
			m_dunDecorTileVec.push_back(new Tiles(t_window, m_tileSize,
				sf::Vector2f(x * m_tileSize, y * m_tileSize), m_coffinTile, "Obstacle"));
			break;
		case 'U':
			m_dunDecorTileVec.push_back(new Tiles(t_window, m_tileSize,
				sf::Vector2f(x * m_tileSize, y * m_tileSize), m_tableSprite, "Obstacle"));
			break;
		case 'B':
			m_dunDecorTileVec.push_back(new Tiles(t_window, m_tileSize,
				sf::Vector2f(x * m_tileSize, y * m_tileSize), m_bookCaseTile, "Obstacle"));
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