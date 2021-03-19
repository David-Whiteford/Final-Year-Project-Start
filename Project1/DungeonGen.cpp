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
	generateMap(100);
	FloorDecorTiles();
	createTrapsInRooms();
	playerStartPos();
	createFeastRoom();
	deleteRoom();
	//createLibraryRoom();
	//deleteRoom();
	createUniqueRooms();
	deleteRoom();
	placeDecorOnWalls();
	placeDecorInRoom();
	deleteRoom();
	placeDecorInHalls();
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
		//gets a random side of a random room or corridor
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
	if (getTile(t_x + x, t_y + y) != FloorTile && getTile(t_x + x, t_y + y) != CorridorTile)
	{
		return false;
	}
	if (randomInt(100) < roomChance)
	{
		if (makeRoom(t_x, t_y, t_direction,false))
		{
			if (t_direction == North || t_direction == South)
			{
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
	int roomSizeMin = 4;
	int roomSizeMax = 8;

	Tile room;
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
		if (t_direction != South || t_firstRoom)
		{
			m_exit.emplace_back(Tile{ room.x,room.y - 1,room.width,1 });
		}
		if (t_direction != North || t_firstRoom)
		{
			m_exit.emplace_back(Tile{ room.x,room.y + room.height ,room.width,1 });
		}
		if (t_direction != South || t_firstRoom)
		{
			m_exit.emplace_back(Tile{ room.x - 1, room.y,1,room.height });
		}
		if (t_direction != West || t_firstRoom)
		{
			m_exit.emplace_back(Tile{ room.x + room.width,room.y,1,room.height });
		}
		return true;
	}

	return false;
}

bool DungeonGen::makeCorridor(int t_x, int t_y, Direction t_direction)
{
	int corridorLengthMin = 3;
	int corridorLengthMax = 6;

	Tile corridor;
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
			m_exit.emplace_back(Tile{ corridor.x,corridor.y - 1,corridor.width,1 });
		}
		if (t_direction != North && corridor.width != 1)
		{
			m_exit.emplace_back(Tile{ corridor.x,corridor.y + corridor.height,corridor.width,1 });
		}
		if (t_direction != East && corridor.height != 1)
		{
			m_exit.emplace_back(Tile{ corridor.x - 1, corridor.y, 1, corridor.height });
		}
		if (t_direction != West && corridor.height != 1)
		{
			m_exit.emplace_back(Tile{ corridor.x + corridor.width, corridor.y, 1, corridor.height });
		}
		return true;
	}

	return false;
}

