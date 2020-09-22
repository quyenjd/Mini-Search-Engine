#ifndef SE_OPERATIONS_H
#define SE_OPERATIONS_H

#include "Query.h"
#include <algorithm>
#include <limits>
#include <stack>

#define lD(x, y) (q->words[A[y].first].occurrences[fileId][A[y].second].line - q->words[A[x].first].occurrences[fileId][A[x].second].line)
#define li(x, y) (q->words[A[y].first].occurrences[fileId][A[y].second].id - q->words[A[x].first].occurrences[fileId][A[x].second].id)

namespace Operations
{
    // I use my modified version of Ratcliff/Obershelp for finding similiarity between two vectors.
    struct modifiedRatcliffObershelp
    {
    private:
        int longestCommonSubarray(std::pair<int, int>& endp)
        {
            if (posA.empty() || posB.empty())
                return 0;

            std::vector<std::vector<int> > lcs;
            for (int i = 0; i <= *std::prev(posA.end()); ++i)
                lcs.push_back(std::move(std::vector<int>(*std::prev(posB.end()) + 1, 0)));

            int res = 0, lineDelta = 0, idDelta = 0;
            endp.first = endp.second = -1;
            for (int i : posA)
                for (int j : posB)
                    if (A[i].first == B[j] || A[i].first == -1 || B[j] == -1)
                        if ((lcs[i][j] = (i && j ? lcs[i - 1][j - 1] : 0) + 1) >= res)
                        {
                            if (lcs[i][j] > res)
                            {
                                res = lcs[i][j];
                                endp.first = i;
                                endp.second = j;
                                lineDelta = lD(i - res + 1, i);
                                idDelta = li(i - res + 1, i);
                            }
                            else
                            {
                                int _ld = lD(i - res + 1, i),
                                    _li = li(i - res + 1, i);
                                if (_ld < lineDelta || (_ld == lineDelta && _li < idDelta))
                                {
                                    endp.first = i;
                                    endp.second = j;
                                    lineDelta = _ld;
                                    idDelta = _li;
                                }
                            }
                        }
            return res;
        }

        int recursiveCaller()
        {
            std::pair<int, int> endp;
            int lcs = longestCommonSubarray(endp);
            if (lcs > 0)
            {
                for (int i = endp.first - lcs + 1; i <= endp.first; ++i)
                {
                    savedAPos.push_back(q->words[A[i].first].occurrences[fileId][A[i].second]);
                    posA.erase(i);
                }
                for (int i = endp.second - lcs + 1; i <= endp.second; ++i)
                    posB.erase(i);

                lcs += recursiveCaller();
            }
            return lcs;
        }

        double _score;

        queryData* q;
        int fileId;
        std::vector<pii> A;
        std::vector<int> B;
        std::set<int> posA, posB;
        std::vector<baseNode> savedAPos;

    public:
        modifiedRatcliffObershelp(queryData* _q, int _fileId, const std::vector<pii>& _A, const std::vector<int>& _B)
        {
            savedAPos.clear();
            q = _q, fileId = _fileId, A = _A, B = _B;

            // If one of the vectors are empty, the score is certainly 0
            if (A.empty() || B.empty())
            {
                _score = 0.0;
                return;
            }

            // If the vectors are identical, the score is certainly 1
            if (A.size() == B.size())
            {
                bool flag = true;
                for (size_t i = 0; i < A.size(); ++i)
                    if (A[i].first != B[i])
                    {
                        flag = false;
                        break;
                    }
                if (flag)
                {
                    for (size_t i = 0; i < A.size(); ++i)
                        savedAPos.push_back(q->words[A[i].first].occurrences[fileId][A[i].second]);
                    _score = 1.0;
                    return;
                }
            }

            posA.clear(); posB.clear();
            for (size_t i = 0; i < A.size(); ++i)
                posA.insert(i);
            for (size_t i = 0; i < B.size(); ++i)
                posB.insert(i);

            _score = (double)2.0 * recursiveCaller() / (A.size() + B.size());
        }

        double score() const
        {
            return _score;
        }

        std::vector<baseNode> getLCSPositionsInA() const
        {
            return savedAPos;
        }
    };

