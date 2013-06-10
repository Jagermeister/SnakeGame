#ifndef GAMEITEMLIST_H
#define GAMEITEMLIST_H

#include "GameItem.h"

class GameItemList
{
public:
	GameItem * _head;
	GameItem * _tail; // used for adds
	GameItemList(GameItem * Head)
	{
		_head = _tail = Head;
	}

	void Add(GameItem *);
	void AddCopy(GameItem *);
	GameItem * Get(Point *);
	GameItem * Get(int);
	int Remove(GameItem *);
	int Length_Get();
	
	__declspec(property(get = Length_Get)) int Length;
};

#endif