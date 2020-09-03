#ifndef SE_OPERATIONS_H
#define SE_OPERATIONS_H

#include "Query.h"

const int SEPARATION_THRESHOLD = 3;

namespace Operations
{
    void opFilterByPos (queryData* q); // quyen
    
    void opAND (queryData* q, baseData* bd); // ?
    void opOR (queryData* q, baseData* bd); // ?
    void opInclude (queryData* q, baseData* bd); // an
    void opExclude (queryData* q, baseData* bd); // an
    void opTitle (queryData* q, baseData* bd); // quyen
    void opFileType (queryData* q, baseData* bd); // quyen
    void opWildcard (queryData* q, baseData* bd); // quyen
    void opRange (queryData* q, baseData* bd); // an
    void opSynonym (queryData* q, baseData* bd); // an
}

vector <baseNode> and (vector <baseNode> a, vector <baseNode> b)
{
    int i = 0, j = 0;
    vector <baseNode> res;
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

bool compPos (baseNode a, baseNode b)
{
    return (a.fileInd < b.fileInd || (a.fileInd == b.fileInd && (a.line < b.line || (a.line == b.line && a.pos < b.pos))));
}

vector <baseNode> or (vector <baseNode> a, vector <baseNode> b)
{
    // Just make a simple merging
    int i = 0, j = 0;
    vector <baseNode> res;
    while (i < a.size() && j < b.size())
    {
        if (compPos(a[i], b[j]))
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