bool DungeonGen::placeTile(Tile& t_Tile, char t_tile)
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
////function to loop through and delete rooms with decorations
////---------------------------------------------------------------------------
void DungeonGen::deleteRoom()
{
	for (int i = 0; i < m_rooms.size(); i++)
	{
		if (m_rooms[i].decorInRoom >= 1)
		{
			m_rooms.erase(m_rooms.begin() + i);
		}
	}
}
////---------------------------------------------------------------------------
////Functions to get randon x and a y of the walls top and bottom
////---------------------------------------------------------------------------
bool DungeonGen::CheckXAndYPos(int x, int y)
{
	return true;
}
sf::Vector2i DungeonGen::GenXAndYAtTopWall(int t_i, std::vector<Tile>& t_tileVec)
{
	int x = randomInt(t_tileVec[t_i].x, t_tileVec[t_i].x + t_tileVec[t_i].width - 1);
	int y = t_tileVec[t_i].y;
	return sf::Vector2i(x, y);
}
sf::Vector2i DungeonGen::GenXAndYAtBottomWall(int t_i, std::vector<Tile>& t_tileVec)
{
	int x = randomInt(t_tileVec[t_i].x, t_tileVec[t_i].x + t_tileVec[t_i].width - 1);
	int y = t_tileVec[t_i].y + t_tileVec[t_i].height - 1;
	return sf::Vector2i(x, y);
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
						if (getDecorTile(x + 1, y) != Door1 && getDecorTile(x - 1, y) != Door1
							&& getDecorTile(x, y + 1) != Door2 && getDecorTile(x, y - 1) != Door2
							&& getDecorTile(x, y - 1) != SpawnPoint)
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

					if (getDecorTile(x, y) == FloorTile || getDecorTile(x, y) == StoneFloorTile)

					{
						if (getDecorTile(x + 1, y) != Door1 && getDecorTile(x - 1, y) != Door1
							&& getDecorTile(x, y + 1) != Door2 && getDecorTile(x, y - 1) != Door2
							&& getDecorTile(x, y - 1) != SpawnPoint)
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
						if (getDecorTile(x + 1, y) != Door1 && getDecorTile(x - 1, y) != Door1
							&& getDecorTile(x, y + 1) != Door2 && getDecorTile(x, y - 1) != Door2
							&& getDecorTile(x, y - 1) != SpawnPoint)
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

					if (getDecorTile(x, y) == FloorTile || getDecorTile(x, y) == StoneFloorTile)
					
					{
						if (getDecorTile(x + 1, y) != Door1 && getDecorTile(x - 1, y) != Door1
							&& getDecorTile(x, y + 1) != Door2 && getDecorTile(x, y - 1) != Door2)
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
						if (getDecorTile(x + 1, y) != Door1 && getDecorTile(x - 1, y) != Door1
							&& getDecorTile(x, y + 1) != Door2 && getDecorTile(x, y - 1) != Door2)
						{
							placedDecor = true;
							setDecorTiles(x, y, Money);
							y += 1;
							m_rooms[i].decorInRoom++;
							if (getDecorTile(x, y) == FloorTile || getDecorTile(x, y) == StoneFloorTile)
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
		}

	}
	DEBUG_MSG("Issue placing tiles tile");
}
////---------------------------------------------------------------------------
////Function to create a room of jail cells
////---------------------------------------------------------------------------
void DungeonGen::createJailRoom()
{
	if (m_rooms.empty() == false)
	{
		for (int i = 0; i < m_rooms.size(); i++)
		{
			int offSet = 1;
			int x = m_rooms[i].x;
			int y = m_rooms[i].y - 1;
			int maxDecor = 2;
			for (int decorInRoom = 0; decorInRoom < maxDecor; decorInRoom++)
			{
				if (getDecorTile(x, y) == Wall)
				{
					if (getDecorTile(x, y + offSet) == FloorTile
						|| getDecorTile(x, y + offSet) == StoneFloorTile)
					{
						setDecorTiles(x, y, PrisonTile);
						setDecorTiles(x, y + offSet, UnusedTile);
						x += 2; 
						m_rooms[i].decorInRoom++;
					}
				}
			}	
		}
	}
}
////---------------------------------------------------------------------------
////Function to create a room of coffins
////---------------------------------------------------------------------------
void DungeonGen::createCoffinRoom()
{
	if (m_rooms.empty() == false)
	{
		for (int i = 0; i < m_rooms.size(); i++)
		{
			int maxDecorInRoom = 2;
			for (int decorNum = 0; decorNum < maxDecorInRoom; )
			{
				int offsets = 1;
				int x = m_rooms[i].x + offsets;
				int y = m_rooms[i].y + offsets;
				if (getDecorTile(x, y) == FloorTile || getDecorTile(x, y) == StoneFloorTile)
				{
					if (getDecorTile(x + offsets, y) == Door1 || getDecorTile(x - offsets, y) == Door1)
					{
						y++;
					}
					else if (getDecorTile(x, y + offsets) == Door2 || getDecorTile(x, y - offsets) == Door2 && getDecorTile(x, y - offsets) == SpawnPoint)
					{
						x++;
					}
					setDecorTiles(x, y, CoffinTile);
					setDecorTiles(x, y + offsets, UnusedTile);
					m_rooms[i].decorInRoom++;

				}
				decorNum++;
			}
		}
	}

}
////---------------------------------------------------------------------------
////Function to create a feast room with a table and chairs
////---------------------------------------------------------------------------
void DungeonGen::createFeastRoom()
{
	int roomIndex = 0;
	int offsetFromCentre = 1;
	int maxRoomSizeRequired = 7;
	bool roomFound = false;
	if (m_rooms.empty() == false)
	{
		for (int i = 0; i < m_rooms.size(); i++)
		{
			//find a room remaining of a certain size
			if (m_rooms[i].width >= maxRoomSizeRequired && m_rooms[i].height >= maxRoomSizeRequired)
			{
				roomIndex = i;
				roomFound = true;
			}
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
				std::cout << "Table Placed " << std::endl;

			}
			x = m_rooms[roomIndex].x + 1;
			y = m_rooms[roomIndex].y + 1;
			// add another decoration
			if (getDecorTile(x, y) == StoneFloorTile
				|| getDecorTile(x, y) == FloorTile)
			{
				m_rooms[roomIndex].decorInRoom++;
				setDecorTiles(x, y, Plant);
			}
		}
	}
	DEBUG_MSG("Issue placing starting position tile");
}
void DungeonGen::createLibraryRoom()
{
	int heightOffset = 3;
	int roomIndex = 0;
	int offSet = 1;
	int maxRoomWidth = 7;
	int maxRoomHeight = 4;
	bool roomFound = false;
	if (m_rooms.empty() == false)
	{
		for (int i = 0; i < m_rooms.size(); i++)
		{
			//find a room remaining of a certain size
			if (m_rooms[i].width >= maxRoomWidth && m_rooms[i].height >= maxRoomHeight)
			{
				roomIndex = i;
				roomFound = true;
			}
		}
	}
	if (roomFound)
	{
		//get the middle of the room if one found 
		int x = m_rooms[roomIndex].x + offSet;
		int y = m_rooms[roomIndex].y + offSet;
		int maxXVal = 2;
		for (int i = 0; i < maxXVal; i++)
		{
			if (getDecorTile(x + i, y - offSet) == Door2)
			{
				y += offSet;
				setDecorTiles(x, y, BookShelve);
				y += heightOffset;
				setDecorTiles(x, y, BookShelve);
				//std::cout << "Books Placed" << std::endl;
			}
			else
			{
				setDecorTiles(x, y, BookShelve);
				y += heightOffset;
				setDecorTiles(x, y, BookShelve);
				//std::cout << "Books Placed" << std::endl;
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
		for (int i = 0; i < m_rooms.size(); i++)
		{
			int maxTraps = 2;
			for (int trap = 0; trap < maxTraps; trap++)
			{
				int x = randomInt(m_rooms[i].x, m_rooms[i].x + m_rooms[i].width);
				int y = randomInt(m_rooms[i].y, m_rooms[i].y + m_rooms[i].height);

				if (getDecorTile(x, y) == FloorTile || getDecorTile(x, y) == StoneFloorTile)
				{
					setDecorTiles(x, y, SpikeTrap);
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
			int maxDecor = 3;
			int maxDecorInRoom = randomInt(mixDcor, maxDecor);
			for (int decorType = 0; decorType < maxDecorInRoom; decorType++)
			{
	
				if (decorType == 0)
				{
					int x = m_halls[i].x + 1;
					int y = randomInt(m_halls[i].y +1, m_halls[i].y + m_halls[i].width - 1);

					if (getDecorTile(x, y) == FloorTile
						&& getDecorTile(x + 1, y) != Door1 && getDecorTile(x - 1, y) != Door1
						&& getDecorTile(x, y + 1) != Door2 && getDecorTile(x, y - 1) != Door2)
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
						&& getDecorTile(x + offSets, y) != Door1 && getDecorTile(x - offSets, y) != Door1
						&& getDecorTile(x, y + offSets) != Door2 && getDecorTile(x, y - offSets) != Door2)
					{
						setDecorTiles(x, y, Potion);
					}

				}
				else if (decorType == 2)
				{
					int x = m_halls[i].x + m_halls[i].width - 1;
					int y = randomInt(m_halls[i].y, m_halls[i].y + m_halls[i].width - 1);

					if (getDecorTile(x, y) == FloorTile
						&& getDecorTile(x + 1, y) != Door1 && getDecorTile(x - 1, y) != Door1
						&& getDecorTile(x, y + 1) != Door2 && getDecorTile(x, y - 1) != Door2)
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