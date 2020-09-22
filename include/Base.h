#ifndef SE_BASE_H
#define SE_BASE_H

#define conv(x) ((x >= 'A' && x >= 'Z') ? (x - 65) : ((x >= 'a' && x <= 'z') ? (x - 97) : (x >= '0' && x <= '9' ? (x - 48) : -1)))
#define NOMINMAX

#include "Utility.h"
#include "Gui.h"
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <map>
#include <set>
#include <vector>
#include <sstream>
#include <time.h>
#include <iomanip>
#include <math.h>

// Source: ranks.nl
const std::set<std::string> stopwords = { "A", "ABLE", "ABOUT", "ABOVE", "ABST", "ACCORDANCE", "ACCORDING", "ACCORDINGLY", "ACROSS", "ACT", "ACTUALLY", "ADDED", "ADJ", "AFFECTED", "AFFECTING", "AFFECTS", "AFTER", "AFTERWARDS", "AGAIN", "AGAINST", "AH", "ALL", "ALMOST", "ALONE", "ALONG", "ALREADY", "ALSO", "ALTHOUGH", "ALWAYS", "AM", "AMONG", "AMONGST", "AN", "AND", "ANNOUNCE", "ANOTHER", "ANY", "ANYBODY", "ANYHOW", "ANYMORE", "ANYONE", "ANYTHING", "ANYWAY", "ANYWAYS", "ANYWHERE", "APPARENTLY", "APPROXIMATELY", "ARE", "AREN", "ARENT", "ARISE", "AROUND", "AS", "ASIDE", "ASK", "ASKING", "AT", "AUTH", "AVAILABLE", "AWAY", "AWFULLY", "B", "BACK", "BE", "BECAME", "BECAUSE", "BECOME", "BECOMES", "BECOMING", "BEEN", "BEFORE", "BEFOREHAND", "BEGIN", "BEGINNING", "BEGINNINGS", "BEGINS", "BEHIND", "BEING", "BELIEVE", "BELOW", "BESIDE", "BESIDES", "BETWEEN", "BEYOND", "BIOL", "BOTH", "BRIEF", "BRIEFLY", "BUT", "BY", "C", "CA", "CAME", "CAN", "CANNOT", "CANT", "CAUSE", "CAUSES", "CERTAIN", "CERTAINLY", "CO", "COM", "COME", "COMES", "CONTAIN", "CONTAINING", "CONTAINS", "COULD", "COULDNT", "D", "DATE", "DID", "DIDNT", "DIFFERENT", "DO", "DOES", "DOESNT", "DOING", "DONE", "DONT", "DOWN", "DOWNWARDS", "DUE", "DURING", "E", "EACH", "ED", "EDU", "EFFECT", "EG", "EIGHT", "EIGHTY", "EITHER", "ELSE", "ELSEWHERE", "END", "ENDING", "ENOUGH", "ESPECIALLY", "ET", "ETC", "EVEN", "EVER", "EVERY", "EVERYBODY", "EVERYONE", "EVERYTHING", "EVERYWHERE", "EX", "EXCEPT", "F", "FAR", "FEW", "FF", "FIFTH", "FIRST", "FIVE", "FIX", "FOLLOWED", "FOLLOWING", "FOLLOWS", "FOR", "FORMER", "FORMERLY", "FORTH", "FOUND", "FOUR", "FROM", "FURTHER", "FURTHERMORE", "G", "GAVE", "GET", "GETS", "GETTING", "GIVE", "GIVEN", "GIVES", "GIVING", "GO", "GOES", "GONE", "GOT", "GOTTEN", "H", "HAD", "HAPPENS", "HARDLY", "HAS", "HASNT", "HAVE", "HAVENT", "HAVING", "HE", "HED", "HENCE", "HER", "HERE", "HEREAFTER", "HEREBY", "HEREIN", "HERES", "HEREUPON", "HERS", "HERSELF", "HES", "HI", "HID", "HIM", "HIMSELF", "HIS", "HITHER", "HOME", "HOW", "HOWBEIT", "HOWEVER", "HUNDRED", "I", "ID", "IE", "IF", "ILL", "IM", "IMMEDIATE", "IMMEDIATELY", "IMPORTANCE", "IMPORTANT", "IN", "INC", "INDEED", "INDEX", "INFORMATION", "INSTEAD", "INTO", "INVENTION", "INWARD", "IS", "ISNT", "IT", "ITD", "ITLL", "ITS", "ITSELF", "IVE", "J", "JUST", "K", "KEEP", "KEEPS", "KEPT", "KG", "KM", "KNOW", "KNOWN", "KNOWS", "L", "LARGELY", "LAST", "LATELY", "LATER", "LATTER", "LATTERLY", "LEAST", "LESS", "LEST", "LET", "LETS", "LIKE", "LIKED", "LIKELY", "LINE", "LITTLE", "LL", "LOOK", "LOOKING", "LOOKS", "LTD", "M", "MADE", "MAINLY", "MAKE", "MAKES", "MANY", "MAY", "MAYBE", "ME", "MEAN", "MEANS", "MEANTIME", "MEANWHILE", "MERELY", "MG", "MIGHT", "MILLION", "MISS", "ML", "MORE", "MOREOVER", "MOST", "MOSTLY", "MR", "MRS", "MUCH", "MUG", "MUST", "MY", "MYSELF", "N", "NA", "NAME", "NAMELY", "NAY", "ND", "NEAR", "NEARLY", "NECESSARILY", "NECESSARY", "NEED", "NEEDS", "NEITHER", "NEVER", "NEVERTHELESS", "NEW", "NEXT", "NINE", "NINETY", "NO", "NOBODY", "NON", "NONE", "NONETHELESS", "NOONE", "NOR", "NORMALLY", "NOS", "NOT", "NOTED", "NOTHING", "NOW", "NOWHERE", "O", "OBTAIN", "OBTAINED", "OBVIOUSLY", "OF", "OFF", "OFTEN", "OH", "OK", "OKAY", "OLD", "OMITTED", "ON", "ONCE", "ONE", "ONES", "ONLY", "ONTO", "OR", "ORD", "OTHER", "OTHERS", "OTHERWISE", "OUGHT", "OUR", "OURS", "OURSELVES", "OUT", "OUTSIDE", "OVER", "OVERALL", "OWING", "OWN", "P", "PAGE", "PAGES", "PART", "PARTICULAR", "PARTICULARLY", "PAST", "PER", "PERHAPS", "PLACED", "PLEASE", "PLUS", "POORLY", "POSSIBLE", "POSSIBLY", "POTENTIALLY", "PP", "PREDOMINANTLY", "PRESENT", "PREVIOUSLY", "PRIMARILY", "PROBABLY", "PROMPTLY", "PROUD", "PROVIDES", "PUT", "Q", "QUE", "QUICKLY", "QUITE", "QV", "R", "RAN", "RATHER", "RD", "RE", "READILY", "REALLY", "RECENT", "RECENTLY", "REF", "REFS", "REGARDING", "REGARDLESS", "REGARDS", "RELATED", "RELATIVELY", "RESEARCH", "RESPECTIVELY", "RESULTED", "RESULTING", "RESULTS", "RIGHT", "RUN", "S", "SAID", "SAME", "SAW", "SAY", "SAYING", "SAYS", "SEC", "SECTION", "SEE", "SEEING", "SEEM", "SEEMED", "SEEMING", "SEEMS", "SEEN", "SELF", "SELVES", "SENT", "SEVEN", "SEVERAL", "SHALL", "SHE", "SHED", "SHELL", "SHES", "SHOULD", "SHOULDNT", "SHOW", "SHOWED", "SHOWN", "SHOWNS", "SHOWS", "SIGNIFICANT", "SIGNIFICANTLY", "SIMILAR", "SIMILARLY", "SINCE", "SIX", "SLIGHTLY", "SO", "SOME", "SOMEBODY", "SOMEHOW", "SOMEONE", "SOMETHAN", "SOMETHING", "SOMETIME", "SOMETIMES", "SOMEWHAT", "SOMEWHERE", "SOON", "SORRY", "SPECIFICALLY", "SPECIFIED", "SPECIFY", "SPECIFYING", "STILL", "STOP", "STRONGLY", "SUB", "SUBSTANTIALLY", "SUCCESSFULLY", "SUCH", "SUFFICIENTLY", "SUGGEST", "SUP", "SURE", "T", "TAKE", "TAKEN", "TAKING", "TELL", "TENDS", "TH", "THAN", "THANK", "THANKS", "THANX", "THAT", "THATLL", "THATS", "THATVE", "THE", "THEIR", "THEIRS", "THEM", "THEMSELVES", "THEN", "THENCE", "THERE", "THEREAFTER", "THEREBY", "THERED", "THEREFORE", "THEREIN", "THERELL", "THEREOF", "THERERE", "THERES", "THERETO", "THEREUPON", "THEREVE", "THESE", "THEY", "THEYD", "THEYLL", "THEYRE", "THEYVE", "THINK", "THIS", "THOSE", "THOU", "THOUGH", "THOUGHH", "THOUSAND", "THROUG", "THROUGH", "THROUGHOUT", "THRU", "THUS", "TIL", "TIP", "TO", "TOGETHER", "TOO", "TOOK", "TOWARD", "TOWARDS", "TRIED", "TRIES", "TRULY", "TRY", "TRYING", "TS", "TWICE", "TWO", "U", "UN", "UNDER", "UNFORTUNATELY", "UNLESS", "UNLIKE", "UNLIKELY", "UNTIL", "UNTO", "UP", "UPON", "UPS", "US", "USE", "USED", "USEFUL", "USEFULLY", "USEFULNESS", "USES", "USING", "USUALLY", "V", "VALUE", "VARIOUS", "VE", "VERY", "VIA", "VIZ", "VOL", "VOLS", "VS", "W", "WANT", "WANTS", "WAS", "WASNT", "WAY", "WE", "WED", "WELCOME", "WELL", "WENT", "WERE", "WERENT", "WEVE", "WHAT", "WHATEVER", "WHATLL", "WHATS", "WHEN", "WHENCE", "WHENEVER", "WHERE", "WHEREAFTER", "WHEREAS", "WHEREBY", "WHEREIN", "WHERES", "WHEREUPON", "WHEREVER", "WHETHER", "WHICH", "WHILE", "WHIM", "WHITHER", "WHO", "WHOD", "WHOEVER", "WHOLE", "WHOLL", "WHOM", "WHOMEVER", "WHOS", "WHOSE", "WHY", "WIDELY", "WILLING", "WISH", "WITH", "WITHIN", "WITHOUT", "WONT", "WORDS", "WORLD", "WOULD", "WOULDNT", "WWW", "X", "Y", "YES", "YET", "YOU", "YOUD", "YOULL", "YOUR", "YOURE", "YOURS", "YOURSELF", "YOURSELVES", "YOUVE", "Z", "ZERO" };

