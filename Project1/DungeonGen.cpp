#include "DungeonGen.h"

void DungeonGen::generateMap(int t_maxFeatures)
{
	if (!makeRoom(m_width / 2, m_height / 2,
		static_cast<Direction>(randomInt(4)), true))
	{
		std::cout << "Cant place room" << std::endl;
		return;
	}
	for (int i = 1; i < t_maxFeatures; i++)
	{
		if (!createFeature())
		{
			std::cout << "Cant place more features" << i << std::endl;
			break;
		}
	}
}

void DungeonGen::createRoomFeatures(Tilemap*& t_tilemap)
{
	int numberOfRooms = 2;
	generateMap(100);
	FloorDecorTiles();
	m_decorTiles = m_tiles;
	for (int i = 0; i < numberOfRooms; i++)
	{
		worsipRoomGroundTiles();
		worshipRoom();
		statueRoomTiles();
		statueRoom();
	}
	bossRoomGroundTiles();
	bossRoom();
	createUniqueRooms();
	createTrapsInRooms();
	playerStartPos();
	placeDecorOnWalls();
	placeDecorInRoom();
	placeDecorInHalls();
	std::cout << "Empty Rooms Left " << m_rooms.size() << std::endl;
	placeDecorInRoom();
	bedRoom();
	print();
	Set2DVec(t_tilemap);
}

bool DungeonGen::createFeature()
{
	for (int i = 0; i < 1000; i++)
	{
		if (m_exit.empty())
		{
			break;
		}
		
		int r = randomInt(m_exit.size());
		int x = randomInt(m_exit[r].x, m_exit[r].x + m_exit[r].width - 1);
		int y = randomInt(m_exit[r].y, m_exit[r].y + m_exit[r].height - 1);

		for (int j = 0; j < DirectionNum; j++)
		{
			if (createFeat(x, y, static_cast<Direction>(j)))
			{
				m_exit.erase(m_exit.begin() + r);
				return true;
			}
		}
	}

	return false;
}

bool DungeonGen::createFeat(int t_x, int t_y, Direction t_direction)
{
	static const int roomChance = 50;
	int x = 0;
	int y = 0;

	if (t_direction == North)
	{
		y = 1;
	}
	else if (t_direction == South)
	{
		y = -1;
	}
	else if (t_direction == West)
	{
		x = 1;
	}
	else if (t_direction == East)
	{
		x = -1;
	}
	if (getTile(t_x + x, t_y + y) != FloorTile 
		&& getTile(t_x + x, t_y + y) != CorridorTile)
	{
		return false;
	}
	if (randomInt(100) < roomChance)
	{
		if (makeRoom(t_x, t_y, t_direction,false))
		{
			if (t_direction == North || t_direction == South){
				setTile(t_x, t_y, Door2);
				return true;
			}
			else 
			{
				setTile(t_x, t_y, Door1);
				return true;
			}

		}
	}
	else
	{
		if (makeCorridor(t_x, t_y, t_direction))
		{
			if (getTile(t_x + x, t_y + y) == FloorTile)
			{
				if (t_direction == North || t_direction == South)
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
				setTile(t_x, t_y, CorridorTile);
			}
			return true;
		}
	}
	return false;
}

bool DungeonGen::makeRoom(int t_x, int t_y, Direction t_direction, bool t_firstRoom)
{
	int roomSizeMin = 5;
	int roomSizeMax = 9;

	RoomVals room;
	room.width = randomInt(roomSizeMin, roomSizeMax);
	room.height = randomInt(roomSizeMin, roomSizeMax);

	if (t_direction == North){
		room.x = t_x - room.width / 2;
		room.y = t_y - room.height;
	}
	else if (t_direction == South){
		room.x = t_x - room.width / 2;
		room.y = t_y + 1;
	}
	else if (t_direction == West){
		room.x = t_x - room.width;
		room.y = t_y - room.height / 2;
	}
	else if (t_direction == East){
		room.x = t_x + 1;
		room.y = t_y - room.height / 2;
	}
	if (placeTile(room , FloorTile))
	{
		m_rooms.emplace_back(room);
		//place exit at north Side
		if (t_direction != South || t_firstRoom)
		{
			m_exit.emplace_back(RoomVals{ room.x,room.y - 1,room.width,1 });
		}
		//place exit at south Side
		if (t_direction != North || t_firstRoom)
		{
			m_exit.emplace_back(RoomVals{ room.x,room.y + room.height ,room.width,1 });
		}
		//place exit at west Side
		if (t_direction != East || t_firstRoom)
		{
			m_exit.emplace_back(RoomVals{ room.x - 1, room.y,1,room.height });
		}
		//place exit at east Side
		if (t_direction != West || t_firstRoom)
		{
			m_exit.emplace_back(RoomVals{ room.x + room.width,room.y,1,room.height });
		}
		return true;
	}

	return false;
}

