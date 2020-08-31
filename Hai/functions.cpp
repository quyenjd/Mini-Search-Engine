#include "header.h"

void Trie::saveFiles (string path)
{

  ifstream inp;
  string content;
  int fileInd = 0;

  cout << "[INFO] Looking at path = " << path << "\n";
  for(auto p: fs::directory_iterator(path.c_str()))
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
    string buffer;

    while (1)
    {
      tmp = pos;
      chunk = nextChunk(content, pos);
      if (chunk.size() == 0)
        break;
      Trie::insert(chunk, tmp, fileInd);
    }

    inp.close();
    fileInd++;
  }
}

void Trie::insert(string chunk, int pos, int fileInd)
{
  for (int i = 0; i < min(chunk.size(), MAX_QUERY_LENGTH); i++)
  {
    Trie::table[i][charInd(chunk[i])].data.push_back(Data(fileInd, pos + i));
  }
}

int charInd(char ch)
{
  //a-zA-Z0-9' '
  if (ch >= 'a')
    return ch - 'a';
  else if (ch >= 'A')
    return ch - 'A' + 26;
  else if (ch >= '0')
    return ch - '0' + 52;
  else
    return 62;
}

string nextChunk (string content, int &pos)
{
  if (pos == content.length)
    return "";
  
  string res;
  int cur;

  while (1)
  {
    //Already sure that content[pos] is alphanum
    cur = pos;
    while (cur < content.size() && !isSeparator(content[cur])) cur++;
    if (res.size() + cur - pos > MAX_QUERY_LENGTH)
      break;
    res += (res.size() ? ' ' : '') content.substr(pos, cur - pos);
    while (cur < content.size() && isSeparator(content[cur])) cur++;
    pos = cur;
  }
  
  while (pos < content.size() && isSeparator(content[pos]))
    pos++;
  
  return res;
}

bool isSeparator (char ch)
{
  return ch == ' ' || ch == '.' || ch == ',';
}

bool isTextFile (string name)
{
  return name.substr(name.size() - 4) == ".txt";
}