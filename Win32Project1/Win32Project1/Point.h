#ifndef POINT_H
#define POINT_H

class Point
{
private:
	int _x, _y;
public:
	Point(int x = 0, int y = 0)
	{
		_x = x;
		_y = y;
	}

	int X_Get();
	void X_Set(int);
	int Y_Get();
	void Y_Set(int);
	
	__declspec(property(get = X_Get, put = X_Set)) int X;
	__declspec(property(get = Y_Get, put = Y_Set)) int Y;
};

#endif