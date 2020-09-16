#ifndef SE_BASE_H
#define SE_BASE_H

#define conv(x) ((x >= 'A' && x >= 'Z') ? (x - 65) : ((x >= 'a' && x <= 'z') ? (x - 97) : (x >= '0' && x <= '9' ? (x - 48) : -1)))
#define NOMINMAX

#include "Query.h"
#include "Utility.h"
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

const std::set<std::string> stopwords = {"A", "ABLE", "ABOUT", "ABOVE", "ABST", "ACCORDANCE", "ACCORDING", "ACCORDINGLY", "ACROSS", "ACT", "ACTUALLY", "ADDED", "ADJ", "AFFECTED", "AFFECTING", "AFFECTS", "AFTER", "AFTERWARDS", "AGAIN", "AGAINST", "AH", "ALL", "ALMOST", "ALONE", "ALONG", "ALREADY", "ALSO", "ALTHOUGH", "ALWAYS", "AM", "AMONG", "AMONGST", "AN", "AND", "ANNOUNCE", "ANOTHER", "ANY", "ANYBODY", "ANYHOW", "ANYMORE", "ANYONE", "ANYTHING", "ANYWAY", "ANYWAYS", "ANYWHERE", "APPARENTLY", "APPROXIMATELY", "ARE", "AREN", "ARENT", "ARISE", "AROUND", "AS", "ASIDE", "ASK", "ASKING", "AT", "AUTH", "AVAILABLE", "AWAY", "AWFULLY", "B", "BACK", "BE", "BECAME", "BECAUSE", "BECOME", "BECOMES", "BECOMING", "BEEN", "BEFORE", "BEFOREHAND", "BEGIN", "BEGINNING", "BEGINNINGS", "BEGINS", "BEHIND", "BEING", "BELIEVE", "BELOW", "BESIDE", "BESIDES", "BETWEEN", "BEYOND", "BIOL", "BOTH", "BRIEF", "BRIEFLY", "BUT", "BY", "C", "CA", "CAME", "CAN", "CANNOT", "CANT", "CAUSE", "CAUSES", "CERTAIN", "CERTAINLY", "CO", "COM", "COME", "COMES", "CONTAIN", "CONTAINING", "CONTAINS", "COULD", "COULDNT", "D", "DATE", "DID", "DIDNT", "DIFFERENT", "DO", "DOES", "DOESNT", "DOING", "DONE", "DONT", "DOWN", "DOWNWARDS", "DUE", "DURING", "E", "EACH", "ED", "EDU", "EFFECT", "EG", "EIGHT", "EIGHTY", "EITHER", "ELSE", "ELSEWHERE", "END", "ENDING", "ENOUGH", "ESPECIALLY", "ET", "ETC", "EVEN", "EVER", "EVERY", "EVERYBODY", "EVERYONE", "EVERYTHING", "EVERYWHERE", "EX", "EXCEPT", "F", "FAR", "FEW", "FF", "FIFTH", "FIRST", "FIVE", "FIX", "FOLLOWED", "FOLLOWING", "FOLLOWS", "FOR", "FORMER", "FORMERLY", "FORTH", "FOUND", "FOUR", "FROM", "FURTHER", "FURTHERMORE", "G", "GAVE", "GET", "GETS", "GETTING", "GIVE", "GIVEN", "GIVES", "GIVING", "GO", "GOES", "GONE", "GOT", "GOTTEN", "H", "HAD", "HAPPENS", "HARDLY", "HAS", "HASNT", "HAVE", "HAVENT", "HAVING", "HE", "HED", "HENCE", "HER", "HERE", "HEREAFTER", "HEREBY", "HEREIN", "HERES", "HEREUPON", "HERS", "HERSELF", "HES", "HI", "HID", "HIM", "HIMSELF", "HIS", "HITHER", "HOME", "HOW", "HOWBEIT", "HOWEVER", "HUNDRED", "I", "ID", "IE", "IF", "ILL", "IM", "IMMEDIATE", "IMMEDIATELY", "IMPORTANCE", "IMPORTANT", "IN", "INC", "INDEED", "INDEX", "INFORMATION", "INSTEAD", "INTO", "INVENTION", "INWARD", "IS", "ISNT", "IT", "ITD", "ITLL", "ITS", "ITSELF", "IVE", "J", "JUST", "K", "KEEP", "KEEPS", "KEPT", "KG", "KM", "KNOW", "KNOWN", "KNOWS", "L", "LARGELY", "LAST", "LATELY", "LATER", "LATTER", "LATTERLY", "LEAST", "LESS", "LEST", "LET", "LETS", "LIKE", "LIKED", "LIKELY", "LINE", "LITTLE", "LL", "LOOK", "LOOKING", "LOOKS", "LTD", "M", "MADE", "MAINLY", "MAKE", "MAKES", "MANY", "MAY", "MAYBE", "ME", "MEAN", "MEANS", "MEANTIME", "MEANWHILE", "MERELY", "MG", "MIGHT", "MILLION", "MISS", "ML", "MORE", "MOREOVER", "MOST", "MOSTLY", "MR", "MRS", "MUCH", "MUG", "MUST", "MY", "MYSELF", "N", "NA", "NAME", "NAMELY", "NAY", "ND", "NEAR", "NEARLY", "NECESSARILY", "NECESSARY", "NEED", "NEEDS", "NEITHER", "NEVER", "NEVERTHELESS", "NEW", "NEXT", "NINE", "NINETY", "NO", "NOBODY", "NON", "NONE", "NONETHELESS", "NOONE", "NOR", "NORMALLY", "NOS", "NOT", "NOTED", "NOTHING", "NOW", "NOWHERE", "O", "OBTAIN", "OBTAINED", "OBVIOUSLY", "OF", "OFF", "OFTEN", "OH", "OK", "OKAY", "OLD", "OMITTED", "ON", "ONCE", "ONE", "ONES", "ONLY", "ONTO", "OR", "ORD", "OTHER", "OTHERS", "OTHERWISE", "OUGHT", "OUR", "OURS", "OURSELVES", "OUT", "OUTSIDE", "OVER", "OVERALL", "OWING", "OWN", "P", "PAGE", "PAGES", "PART", "PARTICULAR", "PARTICULARLY", "PAST", "PER", "PERHAPS", "PLACED", "PLEASE", "PLUS", "POORLY", "POSSIBLE", "POSSIBLY", "POTENTIALLY", "PP", "PREDOMINANTLY", "PRESENT", "PREVIOUSLY", "PRIMARILY", "PROBABLY", "PROMPTLY", "PROUD", "PROVIDES", "PUT", "Q", "QUE", "QUICKLY", "QUITE", "QV", "R", "RAN", "RATHER", "RD", "RE", "READILY", "REALLY", "RECENT", "RECENTLY", "REF", "REFS", "REGARDING", "REGARDLESS", "REGARDS", "RELATED", "RELATIVELY", "RESEARCH", "RESPECTIVELY", "RESULTED", "RESULTING", "RESULTS", "RIGHT", "RUN", "S", "SAID", "SAME", "SAW", "SAY", "SAYING", "SAYS", "SEC", "SECTION", "SEE", "SEEING", "SEEM", "SEEMED", "SEEMING", "SEEMS", "SEEN", "SELF", "SELVES", "SENT", "SEVEN", "SEVERAL", "SHALL", "SHE", "SHED", "SHELL", "SHES", "SHOULD", "SHOULDNT", "SHOW", "SHOWED", "SHOWN", "SHOWNS", "SHOWS", "SIGNIFICANT", "SIGNIFICANTLY", "SIMILAR", "SIMILARLY", "SINCE", "SIX", "SLIGHTLY", "SO", "SOME", "SOMEBODY", "SOMEHOW", "SOMEONE", "SOMETHAN", "SOMETHING", "SOMETIME", "SOMETIMES", "SOMEWHAT", "SOMEWHERE", "SOON", "SORRY", "SPECIFICALLY", "SPECIFIED", "SPECIFY", "SPECIFYING", "STILL", "STOP", "STRONGLY", "SUB", "SUBSTANTIALLY", "SUCCESSFULLY", "SUCH", "SUFFICIENTLY", "SUGGEST", "SUP", "SURE", "T", "TAKE", "TAKEN", "TAKING", "TELL", "TENDS", "TH", "THAN", "THANK", "THANKS", "THANX", "THAT", "THATLL", "THATS", "THATVE", "THE", "THEIR", "THEIRS", "THEM", "THEMSELVES", "THEN", "THENCE", "THERE", "THEREAFTER", "THEREBY", "THERED", "THEREFORE", "THEREIN", "THERELL", "THEREOF", "THERERE", "THERES", "THERETO", "THEREUPON", "THEREVE", "THESE", "THEY", "THEYD", "THEYLL", "THEYRE", "THEYVE", "THINK", "THIS", "THOSE", "THOU", "THOUGH", "THOUGHH", "THOUSAND", "THROUG", "THROUGH", "THROUGHOUT", "THRU", "THUS", "TIL", "TIP", "TO", "TOGETHER", "TOO", "TOOK", "TOWARD", "TOWARDS", "TRIED", "TRIES", "TRULY", "TRY", "TRYING", "TS", "TWICE", "TWO", "U", "UN", "UNDER", "UNFORTUNATELY", "UNLESS", "UNLIKE", "UNLIKELY", "UNTIL", "UNTO", "UP", "UPON", "UPS", "US", "USE", "USED", "USEFUL", "USEFULLY", "USEFULNESS", "USES", "USING", "USUALLY", "V", "VALUE", "VARIOUS", "VE", "VERY", "VIA", "VIZ", "VOL", "VOLS", "VS", "W", "WANT", "WANTS", "WAS", "WASNT", "WAY", "WE", "WED", "WELCOME", "WELL", "WENT", "WERE", "WERENT", "WEVE", "WHAT", "WHATEVER", "WHATLL", "WHATS", "WHEN", "WHENCE", "WHENEVER", "WHERE", "WHEREAFTER", "WHEREAS", "WHEREBY", "WHEREIN", "WHERES", "WHEREUPON", "WHEREVER", "WHETHER", "WHICH", "WHILE", "WHIM", "WHITHER", "WHO", "WHOD", "WHOEVER", "WHOLE", "WHOLL", "WHOM", "WHOMEVER", "WHOS", "WHOSE", "WHY", "WIDELY", "WILLING", "WISH", "WITH", "WITHIN", "WITHOUT", "WONT", "WORDS", "WORLD", "WOULD", "WOULDNT", "WWW", "X", "Y", "YES", "YET", "YOU", "YOUD", "YOULL", "YOUR", "YOURE", "YOURS", "YOURSELF", "YOURSELVES", "YOUVE", "Z", "ZERO"};

