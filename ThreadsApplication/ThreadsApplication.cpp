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
	//Wypisanie kulki w losowym punkcie na górze ekranu
	move(ball.y, ball.x);
	printw(&point);
	refresh();
	//Poruszanie siê kulki
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
//Czekanie na wciœniecie klawisza 'x' - wyjœcia
void WaitForInput()
{
	while(c != 'x')
	{
		c = getch();
	}
}
//Program g³ówny
void main()
{
	srand(time(NULL));
	//Inicjalizacja
	initscr();
	noecho();
	//Zczytywanie wielkoœci okna
	getmaxyx(stdscr, maxy, maxx);
	//Tworzenie w¹tku zakoñczenia programu
	threads.push_back(thread(WaitForInput));
	//G³ówna pêtla programu
	while(c != 'x')
	{
		//Tworzenie nowej kulki
		Ball* ball = new Ball(rand() % maxx);
		//Tworzenie nowego w¹tku kulki
		threads.push_back(thread(NewBall, *ball));
		//Czekanie 5 sekund na utworzenie nowego w¹tku
		Sleep(5000);
	}
	//Joinowanie wszystkich w¹tków
	for (auto& th : threads) th.join();
	endwin();
}