const char RETURN_ENTITY = '\0';

const std::string TrieBinaryDir = "trie.bin";
const std::string NumberBinaryDir = "numbers.bin";
const std::string FilenamesDir = "filenames.txt";
const std::string ThesaurusCSVDir = "thesaurus\\thesaurus.csv";
const std::string ThesaurusDir = "thesaurus";

struct baseNode
{
    int id, fileInd, pos, line, numWords;
    bool isTitle;
    baseNode(int _id, int _fileInd, int _pos, int _line, bool _isTitle = 0, int _numWords = 1) :
        id(_id),
        fileInd(_fileInd),
        pos(_pos),
        line(_line),
        isTitle(_isTitle),
        numWords(_numWords)
    {};
    baseNode() :
        id(-1),
        fileInd(0),
        pos(0),
        line(0),
        isTitle(0),
        numWords(0)
    {};
    // Some other metadatas
};

bool operator< (baseNode a, baseNode b)
{
    return a.fileInd != b.fileInd ? (a.fileInd < b.fileInd) : (a.id < b.id);
}

bool operator== (baseNode a, baseNode b)
{
    return a.fileInd == b.fileInd && a.id == b.id && a.isTitle == b.isTitle;
}

std::string cap(std::string x)
{
    std::string res = "";
    for (size_t i = 0; i < x.size(); i++)
        res += (char)('a' <= x[i] && x[i] <= 'z' ? x[i] - 32 : x[i]);
    return res;
}

