#include "header.h"

int main()
{
  Trie *trie = new Trie;
  // trie -> loadFromFiles("./data");
  trie -> readFromFile();
  trie -> analytics();
  // printResult(trie -> search("southern"));
  // trie -> saveToFile();
  trie -> clear();
  delete trie;
}