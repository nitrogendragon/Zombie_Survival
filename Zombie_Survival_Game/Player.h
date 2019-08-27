#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
class Player
{
private:
	const float START_SPEED = 200;
	const float START_HEALTH = 100;
	// Using m_ prefix to clarify that it is a member variable
	// Where is the player
	Vector2f m_Position;

	// Our player sprite for the map
	Sprite m_Sprite;

	// Texture for Sprite
	Texture m_Texture;

	// Get screen resolution
	Vector2f m_Resolution;

	// What is the size of the current arena
	IntRect m_Arena;

	// What is the size of each tile of the arena?
	int m_TileSize;

	// What is the magnitude (for adjusting the player speed.
	float m_Magnitude = 1;

	// Which direction is the player moving?
	bool m_UpPressed;
	bool m_DownPressed;
	bool m_LeftPressed;
	bool m_RightPressed;

	// How much health does the player have left?
	int m_Health;

	// What is the players max health?
	int m_MaxHealth;

	// When was the player last hit?
	Time m_LastHit;

	// Speed in pixels per second
	float m_Speed;

	/*
	*************
	Public Functions
	*************
	*/


public:
	Player();
	~Player();

	void spawn(IntRect arena, Vector2f resolution, int tileSize);

	// Call this at the end of every game
	void resetPlayerStats();

	// Handle the player getting hit by a zombie
	bool hit(Time timHit);

	// How long ago was the player last hit
	Time getLastHitTime();

	// Where is the player?
	FloatRect getPosition();

	// Where is  the center of the player
	Vector2f getCenter();

	// Which angle is the player facing
	float getRotation();

	// Send a copy of the sprite to main
	Sprite getSprite();

	// The next four functions move the player
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();

	// Stop the player moving in a specific direction
	void stopLeft();
	void stopRight();
	void stopUp();
	void stopDown();

	// We will call this function once every frame
	void update(float elapsedTime, Vector2i mousePosition, bool twokeys = false);

	// Give the player a speed boost
	void  upgradeSpeed();

	// Give the player some health
	void upgradeHealth();

	// Increase the maximum amount of health the player can have
	void increaseHealthLevel(int amount);

	// How much health has the player currently got?
	int getHealth();


};