const int QUERY_WORD_LIMIT = 10;
const int MAX_DIFF_CHARS = 36; // 'A' to 'Z' and '0' to '9'
const std::vector <char> SEPARATORS = {' ', ',', ';', '.', '"', '\'', '\n', '\t', '\r', '\0', '-'};
const char RETURN_ENTITY = '\0';

class dictionary
{
private:
    // a hash table that stores all possible words.
    std::unordered_map<std::string, int> dict;

    // a list that stores distinct words for ease of access.
    std::vector<std::string> words;

    int idCnt;

public:
    dictionary()
    {
        idCnt = 0;
    }

    // add directly a string into dictionary (not recommended unless you understand).
    void addWord (const std::string& word)
    {
        if (dict[word])
            return;
        dict[word] = ++idCnt;
    }

    // get hash id of a word.
    int getIdByWord (const std::string& word) const
    {
        std::unordered_map<std::string, int>::const_iterator it = dict.find(word);
        return it == dict.end() ? 0 : it->second;
    }

    // get word that has a hash id
    std::string getWordById (int id) const
    {
        return  id >= 1 && id <= (int)words.size() ? words[id - 1] : "";
    }
};

struct baseNode
{
    int fileInd, pos, line, len;
    bool isTitle;
    baseNode(int _fileInd, int _pos, int _line, int _len = 0, bool _isTitle = 0):
        fileInd(_fileInd),
        pos(_pos),
        line(_line),
        len(_len),
        isTitle(_isTitle)
        {};
    baseNode():
        fileInd(0),
        pos(0),
        line(0),
        len(0),
        isTitle(0)
        {};
        // Some other metadatas
};

