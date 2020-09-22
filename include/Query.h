#ifndef SE_QUERY_H
#define SE_QUERY_H

#include "Base.h"
#include "Utility.h"
#include <set>
#include <string>
#include <vector>

inline void preNormalize(std::string& str)
{
    str.erase(std::remove_if(str.begin(), str.end(), [](char x) { return punc(x); }), str.end());
    for (size_t i = 0; i < str.length(); ++i)
        if (!normal(str[i]))
            str[i] = ' ';
}

class queryNode
{
private:
    multitype V1, V2;
    bool _isWild, _isRange, _isIncluded, _isExcluded, _isSynonym;

    std::vector<int> KMPMatching(const std::vector<pii>& text, const std::vector<int>& pat)
    {
        int N = text.size(), M = pat.size();
        std::vector<int> res;

        if (!N)
            return res;
        if (!M)
        {
            for (int i = 0; i < N; ++i)
                res.push_back(i);
            return res;
        }

        // Compute the longest prefix suffix values
        std::vector<int> lps(M);
        int len = lps[0] = 0;
        for (int i = 1; i < M; )
            if (pat[i] == pat[len])
                lps[i++] = ++len;
            else
            {
                if (len)
                    len = lps[len - 1];
                else
                    lps[i++] = 0;
            }

        // Find the pattern
        for (int i = 0, j = 0; i < N; )
        {
            if (pat[j] == text[i].first)
                ++i, ++j;
            if (j == M)
            {
                res.push_back(i - j);
                j = lps[j - 1];
            }
            else
                if (i < N && pat[j] != text[i].first)
                {
                    if (j)
                        j = lps[j - 1];
                    else
                        ++i;
                }
        }

        return res;
    }

    // Combine the occurrences of the words inside a sequence into a list of occurrences of that sequence.
    void combineOccurrences(std::vector<baseNode>& res, const std::vector<std::vector<baseNode> >& occurs)
    {
        // Prepare a full list of occurrences and sort it
        std::vector<pii> occurPointers;
        for (size_t i = 0; i < occurs.size(); ++i)
            for (size_t j = 0; j < occurs[i].size(); ++j)
                occurPointers.push_back(std::make_pair(i, j));
        std::sort(occurPointers.begin(), occurPointers.end(), [occurs](pii a, pii b)
            { return occurs[a.first][a.second] < occurs[b.first][b.second]; });

        // Make the list unique
        auto it = std::unique(occurPointers.begin(), occurPointers.end());
        occurPointers.resize(distance(occurPointers.begin(), it));

        // Prepare data for KMP
        std::vector<pii> KMPData;
        std::vector<int> KMPQuery;
        for (size_t i = 0; i < occurPointers.size(); ++i)
        {
            pii cur = occurPointers[i];
            if (i)
            {
                pii prev = occurPointers[i - 1];
                baseNode curID = occurs[cur.first][cur.second],
                    prevID = occurs[prev.first][prev.second];
                if (curID.fileInd != prevID.fileInd || curID.id - prevID.id > 1)
                    KMPData.push_back(std::make_pair(-1, 0));
            }
            KMPData.push_back(cur);
        }
        for (size_t i = 0; i < occurs.size(); ++i)
            KMPQuery.push_back(i);
        std::vector<int> KMP = std::move(KMPMatching(KMPData, KMPQuery));

        // Push the rest into the resulting vector
        res.clear();
        for (int x : KMP)
            res.push_back(occurs[KMPData[x].first][KMPData[x].second]);
    }

    void searchWrapper(baseData* bd, std::string query)
    {
        // Normalize the query string
        preNormalize(query);

        std::istringstream iss(query);
        std::vector<baseNode> res;
        std::vector<std::vector<baseNode> > occurs;

        std::string word;
        while (iss >> word)
            if (isNumber(word))
                occurs.push_back(std::move(bd->searchNumber(multitype(word).to_int(), multitype(word).to_int(), inTitle)));
            else
                occurs.push_back(std::move(bd->search(word, inTitle)));
        combineOccurrences(res, occurs);

        for (size_t i = 0; i < res.size(); ++i)
        {
            res[i].numWords = occurs.size();
            occurrences[res[i].fileInd].push_back(res[i]);
        }
    }

public:
    std::unordered_map<int, std::vector<baseNode> > occurrences;
    bool inTitle = false;

    queryNode()
    {
        V1 = V2 = multitype();
        _isWild = true;
        _isRange = _isIncluded = _isExcluded = _isSynonym = false;
    }

