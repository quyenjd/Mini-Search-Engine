#ifndef SE_QUERY_H
#define SE_QUERY_H

#define number(x) (x >= '0' && x <= '9')
#define normal(x) ((x >= 'A' && x <= 'Z') || (x >= 'a' && x <= 'z') || number(x))
#define operat(x) (x == '-' || x == '+' || x == '*' || x == '#' || x == '$' || x == '~' || x == '"')
#define punc(x) (x == '\'')
#define all(x) (normal(x) || operat(x) || punc(x))
#define upper(x) ((x >= 'a' && x <= 'z') ? char(x - 32) : x)

#include "Base.h"
#include "Utility.h"
#include <set>
#include <string>
#include <vector>

const std::set<std::string> stopwords = {"A", "ABLE", "ABOUT", "ABOVE", "ABST", "ACCORDANCE", "ACCORDING", "ACCORDINGLY", "ACROSS", "ACT", "ACTUALLY", "ADDED", "ADJ", "AFFECTED", "AFFECTING", "AFFECTS", "AFTER", "AFTERWARDS", "AGAIN", "AGAINST", "AH", "ALL", "ALMOST", "ALONE", "ALONG", "ALREADY", "ALSO", "ALTHOUGH", "ALWAYS", "AM", "AMONG", "AMONGST", "AN", "AND", "ANNOUNCE", "ANOTHER", "ANY", "ANYBODY", "ANYHOW", "ANYMORE", "ANYONE", "ANYTHING", "ANYWAY", "ANYWAYS", "ANYWHERE", "APPARENTLY", "APPROXIMATELY", "ARE", "AREN", "ARENT", "ARISE", "AROUND", "AS", "ASIDE", "ASK", "ASKING", "AT", "AUTH", "AVAILABLE", "AWAY", "AWFULLY", "B", "BACK", "BE", "BECAME", "BECAUSE", "BECOME", "BECOMES", "BECOMING", "BEEN", "BEFORE", "BEFOREHAND", "BEGIN", "BEGINNING", "BEGINNINGS", "BEGINS", "BEHIND", "BEING", "BELIEVE", "BELOW", "BESIDE", "BESIDES", "BETWEEN", "BEYOND", "BIOL", "BOTH", "BRIEF", "BRIEFLY", "BUT", "BY", "C", "CA", "CAME", "CAN", "CANNOT", "CANT", "CAUSE", "CAUSES", "CERTAIN", "CERTAINLY", "CO", "COM", "COME", "COMES", "CONTAIN", "CONTAINING", "CONTAINS", "COULD", "COULDNT", "D", "DATE", "DID", "DIDNT", "DIFFERENT", "DO", "DOES", "DOESNT", "DOING", "DONE", "DONT", "DOWN", "DOWNWARDS", "DUE", "DURING", "E", "EACH", "ED", "EDU", "EFFECT", "EG", "EIGHT", "EIGHTY", "EITHER", "ELSE", "ELSEWHERE", "END", "ENDING", "ENOUGH", "ESPECIALLY", "ET", "ETC", "EVEN", "EVER", "EVERY", "EVERYBODY", "EVERYONE", "EVERYTHING", "EVERYWHERE", "EX", "EXCEPT", "F", "FAR", "FEW", "FF", "FIFTH", "FIRST", "FIVE", "FIX", "FOLLOWED", "FOLLOWING", "FOLLOWS", "FOR", "FORMER", "FORMERLY", "FORTH", "FOUND", "FOUR", "FROM", "FURTHER", "FURTHERMORE", "G", "GAVE", "GET", "GETS", "GETTING", "GIVE", "GIVEN", "GIVES", "GIVING", "GO", "GOES", "GONE", "GOT", "GOTTEN", "H", "HAD", "HAPPENS", "HARDLY", "HAS", "HASNT", "HAVE", "HAVENT", "HAVING", "HE", "HED", "HENCE", "HER", "HERE", "HEREAFTER", "HEREBY", "HEREIN", "HERES", "HEREUPON", "HERS", "HERSELF", "HES", "HI", "HID", "HIM", "HIMSELF", "HIS", "HITHER", "HOME", "HOW", "HOWBEIT", "HOWEVER", "HUNDRED", "I", "ID", "IE", "IF", "ILL", "IM", "IMMEDIATE", "IMMEDIATELY", "IMPORTANCE", "IMPORTANT", "IN", "INC", "INDEED", "INDEX", "INFORMATION", "INSTEAD", "INTO", "INVENTION", "INWARD", "IS", "ISNT", "IT", "ITD", "ITLL", "ITS", "ITSELF", "IVE", "J", "JUST", "K", "KEEP", "KEEPS", "KEPT", "KG", "KM", "KNOW", "KNOWN", "KNOWS", "L", "LARGELY", "LAST", "LATELY", "LATER", "LATTER", "LATTERLY", "LEAST", "LESS", "LEST", "LET", "LETS", "LIKE", "LIKED", "LIKELY", "LINE", "LITTLE", "LL", "LOOK", "LOOKING", "LOOKS", "LTD", "M", "MADE", "MAINLY", "MAKE", "MAKES", "MANY", "MAY", "MAYBE", "ME", "MEAN", "MEANS", "MEANTIME", "MEANWHILE", "MERELY", "MG", "MIGHT", "MILLION", "MISS", "ML", "MORE", "MOREOVER", "MOST", "MOSTLY", "MR", "MRS", "MUCH", "MUG", "MUST", "MY", "MYSELF", "N", "NA", "NAME", "NAMELY", "NAY", "ND", "NEAR", "NEARLY", "NECESSARILY", "NECESSARY", "NEED", "NEEDS", "NEITHER", "NEVER", "NEVERTHELESS", "NEW", "NEXT", "NINE", "NINETY", "NO", "NOBODY", "NON", "NONE", "NONETHELESS", "NOONE", "NOR", "NORMALLY", "NOS", "NOT", "NOTED", "NOTHING", "NOW", "NOWHERE", "O", "OBTAIN", "OBTAINED", "OBVIOUSLY", "OF", "OFF", "OFTEN", "OH", "OK", "OKAY", "OLD", "OMITTED", "ON", "ONCE", "ONE", "ONES", "ONLY", "ONTO", "OR", "ORD", "OTHER", "OTHERS", "OTHERWISE", "OUGHT", "OUR", "OURS", "OURSELVES", "OUT", "OUTSIDE", "OVER", "OVERALL", "OWING", "OWN", "P", "PAGE", "PAGES", "PART", "PARTICULAR", "PARTICULARLY", "PAST", "PER", "PERHAPS", "PLACED", "PLEASE", "PLUS", "POORLY", "POSSIBLE", "POSSIBLY", "POTENTIALLY", "PP", "PREDOMINANTLY", "PRESENT", "PREVIOUSLY", "PRIMARILY", "PROBABLY", "PROMPTLY", "PROUD", "PROVIDES", "PUT", "Q", "QUE", "QUICKLY", "QUITE", "QV", "R", "RAN", "RATHER", "RD", "RE", "READILY", "REALLY", "RECENT", "RECENTLY", "REF", "REFS", "REGARDING", "REGARDLESS", "REGARDS", "RELATED", "RELATIVELY", "RESEARCH", "RESPECTIVELY", "RESULTED", "RESULTING", "RESULTS", "RIGHT", "RUN", "S", "SAID", "SAME", "SAW", "SAY", "SAYING", "SAYS", "SEC", "SECTION", "SEE", "SEEING", "SEEM", "SEEMED", "SEEMING", "SEEMS", "SEEN", "SELF", "SELVES", "SENT", "SEVEN", "SEVERAL", "SHALL", "SHE", "SHED", "SHELL", "SHES", "SHOULD", "SHOULDNT", "SHOW", "SHOWED", "SHOWN", "SHOWNS", "SHOWS", "SIGNIFICANT", "SIGNIFICANTLY", "SIMILAR", "SIMILARLY", "SINCE", "SIX", "SLIGHTLY", "SO", "SOME", "SOMEBODY", "SOMEHOW", "SOMEONE", "SOMETHAN", "SOMETHING", "SOMETIME", "SOMETIMES", "SOMEWHAT", "SOMEWHERE", "SOON", "SORRY", "SPECIFICALLY", "SPECIFIED", "SPECIFY", "SPECIFYING", "STILL", "STOP", "STRONGLY", "SUB", "SUBSTANTIALLY", "SUCCESSFULLY", "SUCH", "SUFFICIENTLY", "SUGGEST", "SUP", "SURE", "T", "TAKE", "TAKEN", "TAKING", "TELL", "TENDS", "TH", "THAN", "THANK", "THANKS", "THANX", "THAT", "THATLL", "THATS", "THATVE", "THE", "THEIR", "THEIRS", "THEM", "THEMSELVES", "THEN", "THENCE", "THERE", "THEREAFTER", "THEREBY", "THERED", "THEREFORE", "THEREIN", "THERELL", "THEREOF", "THERERE", "THERES", "THERETO", "THEREUPON", "THEREVE", "THESE", "THEY", "THEYD", "THEYLL", "THEYRE", "THEYVE", "THINK", "THIS", "THOSE", "THOU", "THOUGH", "THOUGHH", "THOUSAND", "THROUG", "THROUGH", "THROUGHOUT", "THRU", "THUS", "TIL", "TIP", "TO", "TOGETHER", "TOO", "TOOK", "TOWARD", "TOWARDS", "TRIED", "TRIES", "TRULY", "TRY", "TRYING", "TS", "TWICE", "TWO", "U", "UN", "UNDER", "UNFORTUNATELY", "UNLESS", "UNLIKE", "UNLIKELY", "UNTIL", "UNTO", "UP", "UPON", "UPS", "US", "USE", "USED", "USEFUL", "USEFULLY", "USEFULNESS", "USES", "USING", "USUALLY", "V", "VALUE", "VARIOUS", "VE", "VERY", "VIA", "VIZ", "VOL", "VOLS", "VS", "W", "WANT", "WANTS", "WAS", "WASNT", "WAY", "WE", "WED", "WELCOME", "WELL", "WENT", "WERE", "WERENT", "WEVE", "WHAT", "WHATEVER", "WHATLL", "WHATS", "WHEN", "WHENCE", "WHENEVER", "WHERE", "WHEREAFTER", "WHEREAS", "WHEREBY", "WHEREIN", "WHERES", "WHEREUPON", "WHEREVER", "WHETHER", "WHICH", "WHILE", "WHIM", "WHITHER", "WHO", "WHOD", "WHOEVER", "WHOLE", "WHOLL", "WHOM", "WHOMEVER", "WHOS", "WHOSE", "WHY", "WIDELY", "WILLING", "WISH", "WITH", "WITHIN", "WITHOUT", "WONT", "WORDS", "WORLD", "WOULD", "WOULDNT", "WWW", "X", "Y", "YES", "YET", "YOU", "YOUD", "YOULL", "YOUR", "YOURE", "YOURS", "YOURSELF", "YOURSELVES", "YOUVE", "Z", "ZERO"};

