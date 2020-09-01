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
using namespace std;
namespace fs = std::filesystem;

const int MAX_QUERY_LENGTH = 50;
const int NUM_OF_ENTITIES = 63; // 2 * 26 (A-Za-z) + 10 (0-9) + 1 (' ')
const char RETURN_ENTITY = '\0';

struct Data
{
  int fileInd, pos, line;
  bool isTitle;
  Data(int _fileInd, int _pos, int _line, bool _isTitle = 0): 
    fileInd(_fileInd), 
    pos(_pos),
    line(_line),
    isTitle(_isTitle) 
    {};
  Data(): 
    fileInd(0), 
    pos(0),
    line(0),
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
  void getSize (vector <int> &size);
};

struct Trie
{
  TrieNode *root;
  Trie (): root(new TrieNode) {};

  void loadFromFiles (string path);
  void saveToFile ();
  void readFromFile ();
  void insert (string word, int fileInd, int pos, int line);
  vector <Data> search (string query);
  void analytics ();
  void clear ();
};

vector <vector <string> > contentToWords (string content);
string nextWord (string content, int &ind, int &pos);
void printResult (vector <Data> result);
bool isTextFile (string name);
bool isSeparator (char ch);