bool operator< (baseNode a, baseNode b)
{
    return a.fileInd < b.fileInd || (a.fileInd == b.fileInd && (a.line < b.line || (a.line == b.line && (a.pos < b.pos || (a.pos == b.pos && a.len < b.len)))));
}

std::string cap (std::string x)
{
    std::string res = "";
    for (size_t i = 0; i < x.size(); i++)
        res += (char)('a' <= x[i] && x[i] <= 'z' ? x[i] - 32 : x[i]);
    return res;
}

void printResult (std::vector <baseNode> res)
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

bool isTextFile (std::string name)
{
    return name.substr(name.size() - 4) == ".txt";
}

bool isSeparator (char ch)
{
    for (size_t i = 0; i < SEPARATORS.size(); i++)
        if (ch == SEPARATORS[i])
            return 1;
    return 0;
}

int strToNum (std::string num)
{
    int res = 0;
    for (size_t i = 0; i < num.size(); i++)
        res = res * 10 + num[i] - '0';
    return res;
}

bool isNumber (std::string word)
{
    for (size_t i = (word[0] == '$'); i < word.size(); i++)
        if (word[i] < '0' || word[i] > '9')
            return 0;
    return 1;
}

std::string nextWord (std::string content, int &ind, int &pos, int &len)
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

    len = cur - ind;
    ind = cur;

    return res;
}

