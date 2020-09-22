#ifndef SE_UTILITY_H
#define SE_UTILITY_H

#define _WIN32_WINNT 0x0500

#include <cinttypes>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <codecvt>
#include <exception>
#include <fstream>
#include <locale>
#include <sstream>
#include <streambuf>
#include <string>
#include <utility>
#include <vector>
#include <windows.h>

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define REAL_EPS 1e-6
#define STR_BUFFER 256
#define Int int
#define Long long long
#define Char char
#define Pchar char*
#define Double double

#define upper(x) ((x >= 'a' && x <= 'z') ? char(x - 32) : x)
#define number(x) (x >= '0' && x <= '9')
#define normal(x) ((x >= 'A' && x <= 'Z') || (x >= 'a' && x <= 'z') || number(x))
#define operat(x) (x == '-' || x == '+' || x == '*' || x == '#' || x == '$' || x == '~' || x == '"')
#define punc(x) (x == '\'')
#define all(x) (normal(x) || operat(x) || punc(x))

#define ENCODING_ASCII      0
#define ENCODING_UTF8       1
#define ENCODING_UTF16LE    2
#define ENCODING_UTF16BE    3

typedef std::pair<int, int> pii;

inline void string_copy(const Pchar from, Pchar& to)
{
    to = (Pchar)malloc(strlen(from) + 1);
    for (Pchar p = to; (*p = *from) != '\0'; ++p, ++from);
}

class multitype
{
private:
    union _tunknown
    {
        Int    _tint;
        Long   _tlong;
        Char   _tchar;
        Pchar  _tpchar;
        Double _tdouble;
    } values;
    Pchar _type;

public:
    // constructors.
    multitype()
    {
        init();
    }
    multitype(const Int e)
    {
        assign(e);
    }
    multitype(const Long e)
    {
        assign(e);
    }
    multitype(const Char e)
    {
        assign(e);
    }
    multitype(const Pchar e)
    {
        assign(e);
    }
    multitype(const Double e)
    {
        assign(e);
    }
    multitype(const multitype& e)
    {
        assign(e);
    }
    multitype(const std::string& e)
    {
        assign(e.c_str());
    }

    // set multitype back to default value.
    void init()
    {
        string_copy("unknown", _type);
    }

    // assign functions
    void assign(const Int e)
    {
        string_copy("int", _type);
        values._tint = e;
    }
    void assign(const Long e)
    {
        string_copy("long", _type);
        values._tlong = e;
    }
    void assign(const Char e)
    {
        string_copy("char", _type);
        values._tchar = e;
    }
    void assign(const Pchar e)
    {
        string_copy("str", _type);
        string_copy(e, values._tpchar);
    }
    void assign(const Double e)
    {
        string_copy("double", _type);
        values._tdouble = e;
    }
    void assign(const multitype& e)
    {
        string_copy(e.type(), _type);
        if (e.is_type("int"))
            values._tint = e.to_int();
        if (e.is_type("long"))
            values._tlong = e.to_long();
        if (e.is_type("char"))
            values._tchar = e.to_char();
        if (e.is_type("str"))
            string_copy(e.to_str(), values._tpchar);
        if (e.is_type("double"))
            values._tdouble = e.to_double();
    }
    void assign(const std::string& e)
    {
        assign(e.c_str());
    }

    multitype& operator= (const Int e)
    {
        assign(e);
        return *this;
    }
    multitype& operator= (const Long e)
    {
        assign(e);
        return *this;
    }
    multitype& operator= (const Char e)
    {
        assign(e);
        return *this;
    }
    multitype& operator= (const Pchar e)
    {
        assign(e);
        return *this;
    }
    multitype& operator= (const Double e)
    {
        assign(e);
        return *this;
    }
    multitype& operator= (const multitype& e)
    {
        assign(e);
        return *this;
    }
    multitype& operator= (const std::string& e)
    {
        assign(e);
        return *this;
    }

    // append e.to_str() to the end of this->to_str().
    // this changes the type of 'this' to "str".
    multitype& append(const multitype& e)
    {
        Char* cur = to_str(),
            * app = e.to_str(),
            * res = new Char[strlen(cur) + strlen(app) + 1];
        Int j = 0;
        for (Int i = 0; i < (Int)strlen(cur); res[j++] = cur[i++]);
        for (Int i = 0; i < (Int)strlen(app); res[j++] = app[i++]);
        res[j] = '\0';
        assign(res);
        return *this;
    }

