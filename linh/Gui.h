#ifndef _GUI_H_
#define _GUI_H_

#include <Windows.h>
#include <string>
#include <conio.h>
using namespace std;

void resizeConsole(int width, int height);
void textColor(int background, int text);
void goToXY(int x, int y);
void fixConsoleWindow();
void HidePointer();
void ShowPointer();
void drawBoard(int x, int y, int height, int width, int color);
bool getQuery(std::string& str);

void resizeConsole(int width, int height)
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, width, height, TRUE);
}
void textColor(int background, int text)
{
	int x = 16 * background + text;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), x);
}
void goToXY(int x, int y)
{
	HANDLE hConsoleOutput;
	COORD Cursor_an_Pos = { x - 1,y - 1 };
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsoleOutput, Cursor_an_Pos);
}
void fixConsoleWindow()
{
	HWND consoleWindow = GetConsoleWindow();
	LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
	style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
	SetWindowLong(consoleWindow, GWL_STYLE, style);
}
void HidePointer()
{
	CONSOLE_CURSOR_INFO Info;
	Info.bVisible = FALSE;
	Info.dwSize = 20;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
}
void ShowPointer()
{
	CONSOLE_CURSOR_INFO ConCurInf;
	ConCurInf.dwSize = 10;
	ConCurInf.bVisible = TRUE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ConCurInf);
}
void drawBoard(int x, int y, int height, int width, int color)
{
	int u = x + width, v = y + height;
	textColor(0, color);
	goToXY(x, y);	std::cout << char(201);
	goToXY(x, v);	std::cout << char(200);
	goToXY(u, y);	std::cout << char(187);
	goToXY(u, v);	std::cout << char(188);
	goToXY(x + 1, y);
	for (int i = x + 1; i < u; i++) std::cout << char(205);
	goToXY(x + 1, v);
	for (int i = x + 1; i < u; i++) std::cout << char(205);
	for (int i = y + 1; i < v; i++) {
		goToXY(x, i); std::cout << char(186);
	}
	for (int i = y + 1; i < v; i++) {
		goToXY(u, i); std::cout << char(186);
	}
}
bool getQuery(std::string& str)
{
	int keyValue;
	while (true)
	{
		if (_kbhit())
		{
			keyValue = _getch();
			if (keyValue == 27)
				return false;
			if (32 <= keyValue && keyValue <= 126)
			{
				cout << (char)keyValue;
				str.push_back((char)keyValue);
			}
			if (keyValue == '\b')
			{
				cout << "\b \b";
				str.pop_back();
			}
			if (keyValue == 13)
				break;
		}
	}
	return true;
}

#endif // !_GUI_H_


