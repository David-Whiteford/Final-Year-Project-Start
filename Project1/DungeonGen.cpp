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
void DungeonGen::createDunExtras(Tilemap*& t_tilemap)
{
	int numberOfRooms = 2;
	generateDungeon(100);
	FloorDecorTiles();
	//create a vector to hold the background tiles and decor tiles
	m_decorTiles = m_tiles;
	//loop to create 2 of the below rooms
	for (int i = 0; i < numberOfRooms; i++)
	{
		worsipRoomGroundTiles();
		worshipRoom();
		statueRoomTiles();
		statueRoom();
	}
	//create a boss room ground tiles and the room itself
	bossRoomGroundTiles();
	bossRoom();
	//create unique rooms
	createUniqueRooms();
	//example of a trigger that was set up by procedual generation
	createTrapsInRooms();
	//place triggers in rooms to exit dungeon
	playerStartPos();
	//place decor on the walls,in rooms, halls
	placeDecorOnWalls();
	placeDecorInRoom();
	placeDecorInHalls();
	//if rooms are left call the place decor to fill them
	if (m_rooms.empty() == false){
		placeDecorInRoom();
	}
	//create a bedroom
	bedRoom();
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
	if (getTile(newX, newY) != FloorTile)
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
				setTile(t_x, t_y, Door2);
				return true;
			}
			else {
				setTile(t_x, t_y, Door1);
				return true;
			}
		}
	}
	else
	{
		if (makeCorridor(t_x, t_y, t_direction))
		{
			//check if its a floor tile and if not then make it one(prvents door placesd in middle of corridor)
			if (getTile(t_x + t_x2, t_y + t_y2) == FloorTile)
			{
				//place two types of door2 for north and south and door1 for east,west
				if (t_direction == Direction::North || t_direction == Direction::South)
				{
					setTile(t_x, t_y, Door2);
					return true;
				}
				else {
					setTile(t_x, t_y, Door1);
					return true;
				}
			}
			else
			{
				setTile(t_x, t_y, FloorTile);
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
				setTile(x, y, Wall);
				tileplaced = true;
			}
			//otherwise there all what ever tile you want
			else
			{
				setTile(x, y, t_tile);
				tileplaced = true;
			}
		}
	}
	return tileplaced;
}
////---------------------------------------------------------------------------
////Place new floor tiles on the ground and store in background tiles vector
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
					if (getTile(x, yStart) == FloorTile || getTile(x, yBottom) == FloorTile)
					{
						setTile(x, yStart, StoneFloorTile);
						setTile(x, yBottom, StoneFloorTile);
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
					if (getTile(xStart, y) == FloorTile || getTile(xEnd, y) == FloorTile)
					{
						setTile(xStart, y, StoneFloorTile);
						setTile(xEnd, y, StoneFloorTile);
					}
				}
			}
		}
	}
}

////---------------------------------------------------------------------------
////Functions to get randon x and a y of the walls top and bottom, also to check all rooms and get a index
////---------------------------------------------------------------------------
int DungeonGen::GetRoom(int t_roomWidth, int t_roomHeight)
{
	//if the room is found the index will be from 0 to room size
	//if its 1000 then no room fount, used 1000 as 0 is a index in the rooms vec
	int roomIndex = 1000;
	if (m_rooms.empty() == false)
	{
		for (int i = 0; i < m_rooms.size(); i++)
		{
			//find a room remaining of a certain size
			if (m_rooms[i].width >= t_roomWidth && m_rooms[i].height >= t_roomHeight)
			{
				roomIndex = i;
			}
		}
	}
	return roomIndex;
}
void DungeonGen::setUnusedTile(int x, int y, char t_tile,char t_secondTile)
{
	//set a max size value to 2
	int maxSize = 2;
	//set the x and the y values and max x value
	int xVal = x;
	int yVal = y;
	int maxX = xVal + maxSize;
	//create a nested for looop to loop through the x ,y 
	for (int i = 0; i < maxSize; i++)
	{
		for (int j = 0; j < maxSize; j++)
		{
			//check the tiles and set unused and replace tiles under large decor tiles to unused
			if (getDecorTile(xVal, yVal) == t_tile 
				|| getDecorTile(xVal, yVal) == t_secondTile)
			{
				setDecorTiles(xVal, yVal, UnusedTile);
			}
			//loop through the x,y same way as the for loop
			xVal++;
			if (xVal == maxX)
			{
				yVal++;
				xVal = x;
			}
		}
	}
}
////---------------------------------------------------------------------------
////Functions that can be used multiple times
////They are to check if there are doors,check the x,y,check if x ,y is at top or bottom walls of room
////set the spawn and Function to create rooms with different tiles from the standard roooms
////takes a tile type to replace all others
////---------------------------------------------------------------------------
bool DungeonGen::CheckXAndYPos(int x, int y)
{
	return true;
}
sf::Vector2i DungeonGen::GenXAndYAtTopWall(int t_i, std::vector<RoomVals>& t_tileVec)
{
	//get the x ,y at the top wall
	int x = randomInt(t_tileVec[t_i].x, t_tileVec[t_i].x + t_tileVec[t_i].width - 1);
	int y = t_tileVec[t_i].y;
	return sf::Vector2i(x, y);
}
sf::Vector2i DungeonGen::GenXAndYAtBottomWall(int t_i, std::vector<RoomVals>& t_tileVec)
{
	//get the x ,y at the bottom wall
	int x = randomInt(t_tileVec[t_i].x, t_tileVec[t_i].x + t_tileVec[t_i].width - 1);
	int y = t_tileVec[t_i].y + t_tileVec[t_i].height - 1;
	return sf::Vector2i(x, y);
}
void DungeonGen::setSpawnToWall(int t_roomIndex)
{
	//get the y at the top wall
	int y = m_rooms[t_roomIndex].y - 1;
	//loop from the starting x of the room to the ending x and set all spawns to walls. this loops through all top walls
	for (int x = m_rooms[t_roomIndex].x; x < m_rooms[t_roomIndex].x + m_rooms[t_roomIndex].width - 1; x++)
	{
		if (getDecorTile(x, y) == SpawnPoint)
		{
			setDecorTiles(x, y, Wall);
		}
	}
}
bool DungeonGen::checkForDoors(int t_x, int t_y)
{
	bool doorDetected = false;
	int offSet = 1;
	//check if the tiles one to the north ,south,east,west directions are doors
	if (getTile(t_x - offSet,t_y) == Door1 || getTile(t_x + offSet, t_y) == Door1
		|| getTile(t_x, t_y - offSet) == Door2 || getTile(t_x, t_y + offSet) == Door2
		|| getDecorTile(t_x ,t_y - offSet) == SpawnPoint)
	{
		//set bool if door was detected and return
		doorDetected = true;
	}
	return doorDetected;
}
void DungeonGen::setUniqueGroundTiles(char t_newTile, int t_maxRoomSizeWidth, int t_maxRoomSizeHeight, int t_index)
{
	//this loops through all ground tiles in nested loop
	for (int x = m_rooms[t_index].x; x < m_rooms[t_index].x
		+ m_rooms[t_index].width; x++)
	{
		for (int y = m_rooms[t_index].y; y < m_rooms[t_index].y
			+ m_rooms[t_index].height; y++)
		{
			//then replace all stone and normal floor tiles with whatever was passed in
			if (getTile(x, y) == StoneFloorTile
				|| getTile(x, y) == FloorTile)
			{
				setTile(x, y, t_newTile);
				setDecorTiles(x, y, t_newTile);
			}
		}
	}
}