    // get type of current value. returns "int", "long", "char", "str", "double"
    // or "unknown" (default).
    Pchar type() const
    {
        return _type;
    }

    // check if current value has the type of e. this equals to 'type() == e'.
    bool is_type(const Pchar e) const
    {
        return strcmp(_type, e) == 0;
    }

    // check if the value has not been set so far. this equals to 'type() == "unknown"'
    // and 'is_type("unknown")'
    bool is_unknown() const
    {
        return is_type("unknown");
    }

    // convert current value (of any type) to int.
    Int to_int() const
    {
        if (is_type("int"))
            return values._tint;
        if (is_type("long"))
            return values._tlong;
        if (is_type("char"))
            return values._tchar >= '0' && values._tchar <= '9' ? (values._tchar - '0') : (Int)values._tchar;
        if (is_type("str"))
        {
            bool neg = values._tpchar[0] == '-';
            Int ret = 0;
            for (Int i = neg; i < (Int)strlen(values._tpchar); ++i)
                if (values._tpchar[i] >= '0' && values._tpchar[i] <= '9')
                    ret = ret * 10 + values._tpchar[i] - '0';
                else
                    break;
            if (neg)
                ret = -ret;
            return ret;
        }
        if (is_type("double"))
            return (Int)values._tdouble;
        return 0;
    }

    // convert current value (of any type) to long long.
    Long to_long() const
    {
        if (is_type("int"))
            return values._tint;
        if (is_type("long"))
            return values._tlong;
        if (is_type("char"))
            return values._tchar >= '0' && values._tchar <= '9' ? (values._tchar - '0') : (Long)values._tchar;
        if (is_type("str"))
        {
            bool neg = values._tpchar[0] == '-';
            Long ret = 0;
            for (Int i = neg; i < (Int)strlen(values._tpchar); ++i)
                if (values._tpchar[i] >= '0' && values._tpchar[i] <= '9')
                    ret = ret * 10 + values._tpchar[i] - '0';
                else
                    break;
            if (neg)
                ret = -ret;
            return ret;
        }
        if (is_type("double"))
            return (Long)values._tdouble;
        return 0;
    }

    // convert current value (of any type) to char.
    Char to_char() const
    {
        if (is_type("int"))
        {
            Char buf[2];
            snprintf(buf, 2, "%d", values._tint);
            return buf[0];
        }
        if (is_type("char"))
            return values._tchar;
        if (is_type("str"))
            return values._tpchar[0];
        if (is_type("double"))
        {
            Char buf[2];
            snprintf(buf, 2, "%f", values._tdouble);
            return buf[0];
        }
        return 0;
    }

    // convert current value (of any type) to string.
    Pchar to_str() const
    {
        if (is_type("int"))
        {
            Pchar buf = new Char[STR_BUFFER];
            snprintf(buf, STR_BUFFER, "%d", values._tint);
            return buf;
        }
        if (is_type("long"))
        {
            Pchar buf = new Char[STR_BUFFER];
            snprintf(buf, STR_BUFFER, "%" PRId64, values._tlong);
            return buf;
        }
        if (is_type("char"))
        {
            Pchar buf = new Char[2];
            buf[0] = values._tchar;
            return buf;
        }
        if (is_type("str"))
            return values._tpchar;
        if (is_type("double"))
        {
            Pchar buf = new Char[STR_BUFFER];
            snprintf(buf, STR_BUFFER, "%.15f", values._tdouble);

            // trim trailing zeroes
            char* p = strchr(buf, '.');
            int count;

            if (p != NULL)
            {
                count = 10;
                while (count >= 0)
                {
                    --count;
                    if (*p == '\0')
                        break;
                    ++p;
                }
                *p-- = '\0';
                while (*p == '0')
                    *p-- = '\0';
                if (*p == '.')
                    *p = '\0';
            }
            return buf;
        }
        Pchar buf = new Char[1]; buf[0] = '\0';
        return buf;
    }

