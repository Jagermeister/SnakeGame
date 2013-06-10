#include <windows.h>
#include <d2d1.h>
#include <assert.h>
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite.lib") 
#include "basewin.h"
#include <dwrite.h>
#include <tchar.h>
#include <string>
#include <time.h>
#include <math.h>
#include <map>

#include "GameDirector.h"



using namespace std;
/*
template <class T> void SafeRelease(T **ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = 0;
    }
}*/

/*

class SlowPill
{
public:
	Point * _location;
	SlowPill * _next;
	SlowPill(Point * location)
	{
		_location = location;
		_next = 0;
	}
};

class PillList
{
	public:
	SlowPill * _head, * _tail;
	int _timelastremove;

	PillList()
	{
		_head = _tail = 0;
		_timelastremove = 0;
	}

	void Add(SlowPill * fruit)
	{
		if(_head == 0)
		{
			_head = _tail = fruit;
		}
		else
		{
			_tail->_next = fruit;
			_tail = fruit;
		}
	}

	int Remove(Point * location)
	{
		if(_head == 0)
			return - 1;

		SlowPill * f = _head, * prev = _head;
		int index = 0;

		if(location->X == _head->_location->X && location->Y == _head->_location->Y)
		{
			f = f->_next;
			if(_tail == _head)
			{
				_tail = f;
			}
			
			_timelastremove = GetTickCount();
			delete _head;
			_head = f;
			return index;
		}
		
		while(f->_next != 0)
		{
			f = f->_next;
			index++;
			if(location->X == f->_location->X && location->Y == f->_location->Y)
			{
				_timelastremove = GetTickCount();
				f = f->_next;
				delete &_head;
				_head = f;
				return index;
			}

			prev = f;
		}

		return -1;
	}
};

class PitTrap
{
public:
	Point * _location;
	PitTrap * _next;
	PitTrap(Point * location)
	{
		_location = location;
		_next = 0;
	}
};

class TrapList
{
	public:
	PitTrap * _head, * _tail;
	int _timelastadd;

	TrapList()
	{
		_head = _tail = 0;
		_timelastadd = 0;
	}

	void Add(PitTrap * fruit)
	{
		_timelastadd = GetTickCount();
		if(_head == 0)
		{
			_head = _tail = fruit;
		}
		else
		{
			_tail->_next = fruit;
			_tail = fruit;
		}
	}

	int Remove(Point * location)
	{
		if(_head == 0)
			return - 1;

		PitTrap * f = _head, * prev = _head;
		int index = 0;

		if(location->X == _head->_location->X && location->Y == _head->_location->Y)
		{
			f = f->_next;
			if(_tail == _head)
			{
				_tail = f;
			}

			delete _head;
			_head = f;
			return index;
		}
		
		while(f->_next != 0)
		{
			f = f->_next;
			index++;
			if(location->X == f->_location->X && location->Y == f->_location->Y)
			{
				f = f->_next;
				delete _head;
				_head = f;
				return index;
			}

			prev = f;
		}

		return -1;
	}
};

class Fruit
{
public:
	Point * _location;
	Fruit * _next;
	Fruit(Point * location)
	{
		_location = location;
		_next = 0;
	}
};

class FruitList
{
public:
	Fruit * _head, * _tail;

	FruitList()
	{
		_head = _tail = 0;
	}

	void Add(Fruit * fruit)
	{
		if(_head == 0)
		{
			_head = _tail = fruit;
		}
		else
		{
			_tail->_next = fruit;
			_tail = fruit;
		}
	}

	int Remove(Point * location)
	{
		if(_head == 0)
			return - 1;

		Fruit * f = _head, * prev = _head;
		int index = 0;

		if(location->X == _head->_location->X && location->Y == _head->_location->Y)
		{
			f = f->_next;
			if(_tail == _head)
			{
				_tail = f;
			}

			delete _head;
			_head = f;
			return index;
		}
		
		while(f->_next != 0)
		{
			f = f->_next;
			index++;
			if(location->X == f->_location->X && location->Y == f->_location->Y)
			{
				f = f->_next;
				delete &_head;
				_head = f;
				return index;
			}

			prev = f;
		}

		return -1;
	}
};

class GameItem
{
	Point * _current;
	Point * _previous;
	GameItem *_next;
public:
	GameItem(Point * Current)
	{
		_current = Current;
		_previous = new Point(1, 2);
		_next = 0;
	}
	Point * Current { return _current; }
	void Current(Point * c) { _current = c; }
	Point * PreviousGet() { return _previous; }
	void PreviousSet(Point * p) { _previous = p; }
	void NextSet(GameItem * Next)
	{
		_next = Next;
	}
	GameItem * Next
	{
		return _next;

	}
};


enum CardinalDirection { North, South, East, West};
class Snake
{
	GameItem *_head;
	GameItem *_tail;
	CardinalDirection _bearing, _bearingPending;
	int _length;
public:
	Snake()
	{
		_head = 0, _tail = 0;
		_bearing = East;
		_bearingPending = East;
		_length = 0;
		SnakeAdd(new GameItem(new Point(2, 2)));
		_head->Previous = new Point(1, 2);
		SnakeAdd();
	}

	void SnakeAdd(GameItem *Next)
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

	void SnakeAdd()
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

	bool SnakeExists(Point * point)
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

	bool SnakeBodyExist(Point * point)
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

	void BearingSet(CardinalDirection cd)
	{
		if(
			((_bearing == CardinalDirection::East || _bearing == CardinalDirection::West) && (cd == CardinalDirection::North || cd == CardinalDirection::South))
			||
			((_bearing == CardinalDirection::North || _bearing == CardinalDirection::South) && (cd == CardinalDirection::East || cd == CardinalDirection::West))
			)
		{
			_bearingPending = cd;
		}
	}

	CardinalDirection BearingGet()
	{
		return _bearing;
	}

	GameItem * HeadGet()
	{
		return _head;
	}

	GameItem * TailGet()
	{
		return _tail;
	}

	int Length()
	{
		return _length;
	}

	void SnakeUpdate()
	{
		GameItem * go = _head;
		Point * prev = _head->Current;
		go->Previous = prev;
		switch(_bearingPending)
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

		_bearing = _bearingPending;
		
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
};
*/
/*
class Score
{
public:
	string _date;
	int _fruit, _score;
	Score * _next;

	Score(int fruit, int score)
	{
		_next = 0;
		_fruit = fruit;
		_score = score;
		time_t t = time(0);
		struct tm * now = localtime(&t);
		_date = to_string(now->tm_year + 1900) + "/" + to_string(now->tm_mon) + "/" + to_string(now->tm_mday) + " " + to_string(now->tm_hour) + ":" + to_string(now->tm_min) + ":" + to_string(now->tm_sec);
	}
};

class HighScore
{
	Score * _first;
	int _scoresMax, _scores;
public:
	HighScore()
	{
		_first = 0;
		_scores = 0;
		_scoresMax = 10;
	}

	void ScoreAdd(int fruit, int score)
	{
		if(_first == 0)
		{
			_first = new Score(fruit, score);
		}
		else
		{
			Score * _prev = 0, * s = _first;
			while(s != 0)
			{
				if(s->_fruit < fruit || (s->_fruit == fruit && s->_score < score))
				{
					if(_prev == 0)
					{
						_first = new Score(fruit, score);
						_first->_next = s;
					}
					else
					{
						_prev->_next = new Score(fruit, score);
						_prev->_next->_next = s;
					}

					return;
				}

				_prev = s;
				s = s->_next;
			}

			if(_prev != 0)
			{
				_prev->_next = new Score(fruit, score);
			}
		}
	}

	Score * FirstGet()
	{
		return _first;
	}
};

typedef map<string, int> StatMap;

class StatKeeper
{
	StatMap _snakegame;
public:
	StatKeeper()
	{
		_snakegame.insert(pair<string, int>("fruit", 0));
		_snakegame.insert(pair<string, int>("level", 0));
		_snakegame.insert(pair<string, int>("lives", 3));
		_snakegame.insert(pair<string, int>("score", 0));
	}

	void StatAddUpdate(string Key, int Value)
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
	
	void StatIncrement(string Key, int Value = 1)
	{
		pair<const string, int> * stat = StatGet(Key);
		if(stat != 0)
		{
			stat->second = stat->second + Value;
		}
	}

	void StatDecrement(string Key)
	{
		pair<const string, int> * stat = StatGet(Key);
		if(stat != 0)
		{
			stat->second = stat->second - 1;
		}
	}

	pair<const string, int> * StatGet(string Key)
	{
		StatMap::iterator i = _snakegame.find(Key);
		if(i != _snakegame.end())
		{
			return &*i;
		}

		return 0;
	}
};
*/

