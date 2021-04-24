#pragma once
#include <iostream>
#include <random>
#include <vector>
#include "Tilemap.h"
#include "Debug.h"
#include "Globals.h"
class DunDecor
{
public:
	DunDecor(int t_width, int t_height) :
		m_width(t_width),
		m_height(t_height),
		m_tiles(t_width* t_height, UnusedTile)
		
	{
	}
	~DunDecor();
	void setVecs(std::vector<char>& t_tiles, std::vector<RoomVals>& t_rooms,
		std::vector<RoomVals>& t_exits, std::vector<RoomVals>& t_halls) {
		setTileVec(t_tiles);
		setRooms(t_rooms);
		setHalls(t_halls);
		setExits(t_exits);
	}
	void setTileVec(std::vector<char>& t_tiles){
		m_tiles = t_tiles;
		m_decorTiles = t_tiles;
	}
	void setRooms(std::vector<RoomVals>& t_rooms) {
		m_rooms = t_rooms;
	}
	void setHalls(std::vector<RoomVals>& t_halls) {
		m_halls = t_halls;

	}
	void setExits(std::vector<RoomVals>& t_exits) {
		m_exit = t_exits;
	}

	void resetTileVecs() {
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
	std::vector<char>& getTileMapVec()
	{
		//returns background tiles
		return m_tiles;
	}
	std::vector<char>& getDecorTileVec()
	{
		//returns decoration tiles
		return m_decorTiles;
	}


	void createDunExtras();
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
	void deleteRoom(int i) { m_rooms.erase(m_rooms.begin() + i); }
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
	void setUniqueGroundTiles(char t_newTile, int t_maxRoomSizeWidth, int t_maxRoomSizeHeight, int t_index);
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
	std::vector<char> m_tiles;
	std::vector<char> m_decorTiles;
	std::vector<RoomVals> m_halls;
	std::vector<RoomVals> m_rooms;
	std::vector<RoomVals> m_exit;
	int m_width;
	int m_height;
	int m_bossRoomIndex = 1000;
	int m_worshipRoomIndex = 1000;
	int m_statueRoomIndex = 1000;
	

	int m_index = 0;
	int m_numJailRoomsPlaced = 0;
	int m_maxJailRooms = 3;
};