void printResult(std::vector <baseNode> res)
{
    if (res.size() == 0)
    {
        std::cout << "[INFO] There is no result for the query\n";
        return;
    }

    std::cout << "[INFO] There are " << res.size() << " results of the query\n";
    for (size_t i = 0; i < res.size(); i++)
        std::cout << i + 1 << ". fileInd = " << res[i].fileInd << " at line " << res[i].line + 1 << " and position " << res[i].pos + 1 << "\n";
    std::cout << "[INFO] End of result\n";
}

bool isTextFile(std::string name)
{
    return name.substr(name.size() - 4) == ".txt";
}

bool isSeparator(char ch)
{
    return !(normal(ch) || punc(ch));
}

std::string filterIgnored(std::string s)
{
    std::string res = "";
    for (size_t i = 0; i < s.size(); i++)
        if (!punc(s[i]))
            res += s[i];
    return res;
}

int strToNum(std::string num)
{
    int res = 0;
    for (size_t i = 0; i < num.size(); i++)
        res = res * 10 + num[i] - '0';
    return res;
}

bool isNumber(std::string word)
{
    for (size_t i = (word[0] == '$'); i < word.size(); i++)
        if (word[i] < '0' || word[i] > '9')
            return 0;
    return 1;
}

std::string nextWord(std::string content, int& ind, int& pos)
{
    if (ind == (int)content.size())
        return "";

    std::string res;
    int cur;

    //Make sure that content[ind] is alphanum
    while (ind < (int)content.size() && isSeparator(content[ind])) ind++, pos++;
    pos = cur = ind;
    while (cur < (int)content.size() && !isSeparator(content[cur])) cur++;

    res = content.substr(ind, cur - ind);
    if (!isNumber(res))
        res = cap(res);

    ind = cur;

    return res;
}

