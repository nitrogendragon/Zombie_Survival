#pragma once
#include "pch.h"
#include "Bullet.h"
#include <cmath>
#include <iostream>
#include <stdlib.h>

Bullet::Bullet()
{
	m_BulletShape.setSize(sf::Vector2f(2, 2));
}



void Bullet::shoot(float startX, float startY, float targetX, float targetY)
{
	// Keep track of the bullet
	m_InFlight = true;
	// We shoot the bullet before anything else so it is ok to initialize 
	// Starting positions here though maybe doing something in the constructor would be a good idea too.
	
	m_Position.x = startX;
	m_startX = startX;
	m_Position.y = startY;
	m_startY = startY;
	std::cout << m_Position.x << ", " << m_Position.y << "\n";

	// Calculate the gradient of the flight path
	float gradient = (startX - targetX) / (startY - targetY);

	// Any gradient less than zero needs to be negative
	if (gradient < 0)
	{
		gradient *= -1;
	}

	// Calculate the ratio between x and y
	float ratioXY = m_BulletSpeed / (1 + gradient);

	// Set the "speed" horizontally and vertically
	m_BulletDistanceY = ratioXY;
	m_BulletDistanceX = ratioXY * gradient;

	// Point the bullet in the right direction
	
	if (targetX < startX)
	{
		m_BulletDistanceX *= -1;
	}

	if (targetY < startY)
	{
		m_BulletDistanceY *= -1;
	}

	// Finally, assign the results to the member variables
	m_targetY = targetY;
	m_targetX = targetX;

	// Set a max m_range the bullet can travel in pixels
	m_range = 1000;
	m_MinX = startX - m_range;
	m_MaxX = startX + m_range;
	m_MinY = startY - m_range;
	m_MaxY = startY + m_range;

	// Position the bullet so it is ready to be drawn
	m_BulletShape.setPosition(m_Position);
	

}

void Bullet::stop()
{
	m_InFlight = false;
}

bool Bullet::isInFlight()
{
	return m_InFlight;
}

FloatRect Bullet::getPosition()
{
	return m_BulletShape.getGlobalBounds();
}

RectangleShape Bullet::getShape()
{
	return m_BulletShape;
}

float Bullet::distanceTraveled(float startX, float startY, float endX, float endY)
{
	return sqrt(pow(abs(endX) - startX, 2) + pow(abs(endY) - startY, 2) * 1.0);
}

void Bullet::update(float elapsedTime)
{
	// Update the bullet position variables
	m_Position.x += m_BulletDistanceX * elapsedTime;
	m_Position.y += m_BulletDistanceY * elapsedTime;

	// Move the bullet
	m_BulletShape.setPosition(m_Position);

	// Has the bullet gone out of m_range?
	if (distanceTraveled(m_startX, m_startY, m_BulletDistanceX, m_BulletDistanceY) > m_range )
	{
		m_InFlight = false;
	}

}


