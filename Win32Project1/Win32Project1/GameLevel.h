#ifndef GAMELEVEL_H
#define GAMELEVEL_H

#include "FruitList.h"

class GameLevel
{
public:
	int GameGridSize; // Square x by x grid
	int GameSpeedStart, GameSpeedMin, GameSpeedMax; // Starting speed for snake update, min and max
	Point * StartingPosition; // Starting Snake potision
	FruitList * FruitPool; // Allowed fruit
	int TrapMax, TrapTimeout; // Number of traps allowed, timeout for next trap spawn once added
	int PillTimeout; // Timeout for next spawn once removed
};

#endif