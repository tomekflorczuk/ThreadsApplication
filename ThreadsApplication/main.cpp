// ThreadsApplication.cpp : Defines the entry point for the console application.
//
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
	//Wypisanie kulki w losowym punkcie na g�rze ekranu
	move(ball.y, ball.x);
	printw(&point);
	refresh();
	//Poruszanie si� kulki
	do
	{
		ball.CheckFrame(maxx, maxy);
		ball.MoveBall();
		move(ball.prevy, ball.prevx);
		printw(" ");
		move(ball.y, ball.x);
		Sleep(ball.velocity);
		printw(&point);
		refresh();
	} while (ball.velocity < 1000);
	//Usuwanie kulki
	move(ball.y, ball.x);
	printw(" ");
	refresh();
}
//Czekanie na wci�niecie klawisza 'x' - wyj�cia
void WaitForInput()
{
	while(c != 'x')
	{
		c = getch();
	}
}
//Program g��wny
void main()
{
	srand(time(NULL));
	//Inicjalizacja
	initscr();
	noecho();
	//Zczytywanie wielko�ci okna
	getmaxyx(stdscr, maxy, maxx);
	//Tworzenie w�tku zako�czenia programu
	threads.push_back(thread(WaitForInput));
	//G��wna p�tla programu
	while(c != 'x')
	{
		//Tworzenie nowej kulki
		Ball* ball = new Ball(rand() % maxx);
		//Tworzenie nowego w�tku kulki
		threads.push_back(thread(NewBall, *ball));
		//Czekanie 5 sekund na utworzenie nowego w�tku
		Sleep(5000);
	}
	//Joinowanie wszystkich w�tk�w
	for (auto& th : threads) th.join();
	endwin();
}