#include <iostream>
#include "Query.h"
#include "Base.h"

#define yn(x) ((x) ? "[Y]" : "[N]")

using namespace std;

int main()
{
    while (true)
    {
        system("cls");

        cout << "    Your query: ";
        std::string x; getline(cin, x);

        queryData q1 = queryData(x), q2 = queryData(x, true);

        cout << endl
             << " [With stopwords]" << endl;
        for (auto x: q1.words)
            cout << " V1=" << x.fi().to_str() << "  V2=" << x.se().to_str() << endl
                 << "    Prop:    (*)? " << yn(x.isWild()) << "  (A..B)? " << yn(x.isRange())
                            << "  (+A)? " << yn(x.isIncluded()) << "  (-A)? " << yn(x.isExcluded())
                            << "  (~A)? " << yn(x.isSynonym()) << endl;

        cout << endl
             << " [Without stopwords]" << endl;
        for (auto x: q2.words)
            cout << " V1=" << x.fi().to_str() << "  V2=" << x.se().to_str() << endl
                 << "    Prop:    (*)? " << yn(x.isWild()) << "  (A..B)? " << yn(x.isRange())
                            << "  (+A)? " << yn(x.isIncluded()) << "  (-A)? " << yn(x.isExcluded())
                            << "  (~A)? " << yn(x.isSynonym()) << endl;

        cout << endl;
        system("pause");
    }
    return 0;
}
