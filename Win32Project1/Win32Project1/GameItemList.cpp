#include "GameItemList.h"

void GameItemList::Add(GameItem * Item)
{
	if(_head == 0)
	{
		_head = Item;
	}
	else
	{
		_tail->Next = Item;
	}
	
	while(Item->Next != 0)
	{
		Item = Item->Next;
	}
		
	_tail = Item;
}

void GameItemList::AddCopy(GameItem * Item)
{
	GameItem * temp = Item;
	while(temp != 0)
	{
		if(_head == 0)
		{
			_head = _tail = new GameItem(*temp);
		}
		else
		{
			_tail->Next = new GameItem(*temp);
			_tail = _tail->Next;
		}

		temp = temp->Next;
	}
}

int GameItemList::Remove(GameItem * Item)
{
	if(Item == 0)
	{
		return -1;
	}

	GameItem * currentItem = _head;
	GameItem * previousItem = 0;
	int index = 0;
	while(currentItem != 0)
	{
		if(Item == currentItem)
		{
			//fix head
			if(previousItem == 0)
			{
				_head = currentItem->Next;
			}
			else
			{
				previousItem->Next = currentItem->Next;
			}

			//fix tail
			if(currentItem->Next == 0)
			{
				_tail = previousItem;
			}

			//!!!!!!!!!!!!!!!!!!!
			//delete currentItem;
			return index;
		}

		currentItem = currentItem->Next;
		index++;
	}

	return -1;
}

GameItem * GameItemList::Get(Point * p)
{
	GameItem * current = _head;
	while(current != 0)
	{
		if(current->Current->X == p->X && current->Current->Y == p->Y)
		{
			return current;
		}

		current = current->Next;
	}

	return 0;
}

GameItem * GameItemList::Get(int index)
{
	GameItem * current = _head;
	int i = 0;
	do
	{
		if(i == index)
		{
			return current;
		}

		current = current->Next;
	} while(i != index && current != 0);

	return 0;
}

int GameItemList::Length_Get()
{
	int length = 0;
	GameItem * item = _head;
	while(item != 0)
	{
		length++;
		item = item->Next;
	}

	return length;
}