void explode(std::vector<std::string>& res, const std::string& s, char delim)
{
    std::istringstream iss(s);
    std::string item;
    res.clear();

    while (std::getline(iss, item, delim))
        res.push_back(std::move(item));
}

template<typename T>
struct TrieNode
{
    std::unordered_map <char, TrieNode*> child;
    std::vector <T> data;

    void saveToFile(std::ofstream& out)
    {
        int size = TrieNode::data.size();
        out.write((char*)&size, 4);

        for (int i = 0; i < size; i++)
            out.write((char*)&TrieNode::data[i], sizeof(T));

        char tmp;

        for (auto p = TrieNode::child.begin(); p != TrieNode::child.end(); p++)
        {
            tmp = p->first;
            out.write(&tmp, 1);
            p->second->saveToFile(out);
        }

        tmp = RETURN_ENTITY;
        out.write(&tmp, 1);
        return;
    }

    void readFromFile(std::ifstream& inp)
    {
        int size;
        inp.read((char*)&size, 4);
        for (int i = 0; i < size; i++)
        {
            T tmp;
            inp.read((char*)&tmp, sizeof(T));
            TrieNode::data.push_back(tmp);
        }

        char tmp;
        while (1)
        {
            inp.read(&tmp, 1);
            if (tmp == RETURN_ENTITY)
                break;

            TrieNode::child[tmp] = new TrieNode<T>;
            TrieNode::child[tmp]->readFromFile(inp);
        }
    }

    void clear()
    {
        for (auto p = TrieNode::child.begin(); p != TrieNode::child.end(); p++)
        {
            p->second->clear();
            delete p->second;
        }
    }

    void getSize(std::vector <int>& size)
    {
        size.push_back((sizeof(TrieNode::child) + TrieNode::child.size() * 13) + (sizeof(TrieNode::data) + TrieNode::data.size() * 4));
        for (auto p = TrieNode::child.begin(); p != TrieNode::child.end(); p++)
        {
            p->second->getSize(size);
        }
        return;
    }
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
    bool isEmpty = true;
    TrieNode<baseNode>* root;
    std::vector <std::pair <int, baseNode> > numbers;
    std::vector <std::string> fileNames;

