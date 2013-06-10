#include "Score.h"

int Score::Fruit_Get()
{
	return _fruit;
}

void Score::Fruit_Set(int Fruit)
{
	_fruit = Fruit;
}

int Score::Points_Get()
{
	return _points;
}

void Score::Points_Set(int Points)
{
	_points = Points;
}

Score * Score::Next_Get()
{
	return _next;
}

void Score::Next_Set(Score * Next)
{
	_next = Next;
}

std::string Score::TimeScored_Get()
{
	return _date;
}
