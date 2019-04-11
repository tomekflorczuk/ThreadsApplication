#include "stdafx.h"
#include "Ball.h"

//Konstruktor
Ball::Ball(int initx)
{
	prevy = -1;
	prevx = -1;
	x = initx;
	y = 0;
	dir = rand() % 4;
	velocity = rand() % 50;
}
//Destruktor
Ball::~Ball()
{
}
//Poruszanie kulki w zaleznosci od kieruku
void Ball::MoveBall()
{
	//Poruszanie siê w zale¿noœci od kierunku
	switch (dir)
	{
	//W
	case(0):
	{
		prevy = y;
		prevx = x;
		x--;
		break;
	}
	//SW
	case(1):
	{
		prevx = x;
		x--;
		prevy = y;
		y++;
		break;
	}
	//S
	case(2):
	{
		prevx = x;
		prevy = y;
		y++;
		break;
	}
	//SE
	case(3):
	{
		prevx = x;
		x++;
		prevy = y;
		y++;
		break;
	}
	//E
	case(4):
	{
		prevy = y;
		prevx = x;
		x++;
		break;
	}
	//NE
	case(5):
	{
		prevx = x;
		x++;
		prevy = y;
		y--;
		break;
	}
	//N
	case(6):
	{
		prevx = x;
		prevy = y;
		y--;
		break;
	}
	//NW
	case(7):
	{
		prevx = x;
		x--;
		prevy = y;
		y--;
		break;
	}
	}
}
//Sprawdzanie osiagniecia granic ekranu
void Ball::CheckFrame(int maxx, int maxy)
{
	if (prevy != -1 && prevx != -1)
	{
		if (y == maxy - 1)
		{	
			if (x > prevx || x == 0) dir = 5;
			else if (x < prevx || x == maxx - 1) dir = 7;
			else dir = 6;	
			velocity *= 2;
		}
		else if (y == 0)
		{
			if (x > prevx || x == 0) dir = 3;
			else if (x < prevx || x == maxx - 1) dir = 1;
			else dir = 2;
			velocity *= 2;
		}
		else if (x == maxx - 1)
		{
			if (y > prevy) dir = 1;
			else if (y < prevy) dir = 7;
			else dir = 0;
			velocity *= 2;
		}
		else if (x == 0)
		{
			if (y > prevy) dir = 3;
			else if (y < prevy) dir = 5;
			else dir = 4;
			velocity *= 2;
		}
	}
}