#pragma once
#include <iostream>
#include <random>
#include <vector>
#include "Tilemap.h"
#include "Debug.h"
namespace
{
	std::random_device rd;
	std::mt19937 mt(rd());
	int randomInt(int t_exclusiveMax)
	{
		std::uniform_int_distribution<> dist(0, t_exclusiveMax - 1);
		return dist(mt);
	}
	int randomInt(int t_min, int t_max)
	{
		std::uniform_int_distribution<> dist(0, t_max - t_min);
		return dist(mt) + t_min;
	}
	bool randomBool(double t_prob = 0.5)
	{
		std::bernoulli_distribution dist(t_prob);
		return dist(mt);
	}
}
class DungeonGen
{
public:

	struct Tile
	{
		int x, y;
		int width, height;
		int decorInRoom = 0;
	};
	enum DungeonTiles
	{
		UnusedTile = 'A',
		Wall = '0',
		FloorTile = '1',
		StoneFloorTile = 'S',
		CorridorTile = '1',
		Plant = '2',
		Door1 = '3',
		Door2 = '4',
		PrisonTileOne = 'V',
		PrisonTileTwo = 'B',
		CoffinTileOne = 'J',
		CoffinTileTwo = 'K',
		Potion = '5',
		Chains = '6',
		Torch = 'T',
		Picture = 'P',
		Money = 'M',
		Chest = '7',
		Skull = '8',
		SpawnPoint = '9',
		ChairL = 'CL',
		ChairR = 'CR',
		ChairF = 'O',
		Table = 'U',
		Chair = 'TB',
		SpikeTrap = 'L',
		TrapDoorTrap = 'TD',
		WallSpikeTrap = 'WS'
	};
	

	enum Direction
	{
		North,
		South,
		West,
		East,
		DirectionNum
	};

	DungeonGen(int t_width, int t_height) :
		m_width(t_width),
		m_height(t_height),
		m_tiles(t_width * t_height,UnusedTile),
		m_decorTiles(t_width* t_height, UnusedTile),
		m_rooms(),
		m_exit()
	{

	}
	void generateMap(int t_maxFeatures);
	void Set2DVec(Tilemap *&t_tilemap)
	{
	

		for (int i = 0; i < m_tiles.size(); i++)
		{
			m_tempTileVec.push_back(m_tiles[i] );
		}
		for (int row = 0; row < m_height; row++) 
		{
			for (int col = 0; col < m_width; col++) 
			{
				m_tileArr[row][col] = m_tempTileVec.front();
				/*if (!m_tempTileVec.empty())
				{
					m_tempTileVec.erase(m_tempTileVec.begin());
				}*/
				
			}
		}
		t_tilemap->LoadMap(m_tileArr,1,1);
		
	}
	std::vector<char> &getTileMapVec()
	{
		return m_tempTileVec;
	}
	void print()
	{
		for (int row = 0; row < m_height; row++)
		{
			for (int col = 0; col < m_width; col++)
			{
				std::cout << getTile(col, row);
				
			}
			std::cout << std::endl;
		}
	}
	char getTile(int t_x, int t_y)
	{
		if (t_x < 0 || t_y < 0 || t_x >= m_width || t_y >= m_height)
			return UnusedTile;
		return m_tiles[t_x + t_y * m_width];
	}
	void setTile(int t_x, int t_y, char t_tile)
	{
		m_tiles[t_x + t_y * m_width] = t_tile;
		m_decorTiles[t_x + t_y * m_width] = t_tile;
	}
	void setDecorTiles(int t_x, int t_y, char t_tile)
	{
		m_decorTiles[t_x + t_y * m_width] = t_tile;
	}
	void createRoomFeatures(Tilemap*& t_tilemap);
	bool createFeature();
	bool createFeat(int t_x ,int t_y, Direction t_direction);
	bool makeRoom(int t_x, int t_y, Direction t_direction,bool t_firstRoom);
	bool makeCorridor(int t_x, int t_y, Direction t_direction);
	bool placeTile(Tile& t_rect, char t_tile);
	void placeDecorInRoom();
	void createUniqueRooms() 
	{
		createJailRoom();
		createCoffinRoom();
	}
	void placeDecorInHalls();
	void FloorDecorTiles();
	void playerStartPos();
	void createJailRoom();
	void createCoffinRoom();
	void createFeastRoom();
	void Test() { placeDecorInRoom(); };
	void placeDecorOnWalls();
	bool CheckXAndYPos(int x, int y);
	sf::Vector2i GenXAndYAtTopWall(int t_i, std::vector<Tile>& t_tileVec);
	sf::Vector2i GenXAndYAtBottomWall(int t_i, std::vector<Tile>& t_tileVec);
	void createTrapsInRooms();



private:
	sf::Vector2f m_startPosition;
	std::vector<char> m_tempTileVec;
	int m_objectCount = 0;
	int m_width;
	int m_height;
	int m_roomMaxDecor = 10;
	std::vector<char> m_tiles;
	std::vector<char> m_decorTiles;
	std::vector<Tile> m_halls;
	std::vector<Tile> m_rooms;
	std::vector<Tile> m_exit;
	std::vector<int> m_tileNums;
	int m_tileDecorArr[30][30];
	int m_tileArr[30][30];
	int m_index = 0;
	Tilemap m_tilemap;
};
