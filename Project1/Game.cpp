#include "Game.h"

#include <iostream>
static double const MS_PER_UPDATE = 10.0;
Game::Game() :
	m_window{ sf::VideoMode{ desktop.width, desktop.height, desktop.bitsPerPixel }, "SFML Game" }
{
	init();
	m_tileMap->PushValsToVec();
	m_tileMap->setMap(m_window);
	m_obstaclesVec = m_tileMap->getOverWorldObstaclesVec();
	m_triggersVec = m_tileMap->getCavesVec();
	m_player->setDebugRects(m_obstaclesVec);
	m_player->setDebugRects(m_triggersVec);
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
		m_playerOrigin = m_player->getOrigin();
		view2.setCenter(m_player->getPosition());
		handleInputs();
		m_player->playerRays();
		m_player->collisionCheck(m_obstaclesVec);
		m_player->triggerCheck(m_triggersVec);
		if (m_player->getIfInTrigger())
		{
			m_player->resetCollisions();
			m_transitionStart = true;
			m_dungeonTest = true;
			m_currentGameState = GameState::Dungeon;
		}
		break;
	case GameState::Dungeon:
		if (m_dungeonTest == true)
		{
			m_player->clearObstacleVec();
			m_player->clearTriggerVec();
			m_player->setIfInTrigger(false);
			m_dungeon->generateMap(100);
			m_dungeon->playerStartPos();
			m_dungeon->placeDecorInRoom();
			m_dungeon->print();
			m_dungeon->Set2DVec(m_tileMap);
			m_tileMap->Dun(m_dungeon->getTileMapVec(), m_window, m_mapSize, m_mapSize);
			m_dunObstaclesVec.clear();
			m_dunObstaclesVec = m_tileMap->getDunObstaclesVec();
			m_player->setDebugRects(m_dunObstaclesVec);
			m_dungeonTest = false;
		}
		m_playerOrigin = m_player->getOrigin();
		view2.setCenter(m_player->getPosition());
		m_player->collisionCheck(m_dunObstaclesVec);
		handleInputs();
		break;
	default:
		break;
	}
	/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && m_transitionStart == false)
	{
		m_transitionStart = true;
		m_dungeonTest = true;
		m_currentGameState = GameState::Dungeon;
	}*/
	//if (timer < 0.5 && m_transitionStart == true)
	//{
	//	//increment timer
	//	timer += 0.01;
	//}
	//else
	//{
	//	//set move to true and reset timer to 0
	//	m_transitionStart = false;
	//	timer = 0.0;
	//}
	


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
		m_player->render(m_window);
		break;
	case GameState::Dungeon:
		m_tileMap->DrawDungeon(view2);
		m_window.draw(m_player->getAnimatedSpriteFrame());
		//m_player->render(m_window);
		m_window.setView(view2);
		break;
	default:
		break;
	}
	m_window.display();
}

void Game::handleInputs()
{
	//switch (event.type)
	//{
	//case sf::Event::Closed:
	//	// Close window : exit
	//	//m_window.close();
	//	break;
	//	// Deal with KeyPressed
	//case sf::Event::KeyPressed:
	//	// Died Event
	//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
	//		DEBUG_MSG("gpp::Events::Event::Move_RIGHT_EVENT");
	//		input.setCurrent(gpp::Events::Event::PLAYERMOVERIGTH);
	//	}
	//	break;
	//	// Deal with KeyReleased
	//case sf::Event::KeyReleased:
	//	// Run and Stop Attack
	//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	//	{
	//		DEBUG_MSG("gpp::Events::Event::RUN_RIGHT_START_EVENT");
	//		input.setCurrent(gpp::Events::Event::IDLE);
	//	}
	//	break;
	//default:
	//	DEBUG_MSG("gpp::Events::Event::NONE");
	//	input.setCurrent(gpp::Events::Event::IDLE);
	//	break;
	//}

	
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

