#pragma once
#include <iostream>
#include <random>
#include <vector>
#include "Tilemap.h"
#include "Debug.h"
//namespace for random generation
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
	// struct to store room values
	struct RoomVals
	{
		int x, y;
		int width, height;
		int decorInRoom = 0;
	};
	//enum for all the types of tiles
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
	//enum for the directions
	enum Direction
	{
		North,
		South,
		West,
		East,
		DirectionNum
	};
	//constructor to set up the dungeon , sets teh size and all decor and background tiles to unused
	DungeonGen(int t_width, int t_height) :
		m_width(t_width),
		m_height(t_height),
		m_tiles(t_width* t_height, UnusedTile),
		m_decorTiles(t_width* t_height, UnusedTile),
		m_rooms(),
		m_exit()
	{
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
				if (getTile(x, y) != UnusedTile)
				{
					return tileCanBePlaced;
				}
			}
		}
	}
	//func to generte the map
	void generateMap(int t_maxFeatures);
	void resetTileVecs()
	{
		if (m_tiles.empty() == false)
		{
			//loop and reset all values in the vector for background and decor tiles
			for (int i = 0; i < m_tiles.size(); i++)
			{
				m_tiles[i] = UnusedTile;
			}
		}
		if (m_decorTiles.empty() == false)
		{
			for (int i = 0; i < m_decorTiles.size(); i++)
			{
				m_decorTiles[i] = UnusedTile;
			}
		}
		//clears the vectors for rooms and the halls if there not empty
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
	std::vector<char>& getDecorTileVec()
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
				std::cout << getDecorTile(col, row);
				
			}
			std::cout << std::endl;
		}
	}
	char getTile(int t_x, int t_y)
	{
		//returns tile at x,y from background tiles
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
		//sets tile at x,y of background tiles
		m_tiles[t_x + t_y * m_width] = t_tile;
	}
	void setDecorTiles(int t_x, int t_y, char t_tile)
	{
		//sets tile at x,y of decoration tiles
		m_decorTiles[t_x + t_y * m_width] = t_tile;
	}
	//functions to start the creation of rooms and halls in dungeon
	void createRoomFeatures(Tilemap*& t_tilemap);
	bool createFeature();
	bool createRoomOrCor(int t_x ,int t_y, Direction t_direction);
	bool createRoomtype(int t_x, int t_y, int t_x2, int t_y2, Direction t_direction);
	bool makeRoom(int t_x, int t_y, Direction t_direction,bool t_firstRoom);
	bool makeCorridor(int t_x, int t_y, Direction t_direction);
	bool placeTile(RoomVals& t_room, char t_tile);
	//functions to create decorations rooms
	void placeDecorInRoom();
	void createUniqueRooms() 
	{
		//creates unique rooms in the dungeon
		createFeastRoom();
		createCoffinRoom();
		createLibraryRoom();
		createJailRoom();
	}
	//functions to create decorations halls, add more floor tiles,delete rooms / halls and place traps in rooms
	void placeDecorInHalls();
	void FloorDecorTiles();
	void deleteRoom(int i){ m_rooms.erase(m_rooms.begin() + i); }
	void deleteHalls(int i) { m_rooms.erase(m_rooms.begin() + i); }
	void placeTrapsInHalls();
	//functions to create rooms with spawn/exits,a jails
	void playerStartPos();
	void createJailRoom();
	//functions to create wall spawns,add chairs to a room,place the bookshelf for library
	void setSpawnToWall(int t_roomIndex);
	void addChairsDecor(int t_roomIndex);
	void placeBookShelfDecor(int t_roomIndex);
	//functions to create rooms with differnt ground tiles
	void setUniqueGroundTiles(char t_newTile, int t_maxRoomSizeWidth, int t_maxRoomSizeHeight,int t_index);
	void bossRoomGroundTiles();
	void worsipRoomGroundTiles();
	void statueRoomTiles();
	//functions to create bedroom,check the rooms xy for doors,setup the boss room
	bool checkForDoors(int t_x, int t_y);
	void bedRoom();
	void placeBeds(int t_x, int t_y);
	void bossRoom();
	void bossRoomSkull();
	void bossRoomHealth();
	void bossRoomWalls();
	//functions to create worship and statue rooms
	void worshipRoom();
	void worshipRoomDecor();
	void statueRoom();
	void statueRoomDecor();
	//functions to help create jails in jail room,create a coffin room,feast and library rooms
	bool createJailCells(int t_roomIndex);
	void createCoffinRoom();
	void createFeastRoom();
	void createLibraryRoom();
	//functions to create room with decor on walls,check the xy of a room
	void placeDecorOnWalls();
	bool CheckXAndYPos(int x, int y);
	//functions to get a x,y pos at the top and bottom of a room
	sf::Vector2i GenXAndYAtTopWall(int t_i, std::vector<RoomVals>& t_tileVec);
	sf::Vector2i GenXAndYAtBottomWall(int t_i, std::vector<RoomVals>& t_tileVec);
	//functions to create traps in each room,get room and set the xy and with a offsert of a  room to ususable tile
	void createTrapsInRooms();
	int GetRoom(int t_roomWidth, int t_roomHeight);
	void setUnusedTile(int x, int y, char t_tile, char t_secondTile);

private:
	//all variables
	//the min and max size of the corridors,and a room chance of 50 percent
	static const int m_corridorLengthMin = 5;
	static const int m_corridorLengthMax = 10;
	static const int m_roomChance = 50;
	//set the number of times to loop when ceating room and halls
	const int m_maxFeatureNum = 1000;
	//min and max room sizes
	static const int m_roomSizeMin = 5;
	static const int m_roomSizeMax = 9;
	//set the index of the boss,worship and statue room to unvalid
	int m_bossRoomIndex = 1000;
	int m_worshipRoomIndex = 1000;
	int m_statueRoomIndex = 1000;
	//set the width and height of map and staring pos and max decor in rooms
	bool m_roomFound = false;
	sf::Vector2f m_startPosition;
	int m_objectCount = 0;
	int m_width;
	int m_height;
	int m_roomMaxDecor = 10;
	//vectors to staore the layout and the rooms etc
	std::vector<char> m_tiles;
	std::vector<char> m_decorTiles;
	std::vector<RoomVals> m_halls;
	std::vector<RoomVals> m_rooms;
	std::vector<RoomVals> m_exit;
	std::vector<int> m_tileNums;
	//set the number of jails and jail rooms
	int m_index = 0;
	int m_numJailRoomsPlaced = 0;
	int m_maxJailRooms = 3;
};