    // convert current value (of any type) to std::string.
    std::string to_Stdstr() const
    {
        return std::string(to_str());
    }

    // convert current value (of any type) to double.
    Double to_double() const
    {
        if (is_type("int"))
            return (Double)values._tint;
        if (is_type("long"))
            return (Double)values._tlong;
        if (is_type("char"))
        {
            if (values._tchar >= '0' && values._tchar <= '9')
                return values._tchar - '0';
        }
        if (is_type("str"))
        {
            bool neg = values._tpchar[0] == '-';
            Double ret = 0;
            Int i = neg;
            for (; i < (Int)strlen(values._tpchar); ++i)
                if (values._tpchar[i] >= '0' && values._tpchar[i] <= '9')
                    ret = ret * 10 + values._tpchar[i] - '0';
                else
                    break;
            if (i < (Int)strlen(values._tpchar) && values._tpchar[i++] == '.')
            {
                for (Double j = 0.1; i < (Int)strlen(values._tpchar); ++i, j /= 10)
                    if (values._tpchar[i] >= '0' && values._tpchar[i] <= '9')
                        ret += j * (values._tpchar[i] - '0');
                    else
                        break;
            }
            if (neg)
                ret = -ret;
            return ret;
        }
        if (is_type("double"))
            return values._tdouble;

#ifdef NAN
        return NAN;
#elif defined INFINITY
        return INFINITY;
#else
        return 0;
#endif
    }

    // compare two multitypes. this function first converts the values into the
    // same logical type before making the comparison.
    bool equal(const multitype& e) const
    {
        if (is_type(e.type()))
            return strict_equal(e);
        if (is_type("str") || e.is_type("str"))
            return strcmp(to_str(), e.to_str()) == 0;
        if (is_type("double") || e.is_type("double"))
            return fabs(to_double() - e.to_double()) < REAL_EPS;
        return to_long() == e.to_long();
    }

    // same as equal but this function does not convert any value. thus, if the
    // values differ in type, it returns FALSE.
    bool strict_equal(const multitype& e) const
    {
        if (!is_type(e.type()))
            return false;
        if (is_type("int"))
            return values._tint == e.to_int();
        if (is_type("long"))
            return values._tlong == e.to_long();
        if (is_type("char"))
            return values._tchar == e.to_char();
        if (is_type("str"))
            return strcmp(values._tpchar, e.to_str()) == 0;
        if (is_type("double"))
            return fabs(values._tdouble - e.to_double()) < REAL_EPS;
        return false;
    }
};

class dirHandler
{
private:
    std::string DIR;
    std::vector<std::string> _files;
    bool isDir;
    size_t _size; // in KB (not showing correct if it is a directory)

    void here()
    {
        char buffer[MAX_PATH];
        GetModuleFileName(NULL, buffer, MAX_PATH);
        DIR = buffer;
        back();
    }

    bool prepare()
    {
        while (DIR.back() == '\\')
            DIR.pop_back();
        _files.clear();
        _size = -1;

        DWORD dwAttrs = GetFileAttributes(DIR.c_str());
        if (dwAttrs == INVALID_FILE_ATTRIBUTES)
            throw std::runtime_error(std::string("'") + DIR + "': Invalid directory");
        if (!(dwAttrs & FILE_ATTRIBUTE_DIRECTORY))
        {
            std::ifstream ifs(DIR);
            ifs.seekg(0, std::ios_base::end);
            _size = ifs.tellg() / 1024;
            return isDir = false;
        }

        WIN32_FIND_DATA ffd;
        HANDLE hFind = FindFirstFile((DIR + "\\*").c_str(), &ffd);
        do
        {
            if ((std::string)ffd.cFileName == "." || (std::string)ffd.cFileName == "..")
                continue;

            std::string filename = ffd.cFileName;
            bool flag = true;
            for (char x : filename)
                if (x < 32 || x > 126)
                {
                    flag = false;
                    break;
                }
            if (flag)
                _files.push_back(filename);
        } while (FindNextFile(hFind, &ffd));

        if (GetLastError() != ERROR_NO_MORE_FILES)
            throw std::runtime_error(std::string("'") + DIR + "': An error occurred when trying to read the directory");
        FindClose(hFind);

        return isDir = true;
    }

public:
    dirHandler()
    {
        here();
        prepare();
    }

