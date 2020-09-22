#ifndef _GUI_H_
#define _GUI_H_

#define _WIN32_WINNT 0x0500
#include <Windows.h>
#include <string>
#include <conio.h>
using std::cout;
using std::string;

#define FILES_POSX 10
#define FILES_POSY 8
#define FILES_CNT 10
#define FILES_LEN 40
#define PAGES_POSX 70
#define PAGES_POSY 8
#define PAGES_CNT 30
#define PAGES_LEN 60
#define OPTION_POSX 20
#define OPTION_POSY 20
#define OPTION_LEN 20
#define NOTE_POSX 20
#define NOTE_POSY 35

void resizeConsole(int width, int height);
void textColor(int background, int text);
void goToXY(int x, int y);
void fixConsoleWindow();
void HidePointer();
void ShowPointer();
void drawBoard(int x, int y, int height, int width, int color);
bool getQuery(string& str);

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
	goToXY(x, y);	cout << char(201);
	goToXY(x, v);	cout << char(200);
	goToXY(u, y);	cout << char(187);
	goToXY(u, v);	cout << char(188);
	goToXY(x + 1, y);
	for (int i = x + 1; i < u; i++) cout << char(205);
	goToXY(x + 1, v);
	for (int i = x + 1; i < u; i++) cout << char(205);
	for (int i = y + 1; i < v; i++) {
		goToXY(x, i); cout << char(186);
	}
	for (int i = y + 1; i < v; i++) {
		goToXY(u, i); cout << char(186);
	}
}
bool getQuery(string& str, int color)
{
	textColor(0, color);
	bool isArrow = false;
	int keyValue;
	while (true)
	{
		if (_kbhit())
		{
			keyValue = _getch();
			if (keyValue == 27)
				return false;
			if (keyValue == 224)
			{
				isArrow = true;
				continue;
			}
			if (isArrow)
			{
				isArrow = false;
				continue;
			}
			if (32 <= keyValue && keyValue <= 126)
			{
				if (str.length() == 140)
				{
					HidePointer();
					textColor(0, 4); 
					goToXY(59, 28); cout << "[INFO] The query has reached its maximum size!";
					textColor(0, color);
					goToXY(129, 25);
					ShowPointer();
				}
				else
				{
					if (str.length() == 70) goToXY(59, 25);
					cout << (char)keyValue;
					str.push_back((char)keyValue);
				}
			}
			if (keyValue == '\b' && str.length() > 0)
			{
				if (str.length() == 140)
				{
					HidePointer();
					goToXY(59, 28); cout << "                                               ";
					goToXY(129, 25);
					ShowPointer();
				}
				cout << "\b \b";
				str.pop_back();
				if (str.length() == 70) goToXY(129, 24);
			}
			if (keyValue == 13)
				return true;
		}
	}
	return true;
}
void printOptionList(int num, int posX, int posY)
{
	textColor(0, 12);
	string option[5] = { "Load new", "Load existed", "Search", "Exit" };
	goToXY(posX, posY); cout << "<<OPTION>>";
	for (int i = 0; i < 4; i++)
		if (i == num)
		{
			textColor(0, 13);
			goToXY(posX - 1, posY + 2 + i * 2); cout << char(16) << option[i];
			textColor(0, 12);
		}
		else
		{
			goToXY(posX - 1, posY + 2 + i * 2);	cout << char(32) << option[i];
		}
}
void printHCMUS(int x, int y)
{
	textColor(0, 13);
	goToXY(x, y);
	cout << " oo      oo       oooooooo    oooo     oooo    oo      oo     oooooo"; y++; goToXY(x, y);
	cout << " oo      oo      oo           oo oo   oo oo    oo      oo    oo     "; y++; goToXY(x, y);
	cout << " oo      oo     oo            oo  oo oo  oo    oo      oo    oo     "; y++; goToXY(x, y);
	cout << " oooooooooo    oo             oo   ooo   oo    oo      oo     ooooo "; y++; goToXY(x, y);
	cout << " oo      oo     oo            oo         oo    oo      oo         oo"; y++; goToXY(x, y);
	cout << " oo      oo      oo           oo         oo    oo      oo         oo"; y++; goToXY(x, y);
	cout << " oo      oo       oooooooo    oo         oo    oooooooooo    oooooo "; y++; goToXY(x, y);
	cout << "                                                                    "; y++; goToXY(x, y);
	cout << "                          <<SEARCH ENGINE>>                         "; y++; goToXY(x, y);
}
void printNote(int x, int y)
{
	textColor(0, 4);
	goToXY(x, y++); cout << "Use arrows to move";
	goToXY(x, y++); cout << "\"Esc\": return";
	goToXY(x, y++); cout << "\"Enter\": choose";
}

#endif // !_GUI_H_


