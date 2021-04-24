#include "DungeonDecor.h"

DunDecor::~DunDecor()
{
}
////---------------------------------------------------------------------------
////Function to decorate and change floor tiles of each room
////---------------------------------------------------------------------------
void DunDecor::createDunExtras()
{
	int numberOfRooms = 2;
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
	if (m_rooms.empty() == false) {
		placeDecorInRoom();
	}
	//create a bedroom
	bedRoom();
}
////---------------------------------------------------------------------------
////Function to add stone to the floor tiles
////---------------------------------------------------------------------------
void DunDecor::FloorDecorTiles()
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

////---------------------------------------------------------------------------
////Functions to get randon x and a y of the walls top and bottom, also to check all rooms and get a index
////---------------------------------------------------------------------------
int DunDecor::GetRoom(int t_roomWidth, int t_roomHeight)
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
////---------------------------------------------------------------------------
////Functionto set unused tiles under larger tiles
////---------------------------------------------------------------------------
void DunDecor::setUnusedTile(int x, int y, char t_tile, char t_secondTile)
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
			if (getTile(xVal, yVal, m_decorTiles, m_width, m_height) == t_tile
				|| getTile(xVal, yVal, m_decorTiles, m_width, m_height) == t_secondTile)
			{
				setTile(xVal, yVal, UnusedTile, m_decorTiles, m_width);
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
bool DunDecor::CheckXAndYPos(int x, int y)
{
	return true;
}
sf::Vector2i DunDecor::GenXAndYAtTopWall(int t_i, std::vector<RoomVals>& t_tileVec)
{
	//get the x ,y at the top wall
	int x = randomInt(t_tileVec[t_i].x, t_tileVec[t_i].x + t_tileVec[t_i].width - 1);
	int y = t_tileVec[t_i].y;
	return sf::Vector2i(x, y);
}
sf::Vector2i DunDecor::GenXAndYAtBottomWall(int t_i, std::vector<RoomVals>& t_tileVec)
{
	//get the x ,y at the bottom wall
	int x = randomInt(t_tileVec[t_i].x, t_tileVec[t_i].x + t_tileVec[t_i].width - 1);
	int y = t_tileVec[t_i].y + t_tileVec[t_i].height - 1;
	return sf::Vector2i(x, y);
}
void DunDecor::setSpawnToWall(int t_roomIndex)
{
	//get the y at the top wall
	int y = m_rooms[t_roomIndex].y - 1;
	//loop from the starting x of the room to the ending x and set all spawns to walls. this loops through all top walls
	for (int x = m_rooms[t_roomIndex].x; x < m_rooms[t_roomIndex].x + m_rooms[t_roomIndex].width - 1; x++)
	{
		if (getTile(x, y,m_decorTiles, m_width,m_height) == SpawnPoint)
		{
			setTile(x, y, Wall, m_decorTiles, m_width);
		}
	}
}
bool DunDecor::checkForDoors(int t_x, int t_y)
{
	bool doorDetected = false;
	int offSet = 1;
	//check if the tiles one to the north ,south,east,west directions are doors
	if (getTile(t_x - offSet, t_y, m_tiles, m_width, m_height) == Door1 || getTile(t_x + offSet, t_y, m_tiles, m_width, m_height) == Door1
		|| getTile(t_x, t_y - offSet, m_tiles, m_width, m_height) == Door2 || getTile(t_x, t_y + offSet, m_tiles, m_width, m_height) == Door2
		|| getTile(t_x, t_y - offSet, m_decorTiles, m_width, m_height) == SpawnPoint)
	{
		//set bool if door was detected and return
		doorDetected = true;
	}
	return doorDetected;
}
void DunDecor::setUniqueGroundTiles(char t_newTile, int t_maxRoomSizeWidth, int t_maxRoomSizeHeight, int t_index)
{
	//this loops through all ground tiles in nested loop
	for (int x = m_rooms[t_index].x; x < m_rooms[t_index].x
		+ m_rooms[t_index].width; x++)
	{
		for (int y = m_rooms[t_index].y; y < m_rooms[t_index].y
			+ m_rooms[t_index].height; y++)
		{
			//then replace all stone and normal floor tiles with whatever was passed in
			if (getTile(x, y, m_tiles, m_width,m_height) == StoneFloorTile
				|| getTile(x, y, m_tiles, m_width, m_height) == FloorTile)
			{
				setTile(x, y, t_newTile, m_tiles,m_width);
				setTile(x, y, t_newTile,m_decorTiles,m_width);
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
void DunDecor::bedRoom()
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
void DunDecor::placeBeds(int t_x, int t_y)
{
	int x = t_x;
	int y = t_y;
	//check the x,y was a wall
	if (getTile(x, y,m_decorTiles,m_width,m_height) == Wall)
	{
		//move down y as the current is at a wall and check now on floor
		y++;
		if (getTile(x, y,m_decorTiles,m_width,m_height) == FloorTile
			|| getTile(x, y, m_decorTiles, m_width, m_height) == StoneFloorTile)
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
						setTile(x, y, Bed, m_decorTiles, m_width);
						tempY++;
						setTile(x, tempY, UnusedTile, m_decorTiles, m_width);
						tempY++;
						setTile(x, tempY, UnusedTile, m_decorTiles, m_width);
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
							if (getTile(x, y, m_decorTiles, m_width, m_height) == StoneFloorTile
								|| getTile(x, y, m_decorTiles, m_width, m_height) == FloorTile) {
								setTile(x, y, NightStand, m_decorTiles, m_width);
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
void DunDecor::placeDecorInRoom()
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
					if (getTile(x, y, m_decorTiles, m_width, m_height) == FloorTile 
						|| getTile(x, y, m_decorTiles, m_width, m_height) == StoneFloorTile)
					{
						//check if theres doors around the tile
						if (checkForDoors(x, y) == false)
						{
							//place decor and change bool to true
							placedDecor = true;
							setTile(x, y, Chest, m_decorTiles, m_width);
							m_rooms[i].decorInRoom++;
						}
					}
					break;
				case 1:
					//get random x y value in the room
					x = randomInt(m_rooms[i].x, m_rooms[i].x + m_rooms[i].width - 1);
					y = randomInt(m_rooms[i].y, m_rooms[i].y + m_rooms[i].height - 1);
					//check tiles are floors
					if (getTile(x, y, m_decorTiles, m_width, m_height) == FloorTile
						|| getTile(x, y, m_decorTiles, m_width, m_height) == StoneFloorTile)
					{
						//check doors around tiles
						if (checkForDoors(x, y) == false)
						{
							//place decor and change bool to true and increment decor num in room
							placedDecor = true;
							setTile(x, y, Skull, m_decorTiles, m_width);
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
					if (getTile(x, y, m_decorTiles, m_width, m_height) == FloorTile 
						|| getTile(x, y, m_decorTiles, m_width, m_height) == StoneFloorTile)
					{
						if (checkForDoors(x, y) == false)
						{
							placedDecor = true;
							//place a potion table and move down 1 on the y to then place chair
							setTile(x, y, Potion, m_decorTiles, m_width);
							y += 1;
							m_rooms[i].decorInRoom++;
							if (getTile(x, y, m_decorTiles, m_width, m_height) == FloorTile 
								|| getTile(x, y, m_decorTiles, m_width, m_height) == StoneFloorTile)
							{
								//check there are no doors or exits around the x,y and then place chair
								if (checkForDoors(x, y) == false)
								{
									setTile(x, y, ChairF, m_decorTiles, m_width);
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
					if (getTile(x, y, m_decorTiles, m_width, m_height) == FloorTile
						|| getTile(x, y, m_decorTiles, m_width, m_height) == StoneFloorTile)

					{
						//check doors around tiles
						if (checkForDoors(x, y) == false)
						{
							placedDecor = true;
							setTile(x, y, Plant, m_decorTiles, m_width);
							m_rooms[i].decorInRoom++;
						}
					}
					break;
				case 4:
					pos = GenXAndYAtBottomWall(i, m_rooms);
					x = pos.x;
					y = pos.y;

					if (getTile(x, y, m_decorTiles, m_width, m_height) == FloorTile 
						|| getTile(x, y, m_decorTiles, m_width, m_height) == StoneFloorTile)
					{
						if (checkForDoors(x, y) == false)
						{
							//place the money at bottom wall
							placedDecor = true;
							setTile(x, y, Money, m_decorTiles, m_width);
							y += 1;
							m_rooms[i].decorInRoom++;
							if (getTile(x, y, m_decorTiles, m_width, m_height) == FloorTile
								|| getTile(x, y, m_decorTiles, m_width, m_height) == StoneFloorTile)
							{
								setTile(x, y, ChairF, m_decorTiles, m_width);
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
void DunDecor::createJailRoom()
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
	if (roomFound)
	{
		//set the spawns in the rooms to be walls
		setSpawnToWall(roomIndex);
		//call jail cell func
		roomCompleted = createJailCells(roomIndex);

		//loop to place a number of skulls
		int maxSkullNum = 3;
		for (int skullNum = 0; skullNum < maxSkullNum; skullNum++)
		{
			// get a random position in the room
			int x = randomInt(m_rooms[roomIndex].x + offSet, m_rooms[roomIndex].x + m_rooms[roomIndex].width - 2);
			int y = randomInt(m_rooms[roomIndex].y + offSet, m_rooms[roomIndex].y + m_rooms[roomIndex].height - 2);
			//check tiles are floor and place skull decor
			if (getTile(x, y, m_decorTiles, m_width, m_height) == StoneFloorTile
				|| getTile(x, y, m_decorTiles, m_width, m_height) == FloorTile)
			{
				setTile(x, y, Skull, m_decorTiles, m_width);
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
bool DunDecor::createJailCells(int t_roomIndex)
{
	//values needed to decorate room
	bool roomCompleted = false;
	int offSet = 1;
	int x = m_rooms[t_roomIndex].x + 1;
	int y = m_rooms[t_roomIndex].y - offSet;
	int maxWidth = m_rooms[t_roomIndex].x + m_rooms[t_roomIndex].width - offSet * 2;
	//loop through all top walls
	for (int index = x; index <= maxWidth; index++)
	{
		//check the tiles are Walls
		if (getTile(index, y, m_decorTiles, m_width, m_height) == Wall)
		{
			//replace them starting at index till you get to the end of the room
			setTile(index, y, PrisonTile, m_decorTiles, m_width);
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
		setTile(index, y, PrisonTile, m_decorTiles, m_width);
		m_rooms[t_roomIndex].decorInRoom++;
		//set the room to be completed and return
		roomCompleted = true;
	}
	return roomCompleted;
}
////---------------------------------------------------------------------------
////Function to create a room of coffins
////---------------------------------------------------------------------------
void DunDecor::createCoffinRoom()
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
		for (int index = x; index <= maxWidth; index += 2)
		{
			//check for floors
			if (getTile(index, y, m_decorTiles, m_width, m_height) == FloorTile
				|| getTile(index, y, m_decorTiles, m_width, m_height) == StoneFloorTile)
			{
				//set the tile to coffins and tiles under coffins to unused
				setTile(index, y, CoffinTile, m_decorTiles, m_width);
				setTile(index, y + offSet, UnusedTile, m_decorTiles, m_width);
				//place a flame caouldron at bottom of coffins
				setTile(index, y + offSet * 2, FlameCauldron, m_decorTiles, m_width);
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
void DunDecor::bossRoomGroundTiles()
{
	//from these width and height get a room index for boss room and change all the floor tiles to dirt
	int maxRoomSizeWidth = 6;
	int maxRoomSizeHeight = 6;
	m_bossRoomIndex = GetRoom(maxRoomSizeWidth, maxRoomSizeHeight);
	setUniqueGroundTiles(DirtTile, maxRoomSizeWidth, maxRoomSizeHeight, m_bossRoomIndex);
}
////---------------------------------------------------------------------------
////Function to create a worship room thats different From the others
////---------------------------------------------------------------------------
void DunDecor::worsipRoomGroundTiles()
{
	//from these width and height get a room index for worship room and change all the floor tiles to dark
	int maxRoomSizeWidth = 5;
	int maxRoomSizeHeight = 5;
	m_worshipRoomIndex = GetRoom(maxRoomSizeWidth, maxRoomSizeHeight);
	setUniqueGroundTiles(DarkTiles, maxRoomSizeWidth, maxRoomSizeHeight, m_worshipRoomIndex);
}
void DunDecor::statueRoomTiles()
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
void DunDecor::bossRoom()
{
	//get x,y near room centre
	int offSetFromCentre = 1;
	int x = m_rooms[m_bossRoomIndex].x + m_rooms[m_bossRoomIndex].width / 2 - offSetFromCentre;
	int y = m_rooms[m_bossRoomIndex].y + m_rooms[m_bossRoomIndex].height / 2 - offSetFromCentre;
	if (getTile(x, y, m_decorTiles, m_width, m_height) == DirtTile)
	{
		//place big hole of death
		setTile(x, y, HoleTile, m_decorTiles, m_width);
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
				if (getTile(xVal, yVal, m_decorTiles, m_width, m_height) == DirtTile)
				{
					setTile(xVal, yVal, UnusedTile, m_decorTiles, m_width);
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
void DunDecor::bossRoomSkull()
{
	//values needed to place skull tile
	int offSet = 1;
	int moveBy = 3;
	int x = m_rooms[m_bossRoomIndex].x + offSet;
	int y = m_rooms[m_bossRoomIndex].y;
	if (getTile(x, y - 1, m_decorTiles, m_width, m_height) != Door2)
	{
		if (getTile(x, y, m_decorTiles, m_width, m_height) == DirtTile)
		{
			//place the skeleton if the tiles dirt
			setTile(x, y, Skull, m_decorTiles, m_width);
			//move the x and y by a certain amount and place another
			x = m_rooms[m_bossRoomIndex].x + moveBy;
			y++;
			setTile(x, y, Skull, m_decorTiles, m_width);
		}
	}
	//get a new x,y position of right corner of room and place flame cauldron decor 
	x = m_rooms[m_bossRoomIndex].x + m_rooms[m_bossRoomIndex].width - offSet;
	y = m_rooms[m_bossRoomIndex].y;
	if (checkForDoors(x, y) == false)
	{
		setTile(x, y, FlameCauldron, m_decorTiles, m_width);
	}
	//get a new x,y position of bottom right corner of room and place flame cauldron decor 
	x = m_rooms[m_bossRoomIndex].x + m_rooms[m_bossRoomIndex].width - offSet;
	y = m_rooms[m_bossRoomIndex].y + m_rooms[m_bossRoomIndex].height - offSet;
	if (checkForDoors(x, y) == false)
	{
		setTile(x, y, FlameCauldron, m_decorTiles, m_width);
	}
}
////---------------------------------------------------------------------------
////Adds health to the boss room
////---------------------------------------------------------------------------
void DunDecor::bossRoomHealth()
{
	//set offset and get x,y not against wall somewhere in room
	int offSet = 1;
	int x = randomInt(m_rooms[m_bossRoomIndex].x + offSet, m_rooms[m_bossRoomIndex].x + m_rooms[m_bossRoomIndex].width - (offSet * 2));
	int y = m_rooms[m_bossRoomIndex].y + m_rooms[m_bossRoomIndex].height - offSet * 2;
	//check if tile is nor dirt and if so move to the right and place health
	if (getTile(x, y, m_decorTiles, m_width, m_height) != DirtTile)
	{
		x++;
		setTile(x, y, HealthPos, m_decorTiles, m_width);
	}
	else
	{
		//place health
		setTile(x, y, HealthPos, m_decorTiles, m_width);
	}
}
////---------------------------------------------------------------------------
////Adds decor to the walls of the boss room
////---------------------------------------------------------------------------
void DunDecor::bossRoomWalls()
{
	//get x ,y starting on the first wall tille in the room
	int offSet = 1;
	int x = m_rooms[m_bossRoomIndex].x;
	int y = m_rooms[m_bossRoomIndex].y - offSet;
	//chack they are walls
	if (getTile(x, y, m_decorTiles, m_width, m_height) == Wall)
	{
		//chack x is not near the right side of the room
		if (x < m_rooms[m_bossRoomIndex].x + m_rooms[m_bossRoomIndex].width - offSet)
		{
			//place torch,chains and move by 2 along x each time
			setTile(x, y, Torch, m_decorTiles, m_width);
			x += 2;
			setTile(x, y, Chains, m_decorTiles, m_width);
			x += 2;
			setTile(x, y, Torch, m_decorTiles, m_width);
		}
	}

}
////---------------------------------------------------------------------------
////creates the worship room
////---------------------------------------------------------------------------
void DunDecor::worshipRoom()
{
	//get x ,y in middle of room
	int x = m_rooms[m_worshipRoomIndex].x + m_rooms[m_worshipRoomIndex].width / 2;
	int y = m_rooms[m_worshipRoomIndex].y + m_rooms[m_worshipRoomIndex].height / 2;
	//check tiles and place worship tile
	if (getTile(x, y, m_decorTiles, m_width, m_height) == DarkTiles)
	{
		setTile(x, y, Worship, m_decorTiles, m_width);
		setUnusedTile(x, y, DarkTiles, FloorTile);
	}
	worshipRoomDecor();

	deleteRoom(m_worshipRoomIndex);

}
////---------------------------------------------------------------------------
////creates the decorations for the worship room
////---------------------------------------------------------------------------
void DunDecor::worshipRoomDecor()
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
			setTile(x, y, ChairL, m_decorTiles, m_width);
			y++;
		}
	}
	//get x ,y at top left corner
	x = m_rooms[m_worshipRoomIndex].x;
	y = m_rooms[m_worshipRoomIndex].y;
	//place plant at top left and bottom left corners
	if (checkForDoors(x, y) == false)
	{
		setTile(x, y, Plant, m_decorTiles, m_width);
		y = m_rooms[m_worshipRoomIndex].y + m_rooms[m_worshipRoomIndex].height - offSet;
		setTile(x, y, Plant, m_decorTiles, m_width);
	}
}
////---------------------------------------------------------------------------
////Function to create a room with a statue of some famous person.
////In this case can be of the boss as a warning to players
////---------------------------------------------------------------------------
void DunDecor::statueRoom()
{
	//get x,y at centre of room
	int offSetFromCentre = 1;
	int x = m_rooms[m_statueRoomIndex].x + m_rooms[m_statueRoomIndex].width / 2 - offSetFromCentre;
	int y = m_rooms[m_statueRoomIndex].y + m_rooms[m_statueRoomIndex].height / 2 - offSetFromCentre;
	//set the statue at that x,y
	if (getTile(x, y, m_decorTiles, m_width, m_height) == TilePattern)
	{
		setTile(x, y, Statue, m_decorTiles, m_width);
		setUnusedTile(x, y, TilePattern, FloorTile);
	}
	//delete room and call decorate room func
	statueRoomDecor();
	deleteRoom(m_statueRoomIndex);
}
////---------------------------------------------------------------------------
////Function to add some decor to the statue room
////---------------------------------------------------------------------------
void DunDecor::statueRoomDecor()
{
	//set x,y to top left corner
	int offSet = 1;
	int x = m_rooms[m_statueRoomIndex].x;
	int y = m_rooms[m_statueRoomIndex].y;
	//place a flaming cauldron in all 4 corners of the room
	if (checkForDoors(x, y) == false)
	{
		setTile(x, y, FlameCauldron, m_decorTiles, m_width);
		x = m_rooms[m_statueRoomIndex].x + m_rooms[m_statueRoomIndex].width - offSet;
		y = m_rooms[m_statueRoomIndex].y + m_rooms[m_statueRoomIndex].height - offSet;
	}
	if (checkForDoors(x, y) == false)
	{
		setTile(x, y, FlameCauldron, m_decorTiles, m_width);
		x = m_rooms[m_statueRoomIndex].x;
		y = m_rooms[m_statueRoomIndex].y + m_rooms[m_statueRoomIndex].height - offSet;
	}
	if (checkForDoors(x, y) == false)
	{
		setTile(x, y, FlameCauldron, m_decorTiles, m_width);
		x = m_rooms[m_statueRoomIndex].x + m_rooms[m_statueRoomIndex].width - offSet;
		y = m_rooms[m_statueRoomIndex].y;
	}
	if (checkForDoors(x, y) == false) {
		setTile(x, y, FlameCauldron, m_decorTiles, m_width);
	}

}
////---------------------------------------------------------------------------
////Function to create a feast room with a table and chairs
////---------------------------------------------------------------------------
void DunDecor::createFeastRoom()
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
			if (getTile(x, y, m_decorTiles, m_width, m_height) == StoneFloorTile
				|| getTile(x, y, m_decorTiles, m_width, m_height) == FloorTile)
			{
				m_rooms[roomIndex].decorInRoom++;
				setTile(x, y, Table, m_decorTiles, m_width);
				setUnusedTile(x, y, FloorTile, StoneFloorTile);
			}
			int newY = 4;
			//set the x y to be half way in the tile
			x += 1;
			y -= 1;
			// add another decoration
			if (getTile(x, y, m_decorTiles, m_width, m_height) == StoneFloorTile
				|| getTile(x, y, m_decorTiles, m_width, m_height) == FloorTile)
			{
				m_rooms[roomIndex].decorInRoom++;
				setTile(x, y, Plant, m_decorTiles, m_width);
				y += newY;
				setTile(x, y, Plant, m_decorTiles, m_width);
			}
			deleteRoom(roomIndex);
		}
	}
	DEBUG_MSG("Issue Feast tiles");
}
////---------------------------------------------------------------------------
////Function to create a library room for the dungeon
////---------------------------------------------------------------------------
void DunDecor::createLibraryRoom()
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
		if (getTile(x, y, m_decorTiles, m_width, m_height) == StoneFloorTile || getTile(x, y, m_decorTiles, m_width, m_height) == FloorTile)
		{
			setTile(x, y, Plant, m_decorTiles, m_width);
			y = m_rooms[roomIndex].y;
			setTile(x, y, Plant, m_decorTiles, m_width);
		}
		//delete this room
		deleteRoom(roomIndex);
	}
}
////---------------------------------------------------------------------------
////Function to place the bookshelf decorations
////---------------------------------------------------------------------------
void DunDecor::placeBookShelfDecor(int t_roomIndex)
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
			if (getTile(x2 + i, y2, m_decorTiles, m_width, m_height) == Door2)
			{
				doorNum = x2 + i;
			}
		}
	}
	//if there were doors found then place bookshelf after door
	if (doorNum > 0)
	{
		x = doorNum + offSet;
		setTile(x, y, BookShelve, m_decorTiles, m_width);
		setTile(x + 2, y, UnusedTile, m_decorTiles, m_width);
	}
	//if not place at room corner
	else if (doorNum == 0)
	{
		setTile(x, y, BookShelve, m_decorTiles, m_width);
		setTile(x + 2, y, UnusedTile, m_decorTiles, m_width);
	}
}
////---------------------------------------------------------------------------
////Function to place the chairs in the library and other decor
////---------------------------------------------------------------------------
void DunDecor::addChairsDecor(int t_roomIndex)
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
			setTile(x, y, ChairF, m_decorTiles, m_width);
			x++;
		}
	}
}
////---------------------------------------------------------------------------
////Function to add traps to the halls 
////---------------------------------------------------------------------------
void DunDecor::placeTrapsInHalls()
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
			if (getTile(x, y, m_decorTiles, m_width, m_height) == FloorTile)
			{
				if (checkForDoors(x, y) == false) {
					setTile(x, y, SpikeTrap, m_decorTiles, m_width);
				}
			}
		}
	}
}

////---------------------------------------------------------------------------
////Function to create cave doors that the player can exit and enter
////---------------------------------------------------------------------------
void DunDecor::playerStartPos()
{

	if (m_rooms.empty() == false)
	{
		//loop through all rooms
		for (int i = 0; i < m_rooms.size(); i++)
		{
			//get a random x along the top walls and y at the top walls
			int x = randomInt(m_rooms[i].x, m_rooms[i].x + m_rooms[i].width - 1);
			int y = m_rooms[i].y - 1;
			//check if its a wall and repalce with exit/spawn
			if (getTile(x, y, m_decorTiles, m_width, m_height) == Wall)
			{
				setTile(x, y, SpawnPoint, m_decorTiles, m_width);
			}
		}
	}
	DEBUG_MSG("Issue placing starting position tile");
}
////---------------------------------------------------------------------------
////Function to place decorations on the walls(eg torch,chains, pictures)
////---------------------------------------------------------------------------
void DunDecor::placeDecorOnWalls()
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
				if (getTile(x, y, m_decorTiles, m_width, m_height) == Wall)
				{
					//if so place chains for decor type 0
					if (decorType == 0) {
						setTile(x, y, Chains, m_decorTiles, m_width);
					}
					//if so place torch for decor type 1
					else if (decorType == 1) {
						setTile(x, y, Torch, m_decorTiles, m_width);
					}
					//if so place one  picture for decor type 2
					else if (decorType == 2) {
						if (picNum == 0)
						{
							picNum++;
							setTile(x, y, Picture, m_decorTiles, m_width);
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
void DunDecor::createTrapsInRooms()
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
				if (getTile(x, y, m_decorTiles, m_width, m_height) == FloorTile
					|| getTile(x, y, m_decorTiles, m_width, m_height) == StoneFloorTile)
				{
					if (checkForDoors(x, y) == false) {
						setTile(x, y, SpikeTrap, m_decorTiles, m_width);
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
void DunDecor::placeDecorInHalls()
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
					int y = randomInt(m_halls[i].y + 1, m_halls[i].y + m_halls[i].width - 1);
					//this is so place plant if it can
					if (getTile(x, y, m_decorTiles, m_width, m_height) == FloorTile
						&& checkForDoors(x, y) == false)
					{
						setTile(x, y, Plant, m_decorTiles, m_width);
					}
				}
				else if (decorType == 1)
				{
					//Create x ,y pos for new tile at left side of room random along y 
					int offSets = 1;
					int x = m_halls[i].x + offSets;
					int y = randomInt(m_halls[i].y + offSets, m_halls[i].y + m_halls[i].width - offSets);
					//this is so place potion if it can
					if (getTile(x, y, m_decorTiles, m_width, m_height) == FloorTile
						&& checkForDoors(x, y) == false)
					{
						setTile(x, y, Potion, m_decorTiles, m_width);
					}

				}
				else if (decorType == 2)
				{
					//Create x ,y pos for new tile at right side of room random along y 
					int x = m_halls[i].x + m_halls[i].width - 1;
					int y = randomInt(m_halls[i].y, m_halls[i].y + m_halls[i].width - 1);
					//this is so place Money if it can
					if (getTile(x, y, m_decorTiles, m_width, m_height) == FloorTile
						&& checkForDoors(x, y) == false)
					{
						setTile(x, y, Money, m_decorTiles, m_width);
					}
				}
			}
			//delete the hall after its decorated
			m_halls.erase(m_halls.begin() + i);
		}

	}
	DEBUG_MSG("Issue placing tiles tile");
}
