#ifndef SE_OPERATIONS_H
#define SE_OPERATIONS_H

#include "Query.h"
#include <algorithm>

namespace Operations
{
    // I use Wagner-Fischer for approximately comparing vectors.
    double LevenshteinDist (const std::vector<int>& row, const std::vector<int>& col)
    {
        int rowLength = row.size(), colLength = col.size(),
            row_i, col_j;

        if (rowLength == 0)
            return colLength;
        if (colLength == 0)
            return rowLength;

        std::vector<int> F0(rowLength + 1), F1(rowLength + 1);

        for (int i = 0; i <= rowLength; ++i)
            F0[i] = i;

        for (int j = 1; j <= colLength; ++j)
        {
            F1[0] = j;
            col_j = col[j - 1];

            for (int i = 1; i <= rowLength; ++i)
            {
                row_i = row[i - 1];
                F1[i] = std::min(std::min(F0[i], F1[i - 1]) + 1, F0[i - 1] + (row_i != -1 && col_j != -1 && row_i != col_j) * 2);
            }

            std::swap(F0, F1);
        }

        // Normalization: inconsistency ratio
        return (double)(F0[rowLength] - abs(rowLength - colLength)) / std::min(rowLength, colLength);
    }


    // This function takes the information (positions, file indexes) of all the words
    // in the query and score the results based on relativeness.
    void opRanking (queryData* q)
    {
        // Loop through all available fileIds
        for (auto it = q->matchIDs.begin(); it != q->matchIDs.end(); ++it)
        {
            int fileId = it->first;

            // Prepare and sort the occurrence list
            std::vector<pii> occurs;
            for (size_t i = 0; i < q->words.size(); ++i)
                if (!q->words[i].isExcluded() && !q->words[i].isWild())
                    for (size_t j = 0; j < q->words[i].occurrences[fileId].size(); ++j)
                        occurs.push_back(std::make_pair(i, j));
            std::sort(occurs.begin(), occurs.end(), [q, fileId](pii a, pii b) {
                          baseNode aa = q->words[a.first].occurrences[fileId][a.second],
                                   bb = q->words[b.first].occurrences[fileId][b.second];
                          return aa == bb ? (q->words[a.first] < q->words[b.first]) : (aa < bb);
                      });

            // If identical occurrences are detected, pick the one with highest priority and remove the rest
            auto res = occurs.begin(), fi = occurs.begin();
            while (++fi != occurs.end())
                if (!(q->words[res->first].occurrences[fileId][res->second] == q->words[fi->first].occurrences[fileId][fi->second]))
                    *(++res) = *fi;
            occurs.resize(distance(occurs.begin(), ++res));

            // Prepare vector of words
            std::vector<int> levData, levQuery;
            for (size_t i = 0; i < occurs.size(); ++i)
                levData.push_back(occurs[i].first);
            for (size_t i = 0; i < q->words.size(); ++i)
                if (!q->words[i].isExcluded())
                    levQuery.push_back(q->words[i].isWild() ? -1 : i);

            // Calculate the score using Levenshtein distance
            it->second = LevenshteinDist(levData, levQuery);
        }
    }

    void opInclude (queryData* q)
    {
        std::map<int, double> matchIDs;
        for (size_t i = 0; i < q->words.size(); ++i)
            if (q->words[i].isIncluded())
            {
                for (auto it = q->words[i].occurrences.begin(); it != q->words[i].occurrences.end(); ++it)
                {
                    auto mit = q->matchIDs.find(it->first);
                    if (mit != q->matchIDs.end())
                        matchIDs[it->first] = mit->second;
                }
                q->matchIDs = matchIDs;
                matchIDs.clear();
            }
    }

    void opExclude (queryData* q)
    {
        for (size_t i = 0; i < q->words.size(); ++i)
            if (q->words[i].isExcluded())
                for (auto it = q->words[i].occurrences.begin(); it != q->words[i].occurrences.end(); ++it)
                    q->matchIDs.erase(it->first);
    }

    void opAND (queryData* q); // an
    void opOR (queryData* q); // an
    void opTitle (queryData* q); // quyen
    void opFileType (queryData* q); // quyen
    void opSynonym (queryData* q); // an

    void opWrapper (queryData* q, baseData* bd)
    {
        // Prepare data for words in query
        for (size_t i = 0; i < q->words.size(); ++i)
            q->words[i].mapOccurrences(bd);

        /* ===== CALLING OPERATOR FUNCTIONS HERE!!! ===== */

        // Call ranking function to evaluate the documents
        opRanking(q);
    }
}

#endif // SE_OPERATIONS_H
