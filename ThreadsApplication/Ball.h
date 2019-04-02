#pragma once
class Ball
{
public:
	int x, y, prevx, prevy, dir, velocity;
	Ball(int initx);
	~Ball();
	void MoveBall();
	void CheckFrame(int maxx, int maxy);
};