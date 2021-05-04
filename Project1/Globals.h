#ifndef MY_GLOBALS
#define MY_GLOBALS

//David Whiteford

/// Global functions that are needed by different classses
/// different levels in game
// struct to store room values
struct RoomVals
{
	int x, y;
	int width, height;
	int decorInRoom = 0;
};
/// </summary>
enum class GameState
{
	//different game states
	None,
	OverWorld,
	Dungeon,
	Exit

};
//enum for all the types of tiles
enum DungeonTiles
{
	UnusedTile = 'A',
	Wall = '0',
	FloorTile = '1',
	StoneFloorTile = 'S',
	CorridorTile = '1',
	Plant = '2',
	Door1 = '3',
	Door2 = '4',
	PrisonTile = 'V',
	BookShelve = 'B',
	CoffinTile = 'J',
	Potion = '5',
	Chains = '6',
	Torch = 'T',
	Picture = 'P',
	Money = 'M',
	NightStand = 'Y',
	Chest = '7',
	Skull = '8',
	SpawnPoint = '9',
	ChairL = 'Z',
	ChairR = 'CR',
	ChairF = 'O',
	Table = 'U',
	TilePattern = 'G',
	Statue = 'I',
	FlameCauldron = 'R',
	DirtTile = 'D',
	HoleTile = 'H',
	Bed = 'K',
	DarkTiles = 'Q',
	Worship = 'W',
	HealthPos = 'N',
	Chair = 'TB',
	SpikeTrap = 'L',
	TrapDoorTrap = 'TD',
	WallSpikeTrap = 'WS'
};
//enum for the directions
enum class Direction
{
	North,
	South,
	West,
	East,
	NoDirection,
	DirectionNum
};
namespace
{
	std::random_device rd;
	std::mt19937 mt(rd());

/***************************************************************************************
*    Title: C++ Example of Dungeon-Building Algorithm
*    Author: MindControlDx
*    Date: 31 July 2015
*    Code version: Version 3
*    Availability: http://www.roguebasin.com/index.php?title=C%2B%2B_Example_of_Dungeon-Building_Algorithm
*
***************************************************************************************/
	int randomInt(int t_exclusiveMax)
	{
		std::uniform_int_distribution<> dist(0, t_exclusiveMax - 1);
		return dist(mt);
	}
	int randomInt(int t_min, int t_max)
	{
		std::uniform_int_distribution<> dist(0, t_max - t_min);
		return dist(mt) + t_min;
	}
	bool randomBool()
	{
		int random = rand() % 2;
		if (random == 1) {
			return true;
		}
		return false;
		
	}
	////---------------------------------------------------------------------------
	//// Functions to set unused tiles under larger tiles,set the tiles,get the tiles,check for doors
	////---------------------------------------------------------------------------
/***************************************************************************************
*    Title: C++ Example of Dungeon-Building Algorithm
*    Author: MindControlDx
*    Date: 31 July 2015
*    Code version: Version 3
*    Availability: http://www.roguebasin.com/index.php?title=C%2B%2B_Example_of_Dungeon-Building_Algorithm
*
***************************************************************************************/
	char getTile(int t_x, int t_y, std::vector<char>& t_tiles, int t_width, int t_height) {
		//gets a decoration tile at a certain x,y
		if (t_x < 0 || t_y < 0 || t_x >= t_width || t_y >= t_height)
			return UnusedTile;
		return t_tiles[t_x + t_y * t_width];
	}
/***************************************************************************************
*    Title: C++ Example of Dungeon-Building Algorithm
*    Author: MindControlDx
*    Date: 31 July 2015
*    Code version: Version 3
*    Availability: http://www.roguebasin.com/index.php?title=C%2B%2B_Example_of_Dungeon-Building_Algorithm
*
***************************************************************************************/
	void setTile(int t_x, int t_y, char t_tile, std::vector<char>& t_tiles, int t_width) {
		//sets tile at x,y of background tiles
		t_tiles[t_x + t_y * t_width] = t_tile;
	}
	bool checkForDoors(int t_x, int t_y,std::vector<char>& t_tiles, int t_width, int t_height)
	{
		bool doorDetected = false;
		int offSet = 1;
		//check if the tiles one to the north ,south,east,west directions are doors
		if (getTile(t_x - offSet, t_y, t_tiles, t_width, t_height) == Door1 || getTile(t_x + offSet, t_y, t_tiles, t_width, t_height) == Door1
			|| getTile(t_x, t_y - offSet, t_tiles, t_width, t_height) == Door2 || getTile(t_x, t_y + offSet, t_tiles, t_width, t_height) == Door2
			|| getTile(t_x, t_y - offSet, t_tiles, t_width, t_height) == SpawnPoint)
		{
			//set bool if door was detected and return
			doorDetected = true;
		}
		return doorDetected;
	}
	void setUnusedTile(int x, int y, char t_tile, char t_secondTile, std::vector<char>& t_tiles, int t_width, int t_height)
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
				if (getTile(xVal, yVal, t_tiles, t_width, t_height) == t_tile
					|| getTile(xVal, yVal, t_tiles, t_width, t_height) == t_secondTile)
				{
					setTile(xVal, yVal, UnusedTile, t_tiles, t_width);
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
	void deleteRoom(int i, std::vector<RoomVals>& t_rooms) { t_rooms.erase(t_rooms.begin() + i); }
	int getRoom(int t_roomWidth, int t_roomHeight, std::vector<RoomVals>& t_rooms)
	{
		//if the room is found the index will be from 0 to room size
		//if its 1000 then no room fount, used 1000 as 0 is a index in the rooms vec
		int roomIndex = 1000;
		if (t_rooms.empty() == false)
		{
			for (int i = 0; i < t_rooms.size(); i++)
			{
				//find a room remaining of a certain size
				if (t_rooms[i].width >= t_roomWidth && t_rooms[i].height >= t_roomHeight)
				{
					roomIndex = i;
				}
			}
		}
		return roomIndex;
	}
	
}

#endif // !MY_ENUMS

