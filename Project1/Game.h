#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <SFML/Graphics.hpp>
#include "Tilemap.h"
#include "Player.h"
#include "DungeonGen.h"
#include "MyEnums.h"
struct CollisionsCheck
{
	bool collisionRight{};
	bool collisionLeft{};
	bool collisionUp{};
	bool collisionDown{};
};

class Game
{
public:
	Game();
	~Game();
	void producer();
	void consumer();
	void run();
	//void setTest(bool t_setGenerate)
	//{
	//	m_dungeonTest = t_setGenerate;
	//}
	GameState m_currentGameState{ GameState::OverWorld };
private:
	
	void init();
	void processEvents();
	void update(double dt);
	void setUpDun();
	void setUpOverWorld();
	void clearVecs()
	{
		m_obstaclesVec.clear();
		m_triggersVec.clear();
		m_player->clearObstacleVec();
		m_player->clearTriggerVec();
	}

	void render();
	void handleInputs();
	Player* m_player;
	sf::Sprite tile;
	AnimatedSprite* player_animated_sprite;
	Tilemap* m_tileMap;
	DungeonGen* m_dungeon;
	sf::Texture player_texture;
	sf::Texture tileTexture;
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	sf::Event event;
	gpp::Events input;
	sf::Vector2f m_textOffset = sf::Vector2f(500.0f, 0.0f);
	sf::Vector2f m_playerOrigin;
	bool m_overWorldSetUp = false;
	bool m_dungeonSetUp = false;
	bool m_transitionStart = false;
	sf::RenderWindow m_window; // main SFML window
	sf::Vector2f m_spawnPos = Vector2f(0.0f,0.0f);
	sf::View view2;
	int m_mapSize = 50;
	double timer = 0.0;
	std::vector<Tiles*> m_obstaclesVec;
	std::vector<Tiles*> m_triggersVec;
	std::mutex* producerLock = new std::mutex;
	std::mutex* consumerLock = new std::mutex;
	std::string m_triggerType;

};
#endif