    // This function takes the information (positions, file indexes) of all the words
    // in the query and score the results based on relativeness.
    void opRanking(queryData* q)
    {
        // Loop through all available fileIds
        for (auto it = q->matchIDs.begin(); it != q->matchIDs.end(); ++it)
        {
            int fileId = it->first, numWords = 0;

            // Prepare and sort the occurrence list
            std::vector<pii> occurs;
            for (size_t i = 0; i < q->words.size(); ++i)
                if (!q->words[i].isExcluded() && !q->words[i].isWild())
                {
                    auto it2 = q->words[i].occurrences.find(fileId);
                    if (it2 == q->words[i].occurrences.end())
                        continue;
                    ++numWords;
                    for (size_t j = 0; j < it2->second.size(); ++j)
                        occurs.push_back(std::make_pair(i, j));
                }
            std::sort(occurs.begin(), occurs.end(), [q, fileId](pii a, pii b) {
                baseNode aa = q->words[a.first].occurrences[fileId][a.second],
                    bb = q->words[b.first].occurrences[fileId][b.second];
                return aa == bb ? (q->words[a.first] < q->words[b.first]) : (aa < bb);
                });

            // If identical occurrences are detected, pick the one with highest priority and remove the rest
            if (occurs.begin() != occurs.end())
            {
                auto res = occurs.begin(), fi = occurs.begin();
                while (++fi != occurs.end())
                    if (!(q->words[res->first].occurrences[fileId][res->second] == q->words[fi->first].occurrences[fileId][fi->second]))
                        *(++res) = *fi;
                occurs.resize(distance(occurs.begin(), ++res));
            }

            // Prepare vector of words
            std::vector<int> levQuery;
            for (size_t i = 0; i < q->words.size(); ++i)
                if (!q->words[i].isExcluded())
                    levQuery.push_back(q->words[i].isWild() ? -1 : i);

            // Calculate the score using Ratcliff/Obershelp formula
            modifiedRatcliffObershelp matcher(q, fileId, occurs, levQuery);
            it->second = matcher.score() + numWords;

            // Insert data for highlighting
            for (baseNode x : matcher.getLCSPositionsInA())
                q->highlights.push_back(x);
        }
    }

    void opInclude(queryData* q)
    {
        for (size_t i = 0; i < q->words.size(); ++i)
            if (q->words[i].isIncluded())
            {
                for (auto it = q->matchIDs.begin(); it != q->matchIDs.end(); )
                    if (q->words[i].occurrences.find(it->first) == q->words[i].occurrences.end())
                        it = q->matchIDs.erase(it);
                    else
                        ++it;
            }
    }

    void opExclude(queryData* q)
    {
        for (size_t i = 0; i < q->words.size(); ++i)
            if (q->words[i].isExcluded())
                for (auto it = q->words[i].occurrences.begin(); it != q->words[i].occurrences.end(); ++it)
                    q->matchIDs.erase(it->first);
    }

    void opResultFilter(queryData* q, baseData* bd, bool includeStopwords) { // an - operator& operator|
        size_t start_index = 0, i = 0;
        std::map<int, double> mapRes = std::move(q->matchIDs);
        std::vector<baseNode> highlights;
        std::stack<std::pair<queryData, int> > qD;
        std::string qq = q->query().to_str();
        while (i < qq.length()) {
            while (qq[i] != '&' && qq[i] != '|') {
                i++;
                if (i == qq.length()) break;
            }
            queryData subq(qq.substr(start_index, i - start_index), !includeStopwords);
            for (size_t i = 0; i < bd->fileNames.size(); ++i)
                subq.matchIDs[i] = -0x7f800000;
            for (size_t i = 0; i < subq.words.size(); i++)
                subq.words[i].mapOccurrences(bd);
            opExclude(&subq);
            opInclude(&subq);
            opRanking(&subq);
            for (baseNode x : subq.highlights)
                highlights.push_back(x);
            qD.push(std::make_pair(std::move(subq), i == qq.length() ? 0 : (qq[i] == '&' ? 1 : 2)));
            start_index = ++i;
        }
        while (!qD.empty()) {
            std::pair<queryData, int> qqq;
            qqq = std::move(qD.top());
            qD.pop();
            if (qqq.second == 0) { //no operation
                mapRes = std::move(qqq.first.matchIDs);
            }
            if (qqq.second == 1) { //AND operation
                for (auto it = mapRes.begin(); it != mapRes.end(); it++) {
                    it->second = std::min(it->second, qqq.first.matchIDs[it->first]);
                }
            }
            if (qqq.second == 2) { //OR operation
                for (auto it = qqq.first.matchIDs.begin(); it != qqq.first.matchIDs.end(); it++) {
                    mapRes[it->first] = std::max(mapRes[it->first], it->second);
                }
            }
        }
        for (auto it = mapRes.begin(); it != mapRes.end();) {
            if (fabs(it->second) < std::numeric_limits<double>::epsilon())
                it = mapRes.erase(it);
            else it++;
        }
        highlights.resize(distance(highlights.begin(),
            std::remove_if(highlights.begin(), highlights.end(),
                [mapRes](baseNode x) { return mapRes.find(x.fileInd) == mapRes.end(); })));
        std::sort(highlights.begin(), highlights.end());
        highlights.resize(distance(highlights.begin(), std::unique(highlights.begin(), highlights.end())));
        q->highlights = std::move(highlights);
        q->matchIDs = std::move(mapRes);
    }

