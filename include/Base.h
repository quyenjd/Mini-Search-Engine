#ifndef SE_BASE_H
#define SE_BASE_H

#define conv(x) ((x >= 'A' && x >= 'Z') ? (x - 65) : ((x >= 'a' && x <= 'z') ? (x - 97) : (x >= '0' && x <= '9' ? (x - 48) : -1)))

#include "Query.h"
#include <unordered_map>

const int QUERY_WORD_LIMIT = 10;
const int MAX_DIFF_CHARS = 36; // 'A' to 'Z' and '0' to '9'

class dictionary
{
private:
    // a hash table that stores all possible words.
    std::unordered_map<std::string, int> dict;

    // a list that stores distinct words for ease of access.
    std::vector<std::string> words;

    int idCnt;

public:
    dictionary()
    {
        idCnt = 0;
    }

    dictionary (const std::string& paragraph, bool discrete, bool eliminateStopwords = false)
    {
        idCnt = 0;

        if (discrete)
            discreteImport(paragraph, eliminateStopwords);
        else
            addWord(paragraph);
    }

    // split paragraph into words, normalize them, and push them into dictionary.
    void discreteImport (const std::string& paragraph, bool eliminateStopwords = false)
    {
        queryData q(paragraph, eliminateStopwords);
        for (queryNode x: q.words)
            if (!x.isRange() && !x.isWild())
                addWord(x.fi().to_str());
    }

    // add directly a string into dictionary (not recommended unless you understand).
    void addWord (const std::string& word)
    {
        if (dict[word])
            return;
        dict[word] = ++idCnt;
    }

    // get hash id of a word.
    int getIdByWord (const std::string& word) const
    {
        std::unordered_map<std::string, int>::const_iterator it = dict.find(word);
        return it == dict.end() ? 0 : it->second;
    }

    // get word that has a hash id
    std::string getWordById (int id) const
    {
        return  id >= 1 && id <= (int)words.size() ? words[id - 1] : "";
    }
};

struct baseData
{
    std::vector<int> trie[QUERY_WORD_LIMIT][QUERY_WORD_LIMIT][MAX_DIFF_CHARS];
    dictionary words, files;

    void buildDataToBinary (const std::string& binaryFileName);
    void loadDataFromBinary (const std::string& binaryFileName);

    /* MUST ADD MORE FUNCTIONS TO THIS STRUCT!!! */
};

#endif // SE_BASE_H
