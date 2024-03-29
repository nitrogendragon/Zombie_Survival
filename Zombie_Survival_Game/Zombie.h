#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
class Zombie
{
private:
	// How fast is each zombie type
	const float BLOATER_SPEED = 40;
	const float CHASER_SPEED = 80;
	const float CRAWLER_SPEED = 20;

	// How tough is each zombie type
	const float BLOATER_HEALTH = 5;
	const float CHASER_HEALTH = 1;
	const float CRAWLER_HEALTH = 3;

	// Make each zombie vary its speed slightly
	const int MAX_VARRIANCE = 30;
	const int OFFSET = 101 - MAX_VARRIANCE;

	// Where is this zombie?
	Vector2f m_Position;

	// A sprite for the zombie
	Sprite m_Sprite;

	// How fast can this one run/crawl?
	float m_Speed;

	// How much health has it got?
	float m_Health;

	// Sprite Scale X
	float m_ScaleX = 1;

	// Sprite Scale Y
	float m_ScaleY = 1;

	// is it still alive?
	bool m_Alive;

	//are we moving left right up down?
	bool m_Right = false;
	bool m_Left = false;
	bool m_Down = false;
	bool m_Up = false;

	// number of directions we are moving in 
	//(question is... are we moving in one or two directions?)
	int m_TwoD = 0;
	// Public prototypes go here
public:

	// Handle when a bullet hits a zombie
	bool hit();

	// Find out if the zombie is alive
	bool isAlive();

	// Spawn a new zombie
	void spawn(float startX, float startY, int type, int seed, float screenScaleAvg);

	// Return a rectangle that is the position in the world
	FloatRect getPosition();

	// Get a copy of the sprite to draw
	Sprite getSprite();

	// set zombie scale X based on screen scale and optionally zombie scale
	void setZombieScaleX(float screenScaleX, float zombieScaleX = 1);

	// set zombie scale X based on screen scale and optionally zombie scale
	void setZombieScaleY(float screenScaleY, float zombieScaleY = 1);

	// update sprite scale
	void updateSpriteScale();

	// Update the zombie each frame
	void update(float elapsedTime, Vector2f playerLocation);
	//Zombie();
	//~Zombie();
};