class queryNode
{
private:
    multitype V1, V2;
    bool _isWild, _isRange, _isIncluded, _isExcluded, _isSynonym;

public:
    std::vector<baseNode> occurrences;

    queryNode()
    {
        V1 = V2 = multitype();
        _isWild = true;
        _isRange = _isIncluded = _isExcluded = _isSynonym = false;
    }

    queryNode (const multitype& _V1,
               bool __isIncluded = false,
               bool __isExcluded = false,
               bool __isSynonym = false)
    {
        V1 = _V1;
        _isWild = _isRange = false;
        _isSynonym = __isSynonym;

        // if SYNONYM mode is ON, INCLUDE mode is ON.
        _isIncluded = _isSynonym ? true : __isIncluded;

        // if INCLUDE mode is ON, EXCLUDE mode must always be OFF.
        _isExcluded = _isIncluded ? false : __isExcluded;
    }

    queryNode (const multitype& _V1, const multitype& _V2)
    {
        V1 = _V1;
        V2 = _V2;
        _isRange = true;
        _isWild = _isIncluded = _isExcluded = _isSynonym = false;
    }

    multitype& fi()
    {
        return V1;
    }
    multitype fi() const
    {
        return V1;
    }

    multitype& se()
    {
        return V2;
    }
    multitype se() const
    {
        return V2;
    }

