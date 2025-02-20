#include <sstream>
#include <iomanip>
#include "movie.h"

using namespace std;

Movie::Movie(const std::string name, double price, int qty, const std::string genre, const std::string rating): 
    Product("movie", name, price, qty),
    genre_(genre),
    rating_(rating)
{
}


Movie::~Movie()
{
}

/**
    * Returns the appropriate keywords that movie should be associated with
*/
std::set<std::string> Movie::keywords() const{
    std::set<std::string> words;
    words.insert(convToLower(genre_));
    // splits up relevant phrases using the parseStringToWords() logic
    std::set<std::string> moreWords = parseStringToWords(name_);
    words = setUnion(words, moreWords);
    // adds to words set before returning
    return words;
}

/**
    * Returns a string to display the product info for hits of the search
*/
std::string Movie::displayString() const {
    std::stringstream temp;
    // work around to ensure price_ is printed with two decimal points
    temp << std::fixed << std::setprecision(2) << price_;
    return name_ + "\n" + "Genre: " + genre_ + " Rating: " + rating_ + "\n" + temp.str() + "   " + std::to_string(qty_) + " left.";
}


/**
 * default implementation...can be overriden in a future
 * assignment
 */
bool Movie::isMatch(std::vector<std::string>& searchTerms) const
{
    return false;
}


void Movie::dump(std::ostream& os) const
{
    // dump to ostream, following format given in assignment
    os << category_ << "\n" << name_ << "\n" << std::fixed << std::setprecision(2) << price_ << "\n" << qty_ << "\n" << genre_ << "\n" << rating_ << endl;
}
