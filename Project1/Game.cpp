#include "Game.h"


static double const MS_PER_UPDATE = 10.0;
Game::Game() :
	m_window{ sf::VideoMode{ desktop.width, desktop.height, desktop.bitsPerPixel }, "SFML Game" }
{
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
		if (m_overWorldSetUp == true)
		{
			setUpOverWorld();
		}
		m_playerOrigin = m_player->getOrigin();
		view2.setCenter(m_player->getPosition());
		handleInputs();
		m_player->playerRays();
		if (DEBUG == 0)
		{
			m_player->collisionCheck();
		}
		m_triggerType = m_player->triggerCheck(m_triggersVec);
		if (m_player->getIfInTrigger() 
 			&& m_triggerType != "Health")
		{
			m_triggerType = "";
			clearVecs();
			m_player->resetCollisions();
			m_dungeonSetUp = true;
			m_currentGameState = GameState::Dungeon;
		}
		break;
	case GameState::Dungeon:
		if (m_dungeonSetUp == true)
		{
			setUpDun();
		}
		m_playerOrigin = m_player->getOrigin();
		view2.setCenter(m_player->getPosition());
		m_player->playerRays();
		if (DEBUG == 0)
		{
			m_player->collisionCheck();
		}
		m_triggerType = m_player->triggerCheck(m_triggersVec);
		if (m_player->getIfInTrigger()
			&& m_triggerType != "Health")
		{
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
	sf::Vector2f offSetFromSpawn = Vector2f(0.0f, 20.0f);
	m_player->setIfInTrigger(false);
    m_dungeon->createRoomFeatures(m_tileMap);
	m_tileMap->Dun(m_dungeon->getTileMapVec(), m_window, m_mapSize, m_mapSize);
	m_tileMap->DunDecor(m_dungeon->getDecorTileVec(), m_window, m_mapSize, m_mapSize);
	sf::Vector2f playerStartPos = sf::Vector2f(0.0f, 0.0f);
	playerStartPos = m_tileMap->getPlayerSpawn();
	m_player->setPosition(playerStartPos + offSetFromSpawn);
	m_triggersVec = m_tileMap->getDunExitsVec();
	m_obstaclesVec = m_tileMap->getDunObstaclesVec();
	m_player->setDebugRects(m_obstaclesVec);
	m_player->setDebugRects(m_triggersVec);
	m_dungeonSetUp = false;
}

void Game::setUpOverWorld()
{
	sf::Vector2f offSetFromSpawn = Vector2f(0.0f, 20.0f);
	m_player->setIfInTrigger(false);
	m_tileMap->clearDunVecs();
	m_dungeon->resetTileVecs();
	sf::Vector2f playerCavePos = sf::Vector2f(0.0f, 0.0f);
	playerCavePos = m_tileMap->getPlayerCave();
	m_player->setPosition(playerCavePos + offSetFromSpawn);
	m_obstaclesVec = m_tileMap->getOverWorldObstaclesVec();
	m_triggersVec = m_tileMap->getCavesVec();
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
		m_player->render(m_window, view2);
		break;
	case GameState::Dungeon:
		m_tileMap->DrawDungeon(view2);
		m_window.draw(m_player->getAnimatedSpriteFrame());
		//to render debug like colliders and rays
		m_player->render(m_window, view2);
		m_window.setView(view2);
		break;
	default:
		break;
	}
	m_window.display();
}

void Game::handleInputs()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) 
	{
		//DEBUG_MSG("gpp::Events::Event::Move_RIGHT_EVENT");
		input.setCurrent(gpp::Events::Event::PLAYERMOVERIGTH);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		//DEBUG_MSG("gpp::Events::Event::Move_LEFT_EVENT");
		input.setCurrent(gpp::Events::Event::PLAYERMOVELEFT);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		//DEBUG_MSG("gpp::Events::Event::Move_UP_EVENT");
		input.setCurrent(gpp::Events::Event::PLAYERMOVEUP);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		//DEBUG_MSG("gpp::Events::Event::Move_DOWN_EVENT");
		input.setCurrent(gpp::Events::Event::PLAYERMOVEDOWN);
	}
	else
	{
		//DEBUG_MSG("gpp::Events::Event::IDLE");
		input.setCurrent(gpp::Events::Event::IDLE);
	}
	bool colDown = m_player->getColDown();
	bool colUp = m_player->getColUp();
	bool colRight = m_player->getColRight();
	bool colLeft = m_player->getColLeft();

	CollisionsCheck collisionDown{ colRight,colLeft,colUp,colDown };
	
	// Handle input to Player
	m_player->handleKeyInput(input);
	m_player->update();
}

