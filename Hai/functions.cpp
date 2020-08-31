#include "header.h"

void Trie::loadFromFiles (string path)
{
  ifstream inp;
  string content;
  int fileInd = 0;

  cout << "[INFO] Looking at path = " << path << "\n";
  for(auto p : fs::directory_iterator(path.c_str()))
  {
    if (!isTextFile(p.path()))
      continue;
    cout << "[INFO] Reading file " << p.path() << "\n";

    inp.open(p.path());
    if (!inp.is_open())
    {
      cout << "[ERROR] Can't open file, aborting...\n";
      continue;
    }
    content.assign(istreambuf_iterator <char> (inp), istreambuf_iterator <char> ());

    //do the saving here
    int pos = 0, tmp;
    string word;

    while (1)
    {
      tmp = pos;
      word = nextWord(content, pos);
      if (word.size() == 0)
        break;
      cout << "[INFO] word = " << word << " | with length = " << word.size() << "\n";
      Trie::insert(word, tmp, fileInd);
    }

    inp.close();
    fileInd++;
  }
}

void Trie::insert (string word, int pos, int fileInd)
{
  TrieNode *cur = Trie::root;
  for (int i = 0; i < word.size(); i++)
  {

    if (cur -> child.find(word[i]) == cur -> child.end())
      cur -> child[word[i]] = new TrieNode;

    cur = cur -> child[word[i]];
    cur -> data.push_back(Data(fileInd, pos + i));
  }
}

vector <Data> Trie::search (string query)
{
  TrieNode *cur = Trie::root;
  for (int i = 0; i < query.size(); i++)
  {
    if (cur -> child.find(query[i]) == cur -> child.end())
      return vector <Data>();
    else
      cur = cur -> child[query[i]];
  }
  return cur -> data;
}

void Trie::saveToFile()
{
  //Read till eof
  //linkChar - "#" if it is leaf, go back
  //#m -size of data vector
  //m lines
  //#fileInd #pos #isTitle
  //#cont - can we continue go down?

  ofstream out;
  out.open("tree.bin", ios::binary);
  Trie::root -> saveToFile(out);
  out.close();
  cout << "[INFO] Tree successfully saved\n";
}

void Trie::readFromFile()
{
  ifstream inp;
  inp.open("tree.bin", ios::binary);
  Trie::root -> readFromFile(inp);
  inp.close();
}

void TrieNode::saveToFile(ofstream &out)
{
  int size = TrieNode::data.size();
  out.write((char*)&size, 4);

  for (int i = 0; i < size; i++)
  {
    out.write((char*)&(TrieNode::data[i].fileInd), 4);
    out.write((char*)&(TrieNode::data[i].pos), 4);
    out.write((char*)&(TrieNode::data[i].isTitle), 1);
  }
  
  char tmp;

  for (auto p = TrieNode::child.begin(); p != TrieNode::child.end(); p++)
  {
    tmp = p -> first;
    out.write(&tmp, 1);
    p -> second -> saveToFile(out);
  }

  tmp = '#';
  out.write(&tmp, 1);
  return;
}

void TrieNode::readFromFile(ifstream &inp)
{
  int size;
  inp.read((char*)&size, 4);
  for (int i = 0; i < size; i++)
  {    
    int fileInd, pos;
    bool isTitle;
    inp.read((char*)&fileInd, 4);
    inp.read((char*)&pos, 4);
    inp.read((char*)&isTitle, sizeof(isTitle));
    TrieNode::data.push_back(Data(fileInd, pos, isTitle));
  }
  
  char tmp;
  while (1)
  {
    inp.read(&tmp, 1);
    // cout << "[DEBUG] Next edge " << tmp << " " << inp.tellg() << "\n";
    if (tmp == '#')
      break;
    
    TrieNode::child[tmp] = new TrieNode;
    TrieNode::child[tmp] -> readFromFile(inp);
  }

  return;
}

void Trie::clear()
{
  Trie::root -> clear();
  delete Trie::root;
  cout << "[INFO] Tree is successfully cleared\n";
}

void TrieNode::clear()
{
  for (auto p = TrieNode::child.begin(); p != TrieNode::child.end(); p++)
  {
    p -> second -> clear();
    delete p -> second;
  }
}

void printResult (vector <Data> res)
{
  if (res.size() == 0)
  {
    cout << "[INFO] There is no result for the query\n";
    return;
  }

  cout << "[INFO] There are " << res.size() << " results of the query\n";
  for (int i = 0; i < res.size(); i++)
    cout << i + 1 << ". fileInd = " << res[i].fileInd << " pos = " << res[i].pos << " isTitle = " << res[i].isTitle << "\n";
  cout << "[INFO] End of result\n";
}

string nextWord (string content, int &pos)
{
  if (pos == content.size())
    return "";
  
  string res;
  int cur;

  //Already sure that content[pos] is alphanum
  cur = pos;
  while (cur < content.size() && !isSeparator(content[cur])) cur++;

  res += (res.size() ? " " : "") + content.substr(pos, cur - pos);
  while (cur < content.size() && isSeparator(content[cur])) cur++;
  pos = cur;
  
  return res;
}

bool isSeparator (char ch)
{
  return ch == ' ' || ch == '.' || ch == ',' || ch == '\n' || ch == '\r' || ch == '\t';
}

bool isTextFile (string name)
{
  return name.substr(name.size() - 4) == ".txt";
}