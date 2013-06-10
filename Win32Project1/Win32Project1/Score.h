#ifndef SCORE_H
#define SCORE_H

#include <string>
#include <time.h>

class Score
{
	std::string _date;
	int _fruit, _points;
	Score * _next;
public:
	Score(int Fruit, int Points)
	{
		_next = 0;
		_fruit = Fruit;
		_points = Points;
		time_t t = time(0);
		struct tm * now = localtime(&t);
		_date = std::to_string(now->tm_year + 1900) + "/" + std::to_string(now->tm_mon) + "/" + std::to_string(now->tm_mday) + " " + std::to_string(now->tm_hour) + ":" + std::to_string(now->tm_min) + ":" + std::to_string(now->tm_sec);
	}
	
	int Fruit_Get();
	void Fruit_Set(int);
	int Points_Get();
	void Points_Set(int);
	Score * Next_Get();
	void Next_Set(Score *);
	std::string TimeScored_Get();
	
	__declspec(property(get = Fruit_Get, put = Fruit_Set)) int Fruit;
	__declspec(property(get = Points_Get, put = Points_Set)) int Points;
	__declspec(property(get = Next_Get, put = Next_Set)) Score * Next;
	__declspec(property(get = TimeScored_Get)) std::string TimeScored;
};

#endif