#include <iostream>
#include <cstring>
#include <map>
#include <vector>
#include <filesystem>
#include <fstream>
using namespace std;
namespace fs = std::filesystem;

const int MAX_QUERY_LENGTH = 50;
const int NUM_OF_ENTITIES = 63; // 2 * 26 (A-Za-z) + 10 (0-9) + 1 (' ')

struct Data
{
  int fileInd, pos;
  bool isTitle;
  Data(int _fileInd, int _pos, bool _isTitle = 0): 
    fileInd(_fileInd), 
    pos(_pos),
    isTitle(_isTitle) 
    {};
  Data(): 
    fileInd(0), 
    pos(0), 
    isTitle(0) 
    {};
    // Some other metadatas
};

struct TrieNode
{
  map <char, TrieNode*> child;
  vector <Data> data;

  void saveToFile (ofstream &out);
  void readFromFile (ifstream &inp);
  void clear();
};

struct Trie
{
  TrieNode *root;
  Trie (): root(new TrieNode) {};

  void loadFromFiles (string path);
  void saveToFile ();
  void readFromFile ();
  void insert (string word, int pos, int fileInd);
  vector <Data> search (string query);
  void clear ();
};

string nextWord (string content, int &pos);
void printResult (vector <Data> result);
bool isTextFile (string name);
bool isSeparator (char ch);