    queryNode(const multitype& _V1,
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

    queryNode(const multitype& _V1, const multitype& _V2)
    {
        V1 = _V1;
        V2 = _V2;
        _isRange = _isIncluded = true;
        _isWild = _isExcluded = _isSynonym = false;
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
            _isIncluded = !(_isWild = _isExcluded = _isSynonym = false);
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

    // Priority comparison
    bool operator< (const queryNode& q)
    {
        if (isWild())
            return !q.isWild();
        if (isExcluded() ^ q.isExcluded())
            return isExcluded();
        if (isIncluded() ^ q.isIncluded())
            return isIncluded();
        return ((isRange() | isSynonym()) ^ (q.isRange() | q.isSynonym())) && !(isRange() | isSynonym());
    }

    // map occurrences with fileInd, given the original data.
    void mapOccurrences(baseData* bd)
    {
        occurrences.clear();

        // if wildcard, no occurrences needed
        if (isWild())
            return;

        // if excluded or no properties, just search for the word
        if (isExcluded() || (!isIncluded() && !isSynonym() && !isRange()))
        {
            searchWrapper(bd, V1.to_str());
            return;
        }

        // if range, just searchNumber instead
        if (isRange())
        {
            std::string _V1(V1.to_str()), _V2(V2.to_str());
            if (_V1[0] == '$')
                _V1.erase(0, 1);
            if (_V2[0] == '$')
                _V2.erase(0, 1);

            std::vector<baseNode> res = bd->searchNumber(multitype(_V1).to_int(), multitype(_V2).to_int());
            for (size_t i = 0; i < res.size(); ++i)
                occurrences[res[i].fileInd].push_back(res[i]);
            return;
        }

        // if synonym, push also the occurrences of its synonym
        if (isSynonym())
        {
            std::vector<int> syns = bd->theSearch(V1.to_str());
            for (int x : syns)
                searchWrapper(bd, bd->theWords[x]);
        }

        // if just included, simply call the wrapper
        searchWrapper(bd, V1.to_str());
    }
};

class queryData
{
private:
    // original query.
    multitype _query;

    bool readNum(size_t& i, std::string& word, const std::string& str)
    {
        bool flag = true;
        for (; i < str.length() && (normal(str[i]) || punc(str[i])); ++i)
        {
            if (!number(str[i]))
                flag = false;
            word += str[i];
        }
        word.erase(std::remove_if(word.begin(), word.end(), [](char x) { return punc(x); }), word.end());
        for (; i < str.length() && all(str[i]) && !normal(str[i]); ++i)
            flag = false;
        return flag;
    }

    void readStr(size_t& i, std::string& word, const std::string& str)
    {
        for (; i < str.length() && (normal(str[i]) || punc(str[i])); ++i)
            word += upper(str[i]);
        word.erase(std::remove_if(word.begin(), word.end(), [](char x) { return punc(x); }), word.end());
        for (; i < str.length() && all(str[i]) && !normal(str[i]); ++i);
    }

    bool readQuoted(size_t& i, std::string& word, const std::string& str)
    {
        size_t tmp = ++i;
        for (; i < str.length(); ++i)
        {
            word += upper(str[i]);
            if (str[i] == '"')
            {
                // Normalize the result word
                preNormalize(word);

                std::istringstream iss(word);
                std::string x;
                word.clear();
                while (iss >> x)
                    word += x + ' ';
                if (!word.empty())
                    word.pop_back();

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

    // array of resulting file indexes (after searching).
    std::map<int, double> matchIDs;

    // array of matching words by file index.
    std::vector<baseNode> highlights;

    queryData() {}

    // this method splits 'queryStr' into words and normalize them
    // before pushing into array 'words'.
    queryData(const multitype& queryStr, bool eliminateStopwords = false)
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

    // get original query string.
    multitype query() const
    {
        return _query;
    }

    // filter the map 'matchIDs' to vector by eliminating underthreshold ones.
    std::vector<std::pair<int, double> > getScores(const double MATCHING_THRESHOLD = 0.5) const
    {
        std::vector<std::pair<int, double> > res;
        for (auto it = matchIDs.begin(); it != matchIDs.end(); ++it)
            if (it->second > MATCHING_THRESHOLD || fabs(it->second - MATCHING_THRESHOLD) < std::numeric_limits<double>::epsilon())
                res.push_back(*it);
        std::sort(res.begin(), res.end(), [](std::pair<int, double> a, std::pair<int, double> b) { return a.second > b.second; });
        return res;
    }

    std::vector<baseNode> getHighlightsByFileId(int fileId) const
    {
        std::vector<baseNode> res;
        for (size_t i = 0; i < highlights.size(); ++i)
            if (highlights[i].fileInd == fileId)
                res.push_back(highlights[i]);
        return res;
    }
};

#endif // SE_QUERY_H
