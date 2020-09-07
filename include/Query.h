#ifndef SE_QUERY_H
#define SE_QUERY_H

#define number(x) (x >= '0' && x <= '9')
#define normal(x) ((x >= 'A' && x <= 'Z') || (x >= 'a' && x <= 'z') || number(x))
#define operat(x) (x == '-' || x == '+' || x == '*' || x == '#' || x == '$' || x == '~' || x == '"')
#define punc(x) (x == '\'')
#define all(x) (normal(x) || operat(x) || punc(x))
#define upper(x) ((x >= 'a' && x <= 'z') ? char(x - 32) : x)

#include "Base.h"
#include "Utility.h"
#include <set>
#include <string>
#include <utility>
#include <vector>

typedef std::pair<double, int> pdi;

class queryNode
{
private:
    multitype V1, V2;
    bool _isWild, _isRange, _isIncluded, _isExcluded, _isSynonym;

public:
    std::map<int, std::vector<baseNode>> occurrences;

    queryNode()
    {
        V1 = V2 = multitype();
        _isWild = true;
        _isRange = _isIncluded = _isExcluded = _isSynonym = false;
    }

    queryNode (const multitype& _V1,
               bool __isIncluded = false,
               bool __isExcluded = false,
               bool __isSynonym = false)
    {
        V1 = _V1;
        _isWild = _isRange = false;
        _isSynonym = __isSynonym;

        // if SYNONYM mode is ON, INCLUDE mode is ON.
        _isIncluded = _isSynonym ? true : __isIncluded;

        // if INCLUDE mode is ON, EXCLUDE mode must always be OFF.
        _isExcluded = _isIncluded ? false : __isExcluded;
    }

    queryNode (const multitype& _V1, const multitype& _V2)
    {
        V1 = _V1;
        V2 = _V2;
        _isRange = true;
        _isWild = _isIncluded = _isExcluded = _isSynonym = false;
    }

    multitype& fi()
    {
        return V1;
    }
    multitype fi() const
    {
        return V1;
    }

    multitype& se()
    {
        return V2;
    }
    multitype se() const
    {
        return V2;
    }

    bool isWild() const
    {
        return _isWild;
    }

    bool isRange() const
    {
        return _isRange;
    }

    bool isIncluded() const
    {
        return _isIncluded;
    }

    bool isExcluded() const
    {
        return _isExcluded;
    }

    bool isSynonym() const
    {
        return _isSynonym;
    }

    // toggle WILD mode ON and OFF.
    // if WILD mode is OFF when calling this method, all variables will be reset.
    void toggleWild()
    {
        if (!_isWild)
        {
            V1 = V2 = multitype();
            _isRange = _isIncluded = _isExcluded = false;
        }
        _isWild ^= 1;
    }

    // toggle RANGE mode ON and OFF.
    // if RANGE mode is ON when calling this method, V2 will be reset.
    // if RANGE mode is OFF when calling this method, INCLUDE and EXCLUDE mode will be OFF.
    void toggleRange()
    {
        if (_isRange)
            V2 = multitype();
        else
            _isWild = _isIncluded = _isExcluded = _isSynonym = false;
        _isRange ^= 1;
    }

    // toggle INCLUDE mode ON and OFF.
    // if INCLUDE mode is OFF when calling this method, RANGE and EXCLUDE mode will be OFF.
    void toggleIncluded()
    {
        if (!_isIncluded)
        {
            if (_isRange)
                toggleRange();
            _isExcluded = false;
        }
        else
            _isSynonym = false;
        _isIncluded ^= 1;
    }

    // toggle EXCLUDE mode ON and OFF.
    // if EXCLUDE mode is OFF when calling this method, RANGE and INCLUDE mode will be OFF.
    void toggleExcluded()
    {
        if (!_isExcluded)
        {
            if (_isRange)
                toggleRange();
            _isIncluded = _isSynonym = false;
        }
        _isExcluded ^= 1;
    }

    // toggle SYNONYM mode ON and OFF.
    // if SYNONYM mode is OFF when calling this method, INCLUDE mode will be ON.
    void toggleSynonym()
    {
        if (!_isSynonym && !_isIncluded)
            toggleIncluded();
        _isSynonym ^= 1;
    }

    // map occurrences with fileInd
    void mapOccurrences (const std::vector<baseNode>& occurs)
    {
        for (size_t i = 0; i < occurs.size(); ++i)
            occurrences[occurs[i].fileInd].push_back(occurs[i]);
    }
};

class queryData
{
private:
    // original query.
    multitype _query;

