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
	createTrapsInRooms();
	FloorDecorTiles();
	playerStartPos();
	createFeastRoom();
	placeDecorOnWalls();
	placeDecorInRoom();
	createUniqueRooms();
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
					if (getTile(x, y) == FloorTile || getTile(x, y) == StoneFloorTile)

					{
						if (getTile(x + 1, y) != Door1 && getTile(x - 1, y) != Door1
							&& getTile(x, y + 1) != Door2 && getTile(x, y - 1) != Door2)
						{
							placedDecor = true;
							setTile(x, y, Chest);
							m_rooms[i].decorInRoom++;
						}
					}
					
				}
				else if (decorType == 1)
				{
					int x = randomInt(m_rooms[i].x, m_rooms[i].x + m_rooms[i].width - 1);
					int y = randomInt(m_rooms[i].y, m_rooms[i].y + m_rooms[i].height - 1);

					if (getTile(x, y) == FloorTile || getTile(x, y) == StoneFloorTile)

					{
						if (getTile(x + 1, y) != Door1 && getTile(x - 1, y) != Door1
							&& getTile(x, y + 1) != Door2 && getTile(x, y - 1) != Door2)
						{
							placedDecor = true;
							setTile(x, y, Skull);
							m_rooms[i].decorInRoom++;
						}
					}
				}
				else if (decorType == 2)
				{
					sf::Vector2i pos = GenXAndYAtTopWall(i, m_rooms);
					int x = pos.x;
					int y = pos.y;

					if (getTile(x, y) == FloorTile || getTile(x, y) == StoneFloorTile)
					{
						if (getTile(x + 1, y) != Door1 && getTile(x - 1, y) != Door1
							&& getTile(x, y + 1) != Door2 && getTile(x, y - 1) != Door2)
						{
							placedDecor = true;
							setTile(x, y, Potion);
							y += 1;
							m_rooms[i].decorInRoom++;
							if (getTile(x, y) == FloorTile || getTile(x, y) == StoneFloorTile)
							{
								setTile(x, y, ChairF);
							}
						}
					}
				}
				else if (decorType == 3) 
				{
					sf::Vector2i pos = GenXAndYAtBottomWall(i, m_rooms);
					int x = pos.x;
					int y = pos.y;

					if (getTile(x, y) == FloorTile || getTile(x, y) == StoneFloorTile)
					
					{
						if (getTile(x + 1, y) != Door1 && getTile(x - 1, y) != Door1
							&& getTile(x, y + 1) != Door2 && getTile(x, y - 1) != Door2)
						{ 
							placedDecor = true;
							setTile(x, y, Plant);
							m_rooms[i].decorInRoom++;
						}
					}
				}
				else if (decorType == 4)
				{
					sf::Vector2i pos = GenXAndYAtBottomWall(i, m_rooms);
					int x = pos.x;
					int y = pos.y;

					if (getTile(x, y) == FloorTile || getTile(x, y) == StoneFloorTile)

					{
						if (getTile(x + 1, y) != Door1 && getTile(x - 1, y) != Door1
							&& getTile(x, y + 1) != Door2 && getTile(x, y - 1) != Door2)
						{
							placedDecor = true;
							setTile(x, y, Money);
							y += 1;
							m_rooms[i].decorInRoom++;
							if (getTile(x, y) == FloorTile || getTile(x, y) == StoneFloorTile)
							{
								setTile(x, y, ChairF);
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
				m_rooms.erase(m_rooms.begin() + i);
			}
			std::cout << "Room Size: " << m_rooms.size() << std::endl;
		}

	}
	DEBUG_MSG("Issue placing tiles tile");
}
void DungeonGen::createJailRoom()
{
	if (m_rooms.empty() == false)
	{
		for (int i = 0; i < m_rooms.size(); i++)
		{
			int x = m_rooms[i].x;
			int y = m_rooms[i].y - 1;
			int maxDecor = 2;
			for (int decorInRoom = 0; decorInRoom < maxDecor; decorInRoom++)
			{
				if (getTile(x, y) == Wall)
				{
					if (getTile(x, y + 1) == FloorTile
						|| getTile(x, y + 1) == StoneFloorTile)
					{
						setTile(x, y, PrisonTileOne);
						setTile(x, y + 1, PrisonTileTwo);
						x += 2; 
						m_rooms[i].decorInRoom++;
					}
				}
			}

			if (m_rooms[i].decorInRoom >= 1)
			{
				m_rooms.erase(m_rooms.begin() + i);
			}
			std::cout << "Room Size2: " << m_rooms.size() << std::endl;
		}
	}
}

