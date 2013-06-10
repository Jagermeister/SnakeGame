#ifndef GAMEDIRECTOR_H
#define GAMEDIRECTOR_H

#include <d2d1.h>
#include <dwrite.h>
#include "GameItemList.h"
#include "GameLevel.h"
#include "FruitList.h"
#include "Apple.h"
#include "TrapList.h"
#include "SnakeList.h"
#include "StatList.h"
#include "Scorelist.h"

enum GameState { Menu, Running, Pause, GameOver };
class GameDirector
{
	ID2D1Factory                *pFactory;
    ID2D1HwndRenderTarget       *pRenderTarget;

	ID2D1SolidColorBrush		*_solidBrush;
    IDWriteTextFormat* pITextFormat;
    IDWriteFactory* pIDWriteFactory;

	long _timeStart;

	HRESULT CreateGraphicsResources(HWND hwnd);
    HRESULT CreateDeviceDependentResources();
    void    DiscardDeviceDependentResources();
	void Destory_DeviceDependent();

	SnakeList * _snakelist;
	Fruit * _fruit;
	TrapList * _traplist;
	GameItemList * _pilllist;
	StatList * _statlist;
	ScoreList * _scorelist;
	GameState _gamestate;
	int _menuKey;

public:
	GameDirector()
		  : pFactory(0),
			pRenderTarget(0),
			_solidBrush(0),
			pITextFormat(0),
			pIDWriteFactory(0)
	{
		LevelCurrent = new GameLevel();
		LevelCurrent->GameGridSize = 16;
		LevelCurrent->GameSpeedStart = 500;
		LevelCurrent->GameSpeedMax = 800;
		LevelCurrent->GameSpeedMin = 200;
		LevelCurrent->StartingPosition = new Point(3, 2);
		//Fruit * f = (Fruit *)new Apple(0);
		//LevelCurrent->FruitPool = new FruitList(f);
		//LevelCurrent->FruitPool = new FruitList((Fruit *)new Apple(0));
		LevelCurrent->PillTimeout = 15000;
		LevelCurrent->TrapMax = 10;
		LevelCurrent->TrapTimeout = 10000;

		winRect = new RECT();
		winRect->top = 200;
		winRect->left = 100;
		winRect->bottom = 1200;
		winRect->right = 900;

		GAME_GRID_SIZE = 16;
		BLOCK_SIZE = 50;

		srand (time(0));
		_scorelist = new ScoreList();
		GameLevelPlay(LevelCurrent);

		GAMESPEED = 500;
	}
	
	void WinMsg_Create();
	void WinMsg_Destroy();
	void WinMsg_DisplayChange();

	void OnKeyDown(WPARAM key);

	void Render();
	void RenderScene();
	void UpdateGame();

	void GameLevelPlay(GameLevel *);
	void FruitAdd();

	GameItemList * GameItems_Get();
	GameItem * GameItem_Get(Point *);
	GameLevel * LevelCurrent;
	HWND _hWnd;

	int GAMESPEED, GAME_GRID_SIZE, BLOCK_SIZE;
	RECT * winRect;
};

#endif