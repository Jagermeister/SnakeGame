#include "StatList.h"

void StatList::StatAddUpdate(string Key, int Value)
{
	pair<const string, int> * stat = StatGet(Key);
	if(stat != 0)
	{
		stat->second = Value;
	}
	else
	{
		_snakegame.insert(pair<string, int>(Key, Value));
	}
}

void StatList::StatIncrement(string Key, int Value)
{
	pair<const string, int> * stat = StatGet(Key);
	if(stat != 0)
	{
		stat->second = stat->second + Value;
	}
}

void StatList::StatDecrement(string Key)
{
	pair<const string, int> * stat = StatGet(Key);
	if(stat != 0)
	{
		stat->second = stat->second - 1;
	}
}

pair<const string, int> * StatList::StatGet(string Key)
{
	StatMap::iterator i = _snakegame.find(Key);
	if(i != _snakegame.end())
	{
		return &*i;
	}

	return 0;
}