void DungeonGen::createCoffinRoom()
{
	if (m_rooms.empty() == false)
	{
		for (int i = 0; i < m_rooms.size(); i++)
		{
			int maxDecorInRoom = 2;
			for (int decorType = 0; decorType < maxDecorInRoom; )
			{
				sf::Vector2i pos = GenXAndYAtTopWall(i, m_rooms);
				int x = pos.x;
				int y = pos.y;
				if (getTile(x, y) == FloorTile || getTile(x, y) == StoneFloorTile)
				{
					if (getTile(x + 1, y) == Door1 || getTile(x - 1, y) == Door1)	
					{
						y++;
					}
					else if (getTile(x, y + 1) == Door2 || getTile(x, y - 1) == Door2)
					{
						x++;
					}
					setTile(x, y, CoffinTileOne);
					setTile(x, y++, CoffinTileTwo);
					m_rooms[i].decorInRoom++;

				}
				decorType++;
			}
			if (m_rooms[i].decorInRoom >= 1)
			{
				m_rooms.erase(m_rooms.begin() + i);
			}
		}
	}

}

void DungeonGen::createFeastRoom()
{
	std::cout << "Room Size Left: " << m_rooms.size() << std::endl;
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
				std::cout << "Room Index: " << roomIndex << std::endl;
				roomFound = true;
			}
		}
		if (roomFound)
		{
			//get the middle of the room if one found 
			int x = m_rooms[roomIndex].x + m_rooms[roomIndex].width  / 2;
			int y = m_rooms[roomIndex].y + m_rooms[roomIndex].height / 2;
			x -= offsetFromCentre;
			y -= offsetFromCentre;
			//check the tile at the x and y val and set if its a floor 
			if (getTile(x, y) == StoneFloorTile
				|| getTile(x, y) == FloorTile)
			{
				m_rooms[roomIndex].decorInRoom++;
				setTile(x, y, Table);
				std::cout << "Table Placed " << std::endl;

			}
			x = m_rooms[roomIndex].x;
			y = m_rooms[roomIndex].y;
			// add another decoration
			if (getTile(x, y) == StoneFloorTile
				|| getTile(x, y) == FloorTile)
			{
				m_rooms[roomIndex].decorInRoom++;
				setTile(x, y, Plant);
			}
		}
	}
	//delete the room once its decorated 
	if (m_rooms[roomIndex].decorInRoom >= 1)
	{
		m_rooms.erase(m_rooms.begin() + roomIndex);
	}
	
	DEBUG_MSG("Issue placing starting position tile");
}

void DungeonGen::playerStartPos()
{
	if (m_rooms.empty() == false)
	{
		for (int i = 0; i < m_rooms.size(); i++)
		{
			int x = randomInt(m_rooms[i].x, m_rooms[i].x + m_rooms[i].width -1);
			int y = m_rooms[i].y - 1;

			if (getTile(x , y) == Wall)
			{
				setTile(x, y, SpawnPoint);
			}
		}
	}
	DEBUG_MSG("Issue placing starting position tile");
}

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
				int x = randomInt(m_rooms[i].x + 1, m_rooms[i].x + m_rooms[i].width - 1);
				int y = m_rooms[i].y -1;
				
				if (getTile(x, y) == Wall &&
					getTile(x, y) != Door1 &&
					getTile(x, y) != Door2)
				{
					
					if (decorType == 0) {
						setTile(x, y, Chains);
					}
					else if (decorType == 1) {
						setTile(x, y, Torch);
					}
					else if (decorType == 2) {
						if (picNum == 0)
						{
							picNum++;
							setTile(x, y, Picture);
						}
					}
					
				}
				decorType++;
			}
		}
	}
	DEBUG_MSG("Issue placing chain Decor tile");
}

