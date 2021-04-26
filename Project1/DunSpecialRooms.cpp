#include "DunSpecialRooms.h"

DunSpecialRooms::~DunSpecialRooms()
{
}
////---------------------------------------------------------------------------
////Function to decorate and change floor tiles of each room
////---------------------------------------------------------------------------
void DunSpecialRooms::createDunSpecialRooms(std::vector<char>& t_tiles, std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms,
	std::vector<RoomVals>& t_exits, std::vector<RoomVals>& t_halls)
{
	int numberOfRooms = 2;
	//create a vector to hold the background tiles and decor tiles
	t_decorTiles = t_tiles;
	//loop to create 2 of the below rooms
	for (int i = 0; i < numberOfRooms; i++)
	{
		worsipRoomGroundTiles(t_tiles,t_decorTiles,t_rooms);
		worshipRoom(t_decorTiles, t_rooms,t_tiles);
		statueRoomTiles(t_tiles, t_decorTiles, t_rooms);
		statueRoom(t_decorTiles, t_rooms, t_tiles);
	}
	//create a boss room ground tiles and the room itself
	bossRoomGroundTiles(t_tiles, t_decorTiles, t_rooms);
	bossRoom(t_tiles,t_decorTiles, t_rooms);


}

void DunSpecialRooms::setUniqueGroundTiles(char t_newTile, int t_maxRoomSizeWidth, int t_maxRoomSizeHeight, int t_index,
	std::vector<char>& t_tiles, std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms)
{
	//this loops through all ground tiles in nested loop
	for (int x = t_rooms[t_index].x; x < t_rooms[t_index].x
		+ t_rooms[t_index].width; x++)
	{
		for (int y = t_rooms[t_index].y; y < t_rooms[t_index].y
			+ t_rooms[t_index].height; y++)
		{
			//then replace all stone and normal floor tiles with whatever was passed in
			if (getTile(x, y, t_tiles, m_width, m_height) == StoneFloorTile
				|| getTile(x, y, t_tiles, m_width, m_height) == FloorTile)
			{
				setTile(x, y, t_newTile, t_tiles, m_width);
				setTile(x, y, t_newTile, t_decorTiles, m_width);
			}
		}
	}
}
////---------------------------------------------------------------------------
////Function to create a boss room thats different From the others
////---------------------------------------------------------------------------
void DunSpecialRooms::bossRoomGroundTiles(std::vector<char>& t_tiles, std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms)
{
	//from these width and height get a room index for boss room and change all the floor tiles to dirt
	int maxRoomSizeWidth = 6;
	int maxRoomSizeHeight = 6;
	m_bossRoomIndex = getRoom(maxRoomSizeWidth, maxRoomSizeHeight, t_rooms);
	setUniqueGroundTiles(DirtTile, maxRoomSizeWidth, maxRoomSizeHeight, m_bossRoomIndex, t_tiles, t_decorTiles, t_rooms);
}
////---------------------------------------------------------------------------
////Function to create a worship room thats different From the others
////---------------------------------------------------------------------------
void DunSpecialRooms::worsipRoomGroundTiles(std::vector<char>& t_tiles, std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms)
{
	//from these width and height get a room index for worship room and change all the floor tiles to dark
	int maxRoomSizeWidth = 5;
	int maxRoomSizeHeight = 5;
	m_worshipRoomIndex = getRoom(maxRoomSizeWidth, maxRoomSizeHeight, t_rooms);
	setUniqueGroundTiles(DarkTiles, maxRoomSizeWidth, maxRoomSizeHeight, m_worshipRoomIndex,t_tiles,t_decorTiles,t_rooms);
}
void DunSpecialRooms::statueRoomTiles(std::vector<char>& t_tiles, std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms)
{
	//from these width and height get a room index for statue room and change all the floor tiles to tiles
	int maxRoomSizeWidth = 6;
	int maxRoomSizeHeight = 6;
	m_statueRoomIndex = getRoom(maxRoomSizeWidth, maxRoomSizeHeight,t_rooms);
	setUniqueGroundTiles(TilePattern, maxRoomSizeWidth, maxRoomSizeHeight, m_statueRoomIndex,t_tiles, t_decorTiles, t_rooms);
}
////---------------------------------------------------------------------------
////Creates a room with a pit that tthe boss will appear from
////littered with fallen heroes that failed to defeat the boss
////---------------------------------------------------------------------------
void DunSpecialRooms::bossRoom(std::vector<char>& t_tiles, std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms)
{
	//get x,y near room centre
	int offSetFromCentre = 1;
	int x = t_rooms[m_bossRoomIndex].x + t_rooms[m_bossRoomIndex].width / 2 - offSetFromCentre;
	int y = t_rooms[m_bossRoomIndex].y + t_rooms[m_bossRoomIndex].height / 2 - offSetFromCentre;
	if (getTile(x, y, t_decorTiles, m_width, m_height) == DirtTile)
	{
		//place big hole of death
		setTile(x, y, HoleTile, t_decorTiles, m_width);
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
				if (getTile(xVal, yVal, t_decorTiles, m_width, m_height) == DirtTile)
				{
					setTile(xVal, yVal, UnusedTile, t_decorTiles, m_width);
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
	bossRoomSkull(t_decorTiles,t_rooms);
	bossRoomHealth(t_decorTiles, t_rooms);
	bossRoomWalls(t_decorTiles, t_rooms);
	//delete this room
	deleteRoom(m_bossRoomIndex, t_rooms);

}
////---------------------------------------------------------------------------
////Adds skull decor toi the boss room
////---------------------------------------------------------------------------
void DunSpecialRooms::bossRoomSkull(std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms)
{
	//values needed to place skull tile
	int offSet = 1;
	int moveBy = 3;
	int x = t_rooms[m_bossRoomIndex].x + offSet;
	int y = t_rooms[m_bossRoomIndex].y;
	if (getTile(x, y - 1, t_decorTiles, m_width, m_height) != Door2)
	{
		if (getTile(x, y, t_decorTiles, m_width, m_height) == DirtTile)
		{
			//place the skeleton if the tiles dirt
			setTile(x, y, Skull, t_decorTiles, m_width);
			//move the x and y by a certain amount and place another
			x = t_rooms[m_bossRoomIndex].x + moveBy;
			y++;
			setTile(x, y, Skull, t_decorTiles, m_width);
		}
	}
	//get a new x,y position of right corner of room and place flame cauldron decor 
	x = t_rooms[m_bossRoomIndex].x + t_rooms[m_bossRoomIndex].width - offSet;
	y = t_rooms[m_bossRoomIndex].y;
	if (checkForDoors(x, y, t_decorTiles, m_width, m_height) == false)
	{
		setTile(x, y, FlameCauldron, t_decorTiles, m_width);
	}
	//get a new x,y position of bottom right corner of room and place flame cauldron decor 
	x = t_rooms[m_bossRoomIndex].x + t_rooms[m_bossRoomIndex].width - offSet;
	y = t_rooms[m_bossRoomIndex].y + t_rooms[m_bossRoomIndex].height - offSet;
	if (checkForDoors(x, y, t_decorTiles, m_width, m_height) == false)
	{
		setTile(x, y, FlameCauldron, t_decorTiles, m_width);
	}
}
////---------------------------------------------------------------------------
////Adds health to the boss room
////---------------------------------------------------------------------------
void DunSpecialRooms::bossRoomHealth(std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms)
{
	//set offset and get x,y not against wall somewhere in room
	int offSet = 1;
	int x = randomInt(t_rooms[m_bossRoomIndex].x + offSet, t_rooms[m_bossRoomIndex].x + t_rooms[m_bossRoomIndex].width - (offSet * 2));
	int y = t_rooms[m_bossRoomIndex].y + t_rooms[m_bossRoomIndex].height - offSet * 2;
	//check if tile is nor dirt and if so move to the right and place health
	if (getTile(x, y, t_decorTiles, m_width, m_height) != DirtTile)
	{
		x++;
		setTile(x, y, HealthPos, t_decorTiles, m_width);
	}
	else
	{
		//place health
		setTile(x, y, HealthPos, t_decorTiles, m_width);
	}
}
////---------------------------------------------------------------------------
////Adds decor to the walls of the boss room
////---------------------------------------------------------------------------
void DunSpecialRooms::bossRoomWalls(std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms)
{
	//get x ,y starting on the first wall tille in the room
	int offSet = 1;
	int x = t_rooms[m_bossRoomIndex].x;
	int y = t_rooms[m_bossRoomIndex].y - offSet;
	//chack they are walls
	if (getTile(x, y, t_decorTiles, m_width, m_height) == Wall)
	{
		//chack x is not near the right side of the room
		if (x < t_rooms[m_bossRoomIndex].x + t_rooms[m_bossRoomIndex].width - offSet)
		{
			//place torch,chains and move by 2 along x each time
			setTile(x, y, Torch, t_decorTiles, m_width);
			x += 2;
			setTile(x, y, Chains, t_decorTiles, m_width);
			x += 2;
			setTile(x, y, Torch, t_decorTiles, m_width);
		}
	}

}
////---------------------------------------------------------------------------
////creates the worship room
////---------------------------------------------------------------------------
void DunSpecialRooms::worshipRoom(std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms, std::vector<char>& t_tiles)
{
	//get x ,y in middle of room
	int x = t_rooms[m_worshipRoomIndex].x + t_rooms[m_worshipRoomIndex].width / 2;
	int y = t_rooms[m_worshipRoomIndex].y + t_rooms[m_worshipRoomIndex].height / 2;
	//check tiles and place worship tile
	if (getTile(x, y, t_decorTiles, m_width, m_height) == DarkTiles)
	{
		setTile(x, y, Worship, t_decorTiles, m_width);
		setUnusedTile(x, y, DarkTiles, FloorTile, t_tiles, m_width, m_height);
	}
	worshipRoomDecor(t_decorTiles,t_rooms);

	deleteRoom(m_worshipRoomIndex, t_rooms);

}
////---------------------------------------------------------------------------
////creates the decorations for the worship room
////---------------------------------------------------------------------------
void DunSpecialRooms::worshipRoomDecor(std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms)
{
	int offSet = 1;
	//get x ,y at top right of room
	int x = t_rooms[m_worshipRoomIndex].x + t_rooms[m_worshipRoomIndex].width - offSet;
	int y = t_rooms[m_worshipRoomIndex].y;
	//place chairs down from the right corner by 3
	int	maxChairs = 3;
	for (int i = 0; i < maxChairs; i++)
	{
		if (checkForDoors(x, y, t_decorTiles, m_width, m_height) == false)
		{
			setTile(x, y, ChairL, t_decorTiles, m_width);
			y++;
		}
	}
	//get x ,y at top left corner
	x = t_rooms[m_worshipRoomIndex].x;
	y = t_rooms[m_worshipRoomIndex].y;
	//place plant at top left and bottom left corners
	if (checkForDoors(x, y, t_decorTiles, m_width, m_height) == false)
	{
		setTile(x, y, Plant, t_decorTiles, m_width);
		y = t_rooms[m_worshipRoomIndex].y + t_rooms[m_worshipRoomIndex].height - offSet;
		setTile(x, y, Plant, t_decorTiles, m_width);
	}
}
////---------------------------------------------------------------------------
////Function to create a room with a statue of some famous person.
////In this case can be of the boss as a warning to players
////---------------------------------------------------------------------------
void DunSpecialRooms::statueRoom(std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms, std::vector<char>& t_tiles)
{
	//get x,y at centre of room
	int offSetFromCentre = 1;
	int x = t_rooms[m_statueRoomIndex].x + t_rooms[m_statueRoomIndex].width / 2 - offSetFromCentre;
	int y = t_rooms[m_statueRoomIndex].y + t_rooms[m_statueRoomIndex].height / 2 - offSetFromCentre;
	//set the statue at that x,y
	if (getTile(x, y, t_decorTiles, m_width, m_height) == TilePattern)
	{
		setTile(x, y, Statue, t_decorTiles, m_width);
		setUnusedTile(x, y, TilePattern, FloorTile, t_tiles, m_width, m_height);
	}
	//delete room and call decorate room func
	statueRoomDecor(t_decorTiles,t_rooms);
	deleteRoom(m_statueRoomIndex, t_rooms);
}
////---------------------------------------------------------------------------
////Function to add some decor to the statue room
////---------------------------------------------------------------------------
void DunSpecialRooms::statueRoomDecor(std::vector<char>& t_decorTiles, std::vector<RoomVals>& t_rooms)
{
	//set x,y to top left corner
	int offSet = 1;
	int x = t_rooms[m_statueRoomIndex].x;
	int y = t_rooms[m_statueRoomIndex].y;
	//place a flaming cauldron in all 4 corners of the room
	if (checkForDoors(x, y, t_decorTiles, m_width, m_height) == false)
	{
		setTile(x, y, FlameCauldron, t_decorTiles, m_width);
		x = t_rooms[m_statueRoomIndex].x + t_rooms[m_statueRoomIndex].width - offSet;
		y = t_rooms[m_statueRoomIndex].y + t_rooms[m_statueRoomIndex].height - offSet;
	}
	if (checkForDoors(x, y, t_decorTiles, m_width, m_height) == false)
	{
		setTile(x, y, FlameCauldron, t_decorTiles, m_width);
		x = t_rooms[m_statueRoomIndex].x;
		y = t_rooms[m_statueRoomIndex].y + t_rooms[m_statueRoomIndex].height - offSet;
	}
	if (checkForDoors(x, y, t_decorTiles, m_width, m_height) == false)
	{
		setTile(x, y, FlameCauldron, t_decorTiles, m_width);
		x = t_rooms[m_statueRoomIndex].x + t_rooms[m_statueRoomIndex].width - offSet;
		y = t_rooms[m_statueRoomIndex].y;
	}
	if (checkForDoors(x, y, t_decorTiles, m_width, m_height) == false) {
		setTile(x, y, FlameCauldron, t_decorTiles, m_width);
	}

}
