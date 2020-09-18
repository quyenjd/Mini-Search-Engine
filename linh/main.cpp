#include <iostream>
#include "Query.h"
#include "Base.h"
#include "Files.h"
#include "Operations.h"

#define yn(x) ((x) ? "[Y]" : "[N]")

using namespace std;

int main()
{
    resizeConsole(1200, 700);
    fixConsoleWindow();
	baseData _data;
	//_data.loadFromFiles("D://CS163//Final project//FinalProject//FinalProject//data");
	//_data.saveToFile();
	//_data.clear();
	baseData data;
	data.readFromFile();
	while (true)
	{
		system("cls");
		cout << "Your Query: ";
		string str; if (!getQuery(str)) break;
		system("cls");
		goToXY(22, 3); cout << "YOUR QUERY: " << str << endl;
		drawBoard(20, 2, 2, 100, 13);
		queryData q1 = queryData(str);
		vector<pair<int, double>> listFile;
		//listFile = q1.getResults();
		pair<int, double> tmp(0, 10);
		listFile.push_back(tmp);
		listFile.push_back(tmp);

		FilesList filesList(listFile, &data, &q1);
		drawBoard(FILES_POSX - 5, FILES_POSY - 2, FILES_CNT * 2 + 6, FILES_LEN + 10, 12);
		HidePointer();
		filesList.printList(0);
		while (true)
		{
			if (!filesList.moveFiles())
				break;
		}
		ShowPointer();
	}
	data.clear();
    return 0;
}
