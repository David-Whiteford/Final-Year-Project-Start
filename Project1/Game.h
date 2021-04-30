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
#include "Globals.h"
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

	void run();
	GameState m_currentGameState{ GameState::OverWorld };
private:
	//all functions for game class
	void init();
	void processEvents();
	void update(double dt);
	void setUpDun();
	void setUpOverWorld();
	//clears all colliders and trigger vectors
	void clearVecs()
	{
		m_obstaclesVec.clear();
		m_triggersVec.clear();
	}

	void render();
	void handleInputs();
	//player,dungeon,tilemap and other objects needed
	Player* m_player;
	sf::Sprite tile;
	AnimatedSprite* player_animated_sprite;
	Tilemap* m_tileMap;
	DungeonGen* m_dungeon;
	//sets the player texture and dektop for game window 
	sf::Texture player_texture;
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	//sets up the event and inputs
	sf::Event event;
	gpp::Events input;
	sf::Vector2f m_playerOrigin;
	//bool to determine wheather to setup overwold or dungeon
	bool m_overWorldSetUp = false;
	bool m_dungeonSetUp = false;
	sf::RenderWindow m_window; // main SFML window
	sf::Vector2f m_spawnPos = Vector2f(0.0f,0.0f);
	sf::View view2;
	int m_mapSize = 50;
	double timer = 0.0;
	//obstacles and trigger vectors
	std::vector<Tiles*> m_obstaclesVec;
	std::vector<Tiles*> m_triggersVec;
	std::string m_triggerType;

};
#endif
