#ifndef TILEMAP_H
#define TILEMAP_H

#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Tiles.h"

class Tilemap
{
public:
	Tilemap();
	~Tilemap();
	void init();
	void OverWorldTilesSetUp();
    void DungeonTilesSetUp();
	void LoadMap(int arr[30][30],int t_mapWidth, int t_mapHeight);
	
	void SetDunDeor(int arr[30][30]);
	void DrawOverWorld(sf::View t_view);
	void DrawDungeon(sf::View t_view);
	void setMap(sf::RenderWindow& t_window);
	void setDunMap(sf::RenderWindow& t_window);
	void setDunGen(bool t_dunGen);
	std::vector<Tiles*> getDunObstaclesVec();
	std::vector<Tiles*> getOverWorldObstaclesVec();
	std::vector<Tiles*> getCavesVec();
	void Dun(std::vector<char> &t_dunVec, sf::RenderWindow& t_window ,int t_mapWidth,  int t_mapHeight);
	void DunDecor(std::vector<char>& t_dunVec, sf::RenderWindow& t_window, int t_mapWidth, int t_mapHeight);
	void PushValsToVec()
	{
		for (int row = 0; row < m_mapWidth; row++)
		{
			for (int col = 0; col < m_mapHeigth; col++)
			{
				m_tileVecValues.push_back(lvl1[row][col]);
			}
		}
	}
private:
	
	std::vector<Tiles*> m_cavesVec;
	std::vector<Tiles*> m_tileVec;
	std::vector<Tiles*> m_dunTileVec;
	std::vector<Tiles*> m_dunDecorTileVec;
	std::vector<sf::Vector2f> m_enemySpawn;
	std::vector<sf::Vector2f> m_exits;
	sf::Texture m_texture;
	sf::Texture m_dunTexture;
	static const int m_mapWidth = 30;
	static const int m_mapHeigth = 30;
	sf::Sprite m_cliffLeftCornerTile, m_cliffRightCornerTile, 
		m_cliffLeftTile, m_cliffRightTile , m_cliffFront,m_cliffDown;
	sf::Sprite m_grassTile, m_waterTile, m_dirtTile, m_caveTile;
	
	//DungeonTiles
	sf::Sprite m_floorTile, m_stoneFloorTile, m_wallTile, m_corridorTile, m_DoorTile, m_Door2Tile, m_stairsTile;
	//Decor Tiles
	sf::Sprite m_chainsTile, m_chestTile,m_plantTile, m_potionTile,m_torchTile,m_moneyTile,m_pictureTile
		,m_prisonTile, m_chairTileF , m_coffinTile, m_tableSprite,m_bookCaseTile;
	//Trgger Tiles
	sf::Sprite m_skullTile, m_spawnPoint, m_spikeTrap;

	sf::Vector2f m_tilePosition = sf::Vector2f(0.0f, 0.0f);
	int m_tileSize = 16;
	bool m_dungeonGen = false;
	bool m_overWorldGen = true;
	int m_tileMapSize = 30;
	std::vector<int> m_tileVecValues;
	int tilemap[m_mapWidth][m_mapHeigth];

	int lvl1[m_mapWidth][m_mapHeigth] = {
1,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
1,2,2,2,1,1,1,1,1,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,
1,2,2,2,1,1,1,1,1,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,
1,2,2,2,1,1,1,1,1,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,
1,2,2,2,1,6,9,1,1,2,2,1,1,8,7,7,7,7,7,9,1,1,1,1,1,8,5,1,2,1,
1,2,2,2,1,4,9,1,1,2,2,1,1,8,4,4,4,4,4,9,1,1,1,1,1,8,4,1,2,1,
1,2,2,2,1,4,9,1,1,2,2,1,1,8,4,4,4,4,4,9,1,1,1,1,1,8,4,1,2,1,
1,2,2,2,1,4,9,1,1,2,2,1,1,8,4,4,10,4,4,9,1,1,1,1,1,8,4,1,2,1,
1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,3,9,1,3,3,3,1,8,1,1,2,1,
1,2,2,2,1,1,6,7,7,7,7,7,7,5,1,1,2,1,3,9,1,3,3,3,1,8,1,1,2,1,
1,2,2,2,1,1,4,4,4,4,4,4,4,4,1,1,2,3,3,6,7,3,3,3,7,5,1,1,2,1,
1,2,2,2,1,1,4,4,4,4,4,4,4,4,1,1,2,3,3,4,4,3,3,3,4,4,1,1,2,1,
1,2,2,2,1,1,4,4,4,4,4,4,4,10,1,1,2,1,3,4,4,3,3,3,4,4,1,1,2,1,
1,2,2,2,1,1,1,1,1,1,1,1,1,2,2,2,2,1,3,4,4,3,3,3,4,4,1,1,2,1,
3,2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,2,3,3,3,3,3,3,3,3,3,3,3,2,3,
3,2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,2,3,3,3,3,3,3,3,3,3,3,3,2,3,
1,2,2,2,3,3,3,3,3,3,3,3,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,3,
1,2,2,2,1,3,3,3,3,3,3,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,6,7,9,1,1,8,7,1,8,5,
1,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,4,4,9,1,1,8,4,7,5,4,
1,1,1,1,1,1,1,1,1,1,1,2,2,1,1,1,1,1,1,1,4,4,9,1,1,8,4,4,4,4,
1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,1,1,1,1,1,4,4,6,7,7,5,4,4,4,4,
1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,1,1,1,1,1,1,1,4,4,4,4,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,1,1,1,1,1,1,1,4,4,4,4,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,1,1,1,1,1,1,1,4,10,4,4,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,2,2,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,2,2,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
	};
};
#endif


