#ifndef STATLIST_H
#define STATLIST_H

#include <map>
#include <string>

using namespace std;

typedef std::map<std::string, int> StatMap;

class StatList
{
	StatMap _snakegame;
public:
	StatList()
	{
		_snakegame.insert(pair<string, int>("fruit", 0));
		_snakegame.insert(pair<string, int>("level", 0));
		_snakegame.insert(pair<string, int>("lives", 3));
		_snakegame.insert(pair<string, int>("score", 0));
	}

	void StatAddUpdate(string Key, int Value);
	void StatIncrement(string Key, int Value = 1);
	void StatDecrement(string Key);
	pair<const string, int> * StatGet(string Key);
};

#endif