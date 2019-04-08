#include "stdafx.h"

using namespace std;
//Zmienne globalne
const char point = 'o';
int maxx, maxy;
char c;
vector<thread> threads;
//Funkcja pojedynczej kulki
void NewBall(Ball ball)
{
	//Wypisanie kulki w losowym punkcie na górze ekranu
	move(ball.y, ball.x);
	printw(&point);
	refresh();
	//Poruszanie siê kulki
	do
	{
		ball.CheckFrame(maxx, maxy);
		ball.MoveBall();
		mvprintw(ball.prevy, ball.prevx, " ");
		mvprintw(ball.y, ball.x, &point);
		Sleep(ball.velocity);
		refresh();
	} while (ball.velocity < 1000);
	//Usuwanie kulki
	move(ball.y, ball.x);
	printw(" ");
}
//Czekanie na wcisniecie klawisza 'x' - wyjscia
void WaitForInput()
{
	while(c != 'x')
	{
		c = getch();
	}
}
//Odswiezanie ekranu co milisekunde
void WindowRefresh()
{
	//Sleep(50);
	refresh();
}
//Program g³ówny
void main()
{
	srand(time(nullptr));
	//Inicjalizacja
	initscr();
	noecho();
	//Zczytywanie wielkoœci okna
	getmaxyx(stdscr, maxy, maxx);
	//Tworzenie watku zakonczenia programu
	threads.push_back(thread(WaitForInput));
	//Tworzenie watku odswiezania ekranu co sekunde
	//threads.push_back(thread(WindowRefresh));
	//Glowna petla programu
	while(c != 'x')
	{
		//Tworzenie nowej kulki
		Ball ball(rand()% maxx);
		//Tworzenie nowego watku kulki
		threads.push_back(thread(NewBall, ball));
		//Czekanie 5 sekund na utworzenie nowego watku
		Sleep(5000);
	}
	endwin();
	//Joinowanie wszystkich watkow
	for (auto& th : threads) th.join();
	//Usuwanie watkow
	threads.clear();
}