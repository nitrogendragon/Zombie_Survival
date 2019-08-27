#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Bullet
{
private:
	// Where is the bullet?
	Vector2f m_Position;

	// What each bullet looks like
	RectangleShape m_BulletShape;

	// Is this bullet currently whizzing through the air
	bool m_InFlight = false;

	// How fast does a bullet travel?
	float m_BulletSpeed = 1000;

	// What fraction of 1 pixel does the bullet travel,
	// Horizontally and vertically each frame?
	// These values will be derived from m_BulletSpeed
	float m_BulletDistanceX;
	float m_BulletDistanceY;

	// starting points for the bullet
	float m_startX;
	float m_startY;

	// Where is the bullet heading to?
	float m_targetX;
	float m_targetY;

	//set the range of the bullet
	float m_range = 1000;

	// Some boundaries so the bullet doesn't fly forever
	// Note that we don't actually need these but I will keep them for reference purposes
	float m_MaxX;
	float m_MinX;
	float m_MaxY;
	float m_MinY;

	//Public function prototypes below
public:

	Bullet();

	// general function for measuring distance in floats between two points
	float distanceTraveled(float startX, float startY, float targetX, float targetY);

	//stop the bullet
	void stop();

	// Returns the value of m_InFlight
	bool isInFlight();

	// Launch a new bullet
	void shoot(float startX, float startY, float xTarget, float yTarget);

	// Tell the calling code where the bullet is in the world
	FloatRect getPosition();

	// Return the actual shape (for drawing)
	RectangleShape getShape();

	// Update the bullet each frame
	void update(float elapsedTime);



};