    // Raw data to main dataset
    void loadFromFiles(std::string path, int x, int& y)
    {
        root = new TrieNode<baseNode>;
        std::ifstream inp;
        std::string content;
        int fileInd = 0;
        double time, totalTime = 0;

        goToXY(x, y++); std::cout << "[INFO] Looking at path = " << path << "\n";
        dirHandler dir = dirHandler(path);
        std::vector <std::string> files = dir.files();
        for (std::string f : files)
        {
            if (!isTextFile(f))
                continue;

            time = clock();

            dir.next(f);

            baseData::fileNames.push_back(dir.dir());

            // Push file name to trie
            int id = 0, ind = 0, pos, line = 0;
            std::string word, singleLine;

            content = dir.fileName();
            std::string ext = dir.fileExt();
            while (!ext.empty())
                if (content.back() == ext.back())
                {
                    content.pop_back();
                    ext.pop_back();
                }
            if (!content.empty() && content.back() == '.')
                content.pop_back(); // for the dot (.)

            while (1)
            {
                word = filterIgnored(nextWord(content, ind, pos));
                if (word.size() == 0)
                    break;
                if (isNumber(word))
                    baseData::insertNumber(word, baseNode(id, fileInd, pos, line, 1));
                else
                    baseData::insert(word, baseNode(id, fileInd, pos, line, 1));
                ++id;
            }

            // Push content to trie
            content = dir.readAll();
            id = line = 0;
            std::stringstream lines(content);

            while (!lines.eof())
            {
                std::getline(lines, singleLine);
                ind = 0;
                while (1)
                {
                    word = filterIgnored(nextWord(singleLine, ind, pos));
                    if (word.size() == 0)
                        break;
                    /*
                    if (stopwords.find(word) != stopwords.end())
                        continue;
                    */
                    if (isNumber(word))
                        baseData::insertNumber(word, baseNode(id, fileInd, pos, line));
                    else
                        baseData::insert(word, baseNode(id, fileInd, pos, line));
                    ++id;
                }
                ++line;
            }

            dir.back();
            fileInd++;

            time = (clock() - time) / CLOCKS_PER_SEC;
            // cout << "[INFO] " << p.path() << " loaded in " << setprecision(3) << time << "s\n";
            totalTime += time;
        }
        sort(baseData::numbers.begin(), baseData::numbers.end());
        goToXY(x, y++); std::cout << "[INFO] Importing done in " << std::fixed << std::setprecision(3) << totalTime << "s\n";
        isEmpty = false;
    }

    // Insert numbers to vector
    void insertNumber(std::string number, baseNode data)
    {
        int n;
        if (number[0] == '$')
            n = strToNum(number.substr(1));
        else
            n = strToNum(number);

        baseData::numbers.push_back(std::pair <int, baseNode>(n, data));
    }

    //Insert words to trie
    void insert(std::string word, baseNode data)
    {
        TrieNode<baseNode>* cur = baseData::root;
        for (size_t i = 0; i < word.size(); i++)
        {
            if (cur->child.find(word[i]) == cur->child.end())
                cur->child[word[i]] = new TrieNode<baseNode>;
            cur = cur->child[word[i]];
        }
        cur->data.push_back(data);
    }

    // Save datasets to files
    void saveToFile(int x, int& y)
    {
        //Read till eof
        //linkChar - "#" if it is leaf, go back
        //#m -size of data vector
        //m lines
        //#fileInd #pos #isTitle
        //#cont - can we continue go down?
        double time = clock();
        goToXY(x, y++); std::cout << "[INFO] Saving...\n";
        // Trie
        std::ofstream out;
        out.open(TrieBinaryDir, std::ios::binary);
        baseData::root->saveToFile(out);
        out.close();

        // Numbers
        out.open(NumberBinaryDir, std::ios::binary);
        int size = baseData::numbers.size();
        out.write((char*)&size, 4);
        for (int i = 0; i < size; i++)
        {
            out.write((char*)&(baseData::numbers[i].first), 4);
            out.write((char*)&(baseData::numbers[i].second), sizeof(baseNode));
        }
        out.close();

        // FileNames
        out.open(FilenamesDir);
        out << baseData::fileNames.size() << "\n";
        for (auto name : baseData::fileNames)
            out << name << "\n";
        out.close();

        time = (clock() - time) / CLOCKS_PER_SEC;
        goToXY(x, y++); std::cout << "[INFO] Successfully saved in " << std::fixed << std::setprecision(4) << time << "s\n";
    }