    bool isWild() const
    {
        return _isWild;
    }

    bool isRange() const
    {
        return _isRange;
    }

    bool isIncluded() const
    {
        return _isIncluded;
    }

    bool isExcluded() const
    {
        return _isExcluded;
    }

    bool isSynonym() const
    {
        return _isSynonym;
    }

    // toggle WILD mode ON and OFF.
    // if WILD mode is OFF when calling this method, all variables will be reset.
    void toggleWild()
    {
        if (!_isWild)
        {
            V1 = V2 = multitype();
            _isRange = _isIncluded = _isExcluded = false;
        }
        _isWild ^= 1;
    }

    // toggle RANGE mode ON and OFF.
    // if RANGE mode is ON when calling this method, V2 will be reset.
    // if RANGE mode is OFF when calling this method, INCLUDE and EXCLUDE mode will be OFF.
    void toggleRange()
    {
        if (_isRange)
            V2 = multitype();
        else
            _isWild = _isIncluded = _isExcluded = _isSynonym = false;
        _isRange ^= 1;
    }

    // toggle INCLUDE mode ON and OFF.
    // if INCLUDE mode is OFF when calling this method, RANGE and EXCLUDE mode will be OFF.
    void toggleIncluded()
    {
        if (!_isIncluded)
        {
            if (_isRange)
                toggleRange();
            _isExcluded = false;
        }
        else
            _isSynonym = false;
        _isIncluded ^= 1;
    }

