#ifndef FRUIT_H
#define FRUIT_H

#include "GameItem.h"
#include <string>

class Fruit : public GameItem
{
public:
	Fruit(int i) : GameItem(i) { }
	Fruit(Point * p) : GameItem(p) { }
	virtual std::string Name() = 0;
	virtual int Score() = 0;
};

#endif