    // Read datasets from files
    void readFromFile(int x, int& y)
    {
        root = new TrieNode<baseNode>;
        double time = clock();
        // Trie
        goToXY(x, y++); std::cout << "[INFO] Loading...\n";
        std::ifstream inp;
        inp.open(TrieBinaryDir, std::ios::binary);
        if (!inp.is_open())
        {
            goToXY(x, y++); std::cout << "[ERROR] " << TrieBinaryDir << " not found\n";
            goToXY(x, y++); std::cout << "[INFO] Try load new instead!";
            delete root; root = nullptr;
            return;
        }

        baseData::root->readFromFile(inp);
        inp.close();

        // Numbers
        inp.open(NumberBinaryDir, std::ios::binary);
        if (!inp.is_open())
        {
            goToXY(x, y++); std::cout << "[ERROR] " << NumberBinaryDir << " not found\n";
            goToXY(x, y++); std::cout << "[INFO] Try load new instead!";
            delete root; root = nullptr;
            return;
        }

        int size;
        inp.read((char*)&size, 4);
        for (int i = 0; i < size; i++)
        {
            int num;
            baseNode data;
            inp.read((char*)&(num), 4);
            inp.read((char*)&(data), sizeof(baseNode));
            baseData::numbers.push_back(std::pair <int, baseNode>(num, data));
        }
        inp.close();

        // Filenames
        inp.open(FilenamesDir);
        if (!inp.is_open())
        {
            goToXY(x, y++); std::cout << "[ERROR] " << FilenamesDir << " not found\n";
            goToXY(x, y++); std::cout << "[INFO] Try load new instead!";
            delete root; root = nullptr;
            return;
        }

        std::string tmp;
        inp >> size; inp.ignore();
        for (int i = 0; i < size; i++)
        {
            std::getline(inp, tmp);
            baseData::fileNames.push_back(tmp);
        }
        inp.close();

        time = (clock() - time) / CLOCKS_PER_SEC;
        goToXY(x, y++); std::cout << "[INFO] Successfully loaded in " << std::fixed << std::setprecision(4) << time << "s\n";
        isEmpty = false;
    }

    // Search for words
    std::vector <baseNode> search(std::string query, bool isTitle = false)
    {
        TrieNode<baseNode>* cur = baseData::root;

        // Get data from trie
        for (size_t i = 0; i < query.size(); i++)
        {
            if (cur->child.find(query[i]) == cur->child.end())
                return std::vector <baseNode>();
            else
                cur = cur->child[query[i]];
        }

        // Filter by isTitle
        std::vector<baseNode> res;
        for (size_t i = 0; i < cur->data.size(); i++)
        {
            if (cur->data[i].isTitle == isTitle)
                res.push_back(cur->data[i]);
        }
        return res;
    }

    // Search for numbers in the interval
    std::vector <baseNode> searchNumber(int l, int r, bool isTitle = false)
    {
        std::vector <baseNode> res;
        auto it = lower_bound(baseData::numbers.begin(), baseData::numbers.end(), std::pair <int, baseNode>(l, baseNode()));
        while (it != baseData::numbers.end() && it->first <= r)
        {
            if (it->second.isTitle == isTitle)
                res.push_back(it->second);
            it++;
        }
        return res;
    }

    // Analytics about trie
    void analytics()
    {
        std::vector <int> singleNodeSize;
        baseData::root->getSize(singleNodeSize);
        long long total = 0;
        for (int v : singleNodeSize)
            total += v;
        double avg = total / singleNodeSize.size(), variant = 0;
        long long mxSize = 0, mnSize = total;
        for (long long v : singleNodeSize)
        {
            variant += pow(avg - v, 2);
            mxSize = std::max(mxSize, v);
            mnSize = std::min(mnSize, v);
        }
        std::cout << "[INFO] Analytics:\n";
        std::cout << "Total nodes: " << singleNodeSize.size() << "\n";
        std::cout << "Total size: " << total << "\n";
        std::cout << "Average size: " << std::fixed << std::setprecision(4) << avg << "\n";
        std::cout << "Standard deviation: " << std::fixed << std::setprecision(4) << pow(variant, 0.5) << "\n";
        std::cout << "Max size: " << mxSize << " , min size: " << mnSize << "\n";
    }

    // Clear datasets
    void clear()
    {
        if (isEmpty) return;
        baseData::root->clear();
        delete baseData::root;
        //std::cout << "[INFO] Tree is successfully cleared\n";
        isEmpty = true;
    }


