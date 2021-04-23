#ifndef MY_GLOBALS
#define MY_GLOBALS

//David Whiteford

/// <summary>
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
	bool randomBool(double t_prob = 0.5)
	{
		std::bernoulli_distribution dist(t_prob);
		return dist(mt);
	}

}
#endif // !MY_ENUMS

