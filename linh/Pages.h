#ifndef _PAGES_H_
#define _PAGES_H_

#include "Gui.h"
#include <vector>
#include "Base.h"

#define PAGES_POSX 70
#define PAGES_POSY 8
#define PAGES_CNT 30
#define PAGES_LEN 60

string getName(string dir)
{
	int i = dir.rfind("\\") + 1, j = dir.length();
	return dir.substr(i, j - i);
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
	void printLine()
	{
		for (int i = 0; i < str.length(); i++)
		{
			if (check[i])
				textColor(0, 12);
			else
				textColor(0, 13);
			cout << str[i];
		}
	}
};
class Pages
{
private:
	int posX, posY;
	int cntLines, lenLines;
	int xL, xR, yLR;
	vector<Line> lines;
	vector<baseNode> words;
public:
	int numPages, curPages;
	Pages(File file)
	{
		posX = PAGES_POSX; posY = PAGES_POSY;
		cntLines = PAGES_CNT; lenLines = PAGES_LEN;
		words = file.words;
		readFile(file.dir);
		numPages = (lines.size() - 1) / cntLines;
		curPages = 0;
		xL = posX - 2; xR = posX + lenLines + 1;
		yLR = posY + (cntLines - 1) / 2;
	}
	void clear()
	{
		for (int i = 0; i < lines.size(); i++)
			if (lines[i].check)
				delete[] lines[i].check;
		lines.clear();
	}
	bool isHL(int k, int curLine)
	{
		for (int i = 0; i < words.size(); i++)
			if (curLine == words[i].line && k == words[i].pos)
				return true;
		return false;
	}
	void buildLine(Line& line, string str, int i, int j, int curLine)
	{
		line.str = str; 
		line.check = new bool[str.length()+5];
		int k = i;
		while (k <= j)
		{
			if (isHL(k, curLine))
				while (k <= j && isalpha(line.str[k - i]))
				{
					line.check[k - i] = true;
					k++;
				}
			else
			{
				line.check[k - i] = false;
				k++;
			}
				
		}
	}
	void modifyLine(int curLine, string line)
	{
		if (line.length() == 0)
		{
			Line tmp;
			tmp.str = "";
			lines.push_back(tmp);
			return;
		}
		int j, len = line.length();
		int cnt = 0;
		for (int i = 0; i < len;)
		{
			j = min(i + lenLines - 1, len - 1);
			if (j + 1 < len && line[j + 1] != char(32))
				while (i <= j && line[j] != char(32)) j--;
			if (j < i) j = min(i + lenLines - 1, len - 1);
			Line tmp;
			buildLine(tmp, line.substr(i, j - i + 1), i, j, curLine);
			lines.push_back(tmp);
			i = j + 1;
			cnt++;
		}
	}
	void readFile(string fileDir)
	{
		dirHandler dir(fileDir);
		string str = dir.readAll();
		//cout << fileDir << endl;
		//cout << str;
		//return;
		string line = "";
		int curLine = 0;
		for (int i = 0; i < str.length(); i++)
			if (str[i] == '\n')
			{
				modifyLine(curLine++, line);
				line = "";
			}
			else
				line = line + str[i];
		if (line != "") modifyLine(curLine++, line);
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
		for (int i = startLine; i <= endLine; i++) {
			goToXY(posX, posY + i - startLine); lines[i].printLine();
		}
		goToXY(posX + lenLines - 5, posY + cntLines + 1); cout << curPage + 1 << "/" << numPages + 1;
	}
	void clearPage()
	{
		for (int i = posY; i < posY + cntLines; i++)
		{
			goToXY(posX, i);
			for (int i = 0; i < lenLines; i++) cout << ' ';
		}
		goToXY(xL, yLR); cout << ' ';
		goToXY(xR, yLR); cout << ' ';
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