    /* --- Thesaurus --- */
    std::vector<std::string> theWords;
    std::map<int, std::vector<int> > theUnion;

    void theLoadFromCSV()
    {
        theWords.clear();
        theUnion.clear();

        double time = clock();
        std::istringstream iss(dirHandler(ThesaurusCSVDir).readAll());

        std::vector<std::pair<std::string, std::string> > edges;

        // Loop through lines (words) one by one
        while (!iss.eof())
        {
            // Get the current line
            std::string line;
            std::getline(iss, line);

            if (line.empty())
                break;

            // Put in a stringstream and get the word + its synonym list
            std::istringstream issl(line);
            std::string word, synonyms;
            std::getline(issl, word, '\t');
            std::getline(issl, synonyms);

            // Normalization
            for (size_t i = 0; i < word.length(); word[i] = upper(word[i]), ++i);
            for (size_t i = 0; i < synonyms.length(); synonyms[i] = upper(synonyms[i]), ++i);

            // Insert the synonyms
            std::vector<std::string> synonymList;
            explode(synonymList, synonyms, ',');

            theWords.push_back(word);
            for (std::string synonym : synonymList)
            {
                theWords.push_back(synonym);
                edges.push_back(std::make_pair(word, synonym));
            }
        }

        // Normalize the theWords vector
        std::sort(theWords.begin(), theWords.end());
        theWords.resize(distance(theWords.begin(), unique(theWords.begin(), theWords.end())));

        // Construct the map
        for (std::pair<std::string, std::string> t : edges)
        {
            int pos_f = distance(theWords.begin(), lower_bound(theWords.begin(), theWords.end(), t.first)),
                pos_s = distance(theWords.begin(), lower_bound(theWords.begin(), theWords.end(), t.second));
            theUnion[pos_f].push_back(pos_s);
        }

        time = (clock() - time) / CLOCKS_PER_SEC;
        //std::cout << "[INFO] Thesaurus importing done in " << std::fixed << std::setprecision(3) << time << "s\n";
    }

    void theReadText()
    {
        theWords.clear();
        theUnion.clear();

        double time = clock();
        std::cout << "[INFO] Loading thesaurus...\n";

        std::ifstream ifs;
        ifs.open(ThesaurusDir);
        if (!ifs.is_open())
        {
            std::cout << "[ERROR] " << ThesaurusDir << " not found\n";
            return;
        }

        // First N1 lines are for words
        int N; ifs >> N >> std::ws;
        while (N--)
        {
            std::string line;
            std::getline(ifs, line);
            theWords.push_back(line);
        }

        // Next N2 lines are for synonym mapping
        ifs >> N;
        while (N--)
        {
            int x, y; ifs >> x >> y;
            while (y--)
            {
                int z; ifs >> z;
                theUnion[x].push_back(z);
            }
        }

        ifs.close();
        time = (clock() - time) / CLOCKS_PER_SEC;
        std::cout << "[INFO] Successfully loaded thesaurus in " << std::fixed << std::setprecision(4) << time << "s\n";
    }

    void theWriteText()
    {
        double time = clock();
        std::cout << "[INFO] Saving thesaurus...\n";

        std::ofstream ofs;
        ofs.open(ThesaurusDir);

        // First N1 lines are for words
        ofs << theWords.size() << " ";
        for (std::string word : theWords)
            ofs << word << std::endl;

        // Next N2 lines are for synonym mapping
        ofs << theUnion.size() << " ";
        for (auto it = theUnion.begin(); it != theUnion.end(); ++it)
            if (!it->second.empty())
            {
                ofs << it->first << " " << it->second.size() << " ";
                for (int x : it->second)
                    ofs << x << " ";
            }
        ofs << std::endl;

        ofs.close();
        time = (clock() - time) / CLOCKS_PER_SEC;
        std::cout << "[INFO] Successfully saved thesaurus in " << std::fixed << std::setprecision(4) << time << "s\n";
    }

    std::vector<int> theSearch(const std::string& query)
    {
        auto it = std::lower_bound(theWords.begin(), theWords.end(), query);
        if (it == theWords.end() || !(query == *it))
            return std::vector<int>();
        return theUnion[distance(theWords.begin(), it)];
    }
};

#endif // SE_BASE_H
