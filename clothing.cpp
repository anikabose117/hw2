#include <sstream>
#include <iomanip>
#include "clothing.h"

using namespace std;

Clothing::Clothing(const std::string name, double price, int qty, const std::string size, const std::string brand): 
    Product("clothing", name, price, qty),
    size_(size),
    brand_(brand)
{
}


Clothing::~Clothing()
{
}

/**
    * Returns the appropriate keywords that clothing should be associated with
*/
std::set<std::string> Clothing::keywords() const{
        // splits up relevant phrases using the parseStringToWords() logic, adds to words set
    std::set<std::string> words = parseStringToWords(name_);
    std::set<std::string> moreWords = parseStringToWords(brand_);
    words = setUnion(words, moreWords);
    return words;
}

/**
    * Returns a string to display the product info for hits of the search
*/
std::string Clothing::displayString() const {
    std::stringstream temp;
    // work around to ensure price_ is printed with two decimal points
    temp << std::fixed << std::setprecision(2) << price_;
    return name_ + "\n" + "Size: " + size_ + " Brand: " + brand_ + "\n" + temp.str() + "   " + std::to_string(qty_) + " left.";
}


/**
 * default implementation...can be overriden in a future
 * assignment
 */
bool Clothing::isMatch(std::vector<std::string>& searchTerms) const
{
    return false;
}


void Clothing::dump(std::ostream& os) const
{
    // dump to ostream, following format given in assignment
    os << category_ << "\n" << name_ << "\n" << std::fixed << std::setprecision(2) << price_ << "\n" << qty_ << "\n" << size_ << "\n" << brand_ << endl;
}
