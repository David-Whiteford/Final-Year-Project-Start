#pragma once
#include <iostream>
#include <random>
#include <vector>
#include "Tilemap.h"
#include "Debug.h"
#include "Globals.h"
#include "DungeonDecor.h"
#include "DunSpecialRooms.h"
//namespace for random generation

class DungeonGen
{
public:
	//constructor to set up the dungeon , sets teh size and all decor and background tiles to unused
	DungeonGen(int t_width, int t_height) :
		m_width(t_width),
		m_height(t_height),
		m_tiles(t_width* t_height, UnusedTile),
		m_decorTiles(t_width* t_height, UnusedTile)
	{
		DEBUG_MSG("Construct Dungeon");
		m_dunDecor = new DunDecor(m_width, m_height);
		m_dunSpecialDecor = new DunSpecialRooms(m_width, m_height);
	}
	~DungeonGen()
	{
		DEBUG_MSG("De-Construct Dungeon");
		resetTileVecs();
		delete m_dunDecor;
		delete m_dunSpecialDecor;
	}
	//funtions for the creation of the dungeon 
	bool placeTileVal(RoomVals& t_room, char t_tile);
	bool checkTile(RoomVals& t_room)
	{
		bool tileCanBePlaced = false;
		int offSetByOne = 1;
		//if the passed in rooms x,y are outside the rooms square then return false
		if (t_room.x < offSetByOne || t_room.y < offSetByOne
			|| t_room.x + t_room.width > m_width - offSetByOne
			|| t_room.y + t_room.height > m_height - offSetByOne)
		{
			return tileCanBePlaced;
		}
		//loop through the tiles starting at a passed in tile and check if there all unpassable and return false if so
		for (int y = t_room.y; y < t_room.y + t_room.height; y++)
		{
			for (int x = t_room.x; x < t_room.x + t_room.width; x++)
			{
				if (getTile(x, y,m_tiles,m_width,m_height) != UnusedTile)
				{
					return tileCanBePlaced;
				}
			}
		}
	}
	//func to generte the dungeon
	void generateDungeon(int t_maxTilesChanged);
	void FloorDecorTiles();
	void resetTileVecs()
	{
		//clears the vectors for rooms and the halls if there not empty
		//clears the vectors for the tiles and decoration tiles
		if (m_tiles.empty() == false)
		{
			m_tiles.clear();
		}
		if (m_decorTiles.empty() == false)
		{
			m_decorTiles.clear();
		}
		if (m_rooms.empty() == false)
		{
			m_rooms.clear();
		}
		if (m_halls.empty() == false)
		{
			m_halls.clear();
		}
	}
	std::vector<char> &getTileMapVec()
	{
		//returns background tiles
		return m_tiles;
	}
	std::vector<char> &getDecorTileVec()
	{
		//returns decoration tiles
		return m_decorTiles;
	}
	void print()
	{
		//prints the generates dungeon as 2D text
		for (int row = 0; row < m_height; row++)
		{
			for (int col = 0; col < m_width; col++)
			{
				std::cout << getTile(col, row,m_decorTiles,m_width,m_height);
			}
			std::cout << std::endl;
		}
	}
	//functions to start the creation of rooms and halls in dungeon
	void createDunExtras();
	bool dungeonCreationStart();
	bool createRoomOrCor(int t_x ,int t_y, Direction t_direction);
	bool createRoomtype(int t_x, int t_y, int t_x2, int t_y2, Direction t_direction);
	bool makeRoom(int t_x, int t_y, Direction t_direction,bool t_firstRoom);
	bool makeCorridor(int t_x, int t_y, Direction t_direction);
	bool placeTile(RoomVals& t_room, char t_tile);
	void placeCorridorExit(RoomVals& t_room, Direction t_direction);
	void placeRoomExit(RoomVals& t_room, Direction t_direction, bool t_firstRoom);

private:
	DunDecor* m_dunDecor;
	DunSpecialRooms* m_dunSpecialDecor;
	//all variables
	//the min and max size of the corridors,and a room chance of 50 percent
	static const int m_corridorLengthMin = 5;
	static const int m_corridorLengthMax = 10;
	static const int m_roomChance = 50;
	//set the number of times to loop when ceating room and halls
	const int m_maxTilesChanged = 2500;
	int m_maxRoomsCreated = 100;
	int m_corridorNum = 0;
	int m_roomNum = 0;
	//min and max room sizes
	static const int m_roomSizeMin = 5;
	static const int m_roomSizeMax = 9;
	static const int m_corridorWidth = 2;
	static const int m_corridorHeight = 2;
	int m_numberOfDirections = 4;

	//set the width and height of map and staring pos and max decor in rooms
	bool m_roomFound = false;
	sf::Vector2f m_startPosition;
	int m_objectCount = 0;
	int m_width;
	int m_height;
	sf::Vector2i m_mapHalfPoint = sf::Vector2i(m_width / 2,m_height/2);
	int m_roomMaxDecor = 10;
	//vectors to staore the layout and the rooms etc
	std::vector<char> m_tiles;
	std::vector<char> m_decorTiles;
	std::vector<RoomVals> m_halls;
	std::vector<RoomVals> m_rooms;
	std::vector<RoomVals> m_exit;
	std::vector<int> m_tileNums;

};
