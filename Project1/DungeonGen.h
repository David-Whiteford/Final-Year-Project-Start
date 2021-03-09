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
	};
	enum DungeonTiles
	{
		UnusedTile = 'A',
		FloorTile = '1',
		CorridorTile = '2',
		Wall = '0',
		Door1 = '3',
		Door2 = '4',
		UpStairs = '5',
		DownStairs = '6',
		Chest = '7',
		Skull = '8',
		Bones = '9',
		Chains = 'B', 
		MonsterTrigger = 'C',
		Plant = 'P',
		SpawnPoint = 'S'
		
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
		m_rooms(),
		m_exit()
	{

	}
	void generateMap(int t_maxFeatures);
	void Set2DVec(Tilemap *&t_tilemap)
	{
	

		for (int i = 0; i < m_tiles.size(); i++)
		{
			m_tempTileVec.push_back(m_tiles[i] - '0');
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
	std::vector<int> &getTileMapVec()
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
	}
	bool createFeature();
	bool createFeat(int t_x ,int t_y, Direction t_direction);
	bool makeRoom(int t_x, int t_y, Direction t_direction,bool t_firstRoom);
	bool makeCorridor(int t_x, int t_y, Direction t_direction);
	bool placeTile(Tile& t_rect, char t_tile);
	void placeMonsterTrigger();
	void placeDecorInRoom();
	void playerStartPos();


private:
	sf::Vector2f m_startPosition;
	std::vector<int> m_tempTileVec;
	int m_objectCount = 0;
	int m_width;
	int m_height;
	int m_roomMaxDecor = 10;
	std::vector<char> m_tiles;
	std::vector<Tile> m_rooms;
	std::vector<Tile> m_exit;
	std::vector<int> m_tileNums;
	int m_tileArr[30][30];
	int m_index = 0;
	Tilemap m_tilemap;
};