    bool readNum (size_t& i, std::string& word, const std::string& str)
    {
        bool flag = true;
        for (; i < str.length() && all(str[i]); ++i)
        {
            if (!number(str[i]))
                flag = false;
            if ((!flag && normal(str[i])) || number(str[i]))
                word += str[i];
        }
        return flag;
    }

    void readStr (size_t& i, std::string& word, const std::string& str)
    {
        for (; i < str.length() && all(str[i]); ++i)
            if (normal(str[i]))
                word += upper(str[i]);
    }

    bool readQuoted (size_t& i, std::string& word, const std::string& str)
    {
        word = "\"";
        size_t tmp = ++i;
        for (; i < str.length(); ++i)
        {
            word += upper(str[i]);
            if (str[i] == '"')
            {
                ++i;
                return true;
            }
        }
        i = tmp;
        return false;
    }

public:
    // array of keywords in the query.
    std::vector<queryNode> words;

    // array of resulting files' IDs (after searching).
    std::set<pdi> matchIDs;

    queryData() {}

    // this method splits 'queryStr' into words and normalize them
    // before pushing into array 'words'.
    queryData (const multitype& queryStr, bool eliminateStopwords = false)
    {
        _query = queryStr;

        std::string str(queryStr.to_str()), tmp;
        bool rangeFlag = false;
        size_t i = 0;

        while (i < str.length())
        {
            // find first valid character.
            for (; i < str.length() && !all(str[i]); ++i);

            // if end of query, break here
            if (i == str.length())
                break;

            std::string word;
            bool numberFlag = false,
                 includedFlag = false,
                 excludedFlag = false,
                 synonymFlag = false;

            // if exact match, read everything inside the quotes or only one word.
            if (str[i] == '"')
            {
                if (!(includedFlag = readQuoted(i, word, str)))
                    continue;
            }
            else
            // if price, loop through all consequent digits.
            if (str[i] == '$')
            {
                word = "$";

                // if NaN, pop the dollar character.
                if (!(numberFlag = readNum(++i, word, str)) ||
                    /* if a word contains only dollar sign, erase it. */ word.length() == 1)
                    word.erase(0, 1);

                // if the string is now empty,
                // skip the rest to prevent a wild entry from being pushed.
                if (word.empty())
                    continue;
            }
            else
            // if digit, loop through all consequent digits.
            if (number(str[i]))
                numberFlag = readNum(i, word, str);
            else
            // if wild, next character should be empty.
            if (str[i] == '*')
            {
                if (i + 1 < str.length() && all(str[i + 1]))
                {
                    ++i;
                    continue;
                }
            }
            else
            // if included, set flag and read all upcoming characters.
            if (str[i] == '+')
            {
                readStr(++i, word, str);
                includedFlag = true;
            }
            else
            // if excluded, set flag and read all upcoming characters.
            if (str[i] == '-')
            {
                readStr(++i, word, str);
                excludedFlag = true;
            }
            else
            // if synonym, set flag and read all upcoming characters.
            if (str[i] == '~')
            {
                readStr(++i, word, str);
                synonymFlag = true;
            }
            else
            // if hash, just basically read the whole hash.
            if (str[i] == '#')
            {
                word = "#";
                readStr(++i, word, str);

                if (word.length() == 1)
                    continue;

                includedFlag = true;
            }
            else
            // if no operation is specified, just read.
                readStr(i, word, str);

            // after done processing a number,
            // check if a range query is happening.
            if (rangeFlag && numberFlag)
            {
                words.push_back(queryNode(tmp, word));
                rangeFlag = false;
            }
            else
            // check if this is a range query.
            if (numberFlag && i + 2 < str.length() && str[i] == '.' && str[i + 1] == '.')
            {
                i += 2;
                rangeFlag = true;
                tmp = word;
            }
            else
            {
                // if rangeFlag is still ON, push 'tmp' into the array first.
                if (rangeFlag)
                {
                    words.push_back(queryNode(tmp));
                    rangeFlag = false;
                }

                // if some flag is ON, push with flag.
                if (includedFlag || excludedFlag || synonymFlag)
                {
                    if (word.empty())
                        continue;
                    words.push_back(queryNode(word, includedFlag, excludedFlag, synonymFlag));
                }
                else
                // if 'word' is empty, push as wild.
                if (word.empty())
                {
                    ++i;
                    words.push_back(queryNode());
                }
                else
                // if no flag is marked, check stopword and push.
                if (!eliminateStopwords || stopwords.find(word) == stopwords.end())
                    words.push_back(queryNode(word));
            }
        }

        // if range flag is still ON here, push the buffered into array.
        if (rangeFlag)
        {
            words.push_back(queryNode(tmp));
            rangeFlag = false;
        }
    }

    multitype query() const
    {
        return _query;
    }
};

#endif // SE_QUERY_H
