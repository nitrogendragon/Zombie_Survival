#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include "Zombie.h"
using namespace sf;
Zombie* createHorde(int numZombies, IntRect arena);
//will set xpos of object based on resolution/window size...
//meant for text objects, HUD, UI, etc...
float scaledPositionSetX(float xpospercent, float screenwidth);

//will set ypos of object based on resolution/window size...
//meant for text objects, HUD, UI, etc...
float scaledPositionSetY(float ypospercent, float screenheight);

//scales objects x dimensions based on screens scale(for floats)
float scaleObjectFloatX(float screenScale, float objectDimX);

// Scales objects y dimensions based on screens scale(for floats)
float scaleObjectFloatY(float screenScale, float objectDimY);

// Scales objects x dimensions based on screens scale(for ints)
float scaleObjectIntY(float screenScale, int objectDimX);
// Scales objects y dimensions based on screens scale(for ints)
float scaleObjectIntY(float screenScale, int objectDimY);