bool DungeonGen::makeCorridor(int t_x, int t_y, Direction t_direction)
{
	int corridorLengthMin = 5;
	int corridorLengthMax = 10;

	RoomVals corridor;
	corridor.x = t_x;
	corridor.y = t_y;

	if (randomBool())
	{
		corridor.width = randomInt(corridorLengthMin, corridorLengthMax);
		corridor.height =2;

		if (t_direction == North)
		{
			corridor.y = t_y - 1;

			if (randomBool())
			{
				corridor.x = t_x - corridor.width + 1;
			}
		}
		else if (t_direction == South)
		{
			corridor.y = t_y + 1;

			if (randomBool())
			{
				corridor.x = t_x - corridor.width + 1;
			}
		}
		else if (t_direction == West)
		{
			corridor.x = t_x - corridor.width;
		}
		else if (t_direction == East)
		{
			corridor.x = t_x + 1;
		}
	}
	else
	{
		corridor.width = 2;
		corridor.height = randomInt(corridorLengthMin, corridorLengthMax);

		if (t_direction == North)
		{
			corridor.y = t_y - corridor.height;
		}
		else if (t_direction == South)
		{
			corridor.y = t_y + 1;
		}
		else if (t_direction == West)
		{
			corridor.x = t_x - 1;
			if (randomBool())
			{
				corridor.y = t_y - corridor.height + 1;
			}
		}
		else if (t_direction == East)
		{
			corridor.x = t_x + 1;
			if (randomBool())
			{
				corridor.y = t_y - corridor.height + 1;
			}
		}
	}

	if (placeTile(corridor, CorridorTile))
	{
		m_halls.emplace_back(corridor);
		if (t_direction != South && corridor.width != 1)
		{
			m_exit.emplace_back(RoomVals{ corridor.x,corridor.y - 1,corridor.width,1 });
		}
		if (t_direction != North && corridor.width != 1)
		{
			m_exit.emplace_back(RoomVals{ corridor.x,corridor.y + corridor.height,corridor.width,1 });
		}
		if (t_direction != East && corridor.height != 1)
		{
			m_exit.emplace_back(RoomVals{ corridor.x - 1, corridor.y, 1, corridor.height });
		}
		if (t_direction != West && corridor.height != 1)
		{
			m_exit.emplace_back(RoomVals{ corridor.x + corridor.width, corridor.y, 1, corridor.height });
		}
		return true;
	}

	return false;
}
bool DungeonGen::placeTile(RoomVals& t_Tile, char t_tile)
{
	if (t_Tile.x < 1 || t_Tile.y < 1 || t_Tile.x + t_Tile.width > m_width - 1
		|| t_Tile.y + t_Tile.height>m_height - 1)
	{
		return false;
	}
	for (int y = t_Tile.y; y < t_Tile.y + t_Tile.height; y++)
	{
		for (int x = t_Tile.x; x < t_Tile.x + t_Tile.width; x++)
		{
			if (getTile(x, y) != UnusedTile)
			{
				return false;
			}
		}
	}
	for (int y = t_Tile.y - 1; y < t_Tile.y + t_Tile.height + 1; y++)
	{
		for (int x = t_Tile.x - 1; x < t_Tile.x + t_Tile.width + 1; x++)
		{
			if (x == t_Tile.x - 1 || y == t_Tile.y - 1
				|| x == t_Tile.x + t_Tile.width
				|| y == t_Tile.y + t_Tile.height)
			{
				setTile(x, y, Wall);
			}
			else
			{
				setTile(x, y, t_tile);
			}
		}
	}
	return true;
}

