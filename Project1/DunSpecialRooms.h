#pragma once
#include <iostream>
#include <random>
#include <vector>
#include "Tilemap.h"
#include "Debug.h"
#include "Globals.h"
class DunSpecialRooms
{
public:
	DunSpecialRooms(int t_width, int t_height) :
		m_width(t_width),
		m_height(t_height)
	{
	}
	~DunSpecialRooms();
	void createDunSpecialRooms(std::vector<char>& t_tiles, std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms,
		std::vector<RoomVals>& t_exits, std::vector<RoomVals>& t_halls);
	//functions to create rooms with differnt ground tiles
	void setUniqueGroundTiles(char t_newTile, int t_maxRoomSizeWidth, int t_maxRoomSizeHeight, int t_index,
		std::vector<char>& t_tiles, std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms);
	//replace tiles in the boss room with a specific tile for the new room
	void bossRoomGroundTiles(std::vector<char>& t_tiles, std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms);
	void worsipRoomGroundTiles(std::vector<char>& t_tiles, std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms);
	void statueRoomTiles(std::vector<char>& t_tiles, std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms);
	//creates the boss room and adds decorations to it
	void bossRoom(std::vector<char>& t_tiles, std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms);
	void bossRoomSkull(std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms);
	void bossRoomHealth(std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms);
	void bossRoomWalls(std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms);
	//functions to create worship and statue rooms
	void worshipRoom(std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms, std::vector<char>& t_tiles);
	void worshipRoomDecor(std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms);
	void statueRoom(std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms, std::vector<char>& t_tiles);
	void statueRoomDecor(std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms);

private:
	//all rooms give 1000 index as the 0 value is a index within the vector and 1000 means a invalid index
	int m_bossRoomIndex = 1000;
	int m_worshipRoomIndex = 1000;
	int m_statueRoomIndex = 1000;
	//variables for the generation of the dungeon special rooms
	int m_width;
	int m_height;

};
