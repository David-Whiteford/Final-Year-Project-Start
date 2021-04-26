#include "DungeonDecor.h"

DunDecor::~DunDecor()
{
}
////---------------------------------------------------------------------------
////Function to decorate and change floor tiles of each room
////---------------------------------------------------------------------------
void DunDecor::createDunExtras(std::vector<char>& t_tiles, std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms,
	std::vector<RoomVals>& t_exits, std::vector<RoomVals>& t_halls)
{
	//create unique rooms
	createUniqueRooms(t_decorTiles,t_rooms,t_tiles);
	//example of a trigger that was set up by procedual generation
	createTrapsInRooms(t_decorTiles, t_rooms,t_halls);
	//place triggers in rooms to exit dungeon
	playerStartPos(t_decorTiles, t_rooms);
	//place decor on the walls,in rooms, halls
	placeDecorOnWalls(t_decorTiles, t_rooms);
	placeDecorInRoom(t_decorTiles, t_rooms);
	placeDecorInHalls(t_decorTiles,t_halls);
	//if rooms are left call the place decor to fill them
	if (t_rooms.empty() == false) {
		placeDecorInRoom(t_decorTiles, t_rooms);
	}
	//create a bedroom
	bedRoom(t_decorTiles, t_rooms);
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
void DunDecor::setSpawnToWall(int t_roomIndex, std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms)
{
	//get the y at the top wall
	int y = t_rooms[t_roomIndex].y - 1;
	//loop from the starting x of the room to the ending x and set all spawns to walls. this loops through all top walls
	for (int x = t_rooms[t_roomIndex].x; x < t_rooms[t_roomIndex].x + t_rooms[t_roomIndex].width - 1; x++)
	{
		if (getTile(x, y,t_decorTiles, m_width,m_height) == SpawnPoint)
		{
			setTile(x, y, Wall, t_decorTiles, m_width);
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
void DunDecor::bedRoom(std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms)
{
	if (t_rooms.empty() == false)
	{
		int offSet = 1;
		int randRoom = randomInt(t_rooms.size());
		//get the position of the first bed at the left corner
		int x = t_rooms[randRoom].x;
		int y = t_rooms[randRoom].y - offSet;
		//loop to place the number of beds eg 2
		int bedNum = 2;
		for (int i = 0; i < bedNum; i++)
		{
			//place the bed at xy
			placeBeds(x, y, t_decorTiles);
			//change the x y to the top right corner of the room on tile between wall and bed
			int x = t_rooms[randRoom].x + t_rooms[randRoom].width - 2;
			int y = t_rooms[randRoom].y - offSet;
			placeBeds(x, y, t_decorTiles);
		}
		deleteRoom(randRoom,t_rooms);
	}
}
////---------------------------------------------------------------------------
////Function to create a bedroom with beds in it.place the bed and places tiles under the bed as unused tiles
////also place a table next to bed and +1 on the x
////---------------------------------------------------------------------------
void DunDecor::placeBeds(int t_x, int t_y,std::vector<char>& t_decorTiles)
{
	int x = t_x;
	int y = t_y;
	//check the x,y was a wall
	if (getTile(x, y,t_decorTiles,m_width,m_height) == Wall)
	{
		//move down y as the current is at a wall and check now on floor
		y++;
		if (getTile(x, y,t_decorTiles,m_width,m_height) == FloorTile
			|| getTile(x, y, t_decorTiles, m_width, m_height) == StoneFloorTile)
		{
			//check all tiles that the bed will occupy havnt got a door or exit next to them
			if (checkForDoors(x, y, t_decorTiles, m_width, m_height) == false)
			{
				//set up a temporay y
				int newY = y;
				newY++;
				if (checkForDoors(x, newY, t_decorTiles, m_width, m_height) == false)
				{
					newY++;
					if (checkForDoors(x, newY, t_decorTiles, m_width, m_height) == false)
					{
						//place the bed and make all tile under it unused this is cause the bed taks up 3 tiles along y axis
						int tempY = y;
						setTile(x, y, Bed, t_decorTiles, m_width);
						tempY++;
						setTile(x, tempY, UnusedTile, t_decorTiles, m_width);
						tempY++;
						setTile(x, tempY, UnusedTile, t_decorTiles, m_width);
					}
				}
				x++;
				//check all tiles around end table havnt got a door or exit next to them
				if (checkForDoors(x, y, t_decorTiles, m_width, m_height) == false)
				{
					int newY = y;
					newY++;
					if (checkForDoors(x, newY, t_decorTiles, m_width, m_height) == false)
					{
						//check if there are door at the x ,y 
						newY++;
						if (checkForDoors(x, newY, t_decorTiles, m_width, m_height) == false)
						{
							//also check the x ,y is a Floor and place a NightStand
							if (getTile(x, y, t_decorTiles, m_width, m_height) == StoneFloorTile
								|| getTile(x, y, t_decorTiles, m_width, m_height) == FloorTile) {
								setTile(x, y, NightStand, t_decorTiles, m_width);
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
void DunDecor::placeDecorInRoom(std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms)
{
	if (t_rooms.empty() == false)
	{
		//loop through each room
		for (int i = 0; i < t_rooms.size(); i++)
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
					pos = GenXAndYAtTopWall(i, t_rooms);
					x = pos.x;
					y = pos.y;
					//check if tiles are flooors
					if (getTile(x, y, t_decorTiles, m_width, m_height) == FloorTile
						|| getTile(x, y, t_decorTiles, m_width, m_height) == StoneFloorTile)
					{
						//check if theres doors around the tile
						if (checkForDoors(x, y, t_decorTiles, m_width, m_height) == false)
						{
							//place decor and change bool to true
							placedDecor = true;
							setTile(x, y, Chest, t_decorTiles, m_width);
							t_rooms[i].decorInRoom++;
						}
					}
					break;
				case 1:
					//get random x y value in the room
					x = randomInt(t_rooms[i].x, t_rooms[i].x + t_rooms[i].width - 1);
					y = randomInt(t_rooms[i].y, t_rooms[i].y + t_rooms[i].height - 1);
					//check tiles are floors
					if (getTile(x, y, t_decorTiles, m_width, m_height) == FloorTile
						|| getTile(x, y, t_decorTiles, m_width, m_height) == StoneFloorTile)
					{
						//check doors around tiles
						if (checkForDoors(x, y, t_decorTiles, m_width, m_height) == false)
						{
							//place decor and change bool to true and increment decor num in room
							placedDecor = true;
							setTile(x, y, Skull, t_decorTiles, m_width);
							t_rooms[i].decorInRoom++;
						}
					}
					break;
				case 2:
					//get position along top wall
					pos = GenXAndYAtTopWall(i, t_rooms);
					x = pos.x;
					y = pos.y;
					//check tiles are floors
					if (getTile(x, y, t_decorTiles, m_width, m_height) == FloorTile 
						|| getTile(x, y, t_decorTiles, m_width, m_height) == StoneFloorTile)
					{
						if (checkForDoors(x, y,t_decorTiles,m_width,m_height) == false)
						{
							placedDecor = true;
							//place a potion table and move down 1 on the y to then place chair
							setTile(x, y, Potion, t_decorTiles, m_width);
							y += 1;
							t_rooms[i].decorInRoom++;
							if (getTile(x, y, t_decorTiles, m_width, m_height) == FloorTile 
								|| getTile(x, y, t_decorTiles, m_width, m_height) == StoneFloorTile)
							{
								//check there are no doors or exits around the x,y and then place chair
								if (checkForDoors(x, y, t_decorTiles, m_width, m_height) == false)
								{
									setTile(x, y, ChairF, t_decorTiles, m_width);
									t_rooms[i].decorInRoom++;
								}
							}
						}
					}
					break;
				case 3:
					//get position along bottom wall
					pos = GenXAndYAtBottomWall(i, t_rooms);
					x = pos.x;
					y = pos.y;
					//check the x , y are floor tiles
					if (getTile(x, y, t_decorTiles, m_width, m_height) == FloorTile
						|| getTile(x, y, t_decorTiles, m_width, m_height) == StoneFloorTile)

					{
						//check doors around tiles
						if (checkForDoors(x, y, t_decorTiles, m_width, m_height) == false)
						{
							placedDecor = true;
							setTile(x, y, Plant, t_decorTiles, m_width);
							t_rooms[i].decorInRoom++;
						}
					}
					break;
				case 4:
					pos = GenXAndYAtBottomWall(i, t_rooms);
					x = pos.x;
					y = pos.y;

					if (getTile(x, y, t_decorTiles, m_width, m_height) == FloorTile 
						|| getTile(x, y, t_decorTiles, m_width, m_height) == StoneFloorTile)
					{
						if (checkForDoors(x, y, t_decorTiles, m_width, m_height) == false)
						{
							//place the money at bottom wall
							placedDecor = true;
							setTile(x, y, Money, t_decorTiles, m_width);
							y += 1;
							t_rooms[i].decorInRoom++;
							if (getTile(x, y, t_decorTiles, m_width, m_height) == FloorTile
								|| getTile(x, y, t_decorTiles, m_width, m_height) == StoneFloorTile)
							{
								setTile(x, y, ChairF, t_decorTiles, m_width);
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
			if (t_rooms[i].decorInRoom > 0)
			{
				deleteRoom(i,t_rooms);
			}
		}

	}
	DEBUG_MSG("Issue placing tiles tile");
}
////---------------------------------------------------------------------------
////Function to create a room of jail cells
////---------------------------------------------------------------------------
void DunDecor::createJailRoom(std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms)
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
	if (t_rooms.empty() == false)
	{
		//get a room and store its index
		roomIndex = getRoom(maxRoomSizeWidth, maxRoomSizeHeight, t_rooms);
		if (roomIndex != noIndex)
		{
			roomFound = true;
		}
	}
	//if a room was found
	if (roomFound)
	{
		//set the spawns in the rooms to be walls
		setSpawnToWall(roomIndex,t_decorTiles,t_rooms);
		//call jail cell func
		roomCompleted = createJailCells(roomIndex, t_decorTiles, t_rooms);

		//loop to place a number of skulls
		int maxSkullNum = 3;
		for (int skullNum = 0; skullNum < maxSkullNum; skullNum++)
		{
			// get a random position in the room
			int x = randomInt(t_rooms[roomIndex].x + offSet, t_rooms[roomIndex].x + t_rooms[roomIndex].width - 2);
			int y = randomInt(t_rooms[roomIndex].y + offSet, t_rooms[roomIndex].y + t_rooms[roomIndex].height - 2);
			//check tiles are floor and place skull decor
			if (getTile(x, y, t_decorTiles, m_width, m_height) == StoneFloorTile
				|| getTile(x, y, t_decorTiles, m_width, m_height) == FloorTile)
			{
				setTile(x, y, Skull, t_decorTiles, m_width);
			}
		}
		//delete the room and increment the number of jail rooms/delete that room
		if (roomCompleted)
		{
			m_numJailRoomsPlaced++;
			deleteRoom(roomIndex, t_rooms);
		}
	}
}
////---------------------------------------------------------------------------
////Function to place jail cells in the Jail room and place them in a order from one side of room to next
////---------------------------------------------------------------------------
bool DunDecor::createJailCells(int t_roomIndex, std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms)
{
	//values needed to decorate room
	bool roomCompleted = false;
	int offSet = 1;
	int x = t_rooms[t_roomIndex].x + 1;
	int y = t_rooms[t_roomIndex].y - offSet;
	int maxWidth = t_rooms[t_roomIndex].x + t_rooms[t_roomIndex].width - offSet * 2;
	//loop through all top walls
	for (int index = x; index <= maxWidth; index++)
	{
		//check the tiles are Walls
		if (getTile(index, y, t_decorTiles, m_width, m_height) == Wall)
		{
			//replace them starting at index till you get to the end of the room
			setTile(index, y, PrisonTile, t_decorTiles, m_width);
			t_rooms[t_roomIndex].decorInRoom++;
			roomCompleted = true;
		}
	}
	//get the new x , y to place tile in center
	x = t_rooms[t_roomIndex].x + offSet;
	int endX = t_rooms[t_roomIndex].x + t_rooms[t_roomIndex].width - 2;
	y = t_rooms[t_roomIndex].y + t_rooms[t_roomIndex].height / 2;
	//loop through room from index to two tiles from end;
	for (int index = x; index <= endX; index++)
	{
		//set the background tile to a wall and decor tile to prison
		//setTile(index, y, Wall);
		setTile(index, y, PrisonTile, t_decorTiles, m_width);
		t_rooms[t_roomIndex].decorInRoom++;
		//set the room to be completed and return
		roomCompleted = true;
	}
	return roomCompleted;
}
////---------------------------------------------------------------------------
////Function to create a room of coffins
////---------------------------------------------------------------------------
void DunDecor::createCoffinRoom(std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms)
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
	roomIndex = getRoom(maxRoomSizeWidth, maxRoomSizeHeight, t_rooms);
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
		int x = t_rooms[roomIndex].x + offSet;
		int y = t_rooms[roomIndex].y + offSet;
		int maxWidth = t_rooms[roomIndex].x + t_rooms[roomIndex].width - 2;
		//loop startijng at index till the end of the room increment by 2
		for (int index = x; index <= maxWidth; index += 2)
		{
			//check for floors
			if (getTile(index, y, t_decorTiles, m_width, m_height) == FloorTile
				|| getTile(index, y, t_decorTiles, m_width, m_height) == StoneFloorTile)
			{
				//set the tile to coffins and tiles under coffins to unused
				setTile(index, y, CoffinTile, t_decorTiles, m_width);
				setTile(index, y + offSet, UnusedTile, t_decorTiles, m_width);
				//place a flame caouldron at bottom of coffins
				setTile(index, y + offSet * 2, FlameCauldron, t_decorTiles, m_width);
				t_rooms[roomIndex].decorInRoom++;
			}
		}
		//delete the room when completed
		deleteRoom(roomIndex, t_rooms);

	}

}
////---------------------------------------------------------------------------
////Function to create a feast room with a table and chairs
////---------------------------------------------------------------------------
void DunDecor::createFeastRoom(std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms, std::vector<char>& t_tiles)
{
	//set values needed
	int roomIndex = 0;
	int invalidIndex = 1000;
	int offsetFromCentre = 1;
	int maxRoomSizeRequired = 7;
	bool roomFound = false;
	if (t_rooms.empty() == false)
	{
		//get room thats size 7 and store its index and set room was found
		roomIndex = getRoom(maxRoomSizeRequired, maxRoomSizeRequired, t_rooms);
		if (roomIndex != invalidIndex)
		{
			roomFound = true;
		}

		if (roomFound)
		{
			//get the middle of the room if one found 
			int x = t_rooms[roomIndex].x + t_rooms[roomIndex].width / 2;
			int y = t_rooms[roomIndex].y + t_rooms[roomIndex].height / 2;
			x -= offsetFromCentre;
			y -= offsetFromCentre;
			//check the tile at the x and y val and set if its a floor 
			if (getTile(x, y, t_decorTiles, m_width, m_height) == StoneFloorTile
				|| getTile(x, y, t_decorTiles, m_width, m_height) == FloorTile)
			{
				t_rooms[roomIndex].decorInRoom++;
				setTile(x, y, Table, t_decorTiles, m_width);
				setUnusedTile(x, y, FloorTile, StoneFloorTile, t_decorTiles,m_width,m_height);
			}
			int newY = 4;
			//set the x y to be half way in the tile
			x += 1;
			y -= 1;
			// add another decoration
			if (getTile(x, y, t_decorTiles, m_width, m_height) == StoneFloorTile
				|| getTile(x, y, t_decorTiles, m_width, m_height) == FloorTile)
			{
				t_rooms[roomIndex].decorInRoom++;
				setTile(x, y, Plant, t_decorTiles, m_width);
				y += newY;
				setTile(x, y, Plant, t_decorTiles, m_width);
			}
			deleteRoom(roomIndex, t_rooms);
		}
	}
	DEBUG_MSG("Issue Feast tiles");
}
////---------------------------------------------------------------------------
////Function to create a library room for the dungeon
////---------------------------------------------------------------------------
void DunDecor::createLibraryRoom(std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms)
{
	//set values need eg size of room wanteed,index,roomFound etc
	int heightOffset = 3;
	int roomIndex = 0;
	int invalidIndex = 1000;
	int offSet = 1;
	int maxRoomWidth = 7;
	int maxRoomHeight = 4;
	bool roomFound = false;
	if (t_rooms.empty() == false)
	{
		//get room and store index and set room was found
		roomIndex = getRoom(maxRoomWidth, maxRoomHeight, t_rooms);
		if (roomIndex != invalidIndex)
		{
			roomFound = true;
		}
	}
	if (roomFound)
	{
		//set all spawns in the room to walls
		setSpawnToWall(roomIndex, t_decorTiles, t_rooms);
		//func to place bookshelf
		placeBookShelfDecor(roomIndex, t_decorTiles, t_rooms);
		addChairsDecor(roomIndex, t_decorTiles, t_rooms);
		int xOffSet = 2;
		//get the bottom right of room and add a plant decor
		int x = t_rooms[roomIndex].x + t_rooms[roomIndex].width - xOffSet;
		int y = t_rooms[roomIndex].y + t_rooms[roomIndex].height - offSet;
		//place plant tiles at top right and bottom
		if (getTile(x, y, t_decorTiles, m_width, m_height) == StoneFloorTile || getTile(x, y, t_decorTiles, m_width, m_height) == FloorTile)
		{
			setTile(x, y, Plant, t_decorTiles, m_width);
			y = t_rooms[roomIndex].y;
			setTile(x, y, Plant, t_decorTiles, m_width);
		}
		//delete this room
		deleteRoom(roomIndex, t_rooms);
	}
}
////---------------------------------------------------------------------------
////Function to place the bookshelf decorations
////---------------------------------------------------------------------------
void DunDecor::placeBookShelfDecor(int t_roomIndex, std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms)
{
	int offSet = 1;
	int doorNum = 0;
	//get the middle of the room if one found 
	int x = t_rooms[t_roomIndex].x;
	int y = t_rooms[t_roomIndex].y;
	int maxXVal = 3;
	for (int i = 0; i < maxXVal; i++)
	{
		//check for doors at x y and 3 tiles to right on x 
		if (checkForDoors(x, y, t_decorTiles, m_width, m_height) == false)
		{
			//starting at x and move up 1 on y 
			int x2 = x;
			int y2 = y - 1;
			//allows us to check for doors along wall tiles
			if (getTile(x2 + i, y2, t_decorTiles, m_width, m_height) == Door2)
			{
				doorNum = x2 + i;
			}
		}
	}
	//if there were doors found then place bookshelf after door
	if (doorNum > 0)
	{
		x = doorNum + offSet;
		setTile(x, y, BookShelve, t_decorTiles, m_width);
		setTile(x + 2, y, UnusedTile, t_decorTiles, m_width);
	}
	//if not place at room corner
	else if (doorNum == 0)
	{
		setTile(x, y, BookShelve, t_decorTiles, m_width);
		setTile(x + 2, y, UnusedTile, t_decorTiles, m_width);
	}
}
////---------------------------------------------------------------------------
////Function to place the chairs in the library and other decor
////---------------------------------------------------------------------------
void DunDecor::addChairsDecor(int t_roomIndex, std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms)
{
	int offSet = 1;
	//get the bottom right of room
	int x = t_rooms[t_roomIndex].x;
	int y = t_rooms[t_roomIndex].y + t_rooms[t_roomIndex].height - offSet;
	int maxWidth = 2;
	//loop along the x till you get to a certain width 
	for (int i = 0; i <= maxWidth; i++)
	{
		//check for any doors and place a chair if no doors around x, y
		if (checkForDoors(x, y, t_decorTiles, m_width, m_height) == false)
		{
			setTile(x, y, ChairF, t_decorTiles, m_width);
			x++;
		}
	}
}
////---------------------------------------------------------------------------
////Function to add traps to the halls 
////---------------------------------------------------------------------------
void DunDecor::placeTrapsInHalls(std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_halls)
{
	//place some traps in random halls and up 8 halls
	int offSet = 1;
	int maxHallsWithTraps = 8;
	if (t_halls.empty() == false)
	{
		for (int i = 0; i < maxHallsWithTraps; i++)
		{
			//find a random room and a random x y in that room
			int r = randomInt(0, t_halls.size() - offSet);
			int x = randomInt(t_halls[r].x, t_halls[r].x + t_halls[r].width - offSet);
			int y = randomInt(t_halls[r].y, t_halls[r].y + t_halls[r].height - offSet);
			//check the floor tile and place spike if its a floor tile
			if (getTile(x, y, t_decorTiles, m_width, m_height) == FloorTile)
			{
				if (checkForDoors(x, y, t_decorTiles, m_width, m_height) == false) {
					setTile(x, y, SpikeTrap, t_decorTiles, m_width);
				}
			}
		}
	}
}

////---------------------------------------------------------------------------
////Function to create cave doors that the player can exit and enter
////---------------------------------------------------------------------------
void DunDecor::playerStartPos(std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms)
{

	if (t_rooms.empty() == false)
	{
		//loop through all rooms
		for (int i = 0; i < t_rooms.size(); i++)
		{
			//get a random x along the top walls and y at the top walls
			int x = randomInt(t_rooms[i].x, t_rooms[i].x + t_rooms[i].width - 1);
			int y = t_rooms[i].y - 1;
			//check if its a wall and repalce with exit/spawn
			if (getTile(x, y, t_decorTiles, m_width, m_height) == Wall)
			{
				setTile(x, y, SpawnPoint, t_decorTiles, m_width);
			}
		}
	}
	DEBUG_MSG("Issue placing starting position tile");
}
////---------------------------------------------------------------------------
////Function to place decorations on the walls(eg torch,chains, pictures)
////---------------------------------------------------------------------------
void DunDecor::placeDecorOnWalls(std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms)
{
	if (t_rooms.empty() == false)
	{
		//loop through rooms
		for (int i = 0; i < t_rooms.size(); i++)
		{
			//set up the values for number of pictures in a room and the decor amount
			int picNum = 0;
			int maxDecorInRoom = randomInt(3, 5);
			//loop till max decor was placed
			for (int decorType = 0; decorType < maxDecorInRoom;)
			{
				//set the x ,y  at the top wall
				int offSet = 1;
				int x = randomInt(t_rooms[i].x + offSet, t_rooms[i].x + t_rooms[i].width - offSet);
				int y = t_rooms[i].y - offSet;
				//check for wall
				if (getTile(x, y, t_decorTiles, m_width, m_height) == Wall)
				{
					//if so place chains for decor type 0
					if (decorType == 0) {
						setTile(x, y, Chains, t_decorTiles, m_width);
					}
					//if so place torch for decor type 1
					else if (decorType == 1) {
						setTile(x, y, Torch, t_decorTiles, m_width);
					}
					//if so place one  picture for decor type 2
					else if (decorType == 2) {
						if (picNum == 0)
						{
							picNum++;
							setTile(x, y, Picture, t_decorTiles, m_width);
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
void DunDecor::createTrapsInRooms(std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms, std::vector<RoomVals>& t_halls)
{
	int offSet = 1;
	if (t_rooms.empty() == false)
	{
		//loop through all rooms
		for (int i = 0; i < t_rooms.size(); i++)
		{
			//set teh max amout of traps allowed in a room and create loop till there set
			int maxTraps = 2;
			for (int trap = 0; trap < maxTraps; trap++)
			{
				//get a random x y in each room
				int x = randomInt(t_rooms[i].x, t_rooms[i].x + t_rooms[i].width - offSet);
				int y = randomInt(t_rooms[i].y, t_rooms[i].y + t_rooms[i].height - offSet);
				//check if the tile at x,y is a floor tile and set a spike if so
				if (getTile(x, y, t_decorTiles, m_width, m_height) == FloorTile
					|| getTile(x, y, t_decorTiles, m_width, m_height) == StoneFloorTile)
				{
					if (checkForDoors(x, y, t_decorTiles, m_width, m_height) == false) {
						setTile(x, y, SpikeTrap, t_decorTiles, m_width);
					}
				}
			}
		}
	}
	//call funtion to place traps in halls
	placeTrapsInHalls(t_decorTiles,t_halls);
}

////---------------------------------------------------------------------------
////Function to create decorations in the hall ways
////---------------------------------------------------------------------------
void DunDecor::placeDecorInHalls(std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_halls)
{
	if (t_halls.empty() == false)
	{
		//loop through halls
		for (int i = 0; i < t_halls.size(); i++)
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
					int x = t_halls[i].x + 1;
					int y = randomInt(t_halls[i].y + 1, t_halls[i].y + t_halls[i].width - 1);
					//this is so place plant if it can
					if (getTile(x, y, t_decorTiles, m_width, m_height) == FloorTile
						&& checkForDoors(x, y, t_decorTiles, m_width, m_height) == false)
					{
						setTile(x, y, Plant, t_decorTiles, m_width);
					}
				}
				else if (decorType == 1)
				{
					//Create x ,y pos for new tile at left side of room random along y 
					int offSets = 1;
					int x = t_halls[i].x + offSets;
					int y = randomInt(t_halls[i].y + offSets, t_halls[i].y + t_halls[i].width - offSets);
					//this is so place potion if it can
					if (getTile(x, y, t_decorTiles, m_width, m_height) == FloorTile
						&& checkForDoors(x, y, t_decorTiles, m_width, m_height) == false)
					{
						setTile(x, y, Potion, t_decorTiles, m_width);
					}

				}
				else if (decorType == 2)
				{
					//Create x ,y pos for new tile at right side of room random along y 
					int x = t_halls[i].x + t_halls[i].width - 1;
					int y = randomInt(t_halls[i].y, t_halls[i].y + t_halls[i].width - 1);
					//this is so place Money if it can
					if (getTile(x, y, t_decorTiles, m_width, m_height) == FloorTile
						&& checkForDoors(x, y, t_decorTiles, m_width, m_height) == false)
					{
						setTile(x, y, Money, t_decorTiles, m_width);
					}
				}
			}
			//delete the hall after its decorated
			t_halls.erase(t_halls.begin() + i);
		}

	}
	DEBUG_MSG("Issue placing tiles tile");
}
