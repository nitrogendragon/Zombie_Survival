#include "pch.h"
#include <SFML/Graphics.hpp>
#include "Player.h"

using namespace sf;

int main()
{
	enum class State
	{
		PAUSED, LEVLING_UP, GAME_OVER, PLAYING
	};
	// Start with the Game_OVER state
	State state = State::GAME_OVER;

	// Get the screen resolution and create an SFML window
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;

	RenderWindow window(VideoMode(resolution.x, resolution.y),
		"Zombie Arena", Style::Default);

	// Create an SFML View for the main action
	View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	// Here is our clock for timing everything
	Clock clock;
	//How long has the PLAYING state been active
	Time gameTimeTotal;

	// Where is the mouse in relation to world coordinates
	Vector2f mouseWorldPosition;
	// Where is the mouse in relation to screen coordinates
	Vector2i mouseScreenPosition;

	// Creat an instance of the Player class
	Player player;

	// The boundaries of the arena
	IntRect arena;

	// The main game loop
	while (window.isOpen())
	{

	}
	return 0;

}
