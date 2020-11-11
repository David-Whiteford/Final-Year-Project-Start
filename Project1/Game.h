#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Tilemap.h"
#include "Player.h"
class Game
{
public:
	Game();
	~Game();

	void run();
	

private:
	void init();
	void processEvents();
	void update(double dt);
	void render();
	void handleInputs();
	Player* m_player;
	sf::Sprite tile;
	AnimatedSprite* player_animated_sprite;
	Tilemap* m_tileMap;
	sf::Texture player_texture;
	sf::Texture tileTexture;
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	sf::Event event;
	gpp::Events input;
	sf::Vector2f m_textOffset = sf::Vector2f(500.0f, 0.0f);

	sf::RenderWindow m_window; // main SFML window

};
#endif
