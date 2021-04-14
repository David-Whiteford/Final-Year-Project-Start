#include "Game.h"


static double const MS_PER_UPDATE = 10.0;
Game::Game() :
	m_window{ sf::VideoMode{ desktop.width, desktop.height, desktop.bitsPerPixel }, "SFML Game" }
{
	//set up the tile map and store player spawns
	init();
	m_tileMap->PushValsToVec();
	m_tileMap->setMap(m_window);
	m_spawnPos = m_player->getPosition();
	setUpOverWorld();
}


Game::~Game()
{
}
void Game::run()
{
	sf::Clock clock;
	sf::Int32 lag = 0;

	while (m_window.isOpen())
	{
		sf::Time dt = clock.restart();

		lag += dt.asMilliseconds();

		processEvents();

		while (lag > MS_PER_UPDATE)
		{
			update(MS_PER_UPDATE);
			lag -= MS_PER_UPDATE;
		}
		update(MS_PER_UPDATE);

		render();
	}
}



void Game::init()
{
	//create the player 
	if (!player_texture.loadFromFile("IMAGES//Player.png")) 
	{
		cout << "error" << endl;
	}
	player_animated_sprite = new AnimatedSprite(player_texture);
	m_player = new Player(*player_animated_sprite);
	m_player->setPosition(sf::Vector2f(20,20));
	m_playerOrigin = m_player->getOrigin();
	// create a view with its center and size
	view2.setCenter(m_player->getPosition());
	view2.setSize(sf::Vector2f(50.f, 50.f));
	view2.zoom(3.0f);
	//create the tilemap and dungeon gen
	m_tileMap = new Tilemap();
	m_dungeon = new DungeonGen(m_mapSize, m_mapSize);
}

void Game::processEvents()
{
}

void Game::update(double dt)
{
   //calls the update depending on the screen / gamestate or closes
	switch (m_currentGameState)
	{
	case GameState::None:
		break;
	case GameState::Exit:
		m_window.close();
		break;
	case GameState::OverWorld:
		//set up overworld when switching between levels
		if (m_overWorldSetUp == true)
		{
			setUpOverWorld();
		}
		//player control and position/ view
		m_playerOrigin = m_player->getOrigin();
		view2.setCenter(m_player->getPosition());
		handleInputs();
		m_player->playerRays();
		//check for collisions
		if (DEBUG == 0)
		{
			m_player->collisionCheck();
		}
		//get the type of trigger player entered
		m_triggerType = m_player->triggerCheck(m_triggersVec);
		//check if in trigger for loading dungeon
		if (m_player->getIfInTrigger() 
 			&& m_triggerType != "Health")
		{
			//reset values for dungeon and set the state and reset collisions
			m_triggerType = "";
			clearVecs();
			m_player->resetCollisions();
			m_dungeonSetUp = true;
			m_currentGameState = GameState::Dungeon;
		}
		break;
	case GameState::Dungeon:
		//set up the dungeon
		if (m_dungeonSetUp == true)
		{
			setUpDun();
		}
		//set the player position and allow updating the player
		m_playerOrigin = m_player->getOrigin();
		view2.setCenter(m_player->getPosition());
		m_player->playerRays();
		//check for player collisions
		if (DEBUG == 0)
		{
			m_player->collisionCheck();
		}
		//check for triggers
		m_triggerType = m_player->triggerCheck(m_triggersVec);
		//check the trigger type not health
		if (m_player->getIfInTrigger()
			&& m_triggerType != "Health")
		{
			//if so reset the trigger and reset all vectors
			m_triggerType = "";
			clearVecs();
			m_player->resetCollisions();
			m_overWorldSetUp = true;
			m_currentGameState = GameState::OverWorld;
		}
		handleInputs();
		break;
	default:
		break;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && m_transitionStart == false)
	{
		clearVecs();
		m_transitionStart = true;
		m_dungeonSetUp = true;
		m_currentGameState = GameState::Dungeon;
	}
}