    // toggle EXCLUDE mode ON and OFF.
    // if EXCLUDE mode is OFF when calling this method, RANGE and INCLUDE mode will be OFF.
    void toggleExcluded()
    {
        if (!_isExcluded)
        {
            if (_isRange)
                toggleRange();
            _isIncluded = _isSynonym = false;
        }
        _isExcluded ^= 1;
    }

    // toggle SYNONYM mode ON and OFF.
    // if SYNONYM mode is OFF when calling this method, INCLUDE mode will be ON.
    void toggleSynonym()
    {
        if (!_isSynonym && !_isIncluded)
            toggleIncluded();
        _isSynonym ^= 1;
    }
};

class queryData
{
private:
    bool readNum (size_t& i, std::string& word, const std::string& str)
    {
        bool flag = true;
        for (; i < str.length() && all(str[i]); ++i)
        {
            if (!number(str[i]))
                flag = false;
            if ((!flag && normal(str[i])) || number(str[i]))
                word += str[i];
        }
        return flag;
    }

    void readStr (size_t& i, std::string& word, const std::string& str)
    {
        for (; i < str.length() && all(str[i]); ++i)
            if (normal(str[i]))
                word += upper(str[i]);
    }

    bool readQuoted (size_t& i, std::string& word, const std::string& str)
    {
        word = "\"";
        size_t tmp = ++i;
        for (; i < str.length(); ++i)
        {
            word += upper(str[i]);
            if (str[i] == '"')
            {
                ++i;
                return true;
            }
        }
        i = tmp;
        return false;
    }

public:
    // array of keywords in the query.
    std::vector<queryNode> words;