////---------------------------------------------------------------------------
////Functions that create the decorations in the rooms
////There are normal rooms with specific decor,
////Completley changed rooms,
////Also Unique rooms
////---------------------------------------------------------------------------


////---------------------------------------------------------------------------
////Function to create a bedroom
////---------------------------------------------------------------------------
void DungeonGen::bedRoom()
{
	if (m_rooms.empty() == false)
	{
		int offSet = 1;
		int randRoom = randomInt(m_rooms.size());
		//get the position of the first bed at the left corner
		int x = m_rooms[randRoom].x;
		int y = m_rooms[randRoom].y - offSet;
		//loop to place the number of beds eg 2
		int bedNum = 2;
		for (int i = 0; i < bedNum; i++)
		{
			//place the bed at xy
			placeBeds(x, y);
			//change the x y to the top right corner of the room on tile between wall and bed
			int x = m_rooms[randRoom].x + m_rooms[randRoom].width - 2;
			int y = m_rooms[randRoom].y - offSet;
			placeBeds(x, y);
		}
		deleteRoom(randRoom);
	}
}
////---------------------------------------------------------------------------
////Function to create a bedroom with beds in it.place the bed and places tiles under the bed as unused tiles
////also place a table next to bed and +1 on the x
////---------------------------------------------------------------------------
void DungeonGen::placeBeds(int t_x, int t_y)
{
	int x = t_x;
	int y = t_y;
	//check the x,y was a wall
	if (getDecorTile(x, y) == Wall)
	{
		//move down y as the current is at a wall and check now on floor
		y++;
		if (getDecorTile(x, y) == FloorTile
			|| getDecorTile(x, y) == StoneFloorTile)
		{
			//check all tiles that the bed will occupy havnt got a door or exit next to them
			if (checkForDoors(x, y) == false)
			{
				//set up a temporay y
				int newY = y;
				newY++;
				if (checkForDoors(x, newY) == false)
				{
					newY++;
					if (checkForDoors(x, newY) == false)
					{
						//place the bed and make all tile under it unused this is cause the bed taks up 3 tiles along y axis
						int tempY = y;
 					    setDecorTiles(x, y, Bed);
						tempY++;
						setDecorTiles(x, tempY, UnusedTile);
						tempY++;
						setDecorTiles(x, tempY, UnusedTile);
					}
				}
				x++;
				//check all tiles around end table havnt got a door or exit next to them
				if (checkForDoors(x, y) == false)
				{
					int newY = y;
					newY++;
					if (checkForDoors(x, newY) == false)
					{
						//check if there are door at the x ,y 
						newY++;
						if (checkForDoors(x, newY) == false)
						{
							//also check the x ,y is a Floor and place a NightStand
							if (getDecorTile(x, y) == StoneFloorTile
								|| getDecorTile(x, y) == FloorTile) {
								setDecorTiles(x, y, NightStand);
							}
						}
					}
				}
			}

		}
	}
}
////---------------------------------------------------------------------------
////Functions to place decorations in each of the rooms
////---------------------------------------------------------------------------
void DungeonGen::placeDecorInRoom()
{
	if (m_rooms.empty() == false)
	{
		//loop through each room
		for (int i = 0; i < m_rooms.size(); i++)
		{
			//bool to state weather decor was placed and values for the amount of decor to be placed
			bool placedDecor = false;
			int minDcor = 2;
			int maxDecor = 4;
			int x = 0;
			int y = 0;
			sf::Vector2i pos = sf::Vector2i(0, 0);
			int maxDecorInRoom = randomInt(minDcor, maxDecor);
			//in each room loop till all deor in that room placed
			for (int decorType = 0; decorType < maxDecorInRoom; )
			{

				switch (decorType)
				{
				case 0:
					//get position along top wall
					pos = GenXAndYAtTopWall(i, m_rooms);
					x = pos.x;
					y = pos.y;
					//check if tiles are flooors
					if (getDecorTile(x, y) == FloorTile || getDecorTile(x, y) == StoneFloorTile)
					{
						//check if theres doors around the tile
						if (checkForDoors(x, y) == false)
						{
							//place decor and change bool to true
							placedDecor = true;
							setDecorTiles(x, y, Chest);
							m_rooms[i].decorInRoom++;
						}
					}
					break;
				case 1:
					//get random x y value in the room
					x = randomInt(m_rooms[i].x, m_rooms[i].x + m_rooms[i].width - 1);
					y = randomInt(m_rooms[i].y, m_rooms[i].y + m_rooms[i].height - 1);
					//check tiles are floors
					if (getDecorTile(x, y) == FloorTile
						|| getDecorTile(x, y) == StoneFloorTile)
					{
						//check doors around tiles
						if (checkForDoors(x, y) == false)
						{
							//place decor and change bool to true and increment decor num in room
							placedDecor = true;
							setDecorTiles(x, y, Skull);
							m_rooms[i].decorInRoom++;
						}
					}
					break;
				case 2:
					//get position along top wall
					pos = GenXAndYAtTopWall(i, m_rooms);
					x = pos.x;
					y = pos.y;
					//check tiles are floors
					if (getDecorTile(x, y) == FloorTile || getDecorTile(x, y) == StoneFloorTile)
					{
						if (checkForDoors(x, y) == false)
						{
							placedDecor = true;
							//place a potion table and move down 1 on the y to then place chair
							setDecorTiles(x, y, Potion);
							y += 1;
							m_rooms[i].decorInRoom++;
							if (getDecorTile(x, y) == FloorTile || getDecorTile(x, y) == StoneFloorTile)
							{
								//check there are no doors or exits around the x,y and then place chair
								if (checkForDoors(x, y) == false)
								{
									setDecorTiles(x, y, ChairF);
									m_rooms[i].decorInRoom++;
								}
							}
						}
					}
					break;
				case 3:
					//get position along bottom wall
					pos = GenXAndYAtBottomWall(i, m_rooms);
					x = pos.x;
					y = pos.y;
					//check the x , y are floor tiles
					if (getDecorTile(x, y) == FloorTile
						|| getDecorTile(x, y) == StoneFloorTile)

					{
						//check doors around tiles
						if (checkForDoors(x, y) == false)
						{
							placedDecor = true;
							setDecorTiles(x, y, Plant);
							m_rooms[i].decorInRoom++;
						}
					}
					break;
				case 4:
					pos = GenXAndYAtBottomWall(i, m_rooms);
					x = pos.x;
					y = pos.y;

					if (getDecorTile(x, y) == FloorTile || getDecorTile(x, y) == StoneFloorTile)
					{
						if (checkForDoors(x, y) == false)
						{
							//place the money at bottom wall
							placedDecor = true;
							setDecorTiles(x, y, Money);
							y += 1;
							m_rooms[i].decorInRoom++;
							if (getDecorTile(x, y) == FloorTile
								|| getDecorTile(x, y) == StoneFloorTile)
							{
								setDecorTiles(x, y, ChairF);
							}
						}
					}
					break;
				default:
					break;
				}
				if (placedDecor)
				{
					decorType++;
				}
			//delete the room if decorated
			}
			if (m_rooms[i].decorInRoom > 0)
			{
				deleteRoom(i);
			}
		}

	}
	DEBUG_MSG("Issue placing tiles tile");
}
////---------------------------------------------------------------------------
////Function to create a room of jail cells
////---------------------------------------------------------------------------
void DungeonGen::createJailRoom()
{
	//values needed for the jail room
	int offSet = 1;
	int noIndex = 1000;
	int roomIndex = 0;
	int maxRoomSizeWidth = 5;
	int maxRoomSizeHeight = 4;
	bool roomFound = false;
	bool roomCompleted = false;
	//check if there are rooms to decorate
	if (m_rooms.empty() == false)
	{
		//get a room and store its index
		roomIndex = GetRoom(maxRoomSizeWidth, maxRoomSizeHeight);
		if (roomIndex != noIndex)
		{
			roomFound = true;
		}
	}
	//if a room was found
	if(roomFound)
	{
		//set the spawns in the rooms to be walls
		setSpawnToWall(roomIndex);
		//call jail cell func
		roomCompleted = createJailCells(roomIndex);
	
		//loop to place a number of skulls
	    int maxSkullNum = 3;
		for(int skullNum = 0; skullNum < maxSkullNum; skullNum++)
		{
			// get a random position in the room
			int x = randomInt(m_rooms[roomIndex].x + offSet, m_rooms[roomIndex].x + m_rooms[roomIndex].width - 2);
			int y = randomInt(m_rooms[roomIndex].y + offSet, m_rooms[roomIndex].y + m_rooms[roomIndex].height - 2);
			//check tiles are floor and place skull decor
			if (getDecorTile(x, y) == StoneFloorTile || getDecorTile(x, y) == FloorTile)
			{
				setDecorTiles(x, y, Skull);
			}
		}
		//delete the room and increment the number of jail rooms/delete that room
		if (roomCompleted)
		{
			m_numJailRoomsPlaced++;
			deleteRoom(roomIndex);
		}
	}
}
////---------------------------------------------------------------------------
////Function to place jail cells in the Jail room and place them in a order from one side of room to next
////---------------------------------------------------------------------------
bool DungeonGen::createJailCells(int t_roomIndex)
{
	//values needed to decorate room
	bool roomCompleted = false;
	int offSet = 1;
	int x = m_rooms[t_roomIndex].x +1;
	int y = m_rooms[t_roomIndex].y - offSet;
	int maxWidth = m_rooms[t_roomIndex].x + m_rooms[t_roomIndex].width - offSet *2;
	//loop through all top walls
	for (int index = x; index <= maxWidth; index++)
	{
		//check the tiles are Walls
		if (getDecorTile(index, y) == Wall)
		{
			//replace them starting at index till you get to the end of the room
			setDecorTiles(index, y, PrisonTile);
			m_rooms[t_roomIndex].decorInRoom++;
			roomCompleted = true;
		}
	}
	//get the new x , y to place tile in center
	x = m_rooms[t_roomIndex].x + offSet;
	int endX = m_rooms[t_roomIndex].x + m_rooms[t_roomIndex].width - 2;
	y = m_rooms[t_roomIndex].y + m_rooms[t_roomIndex].height / 2;
	//loop through room from index to two tiles from end;
	for (int index = x; index <= endX; index++)
	{
		//set the background tile to a wall and decor tile to prison
		//setTile(index, y, Wall);
		setDecorTiles(index, y, PrisonTile);
		m_rooms[t_roomIndex].decorInRoom++;
		//set the room to be completed and return
		roomCompleted = true;
	}
	return roomCompleted;
}
////---------------------------------------------------------------------------
////Function to create a room of coffins
////---------------------------------------------------------------------------
void DungeonGen::createCoffinRoom()
{
	//values needed for placing coffin tiles
	int roomIndex = 0;
	int offSet = 1;
	//width and height of room wanted
	int maxRoomSizeWidth = 5;
	int maxRoomSizeHeight = 4;
	bool roomFound = false;
	int noIndexFoundVal = 1000;
	//try get a room and call get room and store its index
	roomIndex = GetRoom(maxRoomSizeWidth, maxRoomSizeHeight);
	//check index is not 1000/invalid and set room found to true
	if (roomIndex != noIndexFoundVal)
	{
		roomFound = true;
	}
	//check room found
	if (roomFound)
	{
		//set the ampunt of decor to be in room
		int maxDecorInRoom = 5;
		//ge the x,y and with of room
		int x = m_rooms[roomIndex].x + offSet;
		int y = m_rooms[roomIndex].y + offSet;
		int maxWidth = m_rooms[roomIndex].x + m_rooms[roomIndex].width - 2;
		//loop startijng at index till the end of the room increment by 2
		for (int index = x; index <= maxWidth; index +=2)
		{
			//check for floors
			if (getDecorTile(index, y) == FloorTile 
				|| getDecorTile(index, y) == StoneFloorTile)
			{
				//set the tile to coffins and tiles under coffins to unused
				setDecorTiles(index, y, CoffinTile);
				setDecorTiles(index, y + offSet, UnusedTile);
				//place a flame caouldron at bottom of coffins
				setDecorTiles(index, y + offSet *2, FlameCauldron);
				m_rooms[roomIndex].decorInRoom++;
			}
		}
		//delete the room when completed
		deleteRoom(roomIndex);
		
	}

}

