#include <iostream>
#include "Query.h"
#include "Base.h"
#include "Operations.h"

using namespace std;

int main()
{
    baseData bd;
    int tx = 0, ty = 0;
    bd.loadFromFiles("data" /* data source path */, tx, ty);
    bd.theLoadFromCSV();

    while (true)
    {
        system("cls");

        cout << "    Your query: ";
        std::string x; getline(cin, x);

        queryData q1 = queryData(x, true);

        cout << endl
             << "-------------- TOKENS --------------" << endl
             << endl;
        for (auto x: q1.words)
        {
            cout << "  V1=" << x.fi().to_str() << "  V2=" << x.se().to_str() << endl
                 << "    Props:  ";

            if (!x.isWild() && !x.isRange() && !x.isIncluded() && !x.isExcluded() && !x.isSynonym())
                cout << "None";
            else
                cout << (x.isWild() ? "(*)  " : "") << (x.isRange() ? "(A..B)  " : "")
                     << (x.isIncluded() ? "(+A)  " : "") << (x.isExcluded() ? "(-A)  " : "")
                     << (x.isSynonym() ? "(~A)  " : "");

            cout << endl;

            if (x.isSynonym())
            {
                vector<int> lis = bd.theSearch(x.fi().to_str());
                cout << "    Syns:   ";

                if (lis.empty())
                    cout << "None";
                else
                    for (size_t i = 0; i < lis.size(); ++i)
                    {
                        cout << bd.theWords[lis[i]];
                        if (i != lis.size() - 1)
                            cout << ",";
                    }

                cout << endl;
            }
        }

        Operations::opWrapper(&q1, &bd, false);
        vector<pair<int, double> > scores = q1.getScores();

        cout << endl
             << "---------- SEARCH RESULTS ----------" << endl
             << endl
             << "  Showing " << scores.size() << " result" << (scores.size() != 1 ? "s..." : "...") << endl
             << endl;

        for (auto x: q1.getScores())
        {
            cout << x.first << "\t" << dirHandler(bd.fileNames[x.first]).fileName() << " (" << x.second << ")" << endl;
            for (baseNode y: q1.getHighlightsByFileId(x.first))
                cout << "\t  line=" << y.line << " pos=" << y.pos << " numWords=" << y.numWords << endl;
        }

        system("pause");
    }

    return 0;
}
