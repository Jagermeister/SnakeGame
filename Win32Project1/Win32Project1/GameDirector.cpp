#include "GameDirector.h"

template <class T> void SafeRelease(T **ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = 0;
    }
}

//changing gameitem->next is changing snake object not new gameitems
GameItemList * GameDirector::GameItems_Get()
{
	GameItemList * gameitems = new GameItemList(0);
	gameitems->AddCopy(_snakelist->Get(0));
	if(_fruit != 0)
	{
		gameitems->AddCopy(_fruit);
	}

	if(_traplist->Get(0) != 0)
	{
		gameitems->AddCopy(_traplist->Get(0));
	}

	if(_pilllist->Get(0) != 0)
	{
		gameitems->AddCopy(_pilllist->Get(0));
	}

	return gameitems;
}

GameItem * GameDirector::GameItem_Get(Point * point)
{
	GameItemList * gameitems = GameItems_Get();
	GameItem * item = gameitems->Get(0);
	while(item != 0)
	{
		if(item->Current->X == point->X && item->Current->Y == point->Y)
		{
			break;
		}

		item = item->Next;
	}

	return item;
}

void GameDirector::WinMsg_Create()
{
    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory);
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&pIDWriteFactory));
}

void GameDirector::WinMsg_Destroy()
{
	Destory_DeviceDependent();
	SafeRelease(&pIDWriteFactory);
    SafeRelease(&pFactory);
}

void GameDirector::Destory_DeviceDependent()
{
    SafeRelease(&pRenderTarget);
    SafeRelease(&_solidBrush);
    SafeRelease(&pITextFormat);
}

void GameDirector::WinMsg_DisplayChange()
{
    pRenderTarget->BeginDraw();
    RenderScene();
    pRenderTarget->EndDraw();
}

#include <typeinfo>
void GameDirector::FruitAdd()
{
	_fruit = 0;
	Point * p = new Point();
	do
	{
		p->X = rand() % GAME_GRID_SIZE;
		p->Y = rand() % GAME_GRID_SIZE;
	}while(GameItem_Get(p) != 0);

	//jfc
	//Fruit * genericFruit = (Fruit*)LevelCurrent->FruitPool->Get(rand() % LevelCurrent->FruitPool->Length);
	//if(typeid(*genericFruit) == typeid(Apple))
	//{
		_fruit = new Apple(p);
	//}
}

void GameDirector::GameLevelPlay(GameLevel *  gl)
{
	_menuKey = 0;
	_timeStart = GetTickCount();
	_gamestate = GameState::Menu;
	_snakelist = new SnakeList(LevelCurrent->StartingPosition);
	_statlist = new StatList();
	_pilllist = new GameItemList(0);
	_traplist = new TrapList();
	FruitAdd();
	GAMESPEED = LevelCurrent->GameSpeedStart;
}

void GameDirector::OnKeyDown(WPARAM key)
{
	if(key == VK_F11)
	{
		RECT workarea, windowarea;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &workarea, 0);
		GetWindowRect(_hWnd, &windowarea);

		if(windowarea.bottom == workarea.bottom && windowarea.top == workarea.top)
		{
			//AdjustWindowRect(winRect, WS_OVERLAPPEDWINDOW, FALSE);
			MoveWindow(_hWnd, winRect->top, winRect->left, winRect->bottom, winRect->right, TRUE);
		}
		else
		{
			MoveWindow(_hWnd, 0, 0, workarea.right, workarea.bottom, TRUE);
		}
	}

	switch(_gamestate)
	{
	case GameState::Menu:
		switch(key)
		{
			case VK_UP:
				if(_menuKey > 0)
					_menuKey--;
				break;
			case VK_DOWN:
				if(_menuKey < 2)
					_menuKey++;
				break;
			case VK_RETURN:
				if(_menuKey == 0)
					_gamestate = GameState::Running;
				else if(_menuKey == 1)
					_gamestate = GameState::Menu;
				else if(_menuKey == 2)
					exit(0);
				break;
		}
		break;
	case GameState::Pause:
		if(key == VK_ESCAPE || key == VK_SPACE)
			_gamestate = GameState::Running;
		break;
	case GameState::Running:
		switch(key)
		{
			case VK_LEFT:
				_snakelist->Bearing = CardinalDirection::West;
				break;
			case VK_RIGHT:
				_snakelist->Bearing = CardinalDirection::East;
				break;
			case VK_UP:
				_snakelist->Bearing = CardinalDirection::North;
				break;
			case VK_DOWN:
				_snakelist->Bearing = CardinalDirection::South;
				break;
			case VK_SPACE:
			case VK_ESCAPE:
				_gamestate = GameState::Pause;
				break;
		}
		break;
	case GameState::GameOver:
		if(key == VK_ESCAPE || key == VK_RETURN)
		{
			GameLevelPlay(LevelCurrent);
		}
		break;
	}
}

