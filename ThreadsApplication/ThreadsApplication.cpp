#include "stdafx.h"
#include <algorithm>

using namespace std;
//Zmienne globalne
const char point = 'o';
unsigned int maxx, maxy;
bool running;
char c;
list<thread> threads;
CRITICAL_SECTION critical;
//Funkcja pojedynczej kulki
void NewBall(Ball ball)
{
	//Wypisanie kulki w losowym punkcie na górze ekranu
	EnterCriticalSection(&critical);
	mvprintw(ball.y, ball.x, &point);
	LeaveCriticalSection(&critical);
	//Poruszanie siê kulki
	do
	{
		ball.CheckFrame(maxx, maxy);
		ball.MoveBall();
		EnterCriticalSection(&critical);
		mvprintw(ball.y, ball.x, &point);
		mvprintw(ball.prevy, ball.prevx, " ");
		LeaveCriticalSection(&critical);
		this_thread::sleep_for(chrono::milliseconds(ball.velocity));
		//Sleep(ball.velocity);
	} while (ball.velocity < 700 && running);
	//Czyszczenie kulki
	EnterCriticalSection(&critical);
	mvprintw(ball.y, ball.x, " ");
	LeaveCriticalSection(&critical);
}
//Czekanie na wcisniecie klawisza 'x' - wyjscia
void WaitForInput(bool& running)
{
	while(running)
	{
		c = getch();
		if (c == 'x')
		{
			running = false;
		}
	}
}
//Odswiezanie ekranu
void WindowRefresh()
{
	while (running)
	{
		this_thread::sleep_for(chrono::milliseconds(50));
		refresh();
	}
}
//Program g³ówny
void main()
{
	srand(time(nullptr));
	//Inicjalizacja
	initscr();
	noecho();
	curs_set(0);
	running = true;
	//Zczytywanie wielkoœci okna
	getmaxyx(stdscr, maxy, maxx);
	InitializeCriticalSection(&critical);
	SetCriticalSectionSpinCount(&critical, 100);
	//Tworzenie watku zakonczenia programu
	thread exitthread(WaitForInput, ref(running));
	//Tworzenie watku odswiezania ekranu
	thread refreshthread(WindowRefresh);
	//Glowna petla programu
	while(running)
	{
		//Tworzenie nowej kulki
		Ball ball(rand()% maxx);
		//Tworzenie nowego watku kulki
		threads.push_front(thread(NewBall, ball));
		//Czekanie 5 sekund na utworzenie nowego watku
		Sleep(5000);
	}
	//Joinowanie wszystkich watkow
	exitthread.join();
	refreshthread.join();
	for (auto& th : threads)
		th.join();
	DeleteCriticalSection(&critical);
	endwin();
}