////---------------------------------------------------------------------------
////Place new floor tiles on the ground and store in background tiles vector
////---------------------------------------------------------------------------
void DungeonGen::FloorDecorTiles()
{
	if (m_rooms.empty() == false)
	{
		for (int i = 0; i < m_rooms.size(); i++)
		{
			int xStart = m_rooms[i].x;
			int xEnd = m_rooms[i].x + m_rooms[i].width;

			int yStart = m_rooms[i].y + 1;
			int yBottom = m_rooms[i].y + m_rooms[i].height - 2;

			for (int x = xStart; x < xEnd; x++)
			{
				if (x != xEnd)
				{
					if (getTile(x, yStart) == FloorTile || getTile(x, yBottom) == FloorTile)
					{
						setTile(x, yStart, StoneFloorTile);
						setTile(x, yBottom, StoneFloorTile);
					}
				}
			}
			xEnd = m_rooms[i].x + m_rooms[i].width - 1;
			for (int y = yStart; y < yBottom; y++)
			{
				if (y != yBottom)
				{
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
bool DungeonGen::CheckXAndYPos(int x, int y)
{
	return true;
}
sf::Vector2i DungeonGen::GenXAndYAtTopWall(int t_i, std::vector<RoomVals>& t_tileVec)
{
	int x = randomInt(t_tileVec[t_i].x, t_tileVec[t_i].x + t_tileVec[t_i].width - 1);
	int y = t_tileVec[t_i].y;
	return sf::Vector2i(x, y);
}
sf::Vector2i DungeonGen::GenXAndYAtBottomWall(int t_i, std::vector<RoomVals>& t_tileVec)
{
	int x = randomInt(t_tileVec[t_i].x, t_tileVec[t_i].x + t_tileVec[t_i].width - 1);
	int y = t_tileVec[t_i].y + t_tileVec[t_i].height - 1;
	return sf::Vector2i(x, y);
}
void DungeonGen::setSpawnToWall(int t_roomIndex)
{
	int y = m_rooms[t_roomIndex].y - 1;
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
	if (getTile(t_x - offSet,t_y) == Door1 || getTile(t_x + offSet, t_y) == Door1
		|| getTile(t_x, t_y - offSet) == Door2 || getTile(t_x, t_y + offSet) == Door2
		|| getDecorTile(t_x ,t_y - offSet) == SpawnPoint)
	{
		doorDetected = true;
	}
	return doorDetected;
}
////---------------------------------------------------------------------------
////Function to create rooms with different tiles from the standard roooms 
//// takes a tile type to replace all others
////---------------------------------------------------------------------------
void DungeonGen::setUniqueGroundTiles(char t_newTile, int t_maxRoomSizeWidth, int t_maxRoomSizeHeight, int t_index)
{
	for (int x = m_rooms[t_index].x; x < m_rooms[t_index].x
		+ m_rooms[t_index].width; x++)
	{
		for (int y = m_rooms[t_index].y; y < m_rooms[t_index].y
			+ m_rooms[t_index].height; y++)
		{
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
////Function to create a bedroom
////---------------------------------------------------------------------------
void DungeonGen::bedRoom()
{
	if (m_rooms.empty() == false)
	{
		int offSet = 1;
		int randRoom = randomInt(m_rooms.size());
		int x = m_rooms[randRoom].x;
		int y = m_rooms[randRoom].y - offSet;
		int bedNum = 2;
		for (int i = 0; i < bedNum; i++)
		{
			placeBeds(x, y);
			int x = m_rooms[randRoom].x + m_rooms[randRoom].width - offSet;
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
	if (getDecorTile(x, y) == Wall)
	{
		y++;
		if (getDecorTile(x, y) == FloorTile
			|| getDecorTile(x, y) == StoneFloorTile)
		{
			//check all tiles that the bed will occupy havnt got a door or exit next to them
			if (checkForDoors(x, y) == false)
			{
				int newY = y;
				newY++;
				if (checkForDoors(x, newY) == false)
				{
					newY++;
					if (checkForDoors(x, newY) == false)
					{
						//place the bed and make all tile under it unused
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
						newY++;
						if (checkForDoors(x, newY) == false)
						{
							setDecorTiles(x, y, NightStand);
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
		for (int i = 0; i < m_rooms.size(); i++)
		{
			bool placedDecor = false;
			int minDcor = 2;
			int maxDecor = 4;
			int maxDecorInRoom = randomInt(minDcor, maxDecor);
			for (int decorType = 0; decorType < maxDecorInRoom; )
			{
				if (decorType == 0) 
				{
					sf::Vector2i pos = GenXAndYAtTopWall(i,m_rooms);
					int x = pos.x;
					int y = pos.y;
					if (getDecorTile(x, y) == FloorTile || getDecorTile(x, y) == StoneFloorTile)
					{
						if (checkForDoors(x,y) == false)
						{
							placedDecor = true;
							setDecorTiles(x, y, Chest);
							m_rooms[i].decorInRoom++;
						}
					}
					
				}
				else if (decorType == 1)
				{
					int x = randomInt(m_rooms[i].x, m_rooms[i].x + m_rooms[i].width - 1);
					int y = randomInt(m_rooms[i].y, m_rooms[i].y + m_rooms[i].height - 1);

					if (getDecorTile(x, y) == FloorTile 
						|| getDecorTile(x, y) == StoneFloorTile)
					{
						if (checkForDoors(x, y) == false)
						{
							placedDecor = true;
							setDecorTiles(x, y, Skull);
							m_rooms[i].decorInRoom++;
						}
					}
				}
				else if (decorType == 2)
				{
					sf::Vector2i pos = GenXAndYAtTopWall(i, m_rooms);
					int x = pos.x;
					int y = pos.y;

					if (getDecorTile(x, y) == FloorTile || getDecorTile(x, y) == StoneFloorTile)
					{
						if (checkForDoors(x, y) == false)
						{
							placedDecor = true;
							setDecorTiles(x, y, Potion);
							y += 1;
							m_rooms[i].decorInRoom++;
							if (getDecorTile(x, y) == FloorTile || getDecorTile(x, y) == StoneFloorTile)
							{
								setDecorTiles(x, y, ChairF);
							}
						}
					}
				}
				else if (decorType == 3) 
				{
					sf::Vector2i pos = GenXAndYAtBottomWall(i, m_rooms);
					int x = pos.x;
					int y = pos.y;

					if (getDecorTile(x, y) == FloorTile 
						|| getDecorTile(x, y) == StoneFloorTile)
					
					{
						if (checkForDoors(x, y) == false)
						{ 
							placedDecor = true;
							setDecorTiles(x, y, Plant);
							m_rooms[i].decorInRoom++;
						}
					}
				}
				else if (decorType == 4)
				{
					sf::Vector2i pos = GenXAndYAtBottomWall(i, m_rooms);
					int x = pos.x;
					int y = pos.y;

					if (getDecorTile(x, y) == FloorTile || getDecorTile(x, y) == StoneFloorTile)

					{
						if (checkForDoors(x, y) == false)
						{
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
				}
				if (placedDecor)
				{
					decorType++;
				}

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
	int offSet = 1;
	int noIndex = 1000;
	int roomIndex = 0;
	int maxRoomSizeWidth = 5;
	int maxRoomSizeHeight = 4;
	bool roomFound = false;
	bool roomCompleted = false;
	if (m_rooms.empty() == false)
	{
		roomIndex = GetRoom(maxRoomSizeWidth, maxRoomSizeHeight);
		if (roomIndex != noIndex)
		{
			roomFound = true;
		}
	}
	if(roomFound)
	{
		setSpawnToWall(roomIndex);
		roomCompleted = createJailCells(roomIndex);
		int x = randomInt(m_rooms[roomIndex].x + offSet, m_rooms[roomIndex].x + m_rooms[roomIndex].width - 2);
		int y = randomInt(m_rooms[roomIndex].y + offSet, m_rooms[roomIndex].y + m_rooms[roomIndex].height - 2);

	    int maxSkullNum = 3;
		for(int skullNum = 0; skullNum < maxSkullNum; skullNum++)
		{
			if (getDecorTile(x, y) == StoneFloorTile || getDecorTile(x, y) == FloorTile)
			{
				setDecorTiles(x, y, Skull);
			}
		}
		if (roomCompleted)
		{
			m_numJailRoomsPlaced++;
			deleteRoom(roomIndex);
		}
	}
}

bool DungeonGen::createJailCells(int t_roomIndex)
{
	bool roomCompleted = false;
	int offSet = 1;
	int x = m_rooms[t_roomIndex].x +1;
	int y = m_rooms[t_roomIndex].y - offSet;
	int maxWidth = m_rooms[t_roomIndex].x + m_rooms[t_roomIndex].width - offSet *2;

	for (int index = x; index <= maxWidth; index++)
	{
		if (getDecorTile(index, y) == Wall)
		{
			setDecorTiles(index, y, PrisonTile);
			m_rooms[t_roomIndex].decorInRoom++;
			roomCompleted = true;
		}
	}
	x = m_rooms[t_roomIndex].x + offSet;
	int endX = m_rooms[t_roomIndex].x + m_rooms[t_roomIndex].width - 2;
	y = m_rooms[t_roomIndex].y + m_rooms[t_roomIndex].height / 2;
	for (int index = x; index <= endX; index++)
	{
		setTile(index, y, Wall);
		setDecorTiles(index, y, PrisonTile);
		m_rooms[t_roomIndex].decorInRoom++;
		roomCompleted = true;
	}
	return roomCompleted;
}
////---------------------------------------------------------------------------
////Function to create a room of coffins
////---------------------------------------------------------------------------
void DungeonGen::createCoffinRoom()
{
	int roomIndex = 0;
	int offSet = 1;
	int maxRoomSizeWidth = 5;
	int maxRoomSizeHeight = 4;
	bool roomFound = false;
	int noIndexFoundVal = 1000;
	roomIndex = GetRoom(maxRoomSizeWidth, maxRoomSizeHeight);
	if (roomIndex != noIndexFoundVal)
	{
		roomFound = true;
	}
	if (roomFound)
	{
		int maxDecorInRoom = 5;
		int x = m_rooms[roomIndex].x + offSet;
		int y = m_rooms[roomIndex].y + offSet;
		int maxWidth = m_rooms[roomIndex].x + m_rooms[roomIndex].width - 2;

		for (int index = x; index <= maxWidth; index +=2)
		{
			if (getDecorTile(index, y) == FloorTile 
				|| getDecorTile(index, y) == StoneFloorTile)
			{
				
				setDecorTiles(index, y, CoffinTile);
				setDecorTiles(index, y + offSet, UnusedTile);
				setDecorTiles(index, y + offSet *2, FlameCauldron);
				m_rooms[roomIndex].decorInRoom++;
			}
		}
		deleteRoom(roomIndex);
		
	}

}



////---------------------------------------------------------------------------
////Function to create a boss room thats different From the others
////---------------------------------------------------------------------------
void DungeonGen::bossRoomGroundTiles()
{
	int maxRoomSizeWidth = 6;
	int maxRoomSizeHeight = 6;
	m_bossRoomIndex = GetRoom(maxRoomSizeWidth, maxRoomSizeHeight);
	setUniqueGroundTiles(DirtTile, maxRoomSizeWidth, maxRoomSizeHeight, m_bossRoomIndex);
}
void DungeonGen::worsipRoomGroundTiles()
{
	int maxRoomSizeWidth = 5;
	int maxRoomSizeHeight = 5;
	m_worshipRoomIndex = GetRoom(maxRoomSizeWidth, maxRoomSizeHeight);
	setUniqueGroundTiles(DarkTiles, maxRoomSizeWidth, maxRoomSizeHeight, m_worshipRoomIndex);
}
void DungeonGen::statueRoomTiles()
{
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
	int offSetFromCentre = 1;
	int x = m_rooms[m_bossRoomIndex].x + m_rooms[m_bossRoomIndex].width / 2 - offSetFromCentre;
	int y = m_rooms[m_bossRoomIndex].y + m_rooms[m_bossRoomIndex].height / 2 - offSetFromCentre;
	if (getDecorTile(x, y) == DirtTile)
	{
		setDecorTiles(x, y, HoleTile);
	}

	bossRoomSkull();
	bossRoomHealth();
	bossRoomWalls();
	deleteRoom(m_bossRoomIndex);
	
}
////---------------------------------------------------------------------------
////Adds skull decor toi the boss room
////---------------------------------------------------------------------------
void DungeonGen::bossRoomSkull()
{
	int offSet = 1;
	int moveBy = 3;
	int x = m_rooms[m_bossRoomIndex].x + offSet;
	int y = m_rooms[m_bossRoomIndex].y;
	if (getDecorTile(x, y - 1) != Door2)
	{
		if (getDecorTile(x, y) == DirtTile)
		{
			std::cout << "skeletom placed" << std::endl;
			setDecorTiles(x, y, Skull);
			x = m_rooms[m_bossRoomIndex].x + moveBy;
			y++;
			setDecorTiles(x, y, Skull);
		}
	}
	x = m_rooms[m_bossRoomIndex].x + m_rooms[m_bossRoomIndex].x - offSet;
	y = m_rooms[m_bossRoomIndex].y;
	if (checkForDoors(x, y) == false)
	{
		setDecorTiles(x, y, FlameCauldron);

	}
	x = m_rooms[m_bossRoomIndex].x + m_rooms[m_bossRoomIndex].x - offSet;
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
	int offSet = 1;
	int x = randomInt(m_rooms[m_bossRoomIndex].x + offSet, m_rooms[m_bossRoomIndex].x + m_rooms[m_bossRoomIndex].width - (offSet * 2));
	int y = m_rooms[m_bossRoomIndex].y + m_rooms[m_bossRoomIndex].height - offSet *2;
	if (getDecorTile(x, y) != DirtTile)
	{
		x++;
		setDecorTiles(x, y, HealthPos);
	}
	else
	{
		setDecorTiles(x, y, HealthPos);
	}
}
////---------------------------------------------------------------------------
////Adds decor to the walls of the boss room
////---------------------------------------------------------------------------
void DungeonGen::bossRoomWalls()
{
	int offSet = 1;
	int x = m_rooms[m_bossRoomIndex].x;
	int y = m_rooms[m_bossRoomIndex].y - offSet;
	if (getDecorTile(x, y) == Wall)
	{
		if (x < m_rooms[m_bossRoomIndex].x + m_rooms[m_bossRoomIndex].width - offSet)
		{
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
	int x = m_rooms[m_worshipRoomIndex].x + m_rooms[m_worshipRoomIndex].width / 2;
	int y = m_rooms[m_worshipRoomIndex].y + m_rooms[m_worshipRoomIndex].height / 2;
	if (getDecorTile(x, y) == DarkTiles)
	{
		setDecorTiles(x, y, Worship);
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
	int x = m_rooms[m_worshipRoomIndex].x + m_rooms[m_worshipRoomIndex].width - offSet;
	int y = m_rooms[m_worshipRoomIndex].y;
	int	maxChairs = 3;
	for (int i = 0; i < maxChairs; i++)
	{
		if (checkForDoors(x, y) == false)
		{
			setDecorTiles(x, y, ChairL);
			y++;
		}
	}
	x = m_rooms[m_worshipRoomIndex].x;
	y = m_rooms[m_worshipRoomIndex].y;
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
	int offSetFromCentre = 1;
	int x = m_rooms[m_statueRoomIndex].x + m_rooms[m_statueRoomIndex].width / 2 - offSetFromCentre;
	int y = m_rooms[m_statueRoomIndex].y + m_rooms[m_statueRoomIndex].height / 2 - offSetFromCentre;
	if (getDecorTile(x, y) == TilePattern)
	{
		setDecorTiles(x, y, Statue);
	}
	statueRoomDecor();
	deleteRoom(m_statueRoomIndex);
}
////---------------------------------------------------------------------------
////Function to add some decor to the statue room
////---------------------------------------------------------------------------
void DungeonGen::statueRoomDecor()
{
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
	int roomIndex = 0;
	int invalidIndex = 1000;
	int offsetFromCentre = 1;
	int maxRoomSizeRequired = 7;
	bool roomFound = false;
	if (m_rooms.empty() == false)
	{
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
			}
			int newY = 4;
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
	DEBUG_MSG("Issue placing starting position tile");
}
void DungeonGen::createLibraryRoom()
{
	int heightOffset = 3;
	int roomIndex = 0;
	int invalidIndex = 1000;
	int offSet = 1;
	int maxRoomWidth = 7;
	int maxRoomHeight = 4;
	bool roomFound = false;
	if (m_rooms.empty() == false)
	{
		roomIndex = GetRoom(maxRoomWidth, maxRoomHeight);
		if (roomIndex != invalidIndex)
		{
			roomFound = true;
		}
	}
	if (roomFound)
	{
		setSpawnToWall(roomIndex);
		
		placeBookShelfDecor(roomIndex);
		addChairsDecor(roomIndex);
		int xOffSet = 2;
		//get the bottom right of room and add a plant decor
		int x = m_rooms[roomIndex].x + m_rooms[roomIndex].width - xOffSet;
		int y = m_rooms[roomIndex].y + m_rooms[roomIndex].height - offSet;
		if (getDecorTile(x, y) == StoneFloorTile || getDecorTile(x, y) == FloorTile)
		{
			setDecorTiles(x, y, Plant);
			y = m_rooms[roomIndex].y;
			setDecorTiles(x, y, Plant);
		}
		deleteRoom(roomIndex);
	}
}
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
		int x2 = x;
		int y2 = y - 1;

		if (getDecorTile(x2 + i, y2) == Door2)
		{
			doorNum = x2 + i;
		}
	}
	if (doorNum > 0)
	{
		x = doorNum + offSet;
		setDecorTiles(x, y, BookShelve);
		setDecorTiles(x + 2, y, UnusedTile);
	}
	else if (doorNum == 0)
	{
		setDecorTiles(x, y, BookShelve);
		setDecorTiles(x + 2, y, UnusedTile);
	}
}
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
		std::cout << "Starting X : " << x << std::endl;
		if (checkForDoors(x, y) == false)
		{
			setDecorTiles(x, y, ChairF);
			x++;
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
		for (int i = 0; i < m_rooms.size(); i++)
		{
			int x = randomInt(m_rooms[i].x, m_rooms[i].x + m_rooms[i].width -1);
			int y = m_rooms[i].y - 1;

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
		for (int i = 0; i < m_rooms.size(); i++)
		{
			int picNum = 0;
			int maxDecorInRoom = randomInt(3, 5);
			for (int decorType = 0; decorType < maxDecorInRoom;)
			{
				int offSet = 1;
				int x = randomInt(m_rooms[i].x + offSet, m_rooms[i].x + m_rooms[i].width - offSet);
				int y = m_rooms[i].y - offSet;
				
				if (getDecorTile(x, y) == Wall &&
					getDecorTile(x, y) != Door1 &&
					getDecorTile(x, y) != Door2)
				{
					
					if (decorType == 0) {
						setDecorTiles(x, y, Chains);
					}
					else if (decorType == 1) {
						setDecorTiles(x, y, Torch);
					}
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
////Function to create traps in the rooms
////---------------------------------------------------------------------------
void DungeonGen::createTrapsInRooms()
{
	if (m_rooms.empty() == false)
	{
		int offSet = 1;
		for (int i = 0; i < m_rooms.size(); i++)
		{
			int maxTraps = 2;
			for (int trap = 0; trap < maxTraps; trap++)
			{
				int x = randomInt(m_rooms[i].x, m_rooms[i].x + m_rooms[i].width - offSet);
				int y = randomInt(m_rooms[i].y, m_rooms[i].y + m_rooms[i].height - offSet);

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
}

////---------------------------------------------------------------------------
////Function to create decorations in the hall ways
////---------------------------------------------------------------------------
void DungeonGen::placeDecorInHalls()
{
	if (m_halls.empty() == false)
	{
		for (int i = 0; i < m_halls.size(); i++)
		{
			int mixDcor = 1;
			int maxDecor = 4;
			int maxDecorInRoom = randomInt(mixDcor, maxDecor);
			for (int decorType = 0; decorType < maxDecorInRoom; decorType++)
			{
				if (decorType == 0)
				{
					int x = m_halls[i].x + 1;
					int y = randomInt(m_halls[i].y +1, m_halls[i].y + m_halls[i].width - 1);

					if (getDecorTile(x, y) == FloorTile
						&& checkForDoors(x,y)==false)
					{
						setDecorTiles(x, y, Plant);
					}
				}
				else if (decorType == 1)
				{
					int offSets = 1;
					int x = m_halls[i].x + offSets;
					int y = randomInt(m_halls[i].y + offSets, m_halls[i].y + m_halls[i].width - offSets);

					if (getDecorTile(x, y) == FloorTile
						&& checkForDoors(x, y) == false)
					{
						setDecorTiles(x, y, Potion);
					}

				}
				else if (decorType == 2)
				{
					int x = m_halls[i].x + m_halls[i].width - 1;
					int y = randomInt(m_halls[i].y, m_halls[i].y + m_halls[i].width - 1);

					if (getDecorTile(x, y) == FloorTile
						&& checkForDoors(x, y) == false)
					{
						setDecorTiles(x, y, Money);
					}
				}
			}
			m_halls.erase(m_halls.begin() + i);
		}

	}
	DEBUG_MSG("Issue placing tiles tile");
}