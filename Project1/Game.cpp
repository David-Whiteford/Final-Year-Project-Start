#include "Game.h"

#include <iostream>
static double const MS_PER_UPDATE = 10.0;
Game::Game() :
	m_window{ sf::VideoMode{ desktop.width, desktop.height, desktop.bitsPerPixel }, "SFML Game" }
{
	init();
	
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
	if (!tileTexture.loadFromFile("IMAGES//Overworld.png"))
	{
		// error...
	}
	tile.setTexture(tileTexture);
	tile.setPosition(sf::Vector2f(10.f, 800.0f));
	tile.setTextureRect(sf::IntRect(0, 0, 16, 16));
	tile.setScale(sf::Vector2f(3.0f, 3.0f));
	if (!player_texture.loadFromFile("IMAGES//Player.png")) 
	{
		cout << "error" << endl;
	}
	
	player_animated_sprite = new AnimatedSprite(player_texture);
	m_player = new Player(*player_animated_sprite);
	m_player->setPosition(sf::Vector2f(100,100));
	m_tileMap = new Tilemap();
}

void Game::processEvents()
{
}

void Game::update(double dt)
{
	handleInputs();
	
}

void Game::render()
{
	m_window.clear(sf::Color::Black);
	//m_player->render(m_window);
	m_window.draw(tile);
	m_tileMap->DrawMap(m_window);
	m_window.draw(m_player->getAnimatedSpriteFrame());
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
		DEBUG_MSG("gpp::Events::Event::Move_RIGHT_EVENT");
		input.setCurrent(gpp::Events::Event::PLAYERMOVERIGTH);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		DEBUG_MSG("gpp::Events::Event::Move_LEFT_EVENT");
		input.setCurrent(gpp::Events::Event::PLAYERMOVELEFT);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		DEBUG_MSG("gpp::Events::Event::Move_UP_EVENT");
		input.setCurrent(gpp::Events::Event::PLAYERMOVEUP);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		DEBUG_MSG("gpp::Events::Event::Move_DOWN_EVENT");
		input.setCurrent(gpp::Events::Event::PLAYERMOVEDOWN);
	}
	else
	{
		DEBUG_MSG("gpp::Events::Event::IDLE");
		input.setCurrent(gpp::Events::Event::IDLE);
	}
	// Handle input to Player
	m_player->handleKeyInput(input);
	m_player->update();
}

