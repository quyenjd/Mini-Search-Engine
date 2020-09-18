#ifndef SE_OPERATIONS_H
#define SE_OPERATIONS_H

#include "Query.h"
#include <algorithm>
#include <stack>

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

        // Normalization: consistency ratio in percentage (%)
        return (double)100.0 - (double)(F0[rowLength] - abs(rowLength - colLength)) * 100.0 / std::min(rowLength, colLength);
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

    void opExclude (queryData* q)
    {
        for (size_t i = 0; i < q->words.size(); ++i)
            if (q->words[i].isExcluded())
                for (auto it = q->words[i].occurrences.begin(); it != q->words[i].occurrences.end(); ++it)
                    q->matchIDs.erase(it->first);
    }

    void opResultFilter (queryData* q,baseData* bd){ // an - operator& operator|
        int start_index=0;
        int i=0;
        int op=0; //1 is AND 2 is OR
        for (int i=0;i<q.words.size();i++)
            q.words[i].mapOccurrences(bd);
        std::stack<std::pair<queryData*,int>> qD;
        std::string qq=q->query().to_str();
        while (i<=qq.length()){
            while (qq[i]!='&'&&qq[i]!='|'){
                i++;
                if (i==qq.length()) break;
            }
            if (qq[i]=='&') op=1;
            else op=2;
            opExclude(qq.substr(start_index,i-1));
            opInclude(qq.substr(start_index,i-1));
            opRanking(qq.substr(start_index,i-1));
            qD.push(std::move(qq.substr(start_index,i-1),op));
            start_index=i+1;
            op=0;
        }
        while (qD.size()>0){
            std::pair<queryData*,int> qqq;
            qqq=qD.top();
            qD.pop();
            op=qqq.second;
            std::map<int,double> mapRes;
            std::map<int,double> mapTemp;
            if (op==0){ //no operation
                mapRes=qqq.first->matchIDs;
                /*for (auto it=mapRes.begin();it!=mapRes.end();){
                    if (mapRes[it->first]==0)
                        it=mapRes.erase(it->first);
                    else it++;
                }*/
            }
            if (op==1){ //ADD operation
                mapTemp=qqq.first->matchIDs;
                for (auto it=mapTemp.begin();it!=mapTemp.end();it++){
                    mapRes[it->first]=std::min(it->second,mapRes[it->first]);
                }
            }
            if (op==2){ //OR operation
                mapTemp=qqq.first->matchIDs;
                for (auto it=mapTemp.begin();it!=mapTemp.end();it++){
                    mapRes[it->first]=mapRes[it->first]+it->second;
                }
            }
        }
    }

    void opDataFilter (queryData* q, baseData* bd) // quyen - operator intitle: filetype:
    {
        std::string str = q->query().to_str();
        for (size_t i = 0; i < str.length(); ++i)
            str[i] = upper(str[i]);

        std::istringstream iss(str);
        std::string formatted, word;
        std::vector<std::string> intitles, filetypes;
        while (iss >> word)
        {
            if (word.substr(0, 8) == "INTITLE:")
                intitles.push_back(word.substr(8, word.length() - 8));
            else
            if (word.substr(0, 9) == "FILETYPE:")
                filetypes.push_back(word.substr(9, word.length() - 9));
            else
                formatted += word + ' ';
        }

        // Get fileInd with specified filetypes
        for (size_t i = 0; i < filetypes.size(); ++i)
            filetypes[i].erase(std::remove_if(filetypes[i].begin(), filetypes[i].end(), [](char x){ return !normal(x); }), filetypes[i].end());
        for (auto it = q->matchIDs.begin(); it != q->matchIDs.end(); ++it)
        {
            std::string extension = dirHandler(bd->fileNames[it->first]).fileExt();
            for (size_t k = 0; k < extension.length(); ++k)
                extension[k] = upper(extension[k]);

            bool flag = false;
            for (size_t i = 0; i < filetypes.size(); ++i)
                if (extension == filetypes[i])
                {
                    flag = true;
                    break;
                }

            if (!flag)
                it = q->matchIDs.erase(it);
            else
                ++it;
        }

        // Get fileInd of which title contains words in 'intitles'
        for (size_t i = 0; i < intitles.size(); ++i)
        {
            std::replace_if(intitles[i].begin(), intitles[i].end(), [](char x){ return !normal(x); }, ' ');
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

        (*q) = std::move(queryData(formatted));
    }

    void opWrapper (queryData* q, baseData* bd)
    {
        // Prepare resulting map
        for (size_t i = 0; i < bd->fileNames.size(); ++i)
            q->matchIDs[i] = 1;

        /* ===== CALLING OPERATOR FUNCTIONS HERE!!! ===== */
        opDataFilter(q, bd);
        opResultFilter(q, bd);
    }
}

#endif // SE_OPERATIONS_H