void Game::setUpDun()
{
	//set the spawn and set that the playes not in trigger
	sf::Vector2f offSetFromSpawn = Vector2f(0.0f, 20.0f);
	m_player->setIfInTrigger(false);
	//generate new dungeon and pass to tilemap
    m_dungeon->createDunExtras(m_tileMap);
	m_tileMap->Dun(m_dungeon->getTileMapVec(), m_window, m_mapSize, m_mapSize);
	m_tileMap->DunDecor(m_dungeon->getDecorTileVec(), m_window, m_mapSize, m_mapSize);
	//set the player to spawn at one of the spawn areas
	sf::Vector2f playerStartPos = sf::Vector2f(0.0f, 0.0f);
	playerStartPos = m_tileMap->getPlayerSpawn();
	m_player->setPosition(playerStartPos + offSetFromSpawn);
	//setup the triggers and all colliders
	m_triggersVec = m_tileMap->getDunExitsVec();
	m_obstaclesVec = m_tileMap->getDunObstaclesVec();
	//set up all debug rectangles
	m_player->setDebugRects(m_obstaclesVec);
	m_player->setDebugRects(m_triggersVec);
	m_dungeonSetUp = false;
}

void Game::setUpOverWorld()
{
	//set the spawn and set that the playes not in trigger
	sf::Vector2f offSetFromSpawn = Vector2f(0.0f, 20.0f);
	m_player->setIfInTrigger(false);
	//clear dundeon vectors
	m_tileMap->clearDunVecs();
	m_dungeon->resetTileVecs();
	//set the player to spawn near cave
	sf::Vector2f playerCavePos = sf::Vector2f(0.0f, 0.0f);
	playerCavePos = m_tileMap->getPlayerCave();
	m_player->setPosition(playerCavePos + offSetFromSpawn);
	//setup the triggers and all colliders
	m_obstaclesVec = m_tileMap->getOverWorldObstaclesVec();
	m_triggersVec = m_tileMap->getCavesVec();
	//set up all debug rectangles
	m_player->setDebugRects(m_obstaclesVec);
	m_player->setDebugRects(m_triggersVec);
	m_overWorldSetUp = false;
}

void Game::render()
{
	m_window.clear(sf::Color::Black);
	//calls the update depending on the screen / gamestate or closes
	switch (m_currentGameState)
	{
	case GameState::None:
		break;
	case GameState::Exit:
		m_window.close();
		break;
	case GameState::OverWorld:
		m_tileMap->DrawOverWorld(view2);
		m_window.draw(m_player->getAnimatedSpriteFrame());
		m_window.setView(view2);
		//to render debug like colliders and rays
		if (DEBUG == 1)
		{
			m_player->render(m_window, view2);
		}
		break;
	case GameState::Dungeon:
		//draw the dungeon tiles
		m_tileMap->DrawDungeon(view2);
		//draw animated player
		m_window.draw(m_player->getAnimatedSpriteFrame());
		//to render debug like colliders and rays
		if (DEBUG == 1)
		{
			m_player->render(m_window, view2);
		}
		m_window.setView(view2);
		break;
	default:
		break;
	}
	m_window.display();
}

void Game::handleInputs()
{
	//input to change player state
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) 
	{
		input.setCurrent(gpp::Events::Event::PLAYERMOVERIGTH);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		input.setCurrent(gpp::Events::Event::PLAYERMOVELEFT);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		input.setCurrent(gpp::Events::Event::PLAYERMOVEUP);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		input.setCurrent(gpp::Events::Event::PLAYERMOVEDOWN);
	}
	else
	{
		input.setCurrent(gpp::Events::Event::IDLE);
	}
	//bools to check collisions
	bool colDown = m_player->getColDown();
	bool colUp = m_player->getColUp();
	bool colRight = m_player->getColRight();
	bool colLeft = m_player->getColLeft();

	CollisionsCheck collisionDown{ colRight,colLeft,colUp,colDown };
	
	// Handle input to Player
	m_player->handleKeyInput(input);
	m_player->update();
}

