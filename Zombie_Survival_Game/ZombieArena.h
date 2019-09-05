#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include "Zombie.h"
using namespace sf;
Zombie* createHorde(int numZombies, IntRect arena, float screenScaleW, float screenScaleH);
//will set xpos of object based on resolution/window size...
//meant for text objects, HUD, UI, etc...
float scaledPositionSetX(float xpospercent, float screenwidth);


//will set ypos of object based on resolution/window size...
//meant for text objects, HUD, UI, etc...
float scaledPositionSetY(float ypospercent, float screenheight);


// Scales objects x dimensions based on screens scale(for ints)
float scaleObjectX(float screenScale, float objectScaleX);


// Scales objects y dimensions based on screens scale(for ints)
float scaleObjectY(float screenScale, float objectScaleY);

//center the mouse
void centerMouse(int xResolution, int yResolution);

// takes the amount of screen pixels our mouse moved and adds the difference to our "fake" mouseScreenPosition
Vector2i calcMouseMovement(Vector2i newPosition);