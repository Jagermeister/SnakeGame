#include "SnakeList.h"

GameItem * SnakeList::Head_Get()
{
	return _head;
}

GameItem * SnakeList::Tail_Get()
{
	return _tail;
}


void SnakeList::Bearing_Set(CardinalDirection cd)
{
	if(
	((_current == CardinalDirection::East || _current == CardinalDirection::West) && (cd == CardinalDirection::North || cd == CardinalDirection::South))
	||
	((_current == CardinalDirection::North || _current == CardinalDirection::South) && (cd == CardinalDirection::East || cd == CardinalDirection::West))
	)
	{
		_pending = cd;
	}
}

CardinalDirection SnakeList::Bearing_Get()
{
	return _current;
}


void SnakeList::SnakeAdd(GameItem *Next)
{
	if(_head == 0)
	{
		_head = _tail = Next;
	}
	else
	{
		_tail->Next = Next;
		_tail = Next;
	}

	_length++;
}

void SnakeList::SnakeAdd()
{
	if(_head == 0)
	{
		return;
	}
	else
	{
		SnakeAdd(new GameItem(_tail->Previous));
	}
}

bool SnakeList::SnakeExists(Point * point)
{
	if(_head == 0)
		return false;

	GameItem * go = _head;
	while(go != 0)
	{
		if(go->Current->X == point->X && go->Current->Y == point->Y)
		{
			return true;
		}

		go = go->Next;
	}

	return false;
}

bool SnakeList::SnakeBodyExist(Point * point)
{
	if(_head == 0)
		return false;

	if(_head->Next == 0)
		return false;

	GameItem * go = _head->Next;
	while(go != 0)
	{
		if(go->Current->X == point->X && go->Current->Y == point->Y)
		{
			return true;
		}

		go = go->Next;
	}

	return false;
}

void SnakeList::SnakeUpdate()
{
	GameItem * go = _head;
	Point * prev = _head->Current;
	go->Previous = prev;
	switch(_pending)
	{
		case CardinalDirection::East:
		go->Current = new Point(prev->X + 1, prev->Y);
		break;
		case CardinalDirection::North:
		go->Current = new Point(prev->X, prev->Y - 1);
		break;
		case CardinalDirection::South:
		go->Current = new Point(prev->X, prev->Y + 1);
		break;
		case CardinalDirection::West:
		go->Current = new Point(prev->X - 1, prev->Y);
		break;
	}

	_current = _pending;
		
	go = go->Next;
	while(go != 0)
	{
		Point * p = go->Current;
		go->Previous = p;
		go->Current = prev;
		go = go->Next;
		prev = p;
	}
}