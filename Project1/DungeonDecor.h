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
		m_height(t_height)
	{
	}
	~DunDecor();
	void createDunExtras(std::vector<char>& t_tiles, std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms,
		std::vector<RoomVals>& t_exits, std::vector<RoomVals>& t_halls);
	//functions to create decorations rooms
	void placeDecorInRoom(std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms);
	void createUniqueRooms(std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms, std::vector<char>& t_tiles)
	{
		//creates unique rooms in the dungeon
		createFeastRoom(t_decorTiles, t_rooms, t_tiles);
		createCoffinRoom(t_decorTiles, t_rooms);
		createLibraryRoom(t_decorTiles, t_rooms);
		createJailRoom(t_decorTiles, t_rooms);
	}
	//functions to create decorations halls, add more floor tiles,delete rooms / halls and place traps in rooms
	void placeDecorInHalls(std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_halls);
	void placeTrapsInHalls(std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_halls);
	//functions to create rooms with spawn/exits,a jails
	void playerStartPos(std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms);
	void createJailRoom(std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms);
	//functions to create wall spawns,add chairs to a room,place the bookshelf for library
	void setSpawnToWall(int t_roomIndex, std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms);
	void addChairsDecor(int t_roomIndex, std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms);
	void placeBookShelfDecor(int t_roomIndex, std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms);
	//creates and places bedrooms and bedroom tiles
	void bedRoom(std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms);
	void placeBeds(int t_x, int t_y,std::vector<char>& t_decorTiles);
	//functions to help create jails in jail room,create a coffin room,feast and library rooms
	bool createJailCells(int t_roomIndex, std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms);
	void createCoffinRoom(std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms);
	void createFeastRoom(std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms,std::vector<char>& t_tiles);
	void createLibraryRoom(std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms);
	//functions to create room with decor on walls,check the xy of a room
	void placeDecorOnWalls(std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms);
	bool CheckXAndYPos(int x, int y);
	//functions to get a x,y pos at the top and bottom of a room
	sf::Vector2i GenXAndYAtTopWall(int t_i, std::vector<RoomVals>& t_tileVec);
	sf::Vector2i GenXAndYAtBottomWall(int t_i, std::vector<RoomVals>& t_tileVec);
	//functions to create traps in each room,get room and set the xy and with a offsert of a  room to ususable tile
	void createTrapsInRooms(std::vector<char>& t_decorTiles,std::vector<RoomVals>& t_rooms, std::vector<RoomVals>& t_halls);
	
	
private:
	//variables needed for special rooms
	int m_width;
	int m_height;

	int m_index = 0;
	int m_numJailRoomsPlaced = 0;
	int m_maxJailRooms = 3;
};

