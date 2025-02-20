#ifndef CLOTHING_H
#define CLOTHING_H
#include "product.h"
#include "util.h"
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <algorithm>

class Clothing : public Product {
public:
    Clothing(const std::string name, double price, int qty, const std::string size, const std::string brand);
    ~Clothing();

    /**
     * Returns the appropriate keywords that clothing should be associated with
     */
    std::set<std::string> keywords() const;

    /**
     * Allows for a more detailed search beyond simple keywords -- might be used in future assignments
     */
    bool isMatch(std::vector<std::string>& searchTerms) const;

    /**
     * Returns a string to display the product info for hits of the search
     */
    std::string displayString() const;

    /**
     * Outputs the product info in the database format
     */
    void dump(std::ostream& os) const;


private:
    std::string size_;
    std::string brand_;
};
#endif
