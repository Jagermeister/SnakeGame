#ifndef FRUITLIST_H
#define FRUITLIST_H

#include "GameItemList.h"
class Fruit;

class FruitList : public GameItemList
{
public:
	FruitList() : GameItemList(0) { }
	FruitList(Fruit * Head) : GameItemList(dynamic_cast<GameItem*>((GameItem*)Head)) { }
};

#endif