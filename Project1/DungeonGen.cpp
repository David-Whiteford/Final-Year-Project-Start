#include "DungeonGen.h"
////---------------------------------------------------------------------------
////Creates the starting room and then creates all corridors or rooms from that
////---------------------------------------------------------------------------
void DungeonGen::generateDungeon(int t_maxRoomsCreated)
{
	bool dungeonCreationStarted = false;
	//create the first room in the map in the middle of the map
	makeRoom(m_mapHalfPoint.x, m_mapHalfPoint.y, static_cast<Direction>(randomInt(4)), true);
	for (int i = 1; i < t_maxRoomsCreated; i++)
	{
		dungeonCreationStarted = dungeonCreationStart();
		if (dungeonCreationStarted == false)
		{
			DEBUG_MSG("Cant place more rooms or corridors");
			break;
		}
	}
}
////---------------------------------------------------------------------------
////This creates the dungeon and then creates the differnt rooms that have different decor
////Can be called to create a dungeon and decorate it
////---------------------------------------------------------------------------
void DungeonGen::createDunExtras()
{
	generateDungeon(m_maxRoomsCreated);
	FloorDecorTiles();
	m_decorTiles = m_tiles;
	m_dunSpecialDecor->createDunSpecialRooms(m_tiles, m_decorTiles, m_rooms, m_exit, m_halls);
	m_dunDecor->createDunExtras(m_tiles,m_decorTiles,m_rooms,m_exit,m_halls);
	//if the debug is active then output the rooms in text
	if (DEBUG == 1)
	{
		std::cout << "Empty Rooms Left " << m_rooms.size() << std::endl;
		print();
	}
}
////---------------------------------------------------------------------------
////Create the starting x ,y values for the next room or corridor. get these from the exits vector 
///the exits vector at this point hold values for the first room
////---------------------------------------------------------------------------
bool DungeonGen::dungeonCreationStart()
{
	//if there are exits present the continue
	if (m_exit.empty() == false)
	{
		//loop till the max features are met
		for (int i = 0; i < m_maxTilesChanged; i++)
		{
			//get a random exit of a romdom room / corridor
			int r = randomInt(m_exit.size());
			int x = randomInt(m_exit[r].x, m_exit[r].x + m_exit[r].width - 1);
			int y = randomInt(m_exit[r].y, m_exit[r].y + m_exit[r].height - 1);
			//loop through all direction
			for (int j = 0; j < m_numberOfDirections; j++)
			{
				if (createRoomOrCor(x, y, static_cast<Direction>(j)))
				{
					//delete the exit once used
					m_exit.erase(m_exit.begin() + r);
					return true;
				}
			}
		}
	}
	return false;
}
////---------------------------------------------------------------------------
////Start the setup to create a room or a corridor, change the x ,y depending on the direction
////---------------------------------------------------------------------------
bool DungeonGen::createRoomOrCor(int t_x, int t_y, Direction t_direction)
{
	bool successCreatingRoom = false;
	int x = 0;
	int y = 0;
	//change the x,y depending on the direction
	int directionVal = 1;
	switch (t_direction)
	{
	case Direction::North:
		y = directionVal;
		break;
	case Direction::South:
		y = -directionVal;
		break;
	case Direction::West:
		x = directionVal;
		break;
	case Direction::East:
		x = -directionVal;
		break;
	case Direction::NoDirection:
		x = 0;
		y = 0;
		break;
	default:
		break;
	}
	//check that the tile is a floor tile
	int newX = t_x + x;
	int newY = t_y + y;
	if (getTile(newX, newY,m_tiles,m_width,m_height) != FloorTile)
	{
		return successCreatingRoom;
	}
	return successCreatingRoom = createRoomtype(t_x, t_y, x, y, t_direction);
	return successCreatingRoom;
}
////---------------------------------------------------------------------------
////Determine the type of room e.g corridor or if its a large room
////---------------------------------------------------------------------------
bool DungeonGen::createRoomtype(int t_x, int t_y, int t_x2, int t_y2, Direction t_direction)
{
	//generate a random number from 0 to 100  
	int randRoomVal = randomInt(0, 100);
	//check if the number is less than room chance and if so place room
	if (randRoomVal <= m_roomChance)
	{
		bool firstRoom = false;
		//create a room thats not the first one
		if (makeRoom(t_x, t_y, t_direction, firstRoom))
		{
			//place two types of door2 for north and south and door1 for east,west
			if (t_direction == Direction::North || t_direction == Direction::South) {
				setTile(t_x, t_y, Door2, m_tiles, m_width);
				return true;
			}
			else {
				setTile(t_x, t_y, Door1,m_tiles,m_width);
				return true;
			}
		}
	}
	else if(randRoomVal > m_roomChance)
	{
		if (makeCorridor(t_x, t_y, t_direction))
		{
			//check if its a floor tile and if not then make it one(prvents door placesd in middle of corridor)
			if (getTile(t_x + t_x2, t_y + t_y2, m_tiles, m_width,m_height) == FloorTile)
			{
				//place two types of door2 for north and south and door1 for east,west
				if (t_direction == Direction::North || t_direction == Direction::South)
				{
					setTile(t_x, t_y, Door2, m_tiles, m_width);
					return true;
				}
				else {
					setTile(t_x, t_y, Door1, m_tiles, m_width);
					return true;
				}
			}
			else
			{
				setTile(t_x, t_y, FloorTile, m_tiles, m_width);
			}
			return true;
		}
	}
	return false;
}
////---------------------------------------------------------------------------
////creates a room that has a random value within a range for the size of the room
////---------------------------------------------------------------------------
bool DungeonGen::makeRoom(int t_x, int t_y, Direction t_direction, bool t_firstRoom)
{
	RoomVals room;
	//get the width and height of room between the max and min Sizes
	room.width = randomInt(m_roomSizeMin, m_roomSizeMax);
	room.height = randomInt(m_roomSizeMin, m_roomSizeMax);
	int half = 2;
	int moveByOne = 1;
	//check the directiona nd change the x,y values depending on the directon
	switch (t_direction)
	{
	case Direction::North:
		room.x = t_x - room.width / half;
		room.y = t_y - room.height;
		break;
	case Direction::South:
		room.x = t_x - room.width / half;
		room.y = t_y + moveByOne;
		break;
	case Direction::West:
		room.x = t_x - room.width;
		room.y = t_y - room.height / half;
		break;
	case Direction::East:
		room.x = t_x + moveByOne;
		room.y = t_y - room.height / half;
		break;
	default:
		break;
	}
	bool tilePlaced = false;
	tilePlaced = placeTile(room, FloorTile);
	if(tilePlaced == true)
	{
		m_rooms.push_back(room);
		placeRoomExit(room, t_direction, t_firstRoom);
		return tilePlaced;
	}
	return tilePlaced;
}
////---------------------------------------------------------------------------
////checks the direction and places a exit in the direction passed
////---------------------------------------------------------------------------
void DungeonGen::placeRoomExit(RoomVals& t_room, Direction t_direction ,bool t_firstRoom)
{
/***************************************************************************************
*    Title: C++ Example of Dungeon-Building Algorithm
*    Author: MindControlDx
*    Date: 31 July 2015
*    Code version: Version 3
*    Availability: http://www.roguebasin.com/index.php?title=C%2B%2B_Example_of_Dungeon-Building_Algorithm
*
***************************************************************************************/
	//place exit at north Side
	if (t_direction == Direction::South || t_firstRoom)
	{
		m_exit.push_back(RoomVals{ t_room.x,t_room.y + t_room.height ,t_room.width,1 });
	}
	//place exit at south Side
	if (t_direction == Direction::North || t_firstRoom)
	{
		m_exit.push_back(RoomVals{ t_room.x,t_room.y - 1,t_room.width,1 });
	}
	//place exit at west Side
	if (t_direction == Direction::East || t_firstRoom)
	{
		m_exit.push_back(RoomVals{ t_room.x + t_room.width,t_room.y,1,t_room.height });
	}
	//place exit at east Side
	if (t_direction == Direction::West || t_firstRoom)
	{
		m_exit.push_back(RoomVals{ t_room.x - 1, t_room.y,1,t_room.height });
	}
}
////---------------------------------------------------------------------------
////creates a corridor in the horizontal and vertical of a certain width and height
////---------------------------------------------------------------------------
bool DungeonGen::makeCorridor(int t_x, int t_y, Direction t_direction)
{
	//create a room of type corridor, with x ,y 
	RoomVals corridor;
	corridor.x = t_x;
	corridor.y = t_y;
	bool tilePlaced = false;
	//generate a random bool to determine if corridor along the x or y axis
	bool corridorVal = randomBool();
	if (corridorVal == true)
	{
		//create the width to be a random int within a certain range and a height of 2
		corridor.width = randomInt(m_corridorLengthMin, m_corridorLengthMax);
		corridor.height = m_corridorHeight;
		//minus 1 from passed y to move one up the y e.g north
		switch (t_direction)
		{
		case Direction::North:
			corridor.y = t_y - 1;
			break;
		case Direction::South:
			corridor.y = t_y + 1;
			break;
		case Direction::West:
			corridor.x = t_x - corridor.width;
			break;
		case Direction::East:
			corridor.x = t_x + 1;
			break;
		default:
			break;
		}
	}
	//this else is for a vertical corridor
	else if(corridorVal == false)
	{
		//create a width of 2 and height thats random between certain range
		corridor.width = m_corridorWidth;
		corridor.height = randomInt(m_corridorLengthMin, m_corridorLengthMax);
		//move the y back from passed in x minus the height if North
		switch (t_direction)
		{
		case Direction::North:
			corridor.y = t_y - corridor.height;
			break;
		case Direction::South:
			corridor.y = t_y + 1;
			break;
		case Direction::West:
			corridor.x = t_x - 1;
			break;
		case Direction::East:
			corridor.x = t_x + 1;
			break;
		default:
			break;
		}
	}
	//create the room e.g the  corridor
	tilePlaced = placeTile(corridor, FloorTile);
	if (tilePlaced == true)
	{
		//add the coridor to the m_halls vector
		m_halls.push_back(corridor);
		//check the direction along the y e.g south or North and that the width is not 1 and create a exit
		placeCorridorExit(corridor,t_direction);
		return tilePlaced;
	}
	return tilePlaced;
}
////---------------------------------------------------------------------------
////checks the direction and places a exit on all exept the passed in direction
////---------------------------------------------------------------------------
void DungeonGen::placeCorridorExit(RoomVals& t_room,Direction t_direction)
{
/***************************************************************************************
*    Title: C++ Example of Dungeon-Building Algorithm
*    Author: MindControlDx
*    Date: 31 July 2015
*    Code version: Version 3
*    Availability: http://www.roguebasin.com/index.php?title=C%2B%2B_Example_of_Dungeon-Building_Algorithm
*
***************************************************************************************/
	if (t_direction != Direction::South && t_room.width != 1)
	{
		m_exit.push_back(RoomVals{ t_room.x,t_room.y - 1,t_room.width,1 });
	}
	if (t_direction != Direction::North && t_room.width != 1)
	{
		m_exit.push_back(RoomVals{ t_room.x,t_room.y + t_room.height,t_room.width,1 });
	}
	//check the direction along the x e.g east or west and that the height is not 1 and create a exit
	if (t_direction != Direction::East && t_room.height != 1)
	{
		m_exit.push_back(RoomVals{ t_room.x - 1, t_room.y, 1, t_room.height });
	}
	if (t_direction != Direction::West && t_room.height != 1)
	{
		m_exit.push_back(RoomVals{ t_room.x + t_room.width, t_room.y, 1, t_room.height });
	}
}
////---------------------------------------------------------------------------
////checks the tiles are unusable and so places a tile of a certain type for the rooms and corridors when called
////---------------------------------------------------------------------------
bool DungeonGen::placeTile(RoomVals& t_room, char t_tile)
{
	//call the tile check
	bool tilePlaced = false;
	if (!checkTile(t_room))
	{
		return tilePlaced;
	}
	//then place the tile function call
	tilePlaced = placeTileVal(t_room, t_tile);
	return tilePlaced;
}
////---------------------------------------------------------------------------
////Given the rooma and a tile type it then loops throgh and places the tiles in a 
////room from the room walls to the tiles within the room
////---------------------------------------------------------------------------
bool DungeonGen::placeTileVal(RoomVals& t_room, char t_tile)
{
	bool tileplaced = false;
	//loop through each tile in the room
	for (int y = t_room.y - 1; y < t_room.y + t_room.height + 1; y++)
	{
		for (int x = t_room.x - 1; x < t_room.x + t_room.width + 1; x++)
		{
			//if the tiles are the outside tiles then make them a wall
			if (x == t_room.x - 1
				|| y == t_room.y - 1
				|| x == t_room.x + t_room.width
				|| y == t_room.y + t_room.height)
			{
				setTile(x, y, Wall, m_tiles, m_width);
				tileplaced = true;
			}
			//otherwise there all what ever tile you want
			else
			{
				setTile(x, y, t_tile, m_tiles, m_width);
				tileplaced = true;
			}
		}
	}
	return tileplaced;
}
////---------------------------------------------------------------------------
////Function to add stone to the floor tiles
////---------------------------------------------------------------------------
void DungeonGen::FloorDecorTiles()
{
	//checks if the room vec is empty 
	if (m_rooms.empty() == false)
	{
		//if not loop through
		for (int i = 0; i < m_rooms.size(); i++)
		{
			// get the starting and the end for the x,y
			int xStart = m_rooms[i].x;
			int xEnd = m_rooms[i].x + m_rooms[i].width;
			int yStart = m_rooms[i].y + 1;
			int yBottom = m_rooms[i].y + m_rooms[i].height - 2;

			//loop till x gets to the end from the starting x pos
			for (int x = xStart; x < xEnd; x++)
			{
				if (x != xEnd)
				{
					//check tiles are floor tile and replace them with stone floor tiles
					if (getTile(x, yStart, m_tiles, m_width, m_height) == FloorTile || getTile(x, yBottom,m_tiles,m_width,m_height) == FloorTile)
					{
						setTile(x, yStart, StoneFloorTile, m_tiles, m_width);
						setTile(x, yBottom, StoneFloorTile, m_tiles, m_width);
					}
				}
			}
			//change the ending of the x
			xEnd = m_rooms[i].x + m_rooms[i].width - 1;
			// loop from the staring y to the ending y 
			for (int y = yStart; y < yBottom; y++)
			{
				if (y != yBottom)
				{
					//check if they are floor tiles and replace them with stone floor tiles
					if (getTile(xStart, y, m_tiles, m_width,m_height) == FloorTile 
						|| getTile(xEnd, y, m_decorTiles, m_width, m_height) == FloorTile)
					{
						setTile(xStart, y, StoneFloorTile, m_tiles, m_width);
						setTile(xEnd, y, StoneFloorTile, m_tiles, m_width);
					}
				}
			}
		}
	}
}