////---------------------------------------------------------------------------
////Function to create a boss room thats different From the others
////---------------------------------------------------------------------------
void DungeonGen::bossRoomGroundTiles()
{
	//from these width and height get a room index for boss room and change all the floor tiles to dirt
	int maxRoomSizeWidth = 6;
	int maxRoomSizeHeight = 6;
	m_bossRoomIndex = GetRoom(maxRoomSizeWidth, maxRoomSizeHeight);
	setUniqueGroundTiles(DirtTile, maxRoomSizeWidth, maxRoomSizeHeight, m_bossRoomIndex);
}
void DungeonGen::worsipRoomGroundTiles()
{
	//from these width and height get a room index for worship room and change all the floor tiles to dark
	int maxRoomSizeWidth = 5;
	int maxRoomSizeHeight = 5;
	m_worshipRoomIndex = GetRoom(maxRoomSizeWidth, maxRoomSizeHeight);
	setUniqueGroundTiles(DarkTiles, maxRoomSizeWidth, maxRoomSizeHeight, m_worshipRoomIndex);
}
void DungeonGen::statueRoomTiles()
{
	//from these width and height get a room index for statue room and change all the floor tiles to tiles
	int maxRoomSizeWidth = 6;
	int maxRoomSizeHeight = 6;
	m_statueRoomIndex = GetRoom(maxRoomSizeWidth, maxRoomSizeHeight);
	setUniqueGroundTiles(TilePattern, maxRoomSizeWidth, maxRoomSizeHeight, m_statueRoomIndex);
}
////---------------------------------------------------------------------------
////Creates a room with a pit that tthe boss will appear from
////littered with fallen heroes that failed to defeat the boss
////---------------------------------------------------------------------------
void DungeonGen::bossRoom()
{
	//get x,y near room centre
	int offSetFromCentre = 1;
	int x = m_rooms[m_bossRoomIndex].x + m_rooms[m_bossRoomIndex].width / 2 - offSetFromCentre;
	int y = m_rooms[m_bossRoomIndex].y + m_rooms[m_bossRoomIndex].height / 2 - offSetFromCentre;
	if (getDecorTile(x, y) == DirtTile)
	{
		//place big hole of death
		setDecorTiles(x, y, HoleTile);
		//create a new x ,y to and size
		int maxSize = 3;
		int xVal = x;
		int yVal = y;
		int maxX = xVal + maxSize;
		//nested loop to replace all tiles under hole with unuseable tiles
		for (int i = 0; i < maxSize; i++)
		{
			for (int j = 0; j < maxSize; j++)
			{
				//make sure tile is dirt and make unusable
				if (getDecorTile(xVal, yVal) == DirtTile)
				{
					setDecorTiles(xVal, yVal, UnusedTile);
				}
				//increment x
				xVal++;
				//when at max x reset x and increment y to next col
				if (xVal == maxX)
				{
					yVal++;
					xVal = x;
				}
			}
		}
	}
	//calls to place skull health and decorate the walls
	bossRoomSkull();
	bossRoomHealth();
	bossRoomWalls();
	//delete this room
	deleteRoom(m_bossRoomIndex);
	
}
////---------------------------------------------------------------------------
////Adds skull decor toi the boss room
////---------------------------------------------------------------------------
void DungeonGen::bossRoomSkull()
{
	//values needed to place skull tile
	int offSet = 1;
	int moveBy = 3;
	int x = m_rooms[m_bossRoomIndex].x + offSet;
	int y = m_rooms[m_bossRoomIndex].y;
	if (getDecorTile(x, y - 1) != Door2)
	{
		if (getDecorTile(x, y) == DirtTile)
		{
			//place the skeleton if the tiles dirt
			setDecorTiles(x, y, Skull);
			//move the x and y by a certain amount and place another
			x = m_rooms[m_bossRoomIndex].x + moveBy;
			y++;
			setDecorTiles(x, y, Skull);
		}
	}
	//get a new x,y position of right corner of room and place flame cauldron decor 
	x = m_rooms[m_bossRoomIndex].x + m_rooms[m_bossRoomIndex].width - offSet;
	y = m_rooms[m_bossRoomIndex].y;
	if (checkForDoors(x, y) == false)
	{
		setDecorTiles(x, y, FlameCauldron);
	}
	//get a new x,y position of bottom right corner of room and place flame cauldron decor 
	x = m_rooms[m_bossRoomIndex].x + m_rooms[m_bossRoomIndex].width - offSet;
	y = m_rooms[m_bossRoomIndex].y + m_rooms[m_bossRoomIndex].height - offSet;
	if (checkForDoors(x, y) == false)
	{
		setDecorTiles(x, y, FlameCauldron);
	}
}
////---------------------------------------------------------------------------
////Adds health to the boss room
////---------------------------------------------------------------------------
void DungeonGen::bossRoomHealth()
{
	//set offset and get x,y not against wall somewhere in room
	int offSet = 1;
	int x = randomInt(m_rooms[m_bossRoomIndex].x + offSet, m_rooms[m_bossRoomIndex].x + m_rooms[m_bossRoomIndex].width - (offSet * 2));
	int y = m_rooms[m_bossRoomIndex].y + m_rooms[m_bossRoomIndex].height - offSet *2;
	//check if tile is nor dirt and if so move to the right and place health
	if (getDecorTile(x, y) != DirtTile)
	{
		x++;
		setDecorTiles(x, y, HealthPos);
	}
	else
	{
		//place health
		setDecorTiles(x, y, HealthPos);
	}
}
////---------------------------------------------------------------------------
////Adds decor to the walls of the boss room
////---------------------------------------------------------------------------
void DungeonGen::bossRoomWalls()
{
	//get x ,y starting on the first wall tille in the room
	int offSet = 1;
	int x = m_rooms[m_bossRoomIndex].x;
	int y = m_rooms[m_bossRoomIndex].y - offSet;
	//chack they are walls
	if (getDecorTile(x, y) == Wall)
	{
		//chack x is not near the right side of the room
		if (x < m_rooms[m_bossRoomIndex].x + m_rooms[m_bossRoomIndex].width - offSet)
		{
			//place torch,chains and move by 2 along x each time
			setDecorTiles(x, y, Torch);
			x += 2;
			setDecorTiles(x, y, Chains);
			x += 2;
			setDecorTiles(x, y, Torch);
		}
	}
	
}
////---------------------------------------------------------------------------
////creates the worship room
////---------------------------------------------------------------------------
void DungeonGen::worshipRoom()
{
	//get x ,y in middle of room
	int x = m_rooms[m_worshipRoomIndex].x + m_rooms[m_worshipRoomIndex].width / 2;
	int y = m_rooms[m_worshipRoomIndex].y + m_rooms[m_worshipRoomIndex].height / 2;
	//check tiles and place worship tile
	if (getDecorTile(x, y) == DarkTiles)
	{
		setDecorTiles(x, y, Worship);
		setUnusedTile(x, y, DarkTiles,FloorTile);
	}
	worshipRoomDecor();
		
	deleteRoom(m_worshipRoomIndex);
	
}
////---------------------------------------------------------------------------
////creates the decorations for the worship room
////---------------------------------------------------------------------------
void DungeonGen::worshipRoomDecor()
{
	int offSet = 1;
	//get x ,y at top right of room
	int x = m_rooms[m_worshipRoomIndex].x + m_rooms[m_worshipRoomIndex].width - offSet;
	int y = m_rooms[m_worshipRoomIndex].y;
	//place chairs down from the right corner by 3
	int	maxChairs = 3;
	for (int i = 0; i < maxChairs; i++)
	{
		if (checkForDoors(x, y) == false)
		{
			setDecorTiles(x, y, ChairL);
			y++;
		}
	}
	//get x ,y at top left corner
	x = m_rooms[m_worshipRoomIndex].x;
	y = m_rooms[m_worshipRoomIndex].y;
	//place plant at top left and bottom left corners
	if (checkForDoors(x, y) == false)
	{
		setDecorTiles(x, y, Plant);
		y = m_rooms[m_worshipRoomIndex].y + m_rooms[m_worshipRoomIndex].height - offSet;
		setDecorTiles(x, y, Plant);
	}
}
////---------------------------------------------------------------------------
////Function to create a room with a statue of some famous person.
////In this case can be of the boss as a warning to players
////---------------------------------------------------------------------------
void DungeonGen::statueRoom()
{
	//get x,y at centre of room
	int offSetFromCentre = 1;
	int x = m_rooms[m_statueRoomIndex].x + m_rooms[m_statueRoomIndex].width / 2 - offSetFromCentre;
	int y = m_rooms[m_statueRoomIndex].y + m_rooms[m_statueRoomIndex].height / 2 - offSetFromCentre;
	//set the statue at that x,y
	if (getDecorTile(x, y) == TilePattern)
	{
		setDecorTiles(x, y, Statue);
		setUnusedTile(x, y, TilePattern, FloorTile);
	}
	//delete room and call decorate room func
	statueRoomDecor();
	deleteRoom(m_statueRoomIndex);
}
////---------------------------------------------------------------------------
////Function to add some decor to the statue room
////---------------------------------------------------------------------------
void DungeonGen::statueRoomDecor()
{
	//set x,y to top left corner
	int offSet = 1;
	int x = m_rooms[m_statueRoomIndex].x;
	int y = m_rooms[m_statueRoomIndex].y;
	//place a flaming cauldron in all 4 corners of the room
	if (checkForDoors(x, y) == false)
	{
		setDecorTiles(x, y, FlameCauldron);
		x = m_rooms[m_statueRoomIndex].x + m_rooms[m_statueRoomIndex].width - offSet;
		y = m_rooms[m_statueRoomIndex].y + m_rooms[m_statueRoomIndex].height - offSet;
	}
	if (checkForDoors(x, y) == false)
	{
		setDecorTiles(x, y, FlameCauldron);
		x = m_rooms[m_statueRoomIndex].x;
		y = m_rooms[m_statueRoomIndex].y + m_rooms[m_statueRoomIndex].height - offSet;
	}
	if (checkForDoors(x, y) == false)
	{
		setDecorTiles(x, y, FlameCauldron);
		x = m_rooms[m_statueRoomIndex].x + m_rooms[m_statueRoomIndex].width - offSet;
		y = m_rooms[m_statueRoomIndex].y;
	}
	if (checkForDoors(x, y) == false){
		setDecorTiles(x, y, FlameCauldron);
	}
	
}
////---------------------------------------------------------------------------
////Function to create a feast room with a table and chairs
////---------------------------------------------------------------------------
void DungeonGen::createFeastRoom()
{
	//set values needed
	int roomIndex = 0;
	int invalidIndex = 1000;
	int offsetFromCentre = 1;
	int maxRoomSizeRequired = 7;
	bool roomFound = false;
	if (m_rooms.empty() == false)
	{
		//get room thats size 7 and store its index and set room was found
		roomIndex = GetRoom(maxRoomSizeRequired, maxRoomSizeRequired);
		if (roomIndex != invalidIndex)
		{
			roomFound = true;
		}
		
		if (roomFound)
		{
			//get the middle of the room if one found 
			int x = m_rooms[roomIndex].x + m_rooms[roomIndex].width / 2;
			int y = m_rooms[roomIndex].y + m_rooms[roomIndex].height / 2;
			x -= offsetFromCentre;
			y -= offsetFromCentre;
			//check the tile at the x and y val and set if its a floor 
			if (getDecorTile(x, y) == StoneFloorTile
				|| getDecorTile(x, y) == FloorTile)
			{
				m_rooms[roomIndex].decorInRoom++;
				setDecorTiles(x, y, Table);
				setUnusedTile(x, y, FloorTile,StoneFloorTile);
			}
			int newY = 4;
			//set the x y to be half way in the tile
			x += 1;
			y -= 1;
			// add another decoration
			if (getDecorTile(x, y) == StoneFloorTile
				|| getDecorTile(x, y) == FloorTile)
			{
				m_rooms[roomIndex].decorInRoom++;
				setDecorTiles(x, y, Plant);
				y += newY;
				setDecorTiles(x, y, Plant);
			}
			deleteRoom(roomIndex);
		}
	}
	DEBUG_MSG("Issue Feast tiles");
}
////---------------------------------------------------------------------------
////Function to create a library room for the dungeon
////---------------------------------------------------------------------------
void DungeonGen::createLibraryRoom()
{
	//set values need eg size of room wanteed,index,roomFound etc
	int heightOffset = 3;
	int roomIndex = 0;
	int invalidIndex = 1000;
	int offSet = 1;
	int maxRoomWidth = 7;
	int maxRoomHeight = 4;
	bool roomFound = false;
	if (m_rooms.empty() == false)
	{
		//get room and store index and set room was found
		roomIndex = GetRoom(maxRoomWidth, maxRoomHeight);
		if (roomIndex != invalidIndex)
		{
			roomFound = true;
		}
	}
	if (roomFound)
	{
		//set all spawns in the room to walls
		setSpawnToWall(roomIndex);
		//func to place bookshelf
		placeBookShelfDecor(roomIndex);
		addChairsDecor(roomIndex);
		int xOffSet = 2;
		//get the bottom right of room and add a plant decor
		int x = m_rooms[roomIndex].x + m_rooms[roomIndex].width - xOffSet;
		int y = m_rooms[roomIndex].y + m_rooms[roomIndex].height - offSet;
		//place plant tiles at top right and bottom
		if (getDecorTile(x, y) == StoneFloorTile || getDecorTile(x, y) == FloorTile)
		{
			setDecorTiles(x, y, Plant);
			y = m_rooms[roomIndex].y;
			setDecorTiles(x, y, Plant);
		}
		//delete this room
		deleteRoom(roomIndex);
	}
}
////---------------------------------------------------------------------------
////Function to place the bookshelf decorations
////---------------------------------------------------------------------------
void DungeonGen::placeBookShelfDecor(int t_roomIndex)
{
	int offSet = 1;
	int doorNum = 0;
	//get the middle of the room if one found 
	int x = m_rooms[t_roomIndex].x;
	int y = m_rooms[t_roomIndex].y;
	int maxXVal = 3;
	for (int i = 0; i < maxXVal; i++)
	{
		//check for doors at x y and 3 tiles to right on x 
		if (checkForDoors(x, y) == false)
		{
			//starting at x and move up 1 on y 
			int x2 = x;
			int y2 = y - 1;
			//allows us to check for doors along wall tiles
			if (getDecorTile(x2 + i, y2) == Door2)
			{
				doorNum = x2 + i;
			}
		}
	}
	//if there were doors found then place bookshelf after door
	if (doorNum > 0)
	{
		x = doorNum + offSet;
		setDecorTiles(x, y, BookShelve);
		setDecorTiles(x + 2, y, UnusedTile);
	}
	//if not place at room corner
	else if (doorNum == 0)
	{
		setDecorTiles(x, y, BookShelve);
		setDecorTiles(x + 2, y, UnusedTile);
	}
}
////---------------------------------------------------------------------------
////Function to place the chairs in the library and other decor
////---------------------------------------------------------------------------
void DungeonGen::addChairsDecor(int t_roomIndex)
{
	int offSet = 1;
	//get the bottom right of room
	int x = m_rooms[t_roomIndex].x;
	int y = m_rooms[t_roomIndex].y + m_rooms[t_roomIndex].height - offSet;
	int maxWidth = 2;
	//loop along the x till you get to a certain width 
	for (int i = 0; i <= maxWidth; i++)
	{
		//check for any doors and place a chair if no doors around x, y
		if (checkForDoors(x, y) == false)
		{
			setDecorTiles(x, y, ChairF);
			x++;
		}
	}
}
////---------------------------------------------------------------------------
////Function to add traps to the halls 
////---------------------------------------------------------------------------
void DungeonGen::placeTrapsInHalls()
{
	//place some traps in random halls and up 8 halls
	int offSet = 1;
	int maxHallsWithTraps = 8;
	if (m_halls.empty() == false)
	{
		for (int i = 0; i < maxHallsWithTraps; i++)
		{
			//find a random room and a random x y in that room
			int r = randomInt(0, m_halls.size() - offSet);
			int x = randomInt(m_halls[r].x, m_halls[r].x + m_halls[r].width - offSet);
			int y = randomInt(m_halls[r].y, m_halls[r].y + m_halls[r].height - offSet);
			//check the floor tile and place spike if its a floor tile
			if (getDecorTile(x, y) == FloorTile)
			{
				if (checkForDoors(x, y) == false) {
					setDecorTiles(x, y, SpikeTrap);
				}
			}
		}
	}
}

