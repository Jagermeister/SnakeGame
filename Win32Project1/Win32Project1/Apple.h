#ifndef APPLE_H
#define APPLE_H

#include "Fruit.h"

class Apple : public Fruit
{
public:
	Apple(int i) : Fruit(i) { }
	Apple(Point * p) : Fruit(p) { }
	std::string Name();
	int Score();
};

#endif APPLE_H