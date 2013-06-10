#include "GameItem.h"

Point * GameItem::Current_Get()
{
	return _current; 
}

void GameItem::Current_Set(Point * c)
{
	_current = c;
}

Point * GameItem::Previous_Get()
{
	return _previous;
}

void GameItem::Previous_Set(Point * p) {
	_previous = p;
}

void GameItem::Next_Set(GameItem *Next)
{
	_next = Next;
}

GameItem * GameItem::Next_Get()
{
	return _next;
}