////---------------------------------------------------------------------------
////Function to create cave doors that the player can exit and enter
////---------------------------------------------------------------------------
void DungeonGen::playerStartPos()
{

	if (m_rooms.empty() == false)
	{
		//loop through all rooms
		for (int i = 0; i < m_rooms.size(); i++)
		{
			//get a random x along the top walls and y at the top walls
			int x = randomInt(m_rooms[i].x, m_rooms[i].x + m_rooms[i].width -1);
			int y = m_rooms[i].y - 1;
			//check if its a wall and repalce with exit/spawn
			if (getDecorTile(x , y) == Wall)
			{
				setDecorTiles(x, y, SpawnPoint);
			}
		}
	}
	DEBUG_MSG("Issue placing starting position tile");
}
////---------------------------------------------------------------------------
////Function to place decorations on the walls(eg torch,chains, pictures)
////---------------------------------------------------------------------------
void DungeonGen::placeDecorOnWalls()
{
	if (m_rooms.empty() == false)
	{
		//loop through rooms
		for (int i = 0; i < m_rooms.size(); i++)
		{
			//set up the values for number of pictures in a room and the decor amount
			int picNum = 0;
			int maxDecorInRoom = randomInt(3, 5);
			//loop till max decor was placed
			for (int decorType = 0; decorType < maxDecorInRoom;)
			{
				//set the x ,y  at the top wall
				int offSet = 1;
				int x = randomInt(m_rooms[i].x + offSet, m_rooms[i].x + m_rooms[i].width - offSet);
				int y = m_rooms[i].y - offSet;
				//check for wall
				if (getDecorTile(x, y) == Wall)
				{
					//if so place chains for decor type 0
					if (decorType == 0) {
						setDecorTiles(x, y, Chains);
					}
					//if so place torch for decor type 1
					else if (decorType == 1) {
						setDecorTiles(x, y, Torch);
					}
					//if so place one  picture for decor type 2
					else if (decorType == 2) {
						if (picNum == 0)
						{
							picNum++;
							setDecorTiles(x, y, Picture);
						}
					}
					
				}
				decorType++;
			}
		}
	}
	DEBUG_MSG("Issue placing chain Decor tile");
}
////---------------------------------------------------------------------------
////Function to create traps in the rooms--------------------------------------
////---------------------------------------------------------------------------
void DungeonGen::createTrapsInRooms()
{
	int offSet = 1;
	if (m_rooms.empty() == false)
	{
		//loop through all rooms
		for (int i = 0; i < m_rooms.size(); i++)
		{
			//set teh max amout of traps allowed in a room and create loop till there set
			int maxTraps = 2;
			for (int trap = 0; trap < maxTraps; trap++)
			{
				//get a random x y in each room
				int x = randomInt(m_rooms[i].x, m_rooms[i].x + m_rooms[i].width - offSet);
				int y = randomInt(m_rooms[i].y, m_rooms[i].y + m_rooms[i].height - offSet);
				//check if the tile at x,y is a floor tile and set a spike if so
				if (getDecorTile(x, y) == FloorTile 
					|| getDecorTile(x, y) == StoneFloorTile)
				{
					if (checkForDoors(x, y) == false) {
						setDecorTiles(x, y, SpikeTrap);
					}
				}
			}
		}
	}
	//call funtion to place traps in halls
	placeTrapsInHalls();
}

