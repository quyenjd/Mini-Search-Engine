#ifndef SE_BASE_H
#define SE_BASE_H

#define conv(x) ((x >= 'A' && x >= 'Z') ? (x - 65) : ((x >= 'a' && x <= 'z') ? (x - 97) : (x >= '0' && x <= '9' ? (x - 48) : -1)))

#include "Query.h"
#include <unordered_map>
#include <iostream>
#include <cstring>
#include <map>
#include <vector>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <time.h>
#include <iomanip>
#include <math.h>

const int QUERY_WORD_LIMIT = 10;
const int MAX_DIFF_CHARS = 36; // 'A' to 'Z' and '0' to '9'
const vector <char> SEPARATORS = {' ', ',', ';', '.', '"', '\'', '\n', '\t', '\r', '\0', '-'};
const char RETURN_ENTITY = '\0';

namespace fs = std::filesystem;
using namespace std;

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

struct baseNode
{
    int fileInd, pos, line, len;
    bool isTitle;
    baseNode(int _fileInd, int _pos, int _line, int _len = 0, bool _isTitle = 0): 
        fileInd(_fileInd), 
        pos(_pos),
        line(_line),
        len(_len),
        isTitle(_isTitle) 
        {};
    baseNode(): 
        fileInd(0), 
        pos(0),
        line(0),
        len(0),
        isTitle(0) 
        {};
        // Some other metadatas
};

struct TrieNode
{
    map <char, TrieNode*> child;
    vector <baseNode> data;

    void saveToFile (ofstream &out)
    {
        int size = TrieNode::data.size();
        out.write((char*)&size, 4);

        for (int i = 0; i < size; i++)
        {
            out.write((char*)&(TrieNode::data[i].fileInd), 4);
            out.write((char*)&(TrieNode::data[i].pos), 4);
            out.write((char*)&(TrieNode::data[i].line), 4);
            out.write((char*)&(TrieNode::data[i].len), 4);
            out.write((char*)&(TrieNode::data[i].isTitle), sizeof(TrieNode::data[i].isTitle));
        }
        
        char tmp;

        for (auto p = TrieNode::child.begin(); p != TrieNode::child.end(); p++)
        {
            tmp = p -> first;
            out.write(&tmp, 1);
            p -> second -> saveToFile(out);
        }

        tmp = RETURN_ENTITY;
        out.write(&tmp, 1);
        return;
    }

    void readFromFile (ifstream &inp)
    {
        int size;
        inp.read((char*)&size, 4);
        for (int i = 0; i < size; i++)
        {    
            int fileInd, pos, line, len;
            bool isTitle;
            inp.read((char*)&fileInd, 4);
            inp.read((char*)&pos, 4);
            inp.read((char*)&line, 4);
            inp.read((char*)&len, 4);
            inp.read((char*)&isTitle, sizeof(isTitle));
            TrieNode::data.push_back(baseNode(fileInd, pos, line, isTitle));
        }
        
        char tmp;
        while (1)
        {
            inp.read(&tmp, 1);
            // cout << "[DEBUG] Next edge " << tmp << " " << inp.tellg() << "\n";
            if (tmp == RETURN_ENTITY)
            break;
            
            TrieNode::child[tmp] = new TrieNode;
            TrieNode::child[tmp] -> readFromFile(inp);
        }
    }

    void clear()
    {
        for (auto p = TrieNode::child.begin(); p != TrieNode::child.end(); p++)
        {
            p -> second -> clear();
            delete p -> second;
        }
    }

    void getSize (vector <int> &size)
    {
        size.push_back((sizeof(TrieNode::child) + TrieNode::child.size() * 13) + (sizeof(TrieNode::data) + TrieNode::data.size() * 4));
        for (auto p = TrieNode::child.begin(); p != TrieNode::child.end(); p++)
        {
            p -> second -> getSize(size);
        }
        return;
    }
};

struct Trie
{
    TrieNode *root;
    Trie (): root(new TrieNode) {};
};