/*
enum GameState { Menu, Running, Pause, GameOver };
class gg
{
	ID2D1Factory                *m_pFactory;
    ID2D1HwndRenderTarget       *m_pRenderTarget;

	ID2D1SolidColorBrush		*_solidBrush;
    IDWriteTextFormat* pITextFormat;
    IDWriteFactory* pIDWriteFactory;

	long _timeStart;

	HRESULT CreateGraphicsResources(HWND hwnd);
    HRESULT CreateDeviceDependentResources();
    void    DiscardDeviceDependentResources();
    void    CalculateLayout();

public:
    void    RenderScene();
	void    UpdateGame();
	int GAME_GRID_SIZE, BLOCK_SIZE;
	SnakeList					*_snake;
	FruitList					*_fruitlist;
	TrapList					*_traplist;
	GameItemList				*_pilllist;
	StatList					*_statkeeper;
	GameState					_gamestate;
	ScoreList					*_highscore;
	int GAMESPEED;
	int _menuKey;
    gg() 
        :	m_pFactory(0),
			m_pRenderTarget(0),
			_solidBrush(0),
			pITextFormat(0),
			pIDWriteFactory(0)
    {
		GAME_GRID_SIZE = 16;
		BLOCK_SIZE = 50;
		_snake = new SnakeList();
		_fruitlist = new FruitList();
		_fruitlist->Add((GameItem *)new Apple(new Point(3, 6)));
		_traplist = new	TrapList();
		_pilllist = new GameItemList(0);
		_statkeeper = new StatList();
		_highscore = new ScoreList();
		_gamestate = GameState::Menu;
		_menuKey = 0;
		_timeStart = GetTickCount();
		GAMESPEED = 500;
		srand (time(0));
    }

    ~gg()
    {
        CleanUp();
    }

    HRESULT Initialize();
    void Render(HWND hwnd);
    HRESULT Resize(int x, int y);

    void CleanUp()
    {
        DiscardDeviceDependentResources();
        // Discard device-independent resources.
        SafeRelease(&m_pFactory);
		//SafeRelease(&m_pWriteFactory);
    }
};


HRESULT gg::Initialize()
{
    HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pFactory);
  
	if (SUCCEEDED(hr))
    {
		DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&pIDWriteFactory));
	}

	return hr;
}


void gg::Render(HWND hwnd)
{
    HRESULT hr = CreateGraphicsResources(hwnd);
    if (FAILED(hr))
    {
        return;
    }
	
    m_pRenderTarget->BeginDraw();

    RenderScene();

    hr = m_pRenderTarget->EndDraw();
    if (hr == D2DERR_RECREATE_TARGET)
    {
        DiscardDeviceDependentResources();
    }
}

HRESULT gg::Resize(int x, int y)
{
    HRESULT hr = S_OK;
    if (m_pRenderTarget)
    {
        hr = m_pRenderTarget->Resize( D2D1::SizeU(x, y) );
        if (SUCCEEDED(hr))
        {
            CalculateLayout();
        }
    }
    return hr;
}


HRESULT gg::CreateGraphicsResources(HWND hwnd)
{
    HRESULT hr = S_OK;
    if (m_pRenderTarget == 0)
    {
        RECT rc;
        GetClientRect(hwnd, &rc);

        hr = m_pFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(hwnd, D2D1::SizeU(rc.right, rc.bottom)),
            &m_pRenderTarget
            );

		if(SUCCEEDED(hr))
		{
			hr = pIDWriteFactory->CreateTextFormat(
				L"Courier", 
				0,
				DWRITE_FONT_WEIGHT_NORMAL, 
				DWRITE_FONT_STYLE_NORMAL, 
				DWRITE_FONT_STRETCH_NORMAL, 
				12.0f * 96.0f/72.0f, 
				L"en-US", 
				&pITextFormat
			);
		}

        if (SUCCEEDED(hr))
        {
            hr = CreateDeviceDependentResources();
        }
        if (SUCCEEDED(hr))
        {
            CalculateLayout();
        }
    }
    return hr;
}


HRESULT gg::CreateDeviceDependentResources()
{
    HRESULT hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1, 1, 1), &_solidBrush);
	return hr;
}


void gg::RenderScene()
{
	switch(_gamestate)
	{
		case GameState::Menu:
			{
				m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
				_solidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::HotPink));
				m_pRenderTarget->DrawText(
					L"Start Game",
					10,
					pITextFormat,
					D2D1::RectF(225.0f, 225.0f, 325.0f, 275.0f), 
					_solidBrush
				);
				m_pRenderTarget->DrawText(
					L"Options",
					10,
					pITextFormat,
					D2D1::RectF(225.0f, 275.0f, 325.0f, 325.0f), 
					_solidBrush
				);
				m_pRenderTarget->DrawText(
					L"Exit Game",
					9,
					pITextFormat,
					D2D1::RectF(225.0f, 325.0f, 325.0f, 375.0f), 
					_solidBrush
				);
				m_pRenderTarget->DrawRectangle(D2D1::RectF(220.0f, 225.0f + (_menuKey * 50.0f), 325.0f, 250.0f + (_menuKey * 50.0f)), _solidBrush, 2.0f);
			}
			break;
		case GameState::Pause:
			m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
			_solidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::HotPink));
			m_pRenderTarget->DrawText(
				L"Game Paused",
				11,
				pITextFormat,
				D2D1::RectF(225.0f, 225.0f, 325.0f, 275.0f), 
				_solidBrush
			);
			break;
		case GameState::Running:
			{
				m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
				_solidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
				//draw the grid -- only parameter
				m_pRenderTarget->DrawLine(
					D2D1::Point2(10.0f, 010.0f),
					D2D1::Point2(10.0f, GAME_GRID_SIZE * BLOCK_SIZE + 10.0f),
					_solidBrush,
					1.0f
				);
				m_pRenderTarget->DrawLine(
					D2D1::Point2(010.0f, 10.0f),
					D2D1::Point2(GAME_GRID_SIZE * BLOCK_SIZE + 10.0f, 10.0f),
					_solidBrush,
					1.0f
				);
				m_pRenderTarget->DrawLine(
					D2D1::Point2(GAME_GRID_SIZE * BLOCK_SIZE + 10.0f, 010.0f),
					D2D1::Point2(GAME_GRID_SIZE * BLOCK_SIZE + 10.0f, GAME_GRID_SIZE * BLOCK_SIZE + 10.0f),
					_solidBrush,
					1.0f
				);
				m_pRenderTarget->DrawLine(
					D2D1::Point2(010.0f, GAME_GRID_SIZE * BLOCK_SIZE + 10.0f),
					D2D1::Point2(GAME_GRID_SIZE * BLOCK_SIZE + 10.0f, GAME_GRID_SIZE * BLOCK_SIZE + 10.0f),
					_solidBrush,
					1.0f
				);
	
				GameItem * snakebody = _snake->Head, * next = 0, * prev = 0;
				int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
				while(snakebody != 0)
				{
					Point * p = snakebody->Current;
					next = snakebody->Next;

					if(snakebody == _snake->Head)
					{
						if(_snake->Bearing == CardinalDirection::East)	{ x1 = 0; y1 = 0; x2 = 1; y2 = 2; }
						else if(_snake->Bearing == CardinalDirection::West) { x1 = 1; y1 = 0; x2 = 2; y2 = 2; }
						else if(_snake->Bearing == CardinalDirection::North) { x1 = 0; y1 = 1; x2 = 2; y2 = 2; }
						else if(_snake->Bearing == CardinalDirection::South) { x1 = 0; y1 = 0; x2 = 2; y2 = 1; }

						//Ellipse Head
						_solidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::LimeGreen));
						m_pRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(BLOCK_SIZE * p->X + BLOCK_SIZE/2 + 10.0f, BLOCK_SIZE * p->Y + BLOCK_SIZE/2 + 10.0f), BLOCK_SIZE/2, BLOCK_SIZE/2), _solidBrush);
						//Square Neck
						m_pRenderTarget->FillRectangle(D2D1::RectF(BLOCK_SIZE * p->X + (x1 * BLOCK_SIZE/2) + 10.0f, BLOCK_SIZE * p->Y + (y1 * BLOCK_SIZE/2) + 10.0f, BLOCK_SIZE * p->X + (x2 * BLOCK_SIZE/2) + 10.0f, BLOCK_SIZE * p->Y + (y2 * BLOCK_SIZE/2) + 10.0f), _solidBrush);
					}
					else if(snakebody == _snake->Tail)
					{
						Point * p1 = prev->Current, * p2 = snakebody->Current;
						_solidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
						if(p2->X < p1->X) //EAST
						{
							m_pRenderTarget->FillRectangle(D2D1::RectF(BLOCK_SIZE * p->X + 25.0f + 10.0f, BLOCK_SIZE * p->Y + 10.0f, BLOCK_SIZE * p->X + BLOCK_SIZE + 10.0f, BLOCK_SIZE * p->Y + BLOCK_SIZE + 10.0f), _solidBrush);
						}
						else if(p2->X > p1->X) //WEST
						{
							m_pRenderTarget->FillRectangle(D2D1::RectF(BLOCK_SIZE * p->X + 10.0f, BLOCK_SIZE * p->Y + 10.0f, BLOCK_SIZE * p->X + 25.0f + 10.0f, BLOCK_SIZE * p->Y + BLOCK_SIZE + 10.0f), _solidBrush);
						}
						else if(p2->Y > p1->Y) //NORTH
						{
							m_pRenderTarget->FillRectangle(D2D1::RectF(BLOCK_SIZE * p->X + 10.0f, BLOCK_SIZE * p->Y + 10.0f, BLOCK_SIZE * p->X + BLOCK_SIZE + 10.0f, BLOCK_SIZE * p->Y + 25.0f + 10.0f), _solidBrush);
						}
						else if(p2->Y < p1->Y) //SOUTH
						{
							m_pRenderTarget->FillRectangle(D2D1::RectF(BLOCK_SIZE * p->X + 10.0f, BLOCK_SIZE * p->Y + 25.0f + 10.0f, BLOCK_SIZE * p->X + BLOCK_SIZE + 10.0f, BLOCK_SIZE * p->Y + BLOCK_SIZE + 10.0f), _solidBrush);
						}

						m_pRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(50.0f * p->X + 25.0f + 10.0f, 50.0f * p->Y + 25.0f + 10.0f), 25.0f, 25.0f), _solidBrush);
					}
					else
					{
						_solidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
						m_pRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(50.0f * p->X + 25.0f + 10.0f, 50.0f * p->Y + 25.0f + 10.0f), 25.0f, 25.0f), _solidBrush);

						Point * p1 = prev->Current, * p2 = snakebody->Current, * p3 = next->Current;
						if((p1->X > p2->X && p2->Y < p3->Y) || (p1->Y > p2->Y && p2->X < p3->X)) //TOP LEFT CORNER
						{
							m_pRenderTarget->FillRectangle(D2D1::RectF(BLOCK_SIZE * p->X + BLOCK_SIZE/2 + 10.0f, BLOCK_SIZE * p->Y + 10.0f, BLOCK_SIZE * p->X + BLOCK_SIZE + 10.0f, BLOCK_SIZE * p->Y + BLOCK_SIZE + 10.0f), _solidBrush);
							m_pRenderTarget->FillRectangle(D2D1::RectF(BLOCK_SIZE * p->X + 10.0f, BLOCK_SIZE * p->Y + BLOCK_SIZE/2 + 10.0f, BLOCK_SIZE * p->X + BLOCK_SIZE + 10.0f, BLOCK_SIZE * p->Y + BLOCK_SIZE + 10.0f), _solidBrush);
						}
						else if((p1->X > p2->X && p2->Y > p3->Y) || (p1->Y < p2->Y && p2->X < p3->X)) //BOT LEFT CORNER
						{
							m_pRenderTarget->FillRectangle(D2D1::RectF(BLOCK_SIZE * p->X + 10.0f, BLOCK_SIZE * p->Y + 10.0f, BLOCK_SIZE * p->X + BLOCK_SIZE + 10.0f, BLOCK_SIZE * p->Y + BLOCK_SIZE/2 + 10.0f), _solidBrush);
							m_pRenderTarget->FillRectangle(D2D1::RectF(BLOCK_SIZE * p->X + BLOCK_SIZE/2 + 10.0f, BLOCK_SIZE * p->Y + 10.0f, BLOCK_SIZE * p->X + BLOCK_SIZE + 10.0f, BLOCK_SIZE * p->Y + BLOCK_SIZE + 10.0f), _solidBrush);
						}
						else if((p1->X < p2->X && p2->Y < p3->Y) || (p1->Y > p2->Y && p2->X > p3->X)) //TOP RIGHT CORNER
						{
							m_pRenderTarget->FillRectangle(D2D1::RectF(BLOCK_SIZE * p->X + 10.0f, BLOCK_SIZE * p->Y + 10.0f, BLOCK_SIZE * p->X + BLOCK_SIZE/2 + 10.0f, BLOCK_SIZE * p->Y + BLOCK_SIZE + 10.0f), _solidBrush);
							m_pRenderTarget->FillRectangle(D2D1::RectF(BLOCK_SIZE * p->X + 10.0f, BLOCK_SIZE * p->Y + BLOCK_SIZE/2 + 10.0f, BLOCK_SIZE * p->X + BLOCK_SIZE + 10.0f, BLOCK_SIZE * p->Y + BLOCK_SIZE + 10.0f), _solidBrush);
						}
						else if((p1->X < p2->X && p2->Y > p3->Y) || (p1->Y < p2->Y && p2->X > p3->X)) //BOT RIGHT CORNER
						{
							m_pRenderTarget->FillRectangle(D2D1::RectF(BLOCK_SIZE * p->X + 10.0f, BLOCK_SIZE * p->Y + 10.0f, BLOCK_SIZE * p->X + BLOCK_SIZE + 10.0f, BLOCK_SIZE * p->Y + BLOCK_SIZE/2 + 10.0f), _solidBrush);
							m_pRenderTarget->FillRectangle(D2D1::RectF(BLOCK_SIZE * p->X + 10.0f, BLOCK_SIZE * p->Y + 10.0f, BLOCK_SIZE * p->X + BLOCK_SIZE/2 + 10.0f, BLOCK_SIZE * p->Y + BLOCK_SIZE + 10.0f), _solidBrush);
						}
						else
						{
							m_pRenderTarget->FillRectangle(
								D2D1::RectF(
									BLOCK_SIZE * p->X + 10.0f,
									BLOCK_SIZE * p->Y + 10.0f,
									BLOCK_SIZE * p->X + BLOCK_SIZE + 10.0f,
									BLOCK_SIZE * p->Y + BLOCK_SIZE + 10.0f)
							, _solidBrush);
						}
					}

					prev = snakebody;
					snakebody = next;
				}

				GameItem * sp = _pilllist->Get(0);
				while(sp != 0)
				{
					_solidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::LightYellow));
					m_pRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(50.0f * sp->Current->X + 25.0f + 10.0f, 50.0f * sp->Current->Y + 25.0f + 10.0f), 12.0f, 25.0f), _solidBrush);
					sp = sp->Next;
				}

				GameItem * pt = _traplist->Get(0);
				while(pt != 0)
				{
					_solidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::LightBlue));
					m_pRenderTarget->FillRectangle(D2D1::RectF(50.0f * pt->Current->X + 10.0f, 50.0f * pt->Current->Y + 10.0f, 50.0f * pt->Current->X + 50.0f + 10.0f, 50.0f * pt->Current->Y + 50.0f + 10.0f), _solidBrush);
					pt = pt->Next;
				}


				ID2D1PathGeometry * pGeo = 0;
				ID2D1GeometrySink *pSink = 0;
				m_pFactory->CreatePathGeometry(&pGeo);

				Fruit * fruit = (Fruit*)_fruitlist->Get(0);
				while(fruit != 0)
				{
					D2D1_RECT_F rectangle2 = D2D1::RectF(
						BLOCK_SIZE * fruit->Current->X + 10.0f + 1.0f,
						BLOCK_SIZE * fruit->Current->Y + 10.0f + 1.0f,
						BLOCK_SIZE * (fruit->Current->X + 1) + 10.0f - 1.0f,
						BLOCK_SIZE * (fruit->Current->Y + 1) + 10.0f - 1.0f
						);
					_solidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Purple));
					//m_pRenderTarget->FillRectangle(&rectangle2, _solidBrush);

					pGeo->Open(&pSink);
					pSink->SetFillMode(D2D1_FILL_MODE_WINDING);

					int R = 24, x = BLOCK_SIZE * fruit->Current->X + 10.0f + 25.0f, y = BLOCK_SIZE * fruit->Current->Y + 10.0f + 25.0f;
					float B2 = tanf(22.5 * 3.141592653589793238462643383279502884 / 180) * R;
					float newB = B2 * 2.0f;
					float newR = sqrtf(R * R + B2 * B2);
					float newC = sqrtf(newB * newB / 2);

					pSink->BeginFigure(
						D2D1::Point2F(x - newB / 2, y - newB / 2 - newC),
						D2D1_FIGURE_BEGIN_FILLED
						);

					D2D1_POINT_2F points[8];
					points[0].x = x - newB / 2; points[0].y = y - newB / 2 - newC;
					points[1].x = x + newB / 2; points[1].y = y - newB / 2 - newC;

					points[2].x = x + newB / 2 + newC; points[2].y = y - newB / 2;
					points[3].x = x + newB / 2 + newC; points[3].y = y + newB / 2;

					points[4].x = x + newB / 2; points[4].y = y + newB / 2 + newC;
					points[5].x = x - newB / 2; points[5].y = y + newB / 2 + newC;

					points[6].x = x - newB / 2 - newC; points[6].y = y + newB / 2;
					points[7].x = x - newB / 2 - newC; points[7].y = y - newB / 2;

					pSink->AddLines(points, ARRAYSIZE(points));
					pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
					pSink->Close();
					m_pRenderTarget->FillGeometry(pGeo, _solidBrush);
					//m_pRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), 2.0f, 2.0f), _solidBrush);
		
					_solidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::LimeGreen));
					m_pRenderTarget->DrawGeometry(pGeo, _solidBrush, 1.f);
				
					fruit = (Fruit*)fruit->Next;
				}

				SafeRelease(&pSink);


				//m_pRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(125, 125), 12.0f, 24.0f), _solidBrush);
	
				// render txt stats.
				string scoreGame = "Score: " + to_string(_statkeeper->StatGet("score")->second);
				wstring sw = wstring(scoreGame.begin(), scoreGame.end());
				_solidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::HotPink));
				m_pRenderTarget->DrawText(
					sw.c_str(),
					sw.length(),
					pITextFormat,
					D2D1::RectF(GAME_GRID_SIZE * BLOCK_SIZE + 10.0f + 15.0f, 0.0f, GAME_GRID_SIZE * BLOCK_SIZE + 10.0f + 15.0f + 100.0f, 100.0f), 
					_solidBrush
				);

				string fruitEaten = "Fruit: " + to_string(_statkeeper->StatGet("fruit")->second);
				sw = wstring(fruitEaten.begin(), fruitEaten.end());
				_solidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::HotPink));
				m_pRenderTarget->DrawText(
					sw.c_str(),
					sw.length(),
					pITextFormat,
					D2D1::RectF(GAME_GRID_SIZE * BLOCK_SIZE + 10.0f + 15.0f, 100.0f, GAME_GRID_SIZE * BLOCK_SIZE + 10.0f + 15.0f + 100.0f, 200.0f), 
					_solidBrush
				);
			}
			break;
		case GameState::GameOver:
			m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
			_solidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::HotPink));
			m_pRenderTarget->DrawText(
				L"Game Over",
				9,
				pITextFormat,
				D2D1::RectF(225.0f, 225.0f, 325.0f, 275.0f), 
				_solidBrush
			);

			Score * s = _highscore->FirstGet();
			int i = 1;
			string sc;
			wstring wsc;
			while(s != 0 && i < 6)
			{
				sc = to_string(i) + " - Fruits: " + to_string(s->Fruit) + " - Score: " + to_string(s->Points) + " - " + s->TimeScored;
				wsc = wstring(sc.begin(), sc.end());
				m_pRenderTarget->DrawText(
					wsc.c_str(),
					wsc.length(),
					pITextFormat,
					D2D1::RectF(225.0f, (i * 50.0f) + 250.0f, 650.0f, (i * 50.0f) + 275.0f), 
					_solidBrush
				);
				if(s->Fruit == _statkeeper->StatGet("fruit")->second && s->Points == _statkeeper->StatGet("score")->second)
				{
					m_pRenderTarget->DrawRectangle(D2D1::RectF(220.0f, (i * 50.0f) + 250.0f, 650.0f, (i * 50.0f) + 275.0f), _solidBrush, 2.0f);
				}
				
				s = s->Next;
				i++;
			}
			break;
	}
}

void gg::UpdateGame()
{
	if(GetTickCount() - _timeStart < GAMESPEED)
	{
		return;
	}
	_timeStart = GetTickCount();


	switch(_gamestate)
	{
		case GameState::Menu:
			{

			}
			break;
		case GameState::Running:
			{
				_snake->SnakeUpdate();
				_statkeeper->StatIncrement("score", _snake->Length);
				Point * snakeHead = _snake->Head->Current;

				//out of bounds
				//snake eat self
				//snake hits trap
				GameItem * trap = _traplist->Get(_snake->Head->Current);
				if(snakeHead->X >= GAME_GRID_SIZE || snakeHead->Y >= GAME_GRID_SIZE || snakeHead->X < 0 || snakeHead->Y < 0 || _snake->SnakeBodyExist(snakeHead) || trap != 0)
				{
					_highscore->ScoreAdd(_statkeeper->StatGet("fruit")->second, _statkeeper->StatGet("score")->second);
					_gamestate = GameState::GameOver;
					return;
				}

				if(GetTickCount() - _traplist->TimeLastAdded > 10000)
				{
					Point * p = new Point();
					do
					{
						p->X = rand() % GAME_GRID_SIZE;
						p->Y = rand() % GAME_GRID_SIZE;
					}while(_snake->SnakeExists(p));
					_traplist->Add(new GameItem(p), GetTickCount());
				}
				
				if(_pilllist->Get(0) == 0)
				{
					Point * p = new Point();
					do
					{
						p->X = rand() % GAME_GRID_SIZE;
						p->Y = rand() % GAME_GRID_SIZE;
					}while(_snake->SnakeExists(p));
					_pilllist->Add(new GameItem(p));
				}

				GameItem * pill = _pilllist->Get(_snake->Head->Current);
				if(pill != 0)
				{
					_pilllist->Remove(pill);
					GAMESPEED += 300;
				}

				Fruit * fruit = (Fruit*)_fruitlist->Get(_snake->Head->Current);
				if(fruit != 0)
				{
					if(GAMESPEED > 150)
					{
						GAMESPEED -= 50;
					}

					_statkeeper->StatIncrement("fruit");
					_statkeeper->StatIncrement("score", fruit->Score());
					_fruitlist->Remove(fruit);
					_snake->SnakeAdd();
					Point * p = new Point();
					do
					{
						p->X = rand() % GAME_GRID_SIZE;
						p->Y = rand() % GAME_GRID_SIZE;
					}while(_snake->SnakeExists(p));

					_fruitlist->Add((GameItem*)new Apple(p));
				}
			}
			break;
	}

}

void gg::CalculateLayout()
{
    D2D1_SIZE_F fSize = m_pRenderTarget->GetSize();
    const float x = fSize.width / 2.0f;
    const float y = fSize.height / 2.0f;
}

void gg::DiscardDeviceDependentResources()
{
    SafeRelease(&m_pRenderTarget);
    SafeRelease(&_solidBrush);
    SafeRelease(&pITextFormat);
    SafeRelease(&pIDWriteFactory);
}
*/

