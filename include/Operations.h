#ifndef SE_OPERATIONS_H
#define SE_OPERATIONS_H

#include "Query.h"
#include <algorithm>

typedef std::pair<int, int> pii;

const int SEPARATION_THRESHOLD = 3;

namespace Operations
{
    // Levenshtein distance for approximately comparing vectors.
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
                F1[i] = std::min(std::min(F0[i], F1[i - 1]) + 1, F0[i - 1] + (row_i != -1 && col_j != -1 && row_i != col_j));
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
            std::sort(occurs.begin(), occurs.end(), [q, fileId](pii a, pii b)
                      { return q->words[a.first].occurrences[fileId][a.second] < q->words[b.first].occurrences[fileId][b.second]; });

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

    void opAND (queryData* q); // ?
    void opOR (queryData* q); // ?
    void opTitle (queryData* q); // quyen
    void opFileType (queryData* q); // quyen
    void opSynonym (queryData* q); // an

    void opWrapper (queryData* q, baseData* bd) // an
    {
        // Prepare data for words in query

        // Call operator functions once by once

        // Call ranking function to evaluate the documents
        opRanking(q);
    }
}

std::vector<baseNode> vectorAnd (std::vector <baseNode> a, std::vector <baseNode> b)
{
    size_t i = 0, j = 0;
    std::vector <baseNode> res;
    while (i < a.size() && j < b.size())
    {
        while (i < a.size() && j < b.size() && (a[i].fileInd != b[j].fileInd  || a[i].line != b[j].line))
        {
            if (a[i].fileInd < b[j].fileInd || (a[i].fileInd == b[j].fileInd && a[i].line < b[j].line))
                i++;
            else
                j++;
        }
        if (a[i].pos < b[j].pos)
        {
            if (a[i].pos + a[i].len + SEPARATION_THRESHOLD >= b[j].pos)
            {
                // a[i] next to b[j]
                res.push_back(baseNode(a[i].fileInd, a[i].pos, a[i].line, b[j].pos - a[i].pos + b[j].len, a[i].isTitle));
                j++;
            }
            i++;
        }
        else
        {
            if (b[j].pos + b[j].len + SEPARATION_THRESHOLD >= a[i].pos)
            {
                // b[j] next to a[i]
                res.push_back(baseNode(b[j].fileInd, b[j].pos, b[j].line, a[i].pos - b[j].pos + a[i].len, b[j].isTitle));
                i++;
            }
            j++;
        }
    }
    return res;
}

std::vector<baseNode> vectorOr (std::vector <baseNode> a, std::vector <baseNode> b)
{
    // Just make a simple merging
    size_t i = 0, j = 0;
    std::vector <baseNode> res;
    while (i < a.size() && j < b.size())
    {
        if (a[i] < b[j])
            res.push_back(a[i++]);
        else
            res.push_back(b[j++]);
    }
    while (i < a.size())
        res.push_back(a[i++]);
    while (j < b.size())
        res.push_back(b[j++]);
    return res;
}

#endif // SE_OPERATIONS_H