void GameDirector::UpdateGame()
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
				_snakelist->SnakeUpdate();
				_statlist->StatIncrement("score", _snakelist->Length);
				Point * snakeHead = _snakelist->Head->Current;

				//GameItemList * allItems = GameItems_Get();
				//check for win conditions of level



				//out of bounds
				//snake eat self
				//snake hits trap
				GameItem * trap = _traplist->Get(_snakelist->Head->Current);
				if(snakeHead->X >= GAME_GRID_SIZE || snakeHead->Y >= GAME_GRID_SIZE || snakeHead->X < 0 || snakeHead->Y < 0 || _snakelist->SnakeBodyExist(snakeHead) || trap != 0)
				{
					_scorelist->ScoreAdd(_statlist->StatGet("fruit")->second, _statlist->StatGet("score")->second);
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
					}while(_snakelist->SnakeExists(p));
					_traplist->Add(new GameItem(p), GetTickCount());
				}
				
				if(_pilllist->Get(0) == 0)
				{
					Point * p = new Point();
					do
					{
						p->X = rand() % GAME_GRID_SIZE;
						p->Y = rand() % GAME_GRID_SIZE;
					}while(_snakelist->SnakeExists(p));
					_pilllist->Add(new GameItem(p));
				}

				GameItem * pill = _pilllist->Get(_snakelist->Head->Current);
				if(pill != 0)
				{
					_pilllist->Remove(pill);
					GAMESPEED += 300;
				}

				if(_fruit->Current->X == _snakelist->Head->Current->X && _fruit->Current->Y == _snakelist->Head->Current->Y)
				{
					if(GAMESPEED > 150)
					{
						GAMESPEED -= 50;
					}

					_statlist->StatIncrement("fruit");
					_statlist->StatIncrement("score", _fruit->Score());
					_snakelist->SnakeAdd();
					Point * p = new Point();
					do
					{
						p->X = rand() % GAME_GRID_SIZE;
						p->Y = rand() % GAME_GRID_SIZE;
					}while(_snakelist->SnakeExists(p));
					
					FruitAdd();
				}
			}
			break;
	}
}

