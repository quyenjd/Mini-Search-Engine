#include <iostream>
#include "Query.h"
#include "Base.h"
#include "Operations.h"

using namespace std;

int main()
{
    baseData bd;
    bd.loadFromFiles("data");
    bd.theLoadFromCSV();

    while (true)
    {
        system("cls");

        cout << "    Your query: ";
        std::string x; getline(cin, x);

        queryData q1 = queryData(x), q2 = queryData(x, true);

        cout << endl
             << " [With stopwords]" << endl;
        for (auto x: q1.words)
        {
            cout << " V1=" << x.fi().to_str() << "  V2=" << x.se().to_str() << endl
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

            cout << "    Occurs:  ---" << endl;
            x.mapOccurrences(&bd);
            for (auto it = x.occurrences.begin(); it != x.occurrences.end(); ++it)
            {
                cout << dirHandler(bd.fileNames[it->first]).fileName()
                     << " (1st occur at line=" << it->second[0].line << " pos=" << it->second[0].pos << ")" << endl;
            }
            cout << "             ---" << endl;
        }

        cout << endl
             << " [Without stopwords]" << endl;
        for (auto x: q2.words)
            cout << " V1=" << x.fi().to_str() << "  V2=" << x.se().to_str() << endl;

        cout << endl;
        system("pause");
    }

    return 0;
}