class MainWindow : public BaseWindow<MainWindow>
{
public:
    GameDirector * gDirector;
	RECT * winRect;
	MainWindow()
	{
		gDirector = new GameDirector();
	}

    PCWSTR  ClassName() const { return L"snake game"; }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    void OnKeyDown(WPARAM key);
};

// Constants 
const WCHAR WINDOW_NAME[] = L"snake game";

/*
void MainWindow::OnKeyDown(WPARAM key)
{
	if(key == VK_F11)
	{
		
		HWND hWnd = FindWindow(ClassName(), WINDOW_NAME);
		RECT workarea, windowarea;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &workarea, 0);
		GetWindowRect(hWnd, &windowarea);

		if(windowarea.bottom == workarea.bottom && windowarea.top == workarea.top)
		{
			AdjustWindowRect(winRect, WS_OVERLAPPEDWINDOW, FALSE);
			MoveWindow(hWnd, winRect->top, winRect->left, winRect->bottom, winRect->right, TRUE);
		}
		else
		{
			MoveWindow(hWnd, 0, 0, workarea.right, workarea.bottom, TRUE);
		}
	}

	switch(m_scene._gamestate)
	{
	case GameState::Menu:
		switch(key)
		{
			case VK_UP:
				if(m_scene._menuKey > 0)
					m_scene._menuKey--;
				break;
			case VK_DOWN:
				if(m_scene._menuKey < 2)
					m_scene._menuKey++;
				break;
			case VK_RETURN:
				if(m_scene._menuKey == 0)
					m_scene._gamestate = GameState::Running;
				else if(m_scene._menuKey == 1)
					m_scene._gamestate = GameState::Menu;
				else if(m_scene._menuKey == 2)
					exit(0);
				break;
		}
		break;
	case GameState::Pause:
		if(key == VK_ESCAPE || key == VK_SPACE)
			m_scene._gamestate = GameState::Running;
		break;
	case GameState::Running:
		switch(key)
		{
			case VK_LEFT:
				m_scene._snake->Bearing = CardinalDirection::West;
				break;
			case VK_RIGHT:
				m_scene._snake->Bearing = CardinalDirection::East;
				break;
			case VK_UP:
				m_scene._snake->Bearing = CardinalDirection::North;
				break;
			case VK_DOWN:
				m_scene._snake->Bearing = CardinalDirection::South;
				break;
			case VK_SPACE:
			case VK_ESCAPE:
				m_scene._gamestate = GameState::Pause;
				break;
		}
		break;
	case GameState::GameOver:
		if(key == VK_ESCAPE || key == VK_RETURN)
		{
			m_scene._gamestate = GameState::Menu;
			m_scene._snake = new SnakeList();
			m_scene._statkeeper = new StatList();
			m_scene._fruitlist = new FruitList();
			m_scene._fruitlist->Add((GameItem*)new Apple(new Point(3, 6)));
			m_scene._pilllist = new GameItemList(0);
			m_scene._traplist = new TrapList();
			m_scene.GAMESPEED = 500;
		}
		break;
}
	}*/

INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, INT nCmdShow)
{
	CoInitializeEx(0, COINIT_MULTITHREADED);
	MainWindow win;
	//win.winRect = new RECT();
	//win.winRect->top = 200;
	//win.winRect->left = 100;
	//win.winRect->bottom = 1200;
	//win.winRect->right = 900;
	win.Create(WINDOW_NAME, ( WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE ), 0, 200, 100, 1200, 900);
	win.gDirector->_hWnd = win.Window();

    ShowWindow(win.Window(), nCmdShow);

    MSG msg = { };
	const int TICKS_PER_SECOND = 25;
	const int SKIP_TICKS = 1000 / TICKS_PER_SECOND;
	const int MAX_FRAMESKIP = 10;

	DWORD next_game_tick = GetTickCount();
    int loops;
	int start_time = GetTickCount();
    while(true)
    {
    	if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
    	{
    		if(msg.message == WM_QUIT || 
                msg.message == WM_CLOSE || 
                msg.message == WM_DESTROY)
    			break;
			if (msg.message == WM_KEYDOWN)
				win.gDirector->OnKeyDown(msg.wParam);
			else
			{
    			TranslateMessage(&msg);
    			DispatchMessage(&msg);		   
			}
		}
    	else
    	{
			loops = 0;
			while( GetTickCount() > next_game_tick && loops < MAX_FRAMESKIP)
			{
				win.gDirector->UpdateGame();
				next_game_tick += SKIP_TICKS;
				loops++;
			}

			win.gDirector->Render();
    	}
    }

    return 0;
}

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HWND hwnd = m_hwnd;
	MINMAXINFO FAR * lpMinMaxInfo;

    switch (uMsg)
    {
		case WM_CREATE:
			gDirector->WinMsg_Create();
			return 0;
		case WM_DESTROY:
			gDirector->WinMsg_Destroy();
			PostQuitMessage(0);
			return 0;
		case WM_GETMINMAXINFO:
			lpMinMaxInfo = (MINMAXINFO FAR *) lParam;
			lpMinMaxInfo->ptMinTrackSize.x = 1200;
			lpMinMaxInfo->ptMinTrackSize.y = 900;
			lpMinMaxInfo->ptMaxTrackSize.x = 1200;
			lpMinMaxInfo->ptMaxTrackSize.y = 900;
			return 0;
		case WM_PAINT:
		case WM_DISPLAYCHANGE:
			{
				PAINTSTRUCT ps;
				BeginPaint(m_hwnd, &ps);
				gDirector->Render();
				EndPaint(m_hwnd, &ps);
			}
			return 0;

		//case WM_SIZE:
		//	{
		//		int x = (int)(short)LOWORD(lParam);
		//		int y = (int)(short)HIWORD(lParam);

		//		GetWindowRect(hwnd, winRect);

		//		m_scene.Resize(x,y);
		//		InvalidateRect(m_hwnd, 0, FALSE);
		//	}
		//	return 0;
		case WM_ERASEBKGND:
			return 1;
		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}