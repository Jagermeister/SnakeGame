#include "ScoreList.h"
#include "Score.h"

void ScoreList::ScoreAdd(int Fruit, int Points)
{
	if(_first == 0)
	{
		_first = new Score(Fruit, Points);
	}
	else
	{
		Score * _prev = 0, * s = _first;
		while(s != 0)
		{
			if(s->Fruit < Fruit || (s->Fruit == Fruit && s->Points < Points))
			{
				if(_prev == 0)
				{
					_first = new Score(Fruit, Points);
					_first->Next = s;
				}
				else
				{
					_prev->Next = new Score(Fruit, Points);
					_prev->Next->Next = s;
				}

				return;
			}

			_prev = s;
			s = s->Next;
		}

		if(_prev != 0)
		{
			_prev->Next = new Score(Fruit, Points);
		}
	}
}

Score * ScoreList::FirstGet()
{
	return _first;
}