    void opDataFilter(queryData* q, baseData* bd) // quyen - operator intitle: filetype:
    {
        q->matchIDs.clear();

        std::string str = q->query().to_str();
        for (size_t i = 0; i < str.length(); ++i)
            str[i] = upper(str[i]);

        std::istringstream iss(str);
        std::string formatted, word;
        std::vector<std::string> intitles, filetypes;
        while (iss >> word)
        {
            if (word.substr(0, 8) == "INTITLE:")
                intitles.push_back(word.substr(8, std::string::npos));
            else
                if (word.substr(0, 9) == "FILETYPE:")
                    filetypes.push_back(word.substr(9, std::string::npos));
                else
                    formatted += word + ' ';
        }

        // Get fileInd with specified filetypes
        for (size_t i = 0; i < bd->fileNames.size(); ++i)
            q->matchIDs[i] = filetypes.empty() ? 0.5 : 0.0;
        for (size_t i = 0; i < filetypes.size(); ++i)
            filetypes[i].erase(std::remove_if(filetypes[i].begin(), filetypes[i].end(), [](char x) { return !normal(x); }), filetypes[i].end());
        for (size_t i = 0; i < filetypes.size(); ++i)
            for (auto it = q->matchIDs.begin(); it != q->matchIDs.end(); ++it)
            {
                std::string extension = dirHandler(bd->fileNames[it->first]).fileExt();
                for (size_t k = 0; k < extension.length(); ++k)
                    extension[k] = upper(extension[k]);

                if (extension == filetypes[i])
                    it->second = 0.5;
            }
        for (auto it = q->matchIDs.begin(); it != q->matchIDs.end(); )
            if (fabs(it->second) < std::numeric_limits<double>::epsilon())
                it = q->matchIDs.erase(it);
            else
                ++it;

        // Get fileInd of which title contains words in 'intitles'
        for (size_t i = 0; i < intitles.size(); ++i)
        {
            std::replace_if(intitles[i].begin(), intitles[i].end(), [](char x) { return !normal(x); }, ' ');
            queryData qi(intitles[i]);
            for (size_t j = 0; j < qi.words.size(); ++j)
            {
                qi.words[j].inTitle = true;
                qi.words[j].mapOccurrences(bd);
            }

            for (auto it = q->matchIDs.begin(); it != q->matchIDs.end(); )
            {
                bool flag = false;
                for (size_t j = 0; j < qi.words.size(); ++j)
                    if (qi.words[j].occurrences.find(it->first) == qi.words[j].occurrences.end())
                    {
                        flag = true;
                        break;
                    }

                if (flag)
                    it = q->matchIDs.erase(it);
                else
                    ++it;
            }
        }

        std::map<int, double> tmp = std::move(q->matchIDs);
        (*q) = std::move(queryData(formatted));
        q->matchIDs = std::move(tmp);
    }

    // The only search function to call, returning the processing time of the query.
    double opWrapper(queryData* q, baseData* bd, bool includeStopwords)
    {
        double time = clock();

        opDataFilter(q, bd);
        opResultFilter(q, bd, includeStopwords);

        return (clock() - time) / CLOCKS_PER_SEC;
    }
}

#endif // SE_OPERATIONS_H
