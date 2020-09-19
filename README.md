# Mini Search Engine
This is our project of building a small document-search engine *entirely* in C++, which is a part of the CS163 university course. We proudly present a **user-friendly console application** that can handle a collection of text files and provide users with a variety of search operations with high precision.

> Click [here](Requirements.pdf) for full list of project requirements.

## Build
The program is built using TDM-GCC Compiler, GCC 5.1.0, MinGW Windows 32-bit edition. All codes are written in C++11.

## Data preprocessing
In order to make searching easily and efficient, we first build a collection out of given raw data.
- Filenames are indexed using numbers.
- The content of the files are split into words (tokens), then the tokens will be normalized before being inserted into a [trie](https://en.wikipedia.org/wiki/Trie). Each token contains the file index that it occurs, the line containing it, the position in the line where it resides, and a flag that determines whether it appears in the filename or not (yes, filenames can be searched too!).
- Thesaurus data is loaded into a [hash table](https://en.wikipedia.org/wiki/Hash_table) for synonym searching (Huge thanks to [zaibacu](https://github.com/zaibacu))
- Drink coffee :coffee:

## Searching

### Query preprocessing
The query string is splitted into terms. Each term can contain one or more words, and be one of the following types: include, exclude, range, synonym, and wild.

To get the occurrences of a term, we first convert it into token.
- If a term has one word, it is already a token
- If a term has two or more words,
	- The occurrences of all words are combined into one list
	- [Knuth–Morris–Pratt](https://en.wikipedia.org/wiki/Knuth%E2%80%93Morris%E2%80%93Pratt_algorithm) algorithm is used to reduce the list of occurrences of all words to the list of occurrences of the first word of each detected pattern.

Then, fetch the occurrences of each token from data.
- If include or exclude, occurrences of the token will be fetched
- If range, occurrences of all the numbers in that range will be fetched
- If synonym, occurrences of token and all of its equivalent ones will be fetched
- If wild, dogs and cats will be fed

Henceforth, operations are conducted based on the tokens' occurrence list.

### Operations
In the manner of listing, here is a complete table of supported operators:
| Operator     | Meaning                                                                                               | Example                                                                                            |
|--------------|-------------------------------------------------------------------------------------------------------|----------------------------------------------------------------------------------------------------|
| `Q1 & Q2`    | Get the intersection of results of two queries                                                        | `dogs & cats` will return the list of documents that contain both words                            |
| `Q1 \| Q2`   | Get the union of results of two queries                                                               | `dogs \| cats` will return the list of documents that contain either of the two words              |
| `FILETYPE:A` | Only perform search operations on files with type of `A`                                              | `FILETYPE:TXT` will reduce the scope of searching to `*.txt` files                                 |
| `INTITLE:A`  | Only perform search operations on files of which title contains `A`                                   | `INTITLE:CATS` will reduce the scope of searching to files that have `CATS` in the title           |
| `+A`         | The word must be included in the search results                                                       | `+dogs` will return the list of documents that contain *dogs*                                      |
| `-A`         | The word must be excluded from the search results                                                     | `-dogs` will return the list of documents that do **not** contain *dogs*                           |
| `~A`         | The word or its synonym must be included in the search results                                        | `~hello` will return the list of documents that contain *hello* or *hi*                            |
| `A..B`       | <small>**Pre-requisite:** A and B must be numbers</small><br/>Any number in the range must be included in the search results | `2019..2021` will return the list of documents that contain *2019* or *2020* or *2021*             |
| `"A B"`      | The phrase must be included in the search results                                                     | `"dogs and cats"` will return the list of document that contain exactly the phrase *dogs and cats* |
| `*`          | Can be any word or nothing                                                                            | You name it!                                                                                       |

### Rankings
- The occurrences of the tokens in files are encoded into lists of values `A_0, A_1, ..., A_k`
- The occurrences of the tokens in the query are encoded into another list of values `B`
- Apply [Ratcliff/Obershelp](https://en.wikipedia.org/wiki/Gestalt_Pattern_Matching) algorithm to calculate the similarity between each token list `A_i` and `B`
- The final score of a file <img src="https://latex.codecogs.com/gif.latex?d" title="d" /> per query <img src="https://latex.codecogs.com/gif.latex?q" title="q" /> is calculated as <img src="https://latex.codecogs.com/gif.latex?score(q,&space;d)&space;=&space;wordCnt(q,&space;d)&space;&plus;&space;Gestalt(q,&space;d)" title="score(q, d) = wordCnt(q, d) + Gestalt(q, d)" />

## To-do list
- Search history (planned, not yet executed)
- Target more filetypes (sadly, only `.txt` files are accepted at present)
- More and more...

## Credits
- Query processing & Searching: Quyen Dinh ([quyenjd](https://github.com/quyenjd)), An Pham ([iluvyw](https://github.com/iluvyw))
- Data processing & API: Hai Le ([pythagore1123](https://github.com/pythagore1123))
- Interface: Linh Nguyen ([linhlrx](https://github.com/linhlrx))

## Mentors
- Dr. Dinh Ba Tien
- M.S. Truong Phuoc Loc
- M.S. Ho Tuan Thanh
