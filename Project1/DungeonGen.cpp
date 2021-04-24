#include "DungeonGen.h"
////---------------------------------------------------------------------------
////Creates the starting room and then creates all corridors or rooms from that
////---------------------------------------------------------------------------
void DungeonGen::generateDungeon(int t_maxTilesChanged)
{
	//create the first room in the map in the middle of the map
	makeRoom(m_width / 2, m_height / 2, static_cast<Direction>(randomInt(4)), true);
	for (int i = 1; i < t_maxTilesChanged; i++)
	{
		if (dungeonCreationStart() ==false)
		{
			DEBUG_MSG("Cant place more features");
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
	
	generateDungeon(100);
	m_dunDecor->setVecs(m_tiles, m_rooms, m_exit, m_halls);
	m_dunDecor->createDunExtras();
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
	int directionVal = 1;
	//change the x,y depending on the direction
	if (t_direction == Direction::North)
	{
		y = directionVal;
	}
	else if (t_direction == Direction::South)
	{
		y = -directionVal;
	}
	else if (t_direction == Direction::West)
	{
		x = directionVal;
	}
	else if (t_direction == Direction::East)
	{
		x = -directionVal;
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
	else
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
	int moveByOne = 1;
	//check the directiona nd change the x,y values depending on the directon
	if (t_direction == Direction::North)
	{
		room.x = t_x - room.width / 2;
		room.y = t_y - room.height;
	}
	else if (t_direction == Direction::South)
	{
		room.x = t_x - room.width / 2;
		room.y = t_y + moveByOne;
	}
	else if (t_direction == Direction::West)
	{
		room.x = t_x - room.width;
		room.y = t_y - room.height / 2;
	}
	else if (t_direction == Direction::East)
	{
		room.x = t_x + moveByOne;
		room.y = t_y - room.height / 2;
	}
	if (placeTile(room , FloorTile))
	{
		m_rooms.emplace_back(room);
		placeRoomExit(room, t_direction, t_firstRoom);
		return true;
	}
	return false;
}
////---------------------------------------------------------------------------
////checks the direction and places a exit in the direction passed
////---------------------------------------------------------------------------
void DungeonGen::placeRoomExit(RoomVals& t_room, Direction t_direction ,bool t_firstRoom)
{
	//place exit at north Side
	if (t_direction == Direction::South || t_firstRoom)
	{
		m_exit.emplace_back(RoomVals{ t_room.x,t_room.y + t_room.height ,t_room.width,1 });
	}
	//place exit at south Side
	if (t_direction == Direction::North || t_firstRoom)
	{
		m_exit.emplace_back(RoomVals{ t_room.x,t_room.y - 1,t_room.width,1 });
	}
	//place exit at west Side
	if (t_direction == Direction::East || t_firstRoom)
	{
		m_exit.emplace_back(RoomVals{ t_room.x + t_room.width,t_room.y,1,t_room.height });
	}
	//place exit at east Side
	if (t_direction == Direction::West || t_firstRoom)
	{
		m_exit.emplace_back(RoomVals{ t_room.x - 1, t_room.y,1,t_room.height });
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
	//generate a random bool to determine the direction of the corridor
	if (randomBool())
	{
		//create the width to be a random int within a certain range and a height of 2
		corridor.width = randomInt(m_corridorLengthMin, m_corridorLengthMax);
		corridor.height = m_corridorHeight;
		//minus 1 from passed y to move one up the y e.g north
		if (t_direction == Direction::North)
		{
			corridor.y = t_y - 1;
		}
		//plus 1 onto passed y to move one down the y e.g south
		else if (t_direction == Direction::South)
		{
			corridor.y = t_y + 1;
		}
		//move the x back from passed in x minus the width e.g West 
		else if (t_direction == Direction::West)
		{
			corridor.x = t_x - corridor.width;
		}
		//if east then move one along the x from passed in x e.g East
		else if (t_direction == Direction::East)
		{
			corridor.x = t_x + 1;
		}
	}
	//this else is for a vertical corridor
	else
	{
		//create a width of 2 and height thats random between certain range
		corridor.width = m_corridorWidth;
		corridor.height = randomInt(m_corridorLengthMin, m_corridorLengthMax);
		//move the y back from passed in x minus the height if North
		if (t_direction == Direction::North)
		{
			corridor.y = t_y - corridor.height;
		}
		//plus 1 onto passed y to move one down the y e.g south
		else if (t_direction == Direction::South)
		{
			corridor.y = t_y + 1;
		}
		//from passed in x move  minus one e.g to the west
		else if (t_direction == Direction::West)
		{
			corridor.x = t_x - 1;
		}
		//from passed in x move plus one e.g to the East
		else if (t_direction == Direction::East)
		{
			corridor.x = t_x + 1;
		}
	}
	//create the room e.g the  corridor
	if (placeTile(corridor, FloorTile))
	{
		//add the coridor to the m_halls vector
		m_halls.emplace_back(corridor);
		//check the direction along the y e.g south or North and that the width is not 1 and create a exit
		placeCorridorExit(corridor,t_direction);
		return true;
	}
	return false;
}
////---------------------------------------------------------------------------
////checks the direction and places a exit on all exept the passed in direction
////---------------------------------------------------------------------------
void DungeonGen::placeCorridorExit(RoomVals& t_room,Direction t_direction)
{
	if (t_direction != Direction::South && t_room.width != 1)
	{
		m_exit.emplace_back(RoomVals{ t_room.x,t_room.y - 1,t_room.width,1 });
	}
	if (t_direction != Direction::North && t_room.width != 1)
	{
		m_exit.emplace_back(RoomVals{ t_room.x,t_room.y + t_room.height,t_room.width,1 });
	}
	//check the direction along the x e.g east or west and that the height is not 1 and create a exit
	if (t_direction != Direction::East && t_room.height != 1)
	{
		m_exit.emplace_back(RoomVals{ t_room.x - 1, t_room.y, 1, t_room.height });
	}
	if (t_direction != Direction::West && t_room.height != 1)
	{
		m_exit.emplace_back(RoomVals{ t_room.x + t_room.width, t_room.y, 1, t_room.height });
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
