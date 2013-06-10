#ifndef TRAPLIST_H
#define TRAPLIST_H

#include "GameItemList.h"

class TrapList : public GameItemList
{
public:
	TrapList() : GameItemList(0)
	{
		TimeLastAdded = 0;
	}
	TrapList(GameItem * Head) : GameItemList((GameItem *)Head) { }

	int TimeLastAdded;

	void Add(GameItem * Item, int TimeCurrent)
	{
		TimeLastAdded = TimeCurrent;
		GameItemList::Add(Item);
	}
};

#endif