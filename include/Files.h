#ifndef _FILES_H_
#define _FILES_H_

#include "Pages.h"
using std::pair;
using std::fixed;
using std::setprecision;

class FilesList
{
private:
	int posX = FILES_POSX, posY = FILES_POSY;
	int cntFiles = FILES_CNT, lenFiles = FILES_LEN;
	vector<File> files;
	double time;
public:
	int numFiles = -1, curFiles = 0, totalFiles;
	FilesList(vector<pair<int, double>> listFile, baseData* data, queryData* query, double processingTime)
	{
		if (listFile.size() == 0) return;
		totalFiles = listFile.size();
		if (totalFiles > 10)
			numFiles = 10;
		else
			numFiles = totalFiles;
		for (int i = 0; i < numFiles; i++)
		{
			File file;
			file.dir = data->fileNames[listFile[i].first];
			file.filename = getName(file.dir);
			file.shortName = shortenName(file.filename, lenFiles);
			file.score = listFile[i].second;
			file.words = query->getHighlightsByFileId(listFile[i].first);
			files.push_back(file);
		}
		time = processingTime;
	}
	void clear()
	{
		files.clear();
	}
	void printList(int num)
	{
		textColor(0, 14);
		goToXY(posX + 1, posY);
		cout << "<<<TOP 10 RESULTS>>> (" << totalFiles << " found in " << fixed << setprecision(2) << time << "s)" << endl;
		textColor(0, 8);
		for (int i = 0; i < numFiles; i++)
			if (i == num)
			{
				textColor(0, 14);
				goToXY(posX - 1, posY + 2 + i * 2); cout << char(16) << files[i].shortName;
				goToXY(posX + 1, posY + 3 + i * 2); cout << fixed << setprecision(2) << "Score: " << files[i].score;
				textColor(0, 8);
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
				{
					drawBoard(FILES_POSX - 5, FILES_POSY - 2, FILES_CNT * 2 + 6, FILES_LEN + 10, 13);
					showFiles(curFiles);
					drawBoard(FILES_POSX - 5, FILES_POSY - 2, FILES_CNT * 2 + 6, FILES_LEN + 10, 12);
				}

			}
		}
		return true;
	}
	void showFiles(int num)
	{
		Pages pages(files[num]);
		drawBoard(PAGES_POSX - 5, PAGES_POSY - 2, PAGES_CNT + 4, PAGES_LEN + 10, 12);
		pages.printPage(pages.curPages);
		while (true)
		{
			if (!pages.movePages())
				break;
		}
		drawBoard(PAGES_POSX - 5, PAGES_POSY - 2, PAGES_CNT + 4, PAGES_LEN + 10, 13);
		pages.clear();
	}
};

#endif // !_FILES_H_

