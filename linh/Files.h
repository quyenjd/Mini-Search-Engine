#ifndef _FILES_H_
#define _FILES_H_

#include "Pages.h"

#define FILES_POSX 10
#define FILES_POSY 8
#define FILES_CNT 10
#define FILES_LEN 40

vector<baseNode> listWord(int fileId, queryData* query, baseData* data)
{
	vector<baseNode> words;
	for (auto x : query->words)
	{
		x.mapOccurrences(data);
		vector<baseNode> word = x.occurrences[fileId];
		words.insert(words.end(), word.begin(), word.end());
	}
	return words;
}

class FilesList
{
private:
	int posX = FILES_POSX, posY = FILES_POSY;
	int cntFiles = FILES_CNT, lenFiles = FILES_LEN;
	vector<File> files;
public:
	int numFiles = -1, curFiles = 0;
	FilesList(vector<pair<int, double>> listFile, baseData* data, queryData* query)
	{
		if (listFile.size() == 0)
		{
			cout << "Not found" << endl;
			return;
		}
		numFiles = listFile.size();
		for (auto x : query->words)
			x.mapOccurrences(data);
		for (int i = 0; i < numFiles; i++)
		{
			File file;
			file.dir = data->fileNames[listFile[i].first];
			file.filename = getName(file.dir);
			file.shortName = shortenName(file.filename);
			file.score = listFile[i].second;
			file.words = listWord(listFile[i].first, query, data);
			files.push_back(file);
		}
	}
	void clear()
	{
		files.clear();
	}
	string shortenName(string name) 
	{
		if (name.length() <= lenFiles)
			return name;
		else
			return name.substr(0, lenFiles - 3) + "...";
	}
	void printList(int num)
	{
		textColor(0, 12);
		goToXY(posX + 11, posY); cout << "<<<LIST FILES>>>" << endl;
		for (int i = 0; i < numFiles; i++)
			if (i == num)
			{
				textColor(0, 13);
				goToXY(posX - 1, posY + 2 + i * 2); cout << char(16) << files[i].shortName;
				goToXY(posX + 1, posY + 3 + i * 2); cout << fixed << setprecision(2) << "Score: " << files[i].score;
				textColor(0, 12);
			}
			else
			{
				goToXY(posX - 1, posY + 2 + i * 2);	cout << char(32) << files[i].shortName;
				goToXY(posX + 1, posY + 3 + i * 2); cout << fixed << setprecision(2) << "Score: " << files[i].score;
			}
	}
	bool moveFiles()
	{
		if (numFiles < 0) return false;
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
					isArrow = false;
					if (keyValue == 72 && curFiles > 0)
						printList(--curFiles);
					if (keyValue == 80 && curFiles < numFiles - 1)
						printList(++curFiles);
					break;
				}
				if (keyValue == 13)
					showFiles(curFiles);
			}
		}
		return true;
	}
	void showFiles(int num)
	{
		Pages pages(files[num]);
		drawBoard(PAGES_POSX - 5, PAGES_POSY - 2, PAGES_CNT + 4, PAGES_LEN + 10, 13);
		pages.printPage(0);
		while (true)
		{
			if (!pages.movePages())
				break;
		}
		pages.clear();
	}
};
#endif // !_FILES_H_