void GameDirector::RenderScene()
{
	switch(_gamestate)
	{
		case GameState::Menu:
			{
				pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
				_solidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::HotPink));
				pRenderTarget->DrawText(
					L"Start Game",
					10,
					pITextFormat,
					D2D1::RectF(225.0f, 225.0f, 325.0f, 275.0f), 
					_solidBrush
				);
				pRenderTarget->DrawText(
					L"Options",
					10,
					pITextFormat,
					D2D1::RectF(225.0f, 275.0f, 325.0f, 325.0f), 
					_solidBrush
				);
				pRenderTarget->DrawText(
					L"Exit Game",
					9,
					pITextFormat,
					D2D1::RectF(225.0f, 325.0f, 325.0f, 375.0f), 
					_solidBrush
				);
				pRenderTarget->DrawRectangle(D2D1::RectF(220.0f, 225.0f + (_menuKey * 50.0f), 325.0f, 250.0f + (_menuKey * 50.0f)), _solidBrush, 2.0f);
			}
			break;
		case GameState::Pause:
			pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
			_solidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::HotPink));
			pRenderTarget->DrawText(
				L"Game Paused",
				11,
				pITextFormat,
				D2D1::RectF(225.0f, 225.0f, 325.0f, 275.0f), 
				_solidBrush
			);
			break;
		case GameState::Running:
			{
				pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
				_solidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
				//draw the grid -- only parameter
				pRenderTarget->DrawLine(
					D2D1::Point2(10.0f, 010.0f),
					D2D1::Point2(10.0f, GAME_GRID_SIZE * BLOCK_SIZE + 10.0f),
					_solidBrush,
					1.0f
				);
				pRenderTarget->DrawLine(
					D2D1::Point2(010.0f, 10.0f),
					D2D1::Point2(GAME_GRID_SIZE * BLOCK_SIZE + 10.0f, 10.0f),
					_solidBrush,
					1.0f
				);
				pRenderTarget->DrawLine(
					D2D1::Point2(GAME_GRID_SIZE * BLOCK_SIZE + 10.0f, 010.0f),
					D2D1::Point2(GAME_GRID_SIZE * BLOCK_SIZE + 10.0f, GAME_GRID_SIZE * BLOCK_SIZE + 10.0f),
					_solidBrush,
					1.0f
				);
				pRenderTarget->DrawLine(
					D2D1::Point2(010.0f, GAME_GRID_SIZE * BLOCK_SIZE + 10.0f),
					D2D1::Point2(GAME_GRID_SIZE * BLOCK_SIZE + 10.0f, GAME_GRID_SIZE * BLOCK_SIZE + 10.0f),
					_solidBrush,
					1.0f
				);
	
				GameItem * snakebody = _snakelist->Head, * next = 0, * prev = 0;
				int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
				while(snakebody != 0)
				{
					Point * p = snakebody->Current;
					next = snakebody->Next;

					if(snakebody == _snakelist->Head)
					{
						if(_snakelist->Bearing == CardinalDirection::East)	{ x1 = 0; y1 = 0; x2 = 1; y2 = 2; }
						else if(_snakelist->Bearing == CardinalDirection::West) { x1 = 1; y1 = 0; x2 = 2; y2 = 2; }
						else if(_snakelist->Bearing == CardinalDirection::North) { x1 = 0; y1 = 1; x2 = 2; y2 = 2; }
						else if(_snakelist->Bearing == CardinalDirection::South) { x1 = 0; y1 = 0; x2 = 2; y2 = 1; }

						//Ellipse Head
						_solidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::LimeGreen));
						pRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(BLOCK_SIZE * p->X + BLOCK_SIZE/2 + 10.0f, BLOCK_SIZE * p->Y + BLOCK_SIZE/2 + 10.0f), BLOCK_SIZE/2, BLOCK_SIZE/2), _solidBrush);
						//Square Neck
						pRenderTarget->FillRectangle(D2D1::RectF(BLOCK_SIZE * p->X + (x1 * BLOCK_SIZE/2) + 10.0f, BLOCK_SIZE * p->Y + (y1 * BLOCK_SIZE/2) + 10.0f, BLOCK_SIZE * p->X + (x2 * BLOCK_SIZE/2) + 10.0f, BLOCK_SIZE * p->Y + (y2 * BLOCK_SIZE/2) + 10.0f), _solidBrush);
					}
					else if(snakebody == _snakelist->Tail)
					{
						Point * p1 = prev->Current, * p2 = snakebody->Current;
						_solidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
						if(p2->X < p1->X) //EAST
						{
							pRenderTarget->FillRectangle(D2D1::RectF(BLOCK_SIZE * p->X + 25.0f + 10.0f, BLOCK_SIZE * p->Y + 10.0f, BLOCK_SIZE * p->X + BLOCK_SIZE + 10.0f, BLOCK_SIZE * p->Y + BLOCK_SIZE + 10.0f), _solidBrush);
						}
						else if(p2->X > p1->X) //WEST
						{
							pRenderTarget->FillRectangle(D2D1::RectF(BLOCK_SIZE * p->X + 10.0f, BLOCK_SIZE * p->Y + 10.0f, BLOCK_SIZE * p->X + 25.0f + 10.0f, BLOCK_SIZE * p->Y + BLOCK_SIZE + 10.0f), _solidBrush);
						}
						else if(p2->Y > p1->Y) //NORTH
						{
							pRenderTarget->FillRectangle(D2D1::RectF(BLOCK_SIZE * p->X + 10.0f, BLOCK_SIZE * p->Y + 10.0f, BLOCK_SIZE * p->X + BLOCK_SIZE + 10.0f, BLOCK_SIZE * p->Y + 25.0f + 10.0f), _solidBrush);
						}
						else if(p2->Y < p1->Y) //SOUTH
						{
							pRenderTarget->FillRectangle(D2D1::RectF(BLOCK_SIZE * p->X + 10.0f, BLOCK_SIZE * p->Y + 25.0f + 10.0f, BLOCK_SIZE * p->X + BLOCK_SIZE + 10.0f, BLOCK_SIZE * p->Y + BLOCK_SIZE + 10.0f), _solidBrush);
						}

						pRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(50.0f * p->X + 25.0f + 10.0f, 50.0f * p->Y + 25.0f + 10.0f), 25.0f, 25.0f), _solidBrush);
					}
					else
					{
						_solidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
						pRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(50.0f * p->X + 25.0f + 10.0f, 50.0f * p->Y + 25.0f + 10.0f), 25.0f, 25.0f), _solidBrush);

						Point * p1 = prev->Current, * p2 = snakebody->Current, * p3 = next->Current;
						if((p1->X > p2->X && p2->Y < p3->Y) || (p1->Y > p2->Y && p2->X < p3->X)) //TOP LEFT CORNER
						{
							pRenderTarget->FillRectangle(D2D1::RectF(BLOCK_SIZE * p->X + BLOCK_SIZE/2 + 10.0f, BLOCK_SIZE * p->Y + 10.0f, BLOCK_SIZE * p->X + BLOCK_SIZE + 10.0f, BLOCK_SIZE * p->Y + BLOCK_SIZE + 10.0f), _solidBrush);
							pRenderTarget->FillRectangle(D2D1::RectF(BLOCK_SIZE * p->X + 10.0f, BLOCK_SIZE * p->Y + BLOCK_SIZE/2 + 10.0f, BLOCK_SIZE * p->X + BLOCK_SIZE + 10.0f, BLOCK_SIZE * p->Y + BLOCK_SIZE + 10.0f), _solidBrush);
						}
						else if((p1->X > p2->X && p2->Y > p3->Y) || (p1->Y < p2->Y && p2->X < p3->X)) //BOT LEFT CORNER
						{
							pRenderTarget->FillRectangle(D2D1::RectF(BLOCK_SIZE * p->X + 10.0f, BLOCK_SIZE * p->Y + 10.0f, BLOCK_SIZE * p->X + BLOCK_SIZE + 10.0f, BLOCK_SIZE * p->Y + BLOCK_SIZE/2 + 10.0f), _solidBrush);
							pRenderTarget->FillRectangle(D2D1::RectF(BLOCK_SIZE * p->X + BLOCK_SIZE/2 + 10.0f, BLOCK_SIZE * p->Y + 10.0f, BLOCK_SIZE * p->X + BLOCK_SIZE + 10.0f, BLOCK_SIZE * p->Y + BLOCK_SIZE + 10.0f), _solidBrush);
						}
						else if((p1->X < p2->X && p2->Y < p3->Y) || (p1->Y > p2->Y && p2->X > p3->X)) //TOP RIGHT CORNER
						{
							pRenderTarget->FillRectangle(D2D1::RectF(BLOCK_SIZE * p->X + 10.0f, BLOCK_SIZE * p->Y + 10.0f, BLOCK_SIZE * p->X + BLOCK_SIZE/2 + 10.0f, BLOCK_SIZE * p->Y + BLOCK_SIZE + 10.0f), _solidBrush);
							pRenderTarget->FillRectangle(D2D1::RectF(BLOCK_SIZE * p->X + 10.0f, BLOCK_SIZE * p->Y + BLOCK_SIZE/2 + 10.0f, BLOCK_SIZE * p->X + BLOCK_SIZE + 10.0f, BLOCK_SIZE * p->Y + BLOCK_SIZE + 10.0f), _solidBrush);
						}
						else if((p1->X < p2->X && p2->Y > p3->Y) || (p1->Y < p2->Y && p2->X > p3->X)) //BOT RIGHT CORNER
						{
							pRenderTarget->FillRectangle(D2D1::RectF(BLOCK_SIZE * p->X + 10.0f, BLOCK_SIZE * p->Y + 10.0f, BLOCK_SIZE * p->X + BLOCK_SIZE + 10.0f, BLOCK_SIZE * p->Y + BLOCK_SIZE/2 + 10.0f), _solidBrush);
							pRenderTarget->FillRectangle(D2D1::RectF(BLOCK_SIZE * p->X + 10.0f, BLOCK_SIZE * p->Y + 10.0f, BLOCK_SIZE * p->X + BLOCK_SIZE/2 + 10.0f, BLOCK_SIZE * p->Y + BLOCK_SIZE + 10.0f), _solidBrush);
						}
						else
						{
							pRenderTarget->FillRectangle(
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
					pRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(50.0f * sp->Current->X + 25.0f + 10.0f, 50.0f * sp->Current->Y + 25.0f + 10.0f), 12.0f, 25.0f), _solidBrush);
					sp = sp->Next;
				}

				GameItem * pt = _traplist->Get(0);
				while(pt != 0)
				{
					_solidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::LightBlue));
					pRenderTarget->FillRectangle(D2D1::RectF(50.0f * pt->Current->X + 10.0f, 50.0f * pt->Current->Y + 10.0f, 50.0f * pt->Current->X + 50.0f + 10.0f, 50.0f * pt->Current->Y + 50.0f + 10.0f), _solidBrush);
					pt = pt->Next;
				}


				ID2D1PathGeometry * pGeo = 0;
				ID2D1GeometrySink *pSink = 0;
				pFactory->CreatePathGeometry(&pGeo);

				if(_fruit != 0)
				{
					D2D1_RECT_F rectangle2 = D2D1::RectF(
						BLOCK_SIZE * _fruit->Current->X + 10.0f + 1.0f,
						BLOCK_SIZE * _fruit->Current->Y + 10.0f + 1.0f,
						BLOCK_SIZE * (_fruit->Current->X + 1) + 10.0f - 1.0f,
						BLOCK_SIZE * (_fruit->Current->Y + 1) + 10.0f - 1.0f
						);
					_solidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Purple));
					//pRenderTarget->FillRectangle(&rectangle2, _solidBrush);

					pGeo->Open(&pSink);
					pSink->SetFillMode(D2D1_FILL_MODE_WINDING);

					int R = 24, x = BLOCK_SIZE * _fruit->Current->X + 10.0f + 25.0f, y = BLOCK_SIZE * _fruit->Current->Y + 10.0f + 25.0f;
					float B2 = tanf(22.5f * 3.141592653589793238462643383279502884f / 180.0f) * R;
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
					pRenderTarget->FillGeometry(pGeo, _solidBrush);
					//pRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), 2.0f, 2.0f), _solidBrush);
		
					_solidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::LimeGreen));
					pRenderTarget->DrawGeometry(pGeo, _solidBrush, 1.f);
				}

				SafeRelease(&pSink);
				//pRenderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(125, 125), 12.0f, 24.0f), _solidBrush);
				// render txt stats.
				string scoreGame = "Score: " + to_string(_statlist->StatGet("score")->second);
				wstring sw = wstring(scoreGame.begin(), scoreGame.end());
				_solidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::HotPink));
				pRenderTarget->DrawText(
					sw.c_str(),
					sw.length(),
					pITextFormat,
					D2D1::RectF(GAME_GRID_SIZE * BLOCK_SIZE + 10.0f + 15.0f, 0.0f, GAME_GRID_SIZE * BLOCK_SIZE + 10.0f + 15.0f + 100.0f, 100.0f), 
					_solidBrush
				);

				string fruitEaten = "Fruit: " + to_string(_statlist->StatGet("fruit")->second);
				sw = wstring(fruitEaten.begin(), fruitEaten.end());
				_solidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::HotPink));
				pRenderTarget->DrawText(
					sw.c_str(),
					sw.length(),
					pITextFormat,
					D2D1::RectF(GAME_GRID_SIZE * BLOCK_SIZE + 10.0f + 15.0f, 100.0f, GAME_GRID_SIZE * BLOCK_SIZE + 10.0f + 15.0f + 100.0f, 200.0f), 
					_solidBrush
				);
			}
			break;
		case GameState::GameOver:
			pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
			_solidBrush->SetColor(D2D1::ColorF(D2D1::ColorF::HotPink));
			pRenderTarget->DrawText(
				L"Game Over",
				9,
				pITextFormat,
				D2D1::RectF(225.0f, 225.0f, 325.0f, 275.0f), 
				_solidBrush
			);

			Score * s = _scorelist->FirstGet();
			int i = 1;
			string sc;
			wstring wsc;
			while(s != 0 && i < 6)
			{
				sc = to_string(i) + " - Fruits: " + to_string(s->Fruit) + " - Score: " + to_string(s->Points) + " - " + s->TimeScored;
				wsc = wstring(sc.begin(), sc.end());
				pRenderTarget->DrawText(
					wsc.c_str(),
					wsc.length(),
					pITextFormat,
					D2D1::RectF(225.0f, (i * 50.0f) + 250.0f, 650.0f, (i * 50.0f) + 275.0f), 
					_solidBrush
				);
				if(s->Fruit == _statlist->StatGet("fruit")->second && s->Points == _statlist->StatGet("score")->second)
				{
					pRenderTarget->DrawRectangle(D2D1::RectF(220.0f, (i * 50.0f) + 250.0f, 650.0f, (i * 50.0f) + 275.0f), _solidBrush, 2.0f);
				}
				
				s = s->Next;
				i++;
			}
			break;
	}
}

void GameDirector::Render()
{
	HRESULT hr = S_OK;
	if (pRenderTarget == 0)
    {
        RECT rc;
        GetClientRect(_hWnd, &rc);

        hr = pFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(_hWnd, D2D1::SizeU(rc.right, rc.bottom)),
            &pRenderTarget
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
            hr = pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1, 1, 1), &_solidBrush);
        }
    }

    pRenderTarget->BeginDraw();
    RenderScene();
    hr = pRenderTarget->EndDraw();
    if (hr == D2DERR_RECREATE_TARGET)
    {
        Destory_DeviceDependent();
    }
}
