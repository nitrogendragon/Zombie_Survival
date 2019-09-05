#include "pch.h"
#include "Zombie.h"
#include "TextureHolder.h"
#include <cstdlib>
#include <ctime>
#include <math.h>

using namespace std;


void Zombie::spawn(float startX, float startY, int type, int seed)
{

	switch (type)
	{
	case 0:
		// Bloater
		m_Sprite = Sprite(TextureHolder::GetTexture(
			"graphics/bloater.png"));

		m_Speed = BLOATER_SPEED;
		m_Health = BLOATER_HEALTH;
		break;

	case 1:
		// Chaser
		m_Sprite = Sprite(TextureHolder::GetTexture(
			"graphics/chaser.png"));

		m_Speed = CHASER_SPEED;
		m_Health = CHASER_HEALTH;
		break;

	case 2:
		// Crawler
		m_Sprite = Sprite(TextureHolder::GetTexture(
			"graphics/crawler.png"));

		m_Speed = CRAWLER_SPEED;
		m_Health = CRAWLER_HEALTH;
		break;
	}

	// Modify the speed to make the zombie unique
	// Every zombie is unique. Create a speed modifier
	srand((int)time(0) * seed);
	// Somewhere between 80 an 100
	float modifier = (rand() % MAX_VARRIANCE) + OFFSET;
	// Express as a fraction of 1
	modifier /= 100; // Now equals between .7 and 1
	m_Speed *= modifier;

	m_Position.x = startX;
	m_Position.y = startY;

	m_Sprite.setOrigin(25, 25);
	m_Sprite.setPosition(m_Position);
}

bool Zombie::hit()
{
	m_Health--;

	if (m_Health < 0)
	{
		// dead
		m_Alive = false;
		m_Sprite.setTexture(TextureHolder::GetTexture(
			"graphics/blood.png"));

		return true;
	}

	// injured but not dead yet
	return false;
}

bool Zombie::isAlive()
{
	return m_Alive;
}

FloatRect Zombie::getPosition()
{
	return m_Sprite.getGlobalBounds();
}


Sprite Zombie::getSprite()
{
	return m_Sprite;
}

// set zombie scale X based on screen scale and optionally zombie scale
void Zombie::setZombieScaleX(float screenScaleX, float zombieScaleX)
{
	m_ScaleX = screenScaleX * zombieScaleX;
}

// set zombie scale Y based on screen scale and optionally zombie scale
void Zombie::setZombieScaleY(float screenScaleY, float zombieScaleY)
{
	m_ScaleY = screenScaleY * zombieScaleY;
}

// update sprite scale
void Zombie::updateSpriteScale()
{
	m_Sprite.setScale(m_ScaleX, m_ScaleY);
}

void Zombie::update(float elapsedTime,
	Vector2f playerLocation)
{
	float playerX = playerLocation.x;
	float playerY = playerLocation.y;

	// Face the sprite in the correct direction
	float angle = (atan2(playerY - m_Position.y,
		playerX - m_Position.x)
		* 180) / 3.141;

	// Update the zombie position variables
	if (playerX > m_Position.x)
	{
		m_Position.x = m_Position.x +
			m_Speed * fabs(cos(angle)) * elapsedTime;
		
		
	}

	if (playerY > m_Position.y)
	{
		m_Position.y = m_Position.y +
			m_Speed * fabs(sin(angle)) * elapsedTime;
	}

	if (playerX < m_Position.x)
	{
		m_Position.x = m_Position.x -
			m_Speed * fabs(cos(angle)) * elapsedTime;
	}

	if (playerY < m_Position.y)
	{
		m_Position.y = m_Position.y -
			m_Speed * fabs(sin(angle))* elapsedTime;
	}

	// Move the sprite
	m_Sprite.setPosition(m_Position);

	

	m_Sprite.setRotation(angle);
	

}