#ifndef TILEMAP_H
#define TILEMAP_H

#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Tiles.h"
#include "Collisions.h"
class Tilemap
{
public:
	Tilemap();
	~Tilemap();
	//func to setup tilemap
	void init();
	void OverWorldTilesSetUp();
    void DungeonTilesSetUp();
	//func to get the spawn points
	sf::Vector2f getPlayerSpawn();
	//func to draw levels 
	sf::Vector2f getPlayerCave();
	void DrawOverWorld(sf::View t_view);
	void DrawDungeon(sf::View t_view);
	//func to set map 
	void setMap(sf::RenderWindow& t_window);
	//func to return obstacles ,caves and exits
	std::vector<Tiles*> getDunObstaclesVec();
	std::vector<Tiles*> getOverWorldObstaclesVec();
	std::vector<Tiles*> getCavesVec();
	std::vector<Tiles*> getDunExitsVec();
	//functions to create dungeons
	void Dun(std::vector<char> &t_dunVec, sf::RenderWindow& t_window ,int t_mapWidth,  int t_mapHeight);
	void DunDecor(std::vector<char>& t_dunVec, sf::RenderWindow& t_window, int t_mapWidth, int t_mapHeight);
	void PushValsToVec()
	{
		//function to level values into vector
		for (int row = 0; row < m_mapWidth; row++)
		{
			for (int col = 0; col < m_mapHeigth; col++)
			{
				m_tileVecValues.push_back(lvl1[row][col]);
			}
		}
	}
	//function to clear dungeon vectors
	void clearDunVecs()
	{
		for (auto dunTiles : m_dunTileVec)
		{
			delete dunTiles;
		}
	    m_dunTileVec.clear();
		for (auto dunTilesDecor : m_dunDecorTileVec)
		{
			delete dunTilesDecor;
		}
		m_dunDecorTileVec.clear();
		m_exits.clear();
		m_enemySpawn.clear();
	}
private:
	Collisions m_col;
	//vector to store caves,tiles for overworld and dungeon tiles
	std::vector<sf::Vector2f> m_caves;
	std::vector<Tiles*> m_tileVec;
	std::vector<Tiles*> m_dunTileVec;
	std::vector<Tiles*> m_dunDecorTileVec;
	//vector to store enemy spawn(when needed) ,exits , caves
	std::vector<sf::Vector2f> m_enemySpawn;
	std::vector<sf::Vector2f> m_exits;
	sf::Texture m_texture;
	sf::Texture m_dunTexture;
	//width and height of map
	static const int m_mapWidth = 30;
	static const int m_mapHeigth = 30;
	//overworld tiles
	sf::Sprite m_cliffLeftCornerTile, m_cliffRightCornerTile, 
		m_cliffLeftTile, m_cliffRightTile , m_cliffFront,m_cliffDown;
	sf::Sprite m_grassTile, m_waterTile, m_dirtTile, m_caveTile;
	
	//DungeonTiles
	sf::Sprite m_floorTile, m_stoneFloorTile, m_wallTile, m_corridorTile, m_DoorTile, m_Door2Tile, m_stairsTile,m_darkFloorTiles,m_tilePatternTile;
	//Decor Tiles
	sf::Sprite m_chainsTile, m_chestTile,m_plantTile, m_potionTile,m_torchTile,m_moneyTile,m_pictureTile,m_statueTile,m_flameCauldron
		,m_prisonTile, m_chairTileF, m_chairTileL, m_coffinTile, m_tableSprite,m_bookCaseTile, m_caveDirtTile,m_holeTile,m_worshipStone ,m_bedTile,m_nightStand;
	//Trgger Tiles
	sf::Sprite m_skullTile, m_spawnPoint, m_spikeTrap,m_health;
	//the size of the hole tile and the tile position
	sf::Vector2f m_holeSize = sf::Vector2f(48,48);
	sf::Vector2f m_tilePosition = sf::Vector2f(0.0f, 0.0f);
	//holds the size of the tiles
	sf::Vector2f m_bedSize, m_statueSize, m_worshipRoomSize, m_tableSize, m_bookCaseSize, m_coffinSize = sf::Vector2f(0.0f, 0.0f);
	int m_tileSize = 16;
	//size of map
	int m_tileMapSize = 30;
	//vec to store tile values
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


