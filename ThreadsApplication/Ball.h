#pragma once
class ball
{
public:
	int x, y, prevx, prevy, dir, velocity, deflectioncounter;
	ball(int initx);
	void move_ball();
	void check_frame(int maxx, int maxy);
};