//The place where data stores
//Create new session with new baseData()
//Call loadFromFiles("path-to-files") to load raw files
//Call saveToFile() to save to .bin
//Call readFromFile() to read from .bin
//Call search() to look for specific words
//Call searchNumber(l, r) to look for specific number range

struct baseData
{
    std::vector<int> trie[QUERY_WORD_LIMIT][QUERY_WORD_LIMIT][MAX_DIFF_CHARS];
    dictionary words, files;
    TrieNode *root;
    vector <pair <int, baseNode> > numbers;
    vector <string> fileNames;

    baseData(): root(new TrieNode) {};

    // Raw data to main dataset
    void loadFromFiles (string path)
    {
        ifstream inp;
        string content;
        int fileInd = 0;
        double time, totalTime = 0;

        cout << "[INFO] Looking at path = " << path << "\n";
        for(auto p : fs::directory_iterator(path.c_str()))
        {
            if (!isTextFile(p.path().string()))
            continue;
            // cout << "[INFO] Reading file " << p.path() << "\n";

            time = clock();

            baseData::fileNames.push_back(p.path().string());

            inp.open(p.path());
            if (!inp.is_open())
            {
                cout << "[ERROR] Can't open file, aborting...\n";
                continue;
            }
            content.assign(istreambuf_iterator <char> (inp), istreambuf_iterator <char> ());
            stringstream lines(content);

            //do the saving here
            int ind = 0, pos, line = 0, len;
            string word, singleLine;

            while (!lines.eof())
            {
            getline(lines, singleLine);
            ind = 0;
            while (1)
            {
                word = nextWord(singleLine, ind, pos, len);
                if (word.size() == 0)
                break;
                // cout << "[INFO] word = " << word << " at line " << line + 1 << " " << " position " << pos + 1 << "\n";
                // cout << "[DEBUG] tmp = " << tmp << " ind = " << ind << "\n";
                if (isNumber(word))
                    baseData::insertNumber(word, fileInd, pos, line);
                else
                    baseData::insert(word, fileInd, pos, line, len);
            }
            line++;
            }

            inp.close();
            fileInd++;

            time = (clock() - time) / CLOCKS_PER_SEC;
            // cout << "[INFO] " << p.path() << " loaded in " << setprecision(3) << time << "s\n";
            totalTime += time;
        }
        sort(baseData::numbers.begin(), baseData::numbers.end());
        cout << "[INFO] Done in " << setprecision(3) << totalTime << "s\n";
    }

    // Insert numbers to vector
    void insertNumber (string number, int fileInd, int pos, int line)
    {
        int n;
        if (number[0] == '$')
            n = strToNum(number.substr(1));
        else
            n = strToNum(number);
        
        baseData::numbers.push_back(pair <int, baseNode> (n, baseNode(fileInd, pos, line)));
    }
    
    //Insert words to trie
    void insert (string word, int fileInd, int pos, int line, int len)
    {
        TrieNode *cur = baseData::root;
        for (int i = 0; i < word.size(); i++)
        {
            if (cur -> child.find(word[i]) == cur -> child.end())
            cur -> child[word[i]] = new TrieNode;

            cur = cur -> child[word[i]];
        }
        cur -> data.push_back(baseNode(fileInd, pos, line, len));
    }

    // Save datasets to files
    void saveToFile ()
    {
        //Read till eof
        //linkChar - "#" if it is leaf, go back
        //#m -size of data vector
        //m lines
        //#fileInd #pos #isTitle
        //#cont - can we continue go down?
        double time = clock();
        cout << "[INFO] Saving...\n";
        // Trie
        ofstream out;
        out.open("tree.bin", ios::binary);
        baseData::root -> saveToFile(out);
        out.close();

        // Numbers
        out.open("numbers.bin", ios::binary);
        int size = baseData::numbers.size();
        out.write((char*)&size, 4);
        for (int i = 0; i < size; i++)
        {
            out.write((char*)&(baseData::numbers[i].first), 4);
            out.write((char*)&(baseData::numbers[i].second.fileInd), 4);
            out.write((char*)&(baseData::numbers[i].second.pos), 4);
            out.write((char*)&(baseData::numbers[i].second.line), 4);
        }
        out.close();

        // FileNames
        out.open("filenames.txt");
        out << baseData::fileNames.size();
        for (auto name : baseData::fileNames)
            out << name << "\n";
        out.close();

        time = (clock() - time) / CLOCKS_PER_SEC;
        cout << "[INFO] Successfully saved in " << fixed << setprecision(4) << time << "s\n";
    }

