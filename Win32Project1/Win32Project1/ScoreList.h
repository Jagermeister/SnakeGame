#ifndef SCORELIST_H
#define SCORELIST_H

#include "Score.h"

class ScoreList
{
	Score * _first;
	int _scoresMax, _scores;
public:
	ScoreList()
	{
		_first = 0;
		_scores = 0;
		_scoresMax = 10;
	}

	void ScoreAdd(int, int);
	Score * FirstGet();
};

#endif