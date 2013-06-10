#ifndef GAMEITEM_H
#define GAMEITEM_H

#include "Point.h"

class GameItem
{
public:
	Point * _current;
	Point * _previous;
	GameItem * _next;

	GameItem(int)
	{
		_current = _previous = 0;
		_next = 0;
	}

	GameItem(Point * Location)
	{
		_current = Location;
		_previous = Location;
		_next = 0;
	}

	~GameItem()
	{
		_next = 0;
		delete _current;
		delete _previous;
	}

	Point * Current_Get();
	void Current_Set(Point *);
	Point * Previous_Get();
	void Previous_Set(Point *);
	GameItem * Next_Get();
	void Next_Set(GameItem *);

	__declspec(property(get = Current_Get, put = Current_Set)) Point * Current;
	__declspec(property(get = Previous_Get, put = Previous_Set)) Point * Previous;
	__declspec(property(get = Next_Get, put = Next_Set)) GameItem * Next;
};

#endif