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
	static const int roomChance = 60;
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
		corridor.height =3;

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
		corridor.width = 3;
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
			corridor.x = t_y - 1;
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
	
	std::cout << "Room Size " << m_rooms.size() << std::endl;
	if (m_rooms.empty() == false)
	{
		for (int i = 0; i < m_rooms.size(); i++)
		{
			int decorType = 0;
			int maxDecorInRoom = randomInt(3, 6);;
			for (int j = 0; j < maxDecorInRoom; j++)
			{
				int x = randomInt(m_rooms[i].x, m_rooms[i].x + m_rooms[i].width - 1);
				int y = randomInt(m_rooms[i].y, m_rooms[i].y + m_rooms[i].height - 1);

				if (getTile(x, y) == FloorTile && getTile(x + 1, y) != Door1
					&& getTile(x - 1, y) != Door1 && getTile(x, y + 1) != Door1
					&& getTile(x, y - 1) != Door1)
				{
					if (decorType == 0) {
						setTile(x, y, Chest);
					}
					else if (decorType == 1) {
						setTile(x, y, Skull);
					}
					else if (decorType == 2) {
						setTile(x, y, Potion);
					}
					else if (decorType == 3) {
						setTile(x, y, Plant);
					}
					else if (decorType == 4) {
						setTile(x, y, Plant);
					}
					decorType++;
					
				}
				
			}
			m_rooms.erase(m_rooms.begin() + i);
		}

	}
	DEBUG_MSG("Issue placing tiles tile");
}

void DungeonGen::playerStartPos()
{
	if (m_rooms.empty() == false)
	{
		for (int i = 0; i < m_rooms.size(); i++)
		{
			int x = randomInt(m_rooms[i].x + m_rooms[i].width / 2, m_rooms[i].x + m_rooms[i].width /2);
			int y = randomInt(m_rooms[i].y + m_rooms[i].height /2 , m_rooms[i].y + m_rooms[i].height / 2);

			if (getTile(x , y) == FloorTile && getTile(x + 1, y) != Door1
				&& getTile(x - 1, y) != Door1 && getTile(x, y + 1) != Door1
				&& getTile(x, y - 1) != Door1)
			{
				setTile(x, y, SpawnPoint);
			}
		}
	}
	DEBUG_MSG("Issue placing starting position tile");
}
void DungeonGen::playerChainDecorOnWalls()
{
	if (m_rooms.empty() == false)
	{
		for (int i = 0; i < m_rooms.size(); i++)
		{
			int x = randomInt(m_rooms[i].x +1, m_rooms[i].x + m_rooms[i].width -1);
			int y = randomInt(m_rooms[i].y, m_rooms[i].y);

			if (getTile(x, y) == Wall && 
				getTile(x , y) != Door1 && 
				getTile(x, y ) != Door2)
			{
				setTile(x, y, Chains);
			}
		}
	}
	DEBUG_MSG("Issue placing chain Decor tile");
}

