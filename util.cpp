#include <iostream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include "util.h"

using namespace std;
std::string convToLower(std::string src)
{
    std::transform(src.begin(), src.end(), src.begin(), ::tolower);
    return src;
}

/** Complete the code to convert a string containing a rawWord
    to a set of words based on the criteria given in the assignment **/
std::set<std::string> parseStringToWords(string rawWords)
{
    std::set<std::string> words;
    // get rid of whitespace on the ends of the string
    rawWords = trim(rawWords);
    std::string tempWord;
    int indexStart = 0;
    int indexEnd = 0;
    // parses through string until we reach the end of it
    while (indexEnd < rawWords.length()){
        // checks each character of string to see if punctuation OR a space,
        if (ispunct(rawWords[indexEnd]) || rawWords[indexEnd] == ' '){
            // substring of current stard index up until the current end index to get characters up to most recent space/ punct
            std::string subStrTemp = rawWords.substr(indexStart, indexEnd - indexStart);
            // trim off excess spaces on the side
            subStrTemp = trim(subStrTemp);
            // only considers word as valid if it is at least length 2
            if (subStrTemp.length() >= 2){
                // converts to lower to it's all the same, add to keywords set
                words.insert(convToLower(subStrTemp));
            }
            // adjust indexes as necessary to move to the next part of the string
            indexStart = indexEnd + 1;
        } 
        indexEnd++;
    }
    // sanity check, in case there happens to be anything at the end not parsed through by the above loop, adds to set if relevant
    // shouldn't be called
    std::string subStrTempEnd = rawWords.substr(indexStart);
    subStrTempEnd = trim(subStrTempEnd);
    if (subStrTempEnd.length() >= 2){
        words.insert(convToLower(subStrTempEnd));
    }
    return words;
}

/**************************************************
 * COMPLETED - You may use the following functions
 **************************************************/

// Used from http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
// trim from start
std::string &ltrim(std::string &s) {
    s.erase(s.begin(), 
	    std::find_if(s.begin(), 
			 s.end(), 
			 std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
std::string &rtrim(std::string &s) {
    s.erase(
	    std::find_if(s.rbegin(), 
			 s.rend(), 
			 std::not1(std::ptr_fun<int, int>(std::isspace))).base(), 
	    s.end());
    return s;
}

// trim from both ends
std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}