    // Read datasets from files
    void readFromFile ()
    {
        double time = clock();
        // Trie
        cout << "[INFO] Reading...\n";
        ifstream inp;
        inp.open("tree.bin", ios::binary);
        if (!inp.is_open())
        {
            cout << "[ERROR] tree.bin not found\n";
            return;
        }

        baseData::root -> readFromFile(inp);
        inp.close();

        // Numbers
        inp.open("numbers.bin", ios::binary);
        if (!inp.is_open())
        {
            cout << "[ERROR] numbers.bin not found\n";
            return;
        }

        int size;
        inp.read((char*)&size, 4);
        for (int i = 0; i < size; i++)
        {
            int num, fileInd, pos, line;
            inp.read((char*)&(num), 4);
            inp.read((char*)&(fileInd), 4);
            inp.read((char*)&(pos), 4);
            inp.read((char*)&(line), 4);
            baseData::numbers.push_back(pair <int, baseNode> (num, baseNode(fileInd, pos, line)));
        }
        inp.close();

        // Filenames
        inp.open("filenames.txt");
        if (!inp.is_open())
        {
            cout << "[ERROR] filenames.txt not found\n";
            return;
        }

        string tmp;
        inp >> size;
        for (int i = 0; i < size; i++)
        {
            getline(cin, tmp);
            baseData::fileNames.push_back(tmp);
        }
        inp.close();

        time = (clock() - time) / CLOCKS_PER_SEC;
        cout << "[INFO] Successfully read in " << fixed << setprecision(4) << time << "s\n";
    }

    // Search for words
    vector <baseNode> search (string query)
    {
        TrieNode *cur = baseData::root;
        for (int i = 0; i < query.size(); i++)
        {
            if (cur -> child.find(query[i]) == cur -> child.end())
            return vector <baseNode>();
            else
            cur = cur -> child[query[i]];
        }
        return cur -> data;
    }

    // Search for numbers in the interval
    vector <baseNode> searchNumber (int l, int r)
    {
        vector <baseNode> res;
        auto it = lower_bound(baseData::numbers.begin(), baseData::numbers.end(), pair <int, baseNode> (l, baseNode()));
        while (it != baseData::numbers.end() && it -> first <= r)
        {
            res.push_back(it -> second);
            it++;
        }
        return res;
    }

    // Analytics about trie
    void analytics ()
    {
        vector <int> singleNodeSize;
        baseData::root -> getSize(singleNodeSize);
        long long total = 0;
        for (int v : singleNodeSize)
            total += v;
        double avg = total / singleNodeSize.size(), variant = 0;
        long long mxSize = 0, mnSize = total;
        for (long long v : singleNodeSize)
        {
            variant += pow(avg - v, 2);
            mxSize = max(mxSize, v);
            mnSize = min(mnSize, v);
        }
        cout << "[INFO] Analytics:\n";
        cout << "Total nodes: " << singleNodeSize.size() << "\n";
        cout << "Total size: " << total << "\n";
        cout << "Average size: " << fixed << setprecision(4) << avg << "\n";
        cout << "Standard deviation: " << fixed << setprecision(4) << pow(variant, 0.5) << "\n";
        cout << "Max size: " << mxSize << " , min size: " << mnSize << "\n";
    }

    // Clear datasets
    void clear ()
    {
        baseData::root -> clear();
        delete baseData::root;
        cout << "[INFO] Tree is successfully cleared\n";
    }

    void buildDataToBinary (const std::string& binaryFileName);
    void loadDataFromBinary (const std::string& binaryFileName);
    /* MUST ADD MORE FUNCTIONS TO THIS STRUCT!!! */
};

#endif // SE_BASE_H
