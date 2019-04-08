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
		mvprintw(ball.prevy, ball.prevx, " ");
		mvprintw(ball.y, ball.x, &point);
		Sleep(ball.velocity);
		refresh();
	} while (ball.velocity < 1000);
	//Usuwanie kulki
	move(ball.y, ball.x);
	printw(" ");
}
//Czekanie na wci�niecie klawisza 'x' - wyj�cia
void WaitForInput()
{
	while(c != 'x')
	{
		c = getch();
	}
}
//Od�wie�anie ekranu co milisekunde
void WindowRefresh()
{
	//Sleep(50);
	refresh();
}
//Program g��wny
void main()
{
	srand(time(nullptr));
	//Inicjalizacja
	initscr();
	noecho();
	//auto n = thread::hardware_concurrency();
	//Zczytywanie wielko�ci okna
	getmaxyx(stdscr, maxy, maxx);
	//Tworzenie w�tku zako�czenia programu
	threads.push_back(thread(WaitForInput));
	//Tworzenie w�tku od�wie�ania erkanu co sekunde
	//threads.push_back(thread(WindowRefresh));
	//G��wna p�tla programu
	while(c != 'x')
	{
		//Tworzenie nowej kulki
		//Ball* ball = new Ball(rand() % maxx);
		Ball ball(rand()% maxx);
		//Tworzenie nowego w�tku kulki
		threads.push_back(thread(NewBall, ball));
		//Czekanie 5 sekund na utworzenie nowego w�tku
		Sleep(5000);
	}
	endwin();
	//Joinowanie wszystkich w�tk�w
	for (auto& th : threads) th.join();
	//Usuwanie w�tk�w
	threads.clear();
}