////---------------------------------------------------------------------------
////Function to create decorations in the hall ways
////---------------------------------------------------------------------------
void DungeonGen::placeDecorInHalls()
{
	if (m_halls.empty() == false)
	{
		//loop through halls
		for (int i = 0; i < m_halls.size(); i++)
		{
			//set the amount of decor in halls
			int mixDcor = 1;
			int maxDecor = 4;
			int maxDecorInRoom = randomInt(mixDcor, maxDecor);
			//loop till max decor reached
			for (int decorType = 0; decorType < maxDecorInRoom; decorType++)
			{
				//each iteration of loop there is new decor
				if (decorType == 0)
				{
					//Create x ,y pos for new tile at left side of room random along y 
					int x = m_halls[i].x + 1;
					int y = randomInt(m_halls[i].y +1, m_halls[i].y + m_halls[i].width - 1);
					//this is so place plant if it can
					if (getDecorTile(x, y) == FloorTile
						&& checkForDoors(x,y)==false)
					{
						setDecorTiles(x, y, Plant);
					}
				}
				else if (decorType == 1)
				{
					//Create x ,y pos for new tile at left side of room random along y 
					int offSets = 1;
					int x = m_halls[i].x + offSets;
					int y = randomInt(m_halls[i].y + offSets, m_halls[i].y + m_halls[i].width - offSets);
					//this is so place potion if it can
					if (getDecorTile(x, y) == FloorTile
						&& checkForDoors(x, y) == false)
					{
						setDecorTiles(x, y, Potion);
					}

				}
				else if (decorType == 2)
				{
					//Create x ,y pos for new tile at right side of room random along y 
					int x = m_halls[i].x + m_halls[i].width - 1;
					int y = randomInt(m_halls[i].y, m_halls[i].y + m_halls[i].width - 1);
					//this is so place Money if it can
					if (getDecorTile(x, y) == FloorTile
						&& checkForDoors(x, y) == false)
					{
						setDecorTiles(x, y, Money);
					}
				}
			}
			//delete the hall after its decorated
			m_halls.erase(m_halls.begin() + i);
		}

	}
	DEBUG_MSG("Issue placing tiles tile");
}