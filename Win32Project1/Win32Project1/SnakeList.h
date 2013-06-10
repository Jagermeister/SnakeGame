#ifndef SNAKELIST_H
#define SNAKELIST_H

#include "GameItemList.h"

enum CardinalDirection { North, South, East, West};
class SnakeList : public GameItemList
{
	CardinalDirection _current, _pending;
	int _length;
public:
	SnakeList(Point * p) : GameItemList(0)
	{ 
		_head = 0, _tail = 0;
		_current = East;
		_pending = East;
		_length = 0;
		SnakeAdd(new GameItem(p));
		_head->Previous = new Point(p->X - 1, p->Y);
		SnakeAdd();
	}

	SnakeList(GameItem * Head) : GameItemList((GameItem *)Head) { }

	GameItem * Head_Get();
	GameItem * Tail_Get();
	CardinalDirection Bearing_Get();
	void Bearing_Set(CardinalDirection);

	__declspec(property(get = Head_Get)) GameItem * Head;
	__declspec(property(get = Tail_Get)) GameItem * Tail;
	__declspec(property(get = Bearing_Get, put = Bearing_Set)) CardinalDirection Bearing;

	void SnakeAdd();
	void SnakeAdd(GameItem * Item);
	void SnakeUpdate();
	bool SnakeExists(Point *);
	bool SnakeBodyExist(Point *);
};

#endif