bool DungeonGen::CheckXAndYPos(int x, int y)
{
	return true;
}
sf::Vector2i DungeonGen::GenXAndYAtTopWall(int t_i , std::vector<Tile> & t_tileVec)
{
	int x = randomInt(t_tileVec[t_i].x, t_tileVec[t_i].x + t_tileVec[t_i].width - 1);
	int y = t_tileVec[t_i].y;
	return sf::Vector2i(x, y);
}
sf::Vector2i DungeonGen::GenXAndYAtBottomWall(int t_i, std::vector<Tile>& t_tileVec)
{
	int x = randomInt(t_tileVec[t_i].x, t_tileVec[t_i].x + t_tileVec[t_i].width - 1);
	int y = t_tileVec[t_i].y + t_tileVec[t_i].height -1;
	return sf::Vector2i(x, y);
}
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

				if (getTile(x, y) == FloorTile || getTile(x, y) == StoneFloorTile)
				{
					setTile(x, y, SpikeTrap);
				}
			}
		}
	}
}
void DungeonGen::FloorDecorTiles()
{
	if (m_rooms.empty() == false)
	{
		for (int i = 0; i < m_rooms.size(); i++)
		{
			int xStart = m_rooms[i].x ;
			int xEnd = m_rooms[i].x + m_rooms[i].width;

			int yStart = m_rooms[i].y +1;
			int yBottom = m_rooms[i].y + m_rooms[i].height -2;

			for (int x = xStart;x < xEnd ; x++)
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
			xEnd = m_rooms[i].x + m_rooms[i].width -1;
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
	/*if (m_halls.empty() == false)
	{
		for (int i = 0; i < m_halls.size(); i++)
		{
			int xStart = m_halls[i].x;
			int xEnd = m_halls[i].x + m_halls[i].width;
			int y = m_halls[i].y + m_halls[i].height / 2;
			for (int x = xStart; x < xEnd; x++)
			{
				if (getTile(x, y) == FloorTile)
				{
					setTile(x, y, StoneFloorTile);
				}
			}

		}
	}*/



}


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

					if (getTile(x, y) == FloorTile
						&& getTile(x + 1, y) != Door1 && getTile(x - 1, y) != Door1
						&& getTile(x, y + 1) != Door2 && getTile(x, y - 1) != Door2)
					{
						setTile(x, y, Chest);
					}
				}
				else if (decorType == 1)
				{
					int x = m_halls[i].x + 1;
					int y = randomInt(m_halls[i].y +1, m_halls[i].y + m_halls[i].width - 1);

					if (getTile(x, y) == FloorTile
						&& getTile(x + 1, y) != Door1 && getTile(x - 1, y) != Door1
						&& getTile(x, y + 1) != Door2 && getTile(x, y - 1) != Door2)
					{
						setTile(x, y, Potion);
					}

				}
				else if (decorType == 2)
				{

					/*int x = m_halls[i].x + m_halls[i].width - 1;
					int y = randomInt(m_halls[i].y, m_halls[i].y + m_halls[i].width - 1);

					if (getTile(x, y) == FloorTile
						&& getTile(x + 1, y) != Door1 && getTile(x - 1, y) != Door1
						&& getTile(x, y + 1) != Door2 && getTile(x, y - 1) != Door2)
					{
						setTile(x, y, Plant);
					}*/
				}
				else if (decorType == 3)
				{
					int x = m_halls[i].x + m_halls[i].width - 1;
					int y = randomInt(m_halls[i].y, m_halls[i].y + m_halls[i].width - 1);

					if (getTile(x, y) == FloorTile
						&& getTile(x + 1, y) != Door1 && getTile(x - 1, y) != Door1
						&& getTile(x, y + 1) != Door2 && getTile(x, y - 1) != Door2)
					{
						setTile(x, y, Money);
					}
				}
			}
			m_halls.erase(m_halls.begin() + i);
		}

	}
	DEBUG_MSG("Issue placing tiles tile");
}