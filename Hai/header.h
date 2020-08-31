#include <iostream>
#include <cstring>
#include <unordered_map>
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
  void Data(int _fileInd, int _pos): fileInd(_fileInd), pos(_pos) {};
  void Data(): fileInd(0), pos(0) {};
    // Some other metadatas
};

struct TrieNode
{
  vector <Data> data;
};

struct Trie
{
  TrieNode table[MAX_QUERY_LENGTH][NUM_OF_ENTITIES];

  void saveFiles (string path);
  void insert (string chunk);
};

bool isTextFile (string name);