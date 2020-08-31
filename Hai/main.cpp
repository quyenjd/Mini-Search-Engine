#include "header.h"

int main()
{
  Trie *trie = new Trie;
  trie -> saveFiles("../data");
  delete trie;
}