struct TrieNode
{
    std::map <char, TrieNode*> child;
    std::vector <baseNode> data;

    void saveToFile (std::ofstream &out)
    {
        int size = TrieNode::data.size();
        out.write((char*)&size, 4);

        for (int i = 0; i < size; i++)
        {
            out.write((char*)&(TrieNode::data[i].fileInd), 4);
            out.write((char*)&(TrieNode::data[i].pos), 4);
            out.write((char*)&(TrieNode::data[i].line), 4);
            out.write((char*)&(TrieNode::data[i].len), 4);
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

    void readFromFile (std::ifstream &inp)
    {
        int size;
        inp.read((char*)&size, 4);
        for (int i = 0; i < size; i++)
        {
            int fileInd, pos, line, len;
            bool isTitle;
            inp.read((char*)&fileInd, 4);
            inp.read((char*)&pos, 4);
            inp.read((char*)&line, 4);
            inp.read((char*)&len, 4);
            inp.read((char*)&isTitle, sizeof(isTitle));
            TrieNode::data.push_back(baseNode(fileInd, pos, line, isTitle));
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
    }

    void clear()
    {
        for (auto p = TrieNode::child.begin(); p != TrieNode::child.end(); p++)
        {
            p -> second -> clear();
            delete p -> second;
        }
    }

    void getSize (std::vector <int> &size)
    {
        size.push_back((sizeof(TrieNode::child) + TrieNode::child.size() * 13) + (sizeof(TrieNode::data) + TrieNode::data.size() * 4));
        for (auto p = TrieNode::child.begin(); p != TrieNode::child.end(); p++)
        {
            p -> second -> getSize(size);
        }
        return;
    }
};

struct Trie
{
    TrieNode *root;
    Trie (): root(new TrieNode) {};
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
    std::vector<int> trie[QUERY_WORD_LIMIT][QUERY_WORD_LIMIT][MAX_DIFF_CHARS];
    dictionary words, files;
    TrieNode *root;
    std::vector <std::pair <int, baseNode> > numbers;
    std::vector <std::string> fileNames;

    baseData(): root(new TrieNode) {};

    // Raw data to main dataset
    void loadFromFiles (std::string path)
    {
        std::ifstream inp;
        std::string content;
        int fileInd = 0;
        double time, totalTime = 0;

        std::cout << "[INFO] Looking at path = " << path << "\n";
        dirHandler dir = dirHandler(path);
        std::vector <std::string> files = dir.files();
        for(std::string f : files)
        {
            if (!isTextFile(f))
            continue;

            time = clock();

            baseData::fileNames.push_back(f);

            // Push file name to trie
            int ind = 0, pos, line = 0, len;
            content = dir.fileName();

            while (1)
            {
                word = nextWord(content, ind, pos, len);
                if (word.size() == 0)
                    break;
                // cout << "[INFO] word = " << word << " at line " << line + 1 << " " << " position " << pos + 1 << "\n";
                // cout << "[DEBUG] tmp = " << tmp << " ind = " << ind << "\n";
                if (isNumber(word))
                    baseData::insertNumber(word, baseNode(fileInd, pos, line, len, 1));
                else
                    baseData::insert(word, baseNode(fileInd, pos, line, len, 1));
            }

            dir.next(f);

            // Push content to trie
            content = dir.readAll();
            line = 0;
            std::stringstream lines(content);
            std::string word, singleLine;

            while (!lines.eof())
            {
                getline(lines, singleLine);
                ind = 0;
                while (1)
                {
                    word = nextWord(singleLine, ind, pos, len);
                    if (word.size() == 0)
                        break;
                    if (stopwords.find(word) != stopwords.end())
                        continue;
                    // cout << "[INFO] word = " << word << " at line " << line + 1 << " " << " position " << pos + 1 << "\n";
                    // cout << "[DEBUG] tmp = " << tmp << " ind = " << ind << "\n";
                    if (isNumber(word))
                        baseData::insertNumber(word, baseNode(fileInd, pos, line));
                    else
                        baseData::insert(word, baseNode(fileInd, pos, line, len));
                }
                line++;
            }

            dir.back();
            fileInd++;

            time = (clock() - time) / CLOCKS_PER_SEC;
            // cout << "[INFO] " << p.path() << " loaded in " << setprecision(3) << time << "s\n";
            totalTime += time;
        }
        sort(baseData::numbers.begin(), baseData::numbers.end());
        std::cout << "[INFO] Done in " << std::setprecision(3) << totalTime << "s\n";
    }

    // Insert numbers to vector
    void insertNumber (std::string number, baseNode data)
    {
        int n;
        if (number[0] == '$')
            n = strToNum(number.substr(1));
        else
            n = strToNum(number);

        baseData::numbers.push_back(std::pair <int, baseNode> (n, data));
    }

    //Insert words to trie
    void insert (std::string word, baseNode DATABITS_16X)
    {
        TrieNode *cur = baseData::root;
        for (size_t i = 0; i < word.size(); i++)
        {
            if (cur -> child.find(word[i]) == cur -> child.end())
            cur -> child[word[i]] = new TrieNode;

            cur = cur -> child[word[i]];
        }
        cur -> data.push_back(data);
    }

    // Save datasets to files
    void saveToFile ()
    {
        //Read till eof
        //linkChar - "#" if it is leaf, go back
        //#m -size of data vector
        //m lines
        //#fileInd #pos #isTitle
        //#cont - can we continue go down?
        double time = clock();
        std::cout << "[INFO] Saving...\n";
        // Trie
        std::ofstream out;
        out.open("tree.bin", std::ios::binary);
        baseData::root -> saveToFile(out);
        out.close();

        // Numbers
        out.open("numbers.bin", std::ios::binary);
        int size = baseData::numbers.size();
        out.write((char*)&size, 4);
        for (int i = 0; i < size; i++)
        {
            out.write((char*)&(baseData::numbers[i].first), 4);
            out.write((char*)&(baseData::numbers[i].second.fileInd), 4);
            out.write((char*)&(baseData::numbers[i].second.pos), 4);
            out.write((char*)&(baseData::numbers[i].second.line), 4);
        }
        out.close();

        // FileNames
        out.open("filenames.txt");
        out << baseData::fileNames.size();
        for (auto name : baseData::fileNames)
            out << name << "\n";
        out.close();

        time = (clock() - time) / CLOCKS_PER_SEC;
        std::cout << "[INFO] Successfully saved in " << std::fixed << std::setprecision(4) << time << "s\n";
    }

    // Read datasets from files
    void readFromFile ()
    {
        double time = clock();
        // Trie
        std::cout << "[INFO] Reading...\n";
        std::ifstream inp;
        inp.open("tree.bin", std::ios::binary);
        if (!inp.is_open())
        {
            std::cout << "[ERROR] tree.bin not found\n";
            return;
        }

        baseData::root -> readFromFile(inp);
        inp.close();

        // Numbers
        inp.open("numbers.bin", std::ios::binary);
        if (!inp.is_open())
        {
            std::cout << "[ERROR] numbers.bin not found\n";
            return;
        }

        int size;
        inp.read((char*)&size, 4);
        for (int i = 0; i < size; i++)
        {
            int num, fileInd, pos, line;
            inp.read((char*)&(num), 4);
            inp.read((char*)&(fileInd), 4);
            inp.read((char*)&(pos), 4);
            inp.read((char*)&(line), 4);
            baseData::numbers.push_back(std::pair <int, baseNode> (num, baseNode(fileInd, pos, line)));
        }
        inp.close();

        // Filenames
        inp.open("filenames.txt");
        if (!inp.is_open())
        {
            std::cout << "[ERROR] filenames.txt not found\n";
            return;
        }

        std::string tmp;
        inp >> size;
        for (int i = 0; i < size; i++)
        {
            getline(std::cin, tmp);
            baseData::fileNames.push_back(tmp);
        }
        inp.close();

        time = (clock() - time) / CLOCKS_PER_SEC;
        std::cout << "[INFO] Successfully read in " << std::fixed << std::setprecision(4) << time << "s\n";
    }

    // Search for words
    std::vector <baseNode> search (std::string query, bool isTitle = 0)
    {
        TrieNode *cur = baseData::root;
        std::vector <baseNode> res;
        for (size_t i = 0; i < query.size(); i++)
        {
            if (cur -> child.find(query[i]) == cur -> child.end())
            return std::vector <baseNode>();
            else
            cur = cur -> child[query[i]];
        }
        for (size_t i = 0; i < cur -> data.size(); i++)
        {
            if (cur -> data[i].isTitle == isTitle)
                res.push_back(cur -> data[i]);
        }
        return res;
    }

    // Search for numbers in the interval
    std::vector <baseNode> searchNumber (int l, int r)
    {
        std::vector <baseNode> res;
        auto it = lower_bound(baseData::numbers.begin(), baseData::numbers.end(), std::pair <int, baseNode> (l, baseNode()));
        while (it != baseData::numbers.end() && it -> first <= r)
        {
            res.push_back(it -> second);
            it++;
        }
        return res;
    }

    // Analytics about trie
    void analytics ()
    {
        std::vector <int> singleNodeSize;
        baseData::root -> getSize(singleNodeSize);
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
    void clear ()
    {
        baseData::root -> clear();
        delete baseData::root;
        std::cout << "[INFO] Tree is successfully cleared\n";
    }

    void buildDataToBinary (const std::string& binaryFileName);
    void loadDataFromBinary (const std::string& binaryFileName);
    /* MUST ADD MORE FUNCTIONS TO THIS STRUCT!!! */
};

#endif // SE_BASE_H
