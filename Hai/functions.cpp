#include "header.h"

void Trie::loadFromFiles (string path)
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

    inp.open(p.path());
    if (!inp.is_open())
    {
      cout << "[ERROR] Can't open file, aborting...\n";
      continue;
    }
    content.assign(istreambuf_iterator <char> (inp), istreambuf_iterator <char> ());
    stringstream lines(content);

    //do the saving here
    int ind = 0, tmp, pos, line = 0;
    string word, singleLine;

    while (!lines.eof())
    {
      getline(lines, singleLine);
      pos = 0;
      ind = 0;
      tmp = 0;
      while (1)
      {
        tmp = ind;
        word = nextWord(singleLine, ind, pos);
        if (word.size() == 0)
          break;
        // cout << "[INFO] word = " << word << " at line " << line + 1 << " " << " position " << pos + 1 << "\n";
        // cout << "[DEBUG] tmp = " << tmp << " ind = " << ind << "\n";
        Trie::insert(word, fileInd, pos, line);
        pos += ind - tmp - (tmp == 0 ? 0 : 1);
      }
      line++;
    }

    inp.close();
    fileInd++;

    time = (clock() - time) / CLOCKS_PER_SEC;
    // cout << "[INFO] " << p.path() << " loaded in " << setprecision(3) << time << "s\n";
    totalTime += time;
  }

  cout << "[INFO] Done in " << setprecision(3) << totalTime << "s\n";
}

string nextWord (string content, int &ind, int &pos)
{
  if (ind == content.size())
    return "";
  
  string res;
  int cur;

  //Make sure that content[ind] is alphanum
  while (ind < content.size() && isSeparator(content[ind])) ind++, pos++;
  cur = ind;
  while (cur < content.size() && !isSeparator(content[cur])) cur++;

  res = content.substr(ind, cur - ind);
  ind = cur;
  
  return res;
}

void Trie::insert (string word, int fileInd, int pos, int line)
{
  TrieNode *cur = Trie::root;
  for (int i = 0; i < word.size(); i++)
  {
    if (cur -> child.find(word[i]) == cur -> child.end())
      cur -> child[word[i]] = new TrieNode;

    cur = cur -> child[word[i]];
  }
  cur -> data.push_back(Data(fileInd, pos, line));
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

void Trie::analytics()
{
  vector <int> singleNodeSize;
  Trie::root -> getSize(singleNodeSize);
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

void TrieNode::getSize(vector <int> &size)
{
  size.push_back((sizeof(TrieNode::child) + TrieNode::child.size() * 13) + (sizeof(TrieNode::data) + TrieNode::data.size() * 4));
  for (auto p = TrieNode::child.begin(); p != TrieNode::child.end(); p++)
  {
    p -> second -> getSize(size);
  }
  return;
}

void Trie::saveToFile()
{
  //Read till eof
  //linkChar - "#" if it is leaf, go back
  //#m -size of data vector
  //m lines
  //#fileInd #pos #isTitle
  //#cont - can we continue go down?
  double time = clock();
  cout << "[INFO] Saving tree...\n";
  ofstream out;
  out.open("tree.bin", ios::binary);
  Trie::root -> saveToFile(out);
  out.close();
  time = (clock() - time) / CLOCKS_PER_SEC;
  cout << "[INFO] Tree successfully saved in " << fixed << setprecision(4) << time << "s\n";
}

void Trie::readFromFile()
{
  double time = clock();
  cout << "[INFO] Reading tree...\n";
  ifstream inp;
  inp.open("tree.bin", ios::binary);
  Trie::root -> readFromFile(inp);
  inp.close();
  time = (clock() - time) / CLOCKS_PER_SEC;
  cout << "[INFO] Tree successfully read in " << fixed << setprecision(4) << time << "s\n";
}

void TrieNode::saveToFile(ofstream &out)
{
  int size = TrieNode::data.size();
  out.write((char*)&size, 4);

  for (int i = 0; i < size; i++)
  {
    out.write((char*)&(TrieNode::data[i].fileInd), 4);
    out.write((char*)&(TrieNode::data[i].pos), 4);
    out.write((char*)&(TrieNode::data[i].line), 4);
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

void TrieNode::readFromFile(ifstream &inp)
{
  int size;
  inp.read((char*)&size, 4);
  for (int i = 0; i < size; i++)
  {    
    int fileInd, pos, line;
    bool isTitle;
    inp.read((char*)&fileInd, 4);
    inp.read((char*)&pos, 4);
    inp.read((char*)&line, 4);
    inp.read((char*)&isTitle, sizeof(isTitle));
    TrieNode::data.push_back(Data(fileInd, pos, line, isTitle));
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
    cout << i + 1 << ". fileInd = " << res[i].fileInd << " at line " << res[i].line + 1 << " and position " << res[i].pos + 1 << "\n";
  cout << "[INFO] End of result\n";
}

bool isSeparator (char ch)
{
  return ch == ' ' || ch == '.' || ch == ',' || ch == '\n' || ch == '\r' || ch == '\t' || ch == '\0';
}

bool isTextFile (string name)
{
  return name.substr(name.size() - 4) == ".txt";
}