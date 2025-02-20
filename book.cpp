#include <sstream>
#include <iomanip>
#include "book.h"

using namespace std;

Book::Book(const std::string name, double price, int qty, const std::string isbn, const std::string author): 
    Product("book", name, price, qty),
    isbn_(isbn),
    author_(author)
{
}


Book::~Book()
{
}

/**
    * Returns the appropriate keywords that book should be associated with
*/
std::set<std::string> Book::keywords() const{
    std::set<std::string> words;
    words.insert(isbn_);
    // splits up relevant phrases using the parseStringToWords() logic
    std::set<std::string> moreWords = parseStringToWords(author_);
    words = setUnion(words, moreWords);
    moreWords = parseStringToWords(name_);
    // adds words to words set
    words = setUnion(words, moreWords);
    return words;
}

/**
    * Returns a string to display the product info for hits of the search
*/
std::string Book::displayString() const {
    std::stringstream temp;
    // work around to ensure price_ is printed with two decimal points
    temp << std::fixed << std::setprecision(2) << price_;
    return name_ + "\n" + "Author: " + author_ + " ISBN: " + isbn_ + "\n" + temp.str() + "   " + std::to_string(qty_) + " left.";
}


/**
 * default implementation...can be overriden in a future
 * assignment
 */
bool Book::isMatch(std::vector<std::string>& searchTerms) const
{
    return false;
}


void Book::dump(std::ostream& os) const
{
    // dump to ostream, following format given in assignment
    os << category_ << "\n" << name_ << "\n" << std::fixed << std::setprecision(2) << price_ << "\n" << qty_ << "\n" << isbn_ << "\n" << author_ << endl;
}