    // array of resulting files' IDs (after searching).
    std::set<int> matchIDs;

    queryData() {}

    // this method splits 'queryStr' into words and normalize them
    // before pushing into array 'words'.
    queryData (const multitype& queryStr, bool eliminateStopwords = false)
    {
        std::string str(queryStr.to_str()), tmp;
        bool rangeFlag = false;
        size_t i = 0;

        while (i < str.length())
        {
            // find first valid character.
            for (; i < str.length() && !all(str[i]); ++i);

            // if end of query, break here
            if (i == str.length())
                break;

            std::string word;
            bool numberFlag = false,
                 includedFlag = false,
                 excludedFlag = false,
                 synonymFlag = false;

            // if exact match, read everything inside the quotes or only one word.
            if (str[i] == '"')
            {
                if (!(includedFlag = readQuoted(i, word, str)))
                    continue;
            }
            else
            // if price, loop through all consequent digits.
            if (str[i] == '$')
            {
                word = "$";

                // if NaN, pop the dollar character.
                if (!(numberFlag = readNum(++i, word, str)) ||
                    /* if a word contains only dollar sign, erase it. */ word.length() == 1)
                    word.erase(0, 1);

                // if the string is now empty,
                // skip the rest to prevent a wild entry from being pushed.
                if (word.empty())
                    continue;
            }
            else
            // if digit, loop through all consequent digits.
            if (number(str[i]))
                numberFlag = readNum(i, word, str);
            else
            // if wild, next character should be empty.
            if (str[i] == '*')
            {
                if (i + 1 < str.length() && all(str[i + 1]))
                {
                    ++i;
                    continue;
                }
            }
            else
            // if included, set flag and read all upcoming characters.
            if (str[i] == '+')
            {
                readStr(++i, word, str);
                includedFlag = true;
            }
            else
            // if excluded, set flag and read all upcoming characters.
            if (str[i] == '-')
            {
                readStr(++i, word, str);
                excludedFlag = true;
            }
            else
            // if synonym, set flag and read all upcoming characters.
            if (str[i] == '~')
            {
                readStr(++i, word, str);
                synonymFlag = true;
            }
            else
            // if hash, just basically read the whole hash.
            if (str[i] == '#')
            {
                word = "#";
                readStr(++i, word, str);

                if (word.length() == 1)
                    continue;

                includedFlag = true;
            }
            else
            // if no operation is specified, just read.
                readStr(i, word, str);

            // after done processing a number,
            // check if a range query is happening.
            if (rangeFlag && numberFlag)
            {
                words.push_back(queryNode(tmp, word));
                rangeFlag = false;
            }
            else
            // check if this is a range query.
            if (numberFlag && i + 2 < str.length() && str[i] == '.' && str[i + 1] == '.')
            {
                i += 2;
                rangeFlag = true;
                tmp = word;
            }
            else
            {
                // if rangeFlag is still ON, push 'tmp' into the array first.
                if (rangeFlag)
                {
                    words.push_back(queryNode(tmp));
                    rangeFlag = false;
                }

                // if some flag is ON, push with flag.
                if (includedFlag || excludedFlag || synonymFlag)
                {
                    if (word.empty())
                        continue;
                    words.push_back(queryNode(word, includedFlag, excludedFlag, synonymFlag));
                }
                else
                // if 'word' is empty, push as wild.
                if (word.empty())
                {
                    ++i;
                    words.push_back(queryNode());
                }
                else
                // if no flag is marked, check stopword and push.
                if (!eliminateStopwords || stopwords.find(word) == stopwords.end())
                    words.push_back(queryNode(word));
            }
        }

        // if range flag is still ON here, push the buffered into array.
        if (rangeFlag)
        {
            words.push_back(queryNode(tmp));
            rangeFlag = false;
        }
    }
};

#endif // SE_QUERY_H
