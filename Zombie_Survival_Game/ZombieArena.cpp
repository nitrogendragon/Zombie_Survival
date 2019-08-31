#pragma once
#include "pch.h"
#include <sstream>
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "ZombieArena.h"
#include "CreateBackground.h"
#include "TextureHolder.h"
#include "Bullet.h"
#include "Pickup.h"
#include <iostream>
#include <stdlib.h>
#include <assert.h>


using namespace sf;

int main()
{
	// Here is the instance of TextureHolder
	TextureHolder holder;

	// The game will always be in one of four states
	enum class State { PAUSED, LEVELING_UP, GAME_OVER, PLAYING };
	// Start with the GAME_OVER state
	State state = State::GAME_OVER;

	// Get the screen resolution and create an SFML window
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;

	//Scale factors for whenever we decide to change the screen size
	float screenScaleH = 1;
	float screenScaleW = 1;
	//Going to use for scaling text for now
	int screenScaleAvg = (int)floor(screenScaleH + screenScaleW) / 2;
	//assert(screenScaleAvg == 1);

	//Scaled resolutions
	float scaledResolutionX = resolution.x * screenScaleW;
	float scaledResolutionY = resolution.y * screenScaleH;

	RenderWindow window(VideoMode(scaledResolutionX, scaledResolutionY),
		"Zombie Arena", Style::Default);

	// Limit FPS 
	int frameRate = 90;
	window.setFramerateLimit(frameRate);

	// Create a an SFML View for the main action
	View mainView(sf::FloatRect(0, 0, scaledResolutionX, scaledResolutionY));

	// Here is our clock for timing everything
	Clock clock;
	// How long has the PLAYING state been active
	Time gameTimeTotal;

	// Where is the mouse in relation to world coordinates
	Vector2f mouseWorldPosition;
	// Where is the mouse in relation to screen coordinates
	Vector2i mouseScreenPosition;

	// Create an instance of the Player class and scale it to the screen
	Player player;
	player.setPlayerScaleX(screenScaleW);
	player.setPlayerScaleY(screenScaleH);
	player.updateSpriteScale();

	// The boundaries of the arena
	IntRect arena;

	// Create the background
	VertexArray background;
	// Load the texture for our background vertex array
	Texture textureBackground = TextureHolder::GetTexture(
		"graphics/background_sheet.png");

	// Prepare for a horde of zombies
	int numZombies;
	int numZombiesAlive;
	Zombie* zombies = NULL;

	// 100 bullets should work for now
	const int totalbullets = 100;
	Bullet bullets[totalbullets];
	int currentBullet = 0;
	int bulletsSpare = 24;
	int bulletsInClip = 6;
	int clipSize = 6;
	float fireRate = 1;
	// When was the fire button last pressed?
	Time lastPressed;

	// Hide the mouse pointer and replace it with a crosshair
	window.setMouseCursorVisible(false);
	Sprite spriteCrosshair;
	Texture textureCrosshair = TextureHolder::GetTexture("graphics/crosshair.png");

	spriteCrosshair.setTexture(textureCrosshair);
	spriteCrosshair.setOrigin(25, 25);
	spriteCrosshair.setScale(screenScaleW, screenScaleH);

	// How many keys are we pressing down on?
	int keysdown = 0;
	bool twokeys = false;

	//Create a couple pickups
	Pickup healthPickup(1);
	Pickup ammoPickup(2);

	//About the game
	int score = 0;
	int highScore = 0;

	// For the home/game over screen
	Sprite spriteGameOver;
	Texture textureGameOver =
		TextureHolder::GetTexture("graphics/background.png");
	spriteGameOver.setTexture(textureGameOver);
	spriteGameOver.setPosition(0, 0);
	spriteGameOver.setScale(scaleObjectX(screenScaleW, 1.5), scaleObjectY(screenScaleH,1));

	// Create a view for the HUD
	View hudView(sf::FloatRect(0, 0, scaledResolutionX, scaledResolutionY));

	// Create a sprite for the ammo icon
	Sprite spriteAmmoIcon;
	Texture textureAmmoIcon = TextureHolder::GetTexture("graphics/ammo_icon.png");
	spriteAmmoIcon.setTexture(textureAmmoIcon);
	//spriteAmmoIcon.setPosition(28, 620);
	spriteAmmoIcon.setPosition(scaledPositionSetX(.1,scaledResolutionX), scaledPositionSetY(.9,scaledResolutionY));
	spriteAmmoIcon.setScale(screenScaleW, screenScaleH);
	// Load our font
	Font font;
	font.loadFromFile("fonts/zombiecontrol.ttf");

	// Paused Text pausedText;
	Text pausedText;
	pausedText.setFont(font);
	pausedText.setCharacterSize(85*screenScaleAvg);
	pausedText.setFillColor(Color::White);
	//pausedText.setPosition(350, 200);
	pausedText.setPosition(scaledPositionSetX(.4, scaledResolutionX), scaledPositionSetY(.45, scaledResolutionY));
	pausedText.setString("Prees Enter \nto continue");

	// Game Over
	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(80*screenScaleAvg);
	//gameOverText.setPosition(280, 640);
	gameOverText.setPosition(scaledPositionSetX(.375, scaledResolutionX), scaledPositionSetY(.45, scaledResolutionY));
	gameOverText.setString("Press Enter to play");

	// Leveling up
	Text levelUpText;
	levelUpText.setFont(font);
	levelUpText.setCharacterSize(60*screenScaleAvg);
	levelUpText.setFillColor(Color::White);
	//levelUpText.setPosition(80, 150);
	levelUpText.setPosition(scaledPositionSetX(.25, scaledResolutionX), scaledPositionSetY(.1, scaledResolutionY));
	std::stringstream levelUpStream;
	levelUpStream <<
		"         Press a number key correlating" << endl <<
		"              to the desired upgrade" << endl << endl <<
		"1- Increased rate of fire" << endl <<
		"2- Increased clip size (next reload)" << endl <<
		"3- Increased max health" << endl <<
		"4- Increased run speed" << endl <<
		"5- More and better health pickups" << endl <<
		"6- More and better ammo pickups" << endl;
	levelUpText.setString(levelUpStream.str());

	// Ammo
	Text ammoText;
	ammoText.setFont(font);
	ammoText.setCharacterSize(50*screenScaleAvg);
	ammoText.setFillColor(Color::White);
	//ammoText.setPosition(116, 620);
	ammoText.setPosition(scaledPositionSetX(.04, scaledResolutionX), scaledPositionSetY(.9, scaledResolutionY));

	// Score
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(50*screenScaleAvg);
	scoreText.setFillColor(Color::White);
	//scoreText.setPosition(20, 0);
	scoreText.setPosition(scaledPositionSetX(.05, scaledResolutionX), scaledPositionSetY(0, scaledResolutionY));

	// High Score
	Text highScoreText;
	highScoreText.setFont(font);
	highScoreText.setCharacterSize(50 * screenScaleAvg);
	highScoreText.setFillColor(Color::White);
	//highScoreText.setPosition(980, 0);
	highScoreText.setPosition(scaledPositionSetX(.75, scaledResolutionX), scaledPositionSetY(0, scaledResolutionY));
	std::stringstream s;
	s << "HighScore:" << highScore;
	highScoreText.setString(s.str());

	// Zombies remaining
	Text zombiesRemainingText;
	zombiesRemainingText.setFont(font);
	zombiesRemainingText.setCharacterSize(50 * screenScaleAvg);
	zombiesRemainingText.setFillColor(Color::White);
	//zombiesRemainingText.setPosition(975, 620);
	zombiesRemainingText.setPosition(scaledPositionSetX(.5, scaledResolutionX), scaledPositionSetY(.9, scaledResolutionY));
	zombiesRemainingText.setString("Zombies Remaining: 100");

	// Wave number
	int wave = 0;
	Text waveNumberText;
	waveNumberText.setFont(font);
	waveNumberText.setCharacterSize(50 * screenScaleAvg);
	waveNumberText.setFillColor(Color::Red);
	//waveNumberText.setPosition(750, 620);
	waveNumberText.setPosition(scaledPositionSetX(.4, scaledResolutionX), scaledPositionSetY(.9, scaledResolutionY));
	waveNumberText.setString("Round: 0");

	// Health bar
	RectangleShape healthBar;
	healthBar.setFillColor(Color::Magenta);
	//healthBar.setPosition(300, 620);
	healthBar.setPosition(scaledPositionSetX(.15, scaledResolutionX), scaledPositionSetY(.9, scaledResolutionY));

	//HealthBar background
	RectangleShape healthBarBG;
	healthBarBG.setFillColor(Color::Red);
	healthBarBG.setPosition(scaledPositionSetX(.15, scaledResolutionX), scaledPositionSetY(.9, scaledResolutionY));

	// When did we last update the HUD?
	int framesSinceLastHUDUpdate = 0;

	// How often (in frames) should we update the HUD
	int fpsMeasurementFrameInterval = frameRate;


	/********************************************
	********************************************
	THE MAIN GAME LOOP
	********************************************
	********************************************/
	while (window.isOpen())
	{
		/*
		************
		Handle input
		************
		*/

		// Handle events
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyPressed)
			{
				// Pause a game while playing
				if (event.key.code == Keyboard::Return &&
					state == State::PLAYING)
				{
					state = State::PAUSED;
				}

				// Restart while paused
				else if (event.key.code == Keyboard::Return &&
					state == State::PAUSED)
				{
					state = State::PLAYING;
					// Reset the clock so there isn't a frame jump
					clock.restart();
				}

				// Start a new game while in GAME_OVER state
				else if (event.key.code == Keyboard::Return &&
					state == State::GAME_OVER)
				{
					state = State::LEVELING_UP;
				}

				if (state == State::PLAYING)
				{
					// reloading
					if (event.key.code == Keyboard::R)
					{
						if (bulletsSpare >= clipSize)
						{
							// Plenty of bullets. Reload.
							bulletsSpare -= clipSize - bulletsInClip;
							bulletsInClip = clipSize;
							
						}
						else if (bulletsSpare > 0)
						{
							// Only a few bullets left
							bulletsInClip = bulletsSpare;
							bulletsSpare = 0;
						}
						else
						{
							//To be added soon
						}
					}
				}

			}
		}// End event polling


		 // Handle the player quitting
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}
		/**********************
		// Handle controls while playing
		*************************/
		if (state == State::PLAYING)
		{
			//reset fror new loop
			keysdown = 0;
			twokeys = false;

			// Handle the pressing and releasing of the WASD keys
			if (Keyboard::isKeyPressed(Keyboard::W))
			{
				player.moveUp();
				keysdown++;
			}
			else
			{
				player.stopUp();
			}

			if (Keyboard::isKeyPressed(Keyboard::S))
			{
				player.moveDown();

				// Making sure our other vertical movement key isn't pressed
				if (Keyboard::isKeyPressed(Keyboard::W))
				{
					//dont want to increment here
				}
				else
				{
					keysdown++;
				}
			}
			else
			{
				player.stopDown();
			}

			if (Keyboard::isKeyPressed(Keyboard::A))
			{
				player.moveLeft();
				keysdown++;
			}
			else
			{
				player.stopLeft();
			}

			if (Keyboard::isKeyPressed(Keyboard::D))
			{
				player.moveRight();
				
				// Making sure our other Horizontal movement key isn't pressed
				if (Keyboard::isKeyPressed(Keyboard::A))
				{
					//dont want to increment here
				}
				else
				{
					keysdown++;
				}
			}
			else
			{
				player.stopRight();
			}
			//we will use this to determine whether we normalize the player speed.
			if (keysdown >= 2)
			{
				twokeys = true;
			}

			//Handle shooting bullets
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds() > 1000 / fireRate && bulletsInClip > 0)
				{
					
					// Pass the center of the player
					// and the centre of the cross-hair
					// to the shoot function
					bullets[currentBullet].shoot(
						player.getCenter().x, player.getCenter().y,
						mouseWorldPosition.x, mouseWorldPosition.y);
					
					currentBullet++;
					if (currentBullet > 99)
					{
						currentBullet = 0;
					}
					lastPressed = gameTimeTotal;

					bulletsInClip--;
				}
			} // End shooting bullet

		}// End WASD while playing

		 // Handle the levelling up state
		if (state == State::LEVELING_UP)
		{
			// Handle the player levelling up
			if (event.key.code == Keyboard::Num1)
			{
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num2)
			{
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num3)
			{
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num4)
			{
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num5)
			{
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num6)
			{
			state = State::PLAYING;
			}

			if (state == State::PLAYING)
			{
				// Prepare thelevel
				// We will modify the next two lines later
				arena.width = 1000;
				arena.height = 1000;
				arena.left = 0;
				arena.top = 0;

				// Pass the vertex array by reference 
				// to the createBackground function
				int tileSize = createBackground(background, arena, screenScaleW, screenScaleH);

				// Spawn the player in the middle of the arena
				player.spawn(arena, resolution, tileSize);

				// Configure the pickups
				healthPickup.setArena(arena,screenScaleW, screenScaleH);
				ammoPickup.setArena(arena, screenScaleW, screenScaleH);

				// Create a horde of zombies
				numZombies = 10;

				// Delete the previously allocated memory (if it exists)
				delete[] zombies;
				zombies = createHorde(numZombies, arena, screenScaleW, screenScaleH);

				// Scale our horde of zombies
				for (int i = 0; i < numZombies; i++)
				{
					zombies[i].setZombieScaleX(screenScaleW);
					zombies[i].setZombieScaleY(screenScaleH);
					zombies[i].updateSpriteScale();
				}
				numZombiesAlive = numZombies;

				// Reset the clock so there isn't a frame jump
				clock.restart();
			}
		}// End levelling up

		 /*
		 ****************
		 UPDATE THE FRAME
		 ****************
		 */
		if (state == State::PLAYING)
		{
			// Update the delta time
			Time dt = clock.restart();
			// Update the total game time
			gameTimeTotal += dt;
			// Make a decimal fraction of 1 from the delta time
			float dtAsSeconds = dt.asSeconds();

			// Where is the mouse pointer
			mouseScreenPosition = Mouse::getPosition();


			// Convert mouse position to world coordinates of mainView
			mouseWorldPosition = window.mapPixelToCoords(
				Mouse::getPosition(), mainView);

			// Set the crosshair to the mouse world location
			spriteCrosshair.setPosition(mouseWorldPosition);

			// Update the player
			player.update(dtAsSeconds, Mouse::getPosition(), twokeys);

			// Make a note of the players new position
			Vector2f playerPosition(player.getCenter());

			// Make the view centre around the player				
			mainView.setCenter(player.getCenter());

			// Loop through each Zombie and update them
			for (int i = 0; i < numZombies; i++)
			{
				if (zombies[i].isAlive())
				{
					zombies[i].update(dt.asSeconds(), playerPosition);
				}
			}

			// Update any bullets that are in-flight
			for (int i = 0; i < 100; i++)
			{
				if (bullets[i].isInFlight())
				{
					bullets[i].update(dtAsSeconds);
				}
			}

			// Update the pickups
			healthPickup.update(dtAsSeconds);
			ammoPickup.update(dtAsSeconds);

			// Collision detection
			// Have any zombies been shot?
			for (int i = 0; i < 100; i++)
			{
				for (int j = 0; j < numZombies; j++)
				{
					if (bullets[i].isInFlight() &&
						zombies[j].isAlive())
					{
						if (bullets[i].getPosition().intersects
						(zombies[j].getPosition()))
						{
							// Stop the bullet
							bullets[i].stop();

							// Register the hit and see if it was a kill
							if (zombies[j].hit()) {
								// Not just a hit but a kill too
								score += 10;
								if (score >= highScore)
								{
									highScore = score;
								}

								numZombiesAlive--;

								// When all the zombies are dead (again)
								if (numZombiesAlive == 0) {
									state = State::LEVELING_UP;
								}
							}

						}
					}

				}
			}// End zombie being shot

			//Have any zombies touched the player			
			for (int i = 0; i < numZombies; i++)
			{
				if (player.getPosition().intersects
				(zombies[i].getPosition()) && zombies[i].isAlive())
				{

					if (player.hit(gameTimeTotal))
					{
						 //More here later
						//std::cout << "We were hit" << endl;
					}

					if (player.getHealth() <= 0)
					{
						state = State::GAME_OVER;

					}
				}
			}// End player touched

			 // Has the player touched health pickup
			if (player.getPosition().intersects
			(healthPickup.getPosition()) && healthPickup.isSpawned())
			{
				std::cout << "We got some health back" << endl;
				player.increaseHealthLevel(healthPickup.gotIt());

			}

			// Has the player touched ammo pickup
			if (player.getPosition().intersects
			(ammoPickup.getPosition()) && ammoPickup.isSpawned())
			{
				std::cout << "We got some bullets back" << endl;
				bulletsSpare += ammoPickup.gotIt();

			}

			// Size the health bar
			healthBar.setSize(Vector2f(player.getHealth() * 3 * screenScaleW, 60 * screenScaleH));
			healthBarBG.setSize(Vector2f(player.getMaxHealth() * 3 * screenScaleW, 60 * screenScaleH));
			

			// Increment the number of frames sinc ethe last HUD calculation
			framesSinceLastHUDUpdate++;
			//Calculate FPS every ffpsMeasurementFrameInterval frame
			if (framesSinceLastHUDUpdate > fpsMeasurementFrameInterval)
			{
				std::cout << "we are updating the HUD" << endl;
				// Update the HUD text
				std::stringstream ssAmmo;
				std::stringstream ssScore;
				std::stringstream ssHighScore;
				std::stringstream ssWave;
				std::stringstream ssZombiesAlive;

				//Update the ammo text
				ssAmmo << bulletsInClip << "/" << bulletsSpare;
				ammoText.setString(ssAmmo.str());

				// Update the score text
				ssScore << "Score: " << score;
				scoreText.setString(ssScore.str());

				// Update the high score text
				ssHighScore << " High Score: " << highScore;
				highScoreText.setString(ssHighScore.str());

				// Update the wave
				ssWave << "Wave: " << wave;
				waveNumberText.setString(ssWave.str());

				// Update the remaining zombies text
				ssZombiesAlive << "Zombies: " << numZombiesAlive;
				zombiesRemainingText.setString(ssZombiesAlive.str());

				framesSinceLastHUDUpdate = 0;
			}// End HUD update
			

		}// End updating the scene

		 /*
		 **************
		 Draw the scene
		 **************
		 */

		if (state == State::PLAYING)
		{
			window.clear();

			// set the mainView to be displayed in the window
			// And draw everything related to it
			window.setView(mainView);

			// Draw the background
			window.draw(background, &textureBackground);

			// Draw the zombies
			for (int i = 0; i < numZombies; i++)
			{
				window.draw(zombies[i].getSprite());
			}

			// Draw the bullets
			for (int i = 0; i < 100; i++)
			{
				if (bullets[i].isInFlight())
				{
					window.draw(bullets[i].getShape());
				}
			}

			// Draw the player
			window.draw(player.getSprite());

			// Draw the pick-ups, if currently spawned
			if (ammoPickup.isSpawned())
			{
				window.draw(ammoPickup.getSprite());
				
			}
			if (healthPickup.isSpawned())
			{
				window.draw(healthPickup.getSprite());
			}
		
			// Draw the crosshair
			window.draw(spriteCrosshair);

			// Switch to the HUD view
			window.setView(hudView);

			// Draw all the HUD elements
			window.draw(spriteAmmoIcon);
			window.draw(ammoText);
			window.draw(scoreText);
			window.draw(highScoreText);
			window.draw(healthBarBG);
			window.draw(healthBar);
			window.draw(waveNumberText);
			window.draw(zombiesRemainingText);
		}

		if (state == State::LEVELING_UP)
		{
			window.draw(spriteGameOver);
			window.draw(levelUpText);
		}

		if (state == State::PAUSED)
		{
			window.draw(pausedText);
		}

		if (state == State::GAME_OVER)
		{
			window.draw(spriteGameOver);
			window.draw(gameOverText);
			window.draw(scoreText);
			window.draw(highScoreText);
		}

		window.display();
		
		

	}// End game loop
	
	return 0;
}



/*******************
ZombieArena.h Function Definitions
********************/


//take in scaledResolutionX for width and then decide the percentage you want
float scaledPositionSetX(float xpospercent, float screenwidth)
{
	return screenwidth * xpospercent;
}


//take in scaledResolutionY for width and then decide the percentage you want
float scaledPositionSetY( float ypospercent, float screenheight)
{
	return screenheight * ypospercent;
}


// take in screenScaleW and the objects x scale
float scaleObjectX(float screenScale, float objectScaleX)
{
	return (screenScale * objectScaleX);
}


// take in screenScaleH and the objects y scale
float scaleObjectY(float screenScale, float objectScaleY)
{
	return (screenScale * objectScaleY);
}