    dirHandler(const std::string& directory)
    {
        char full[MAX_PATH];
        if (!GetFullPathName(directory.c_str(), MAX_PATH, full, nullptr))
            throw std::runtime_error(std::string("'") + directory + "': Invalid directory");
        DIR = full;
        prepare();
    }

    std::string dir() const
    {
        return DIR;
    }

    std::string fileName() const
    {
        std::string fullDir = dir(), res;
        for (size_t j = fullDir.length() - 1; j >= 0; --j)
            if (fullDir[j] == '\\' || fullDir[j] == '/')
                break;
            else
                res += fullDir[j];
        std::reverse(res.begin(), res.end());
        return res;
    }

    std::string fileExt() const
    {
        std::istringstream iss(fileName());
        std::string last;
        while (iss >> last);

        std::string ext;
        bool flag = false;
        while (!last.empty())
        {
            if (last.back() == '.')
            {
                flag = true;
                break;
            }
            ext.push_back(last.back());
            last.pop_back();
        }

        if (!flag)
            return "";
        std::reverse(ext.begin(), ext.end());
        return ext;
    }

    std::vector<std::string> files() const
    {
        return _files;
    }

    bool isDirectory() const
    {
        return isDir;
    }

    size_t size() const
    {
        return _size;
    }

    // Set fillWhiteSpace=true will replace invalid characters with white spaces
    // (so as to keep the position of the words unchanged)
    std::string readAll(bool fillWhiteSpace = true) const
    {
        if (isDir)
            return "";

        std::string result;
        std::ifstream ifs(DIR.c_str(), std::ios::binary);
        std::stringstream ss;
        int encoding = ENCODING_ASCII;

        if (!ifs.is_open())
        {
            result.clear();
            return result;
        }
        else
            if (ifs.eof())
                result.clear();
            else {
                int ch1 = ifs.get();
                int ch2 = ifs.get();
                if (ch1 == 0xff && ch2 == 0xfe)
                    // The file contains UTF-16LE BOM
                    encoding = ENCODING_UTF16LE;
                else
                    if (ch1 == 0xfe && ch2 == 0xff)
                        // The file contains UTF-16BE BOM
                        encoding = ENCODING_UTF16BE;
                    else
                    {
                        int ch3 = ifs.get();
                        if (ch1 == 0xef && ch2 == 0xbb && ch3 == 0xbf)
                            // The file contains UTF-8 BOM
                            encoding = ENCODING_UTF8;
                        else
                        {
                            // The file does not have BOM
                            encoding = ENCODING_ASCII;
                            ifs.seekg(0);
                        }
                    }
            }
        ss << ifs.rdbuf() << '\0';

        if (encoding == ENCODING_UTF16LE || encoding == ENCODING_UTF16BE)
        {
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> utfconv;
            result = utfconv.to_bytes(std::wstring((wchar_t*)ss.str().c_str()));
        }
        else
            result = ss.str();

        if (fillWhiteSpace)
            std::replace_if(result.begin(), result.end(), [](char x) { return (x < 32 || x > 126) && x != '\n' && x != '\r'; }, ' ');
        else
            result.erase(std::remove_if(result.begin(), result.end(), [](char x) { return (x < 32 || x > 126) && x != '\n' && x != '\r'; }), result.end());
        return result;
    }

    dirHandler& back()
    {
        std::string tmp = DIR;

        bool flag = false;
        while (!DIR.empty() && !(flag && DIR.back() == '\\'))
        {
            flag = true;
            DIR.pop_back();
        }

        if (DIR.empty())
            DIR = tmp;
        else
            prepare();

        return *this;
    }

    dirHandler& next(const std::string& filename)
    {
        if (!isDir)
            return *this;

        bool found = false;
        for (std::string file : _files)
        {
            bool ok = file.length() == filename.length();
            for (size_t i = 0; ok && i < file.length(); ++i)
                if (toupper(file[i]) != toupper(filename[i]))
                    ok = false;
            if (ok)
            {
                found = true;
                break;
            }
        }

        if (found)
        {
            DIR = DIR + '\\' + filename;
            prepare();
        }

        return *this;
    }
};

#endif // SE_UTILITY_H
