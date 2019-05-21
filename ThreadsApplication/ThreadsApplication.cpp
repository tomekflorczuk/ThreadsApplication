#include "stdafx.h"

using namespace std;
//Zmienne globalne
const char point = 'o';
unsigned int maxx, maxy;
atomic<bool> running = true;
bool toawake = false;
char c;
vector<unique_ptr<thread>> threads;
vector<unique_ptr<condition_variable>> vars;
//Funkcja pojedynczej kulki
void NewBall(ball ball, condition_variable& var)
{
	mutex mutex1, varmutex;
	//Wypisanie kulki w losowym punkcie na g�rze ekranu
	unique_lock<mutex> lock1(mutex1);
	mvprintw(ball.y, ball.x, &point);
	lock1.unlock();
	//Poruszanie si� kulki
	do
	{
		ball.check_frame(maxx, maxy);
		if (ball.deflectioncounter == 2)
		{
			ball.deflectioncounter = 0;
			unique_lock<mutex> varlock(varmutex);
			//vars[index]->wait(varlock, [] {return toawake == true;});
			//var.wait(varlock, [] {return toawake;});
			var.wait(varlock);

		}
		ball.move_ball();
		lock1.lock();
		mvprintw(ball.prevy, ball.prevx, " ");
		mvprintw(ball.y, ball.x, &point);
		lock1.unlock();
		this_thread::sleep_for(chrono::milliseconds(ball.velocity));
	} while (running && ball.velocity < 600);
	//Czyszczenie kulki
	lock1.lock();
	mvprintw(ball.y, ball.x, " ");
	lock1.unlock();
}
//Czekanie na wcisniecie klawisza 'x' - wyjscia
void WaitForInput(atomic<bool>& running)
{
	mutex mutex2;
	while(running)
	{
		c = getch();
		if (c == 'x')
		{
			scoped_lock<mutex> lock2(mutex2);
			running = false;
		}
	}
}
//Odswiezanie ekranu
void WindowRefresh()
{
	mutex mutex3;
	while (running)
	{
		this_thread::sleep_for(chrono::milliseconds(50));
		unique_lock<mutex> lock3(mutex3);
		refresh();
		lock3.unlock();
	}
}
//Rozbudzanie kulek
void AwakeSignal()
{
	mutex mutex4;
	while(running)
	{
		this_thread::sleep_for(chrono::seconds(5));
		scoped_lock<mutex> lock4(mutex4);
		toawake = true;
		for (auto& var : vars)
			var->notify_one();
		toawake = false;	
	}
}
//Program g��wny
void main()
{	
	srand(time(nullptr));
	//Inicjalizacja
	initscr();
	noecho();
	curs_set(0);
	//Zczytywanie wielko�ci okna
	getmaxyx(stdscr, maxy, maxx);
	//Tworzenie watku zakonczenia programu
	thread exitthread(WaitForInput, ref(running));
	//Tworzenie watku odswiezania ekranu
	thread refreshthread(WindowRefresh);
	//Tworzenie w�tku budzenia w�tk�w
	thread awakethread(AwakeSignal);
	//Glowna petla programu
	while(running)
	{
		//Tworzenie nowej kulki
		ball ball(rand()% maxx);
		//Tworzenie wska�nika na now� zmienn� warunkow� dla nowego w�tku
		unique_ptr<condition_variable> var(new condition_variable);
		//Tworzenie wska�nika na nowy watek kulki
		unique_ptr<thread> th(new thread(NewBall, ball, ref(*var)));
		mutex mainmutex;
		unique_lock<mutex> mainlock(mainmutex);
		vars.push_back(move(var));
		mainlock.unlock();
		threads.push_back(move(th));
		//Czekanie sekundy na utworzenie nowego watku
		if (running) this_thread::sleep_for(chrono::seconds(10));
	}
	//Joinowanie wszystkich watkow
	exitthread.join();
	refreshthread.join();
	awakethread.join();
	for (auto& th : threads)
		th->join();
	endwin();
}