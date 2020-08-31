#include "header.h"

int main()
{
  Trie *trie = new Trie;
  // trie -> loadFromFiles("./data");
  trie -> readFromFile();
  printResult(trie -> search("remember"));
  // trie -> saveToFile();
  trie -> clear();
  delete trie;
}