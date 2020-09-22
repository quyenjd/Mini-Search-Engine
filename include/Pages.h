#ifndef _PAGES_H_
#define _PAGES_H_

#include "Gui.h"
#include "Base.h"
using std::vector;
using std::min;
using std::endl;

string getName(string dir)
{
	int i = dir.rfind("\\") + 1, j = dir.length();
	return dir.substr(i, j - i);
}
string shortenName(string name, int len)
{
	if (int(name.length()) <= len)
		return name;
	else
		return name.substr(0, len - 3) + "...";
}

struct File {
	string dir;
	string filename;
	string shortName;
	double score;
	vector<baseNode> words;
};
struct Line {
	string str;
	bool* check = nullptr;
	void printLine(int color)
	{
		for (int i = 0; i < int(str.length()); i++)
		{
			if (check[i])
				textColor(0, 14);
			else
				textColor(0, color);
			cout << str[i];
		}
	}
};
class Pages
{
private:
	int posX = PAGES_POSX, posY = PAGES_POSY;
	int cntLines = PAGES_CNT, lenLines = PAGES_LEN;
	int xL, xR, yLR;
	vector<Line> name;
	vector<Line> lines;
	vector<baseNode> words;
public:
	int numPages, curPages = -1;
	Pages(File file)
	{
		words = file.words;
		modifyLine(-1, "<<<" + file.filename + ">>>", true); cntLines -= name.size() + 1;
		readFile(file.dir);
		numPages = (lines.size() - 1) / cntLines;
		if (curPages == -1) curPages = 0;
		xL = posX - 2; xR = posX + lenLines + 1;
		yLR = posY + (cntLines - 1) / 2;
	}
	void clear()
	{
		int size = lines.size();
		for (int i = 0; i < size; i++)
			if (lines[i].check)
				delete[] lines[i].check;
		lines.clear();
	}
	bool isHL(int k, int curLine, int& len)
	{
		int size = words.size();
		for (int i = 0; i < size; i++)
			if (curLine == words[i].line && k == words[i].pos)
			{
				len = words[i].numWords;
				if (curPages == -1)
					curPages = lines.size() / cntLines;
				return true;
			}
		len = 0;
		return false;
	}
	void buildLine(Line& line, string str, int i, int j, int curLine, int& cnt, int& len)
	{
		line.str = str;
		line.check = new bool[str.length() + 5];
		int k = i;
		while (k <= j)
		{
			if (cnt < len || isHL(k, curLine, len))
			{
				while (k <= j && cnt <= len)
				{
					if (normal(str[k - i]) && ((k == i) || (k > i && !normal(str[k - i - 1])))) cnt++;
					if (cnt > len) {
						cnt = len = 0;
						break;
					}
					line.check[k - i] = true;
					k++;
				}
			}
			else
			{
				line.check[k - i] = false;
				k++;
			}

		}
	}
	void modifyLine(int curLine, string line, bool isName)
	{
		if (line.length() == 0)
		{
			Line tmp;
			tmp.str = "";
			if (isName)
				name.push_back(tmp);
			else
				lines.push_back(tmp);
			return;
		}
		int j, len = line.length();
		int cnt = 0, count = 0, lenC = 0;
		for (int i = 0; i < len;)
		{
			j = min(i + lenLines - 1, len - 1);
			if (j + 1 < len && line[j + 1] != char(32))
				while (i <= j && line[j] != char(32)) j--;
			if (j < i) j = min(i + lenLines - 1, len - 1);
			Line tmp;
			buildLine(tmp, line.substr(i, j - i + 1), i, j, curLine, count, lenC);
			if (isName)
				name.push_back(tmp);
			else
				lines.push_back(tmp);
			i = j + 1;
			cnt++;
		}
	}
	void readFile(string fileDir)
	{
		dirHandler dir(fileDir);
		string str = dir.readAll();
		string line = "";
		int curLine = 0;
		int len = str.length();
		for (int i = 0; i < len; i++)
			if (str[i] == '\n')
			{
				modifyLine(curLine++, line, false);
				line = "";
			}
			else
				line = line + str[i];
		if (line != "") modifyLine(curLine++, line, false);
	}
	void printPage(int curPage)
	{
		clearPage();
		if (curPage > 0) {
			goToXY(xL, yLR); cout << char(17) << endl;
		}
		if (curPage < numPages) {
			goToXY(xR, yLR); cout << char(16) << endl;
		}
		int len = lines.size() - 1;
		int startLine = curPage * cntLines, endLine = min(startLine + cntLines - 1, len);
		for (int i = 0; i < int(name.size()); i++) {
			goToXY(posX, posY + i); name[i].printLine(14);
		}
		for (int i = startLine; i <= endLine; i++) {
			goToXY(posX, posY + name.size() + 1 + i - startLine); lines[i].printLine(8);
		}
		goToXY(posX + lenLines - 5, posY + cntLines + name.size() + 2); cout << curPage + 1 << "/" << numPages + 1;
	}
	void clearPage()
	{
		int nameSize = name.size();
		for (int i = posY; i < posY + cntLines + nameSize + 1; i++)
		{
			goToXY(posX, i);
			for (int i = 0; i < lenLines; i++) cout << ' ';
		}
		goToXY(xL, yLR); cout << ' ';
		goToXY(xR, yLR); cout << ' ';
		goToXY(posX + lenLines - 5, posY + cntLines + name.size() + 2); cout << "      ";
	}
	bool movePages()
	{
		bool isArrow = false;
		int keyValue;
		while (true)
		{
			if (_kbhit())
			{
				keyValue = toupper(_getch());
				if (keyValue == 27)
					return false;
				if (keyValue == 224)
				{
					isArrow = true;
					continue;
				}
				if (isArrow)
				{
					if (keyValue == 75 && curPages > 0)
						printPage(--curPages);
					if (keyValue == 77 && curPages < numPages)
						printPage(++curPages);
					break;
				}
			}
		}
		return true;
	}
};

#endif // !_PAGES_H_

