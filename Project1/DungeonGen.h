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

	struct RoomVals
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
		PrisonTile = 'V',
		BookShelve = 'B',
		CoffinTile = 'J',
		Potion = '5',
		Chains = '6',
		Torch = 'T',
		Picture = 'P',
		Money = 'M',
		NightStand = 'Y',
		Chest = '7',
		Skull = '8',
		SpawnPoint = '9',
		ChairL = 'Z',
		ChairR = 'CR',
		ChairF = 'O',
		Table = 'U',
		TilePattern = 'G',
		Statue = 'I',
		FlameCauldron = 'R',
		DirtTile = 'D',
		HoleTile = 'H',
		Bed = 'K',
		DarkTiles = 'Q',
		Worship = 'W',
		HealthPos = 'N',
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
			}
		}
	}
	void resetTileVecs()
	{
		for (int i = 0; i < m_tiles.size(); i++)
		{
			m_tiles[i] = UnusedTile;
		}
		for (int i = 0; i < m_decorTiles.size(); i++)
		{
			m_decorTiles[i] = UnusedTile;
		}
		m_tempTileVec.clear();
		m_rooms.clear();
		m_halls.clear();
	}
	std::vector<char> &getTileMapVec()
	{
		return m_tempTileVec;
	}
	std::vector<char>& getDecorTileVec()
	{
		return m_decorTiles;
	}
	void print()
	{
		for (int row = 0; row < m_height; row++)
		{
			for (int col = 0; col < m_width; col++)
			{
				std::cout << getDecorTile(col, row);
				
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
	char getDecorTile(int t_x, int t_y)
	{
		//gets a decoration tile at a certain x,y
		if (t_x < 0 || t_y < 0 || t_x >= m_width || t_y >= m_height)
			return UnusedTile;
		return m_decorTiles[t_x + t_y * m_width];
	}
	void setTile(int t_x, int t_y, char t_tile)
	{
		m_tiles[t_x + t_y * m_width] = t_tile;
	}
	void setDecorTiles(int t_x, int t_y, char t_tile)
	{
		m_decorTiles[t_x + t_y * m_width] = t_tile;
	}
	void createRoomFeatures(Tilemap*& t_tilemap);
	bool createFeature();
	bool createFeat(int t_x ,int t_y, Direction t_direction);
	bool createRoomtype(int t_x, int t_y, int t_x2, int t_y2, Direction t_direction);
	bool makeRoom(int t_x, int t_y, Direction t_direction,bool t_firstRoom);
	bool makeCorridor(int t_x, int t_y, Direction t_direction);
	bool placeTile(RoomVals& t_rect, char t_tile);
	void placeDecorInRoom();
	void createUniqueRooms() 
	{
		//creates unique rooms in the dungeon
		createFeastRoom();
		createCoffinRoom();
		createLibraryRoom();
		createJailRoom();
	}
	void placeDecorInHalls();
	void FloorDecorTiles();
	void deleteRoom(int i){ m_rooms.erase(m_rooms.begin() + i); }
	void deleteHalls(int i) { m_rooms.erase(m_rooms.begin() + i); }
	void placeTrapsInHalls();
	void playerStartPos();
	void createJailRoom();
	void setSpawnToWall(int t_roomIndex);
	void addChairsDecor(int t_roomIndex);
	void placeBookShelfDecor(int t_roomIndex);
	void setUniqueGroundTiles(char t_newTile, int t_maxRoomSizeWidth, int t_maxRoomSizeHeight,int t_index);
	void bossRoomGroundTiles();
	void worsipRoomGroundTiles();
	void statueRoomTiles();
	bool checkForDoors(int t_x, int t_y);
	void bedRoom();
	void placeBeds(int t_x, int t_y);
	void bossRoom();
	void bossRoomSkull();
	void bossRoomHealth();
	void bossRoomWalls();
	void worshipRoom();
	void worshipRoomDecor();
	void statueRoom();
	void statueRoomDecor();
	bool createJailCells(int t_roomIndex);
	void createCoffinRoom();
	void createFeastRoom();
	void createLibraryRoom();
	void Test() { placeDecorInRoom(); };
	void placeDecorOnWalls();
	bool CheckXAndYPos(int x, int y);
	sf::Vector2i GenXAndYAtTopWall(int t_i, std::vector<RoomVals>& t_tileVec);
	sf::Vector2i GenXAndYAtBottomWall(int t_i, std::vector<RoomVals>& t_tileVec);
	void createTrapsInRooms();
	int GetRoom(int t_roomWidth, int t_roomHeight);
	void setUnusedTile(int x, int y, char t_tile, char t_secondTile);

private:
	static const int m_corridorLengthMin = 5;
	static const int m_corridorLengthMax = 10;
	static const int m_roomChance = 50;
	const int m_maxFeatureNum = 1000;
	static const int m_roomSizeMin = 5;
	static const int m_roomSizeMax = 9;
	int m_bossRoomIndex = 1000;
	int m_worshipRoomIndex = 1000;
	int m_statueRoomIndex = 1000;
	bool m_roomFound = false;
	sf::Vector2f m_startPosition;
	std::vector<char> m_tempTileVec;
	int m_objectCount = 0;
	int m_width;
	int m_height;
	int m_roomMaxDecor = 10;
	std::vector<char> m_tiles;
	std::vector<char> m_decorTiles;
	std::vector<RoomVals> m_halls;
	std::vector<RoomVals> m_rooms;
	std::vector<RoomVals> m_exit;
	std::vector<int> m_tileNums;
	int m_tileDecorArr[30][30];
	int m_tileArr[30][30];
	int m_index = 0;
	Tilemap m_tilemap;
	int m_numJailRoomsPlaced = 0;
	int m_maxJailRooms = 3;
};
