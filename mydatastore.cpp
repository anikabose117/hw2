#include <sstream>
#include <iomanip>
#include "mydatastore.h"
#include "util.h"

using namespace std;


MyDataStore::MyDataStore() { }

MyDataStore::~MyDataStore() {
    // deconstructor
    // iterates through every set/ map with dyanmically allocated objects and deletes them
    for(std::set<User*>::iterator itUser = users_.begin(); itUser != users_.end(); ++itUser){
        delete *itUser;
    }
    // ensure set is clear
    users_.clear();

    for(std::set<Product*>::iterator itProd = products_.begin(); itProd != products_.end(); ++itProd){
        delete *itProd;
    }
    products_.clear();

    for (std::map<std::string, std::queue<Product*>>::iterator itCart = carts_.begin(); itCart != carts_.end(); ++itCart){
        std::queue<Product*>& cartQueue = (*itCart).second;  
        while (!cartQueue.empty()){
            cartQueue.pop();
        }     
    }
    carts_.clear(); 
}

/**
* Adds a product to the data store
*/
void MyDataStore::addProduct(Product* p){
    // ensures Product is unique w/ insert
    products_.insert(p);
}

/**
* Adds a user to the data store
*/
void MyDataStore::addUser(User* u){
    // ensures Users are unique
    users_.insert(u);
}

/**
* Performs a search of products whose keywords match the given "terms"
*  type 0 = AND search (intersection of results for each term) while
*  type 1 = OR search (union of results for each term)
*/
std::vector<Product*> MyDataStore::search(std::vector<std::string>& terms, int type){
    // create relevant temp containers
    std::vector<Product*> searchedProducts;
    std::set<std::string> possibleWords;
    std::set<std::string> termsSet;
    vector<std::string>::iterator it;
    // convert terms vector into a set so we can do the setIntersection and setUnions as needed
    for(it = terms.begin() ; it != terms.end(); ++it){
        termsSet.insert(*it);
    }

    if (type == 0){
        // iterate through products_ data member
        for(set<Product*>::iterator it0 = products_.begin(); it0 != products_.end(); ++it0){
            // get keywords for current iterator-to-Product
            std::set<std::string> keyWords = (*it0) -> keywords();
            possibleWords = setIntersection(keyWords, termsSet);
            // checks if keyWords can be found in termsSet
            // alternatively keyWords = termsSet?
            if (possibleWords == termsSet){
                // add to vector if meets search criteria
                searchedProducts.push_back(*it0);
            }
        }      
    } 
    else if (type == 1){
        for(set<Product*>::iterator it1 = products_.begin(); it1 != products_.end(); ++it1){
            std::set<std::string> keyWords = (*it1) -> keywords();
            possibleWords = setIntersection(keyWords, termsSet);
            // if possibleWords > 0, that means at least one of the searched terms can be found in the product's keywords
            if (possibleWords.size() > 0){
                // addd to vector if meets search criteria
                searchedProducts.push_back(*it1);
            }
        } 
    } 
    else {
        cout << "Needs to choose type 0 or 1" << endl;     
    }
    return searchedProducts;   
}


/**
* Reproduce the database file from the current Products and User values
*/
void MyDataStore::dump(std::ostream& ofile){
    // following format given in the assignment/ test output files
    ofile << "<products>" << endl;
    // iterate through relevant sets and call dump on current object
    for(set<Product*>::iterator it1 = products_.begin(); it1 != products_.end(); ++it1){
        (*it1) -> dump(ofile);
    }
    ofile << "</products>" << endl;
    ofile << "<users>" << endl;
    for(set<User*>::iterator it2 = users_.begin(); it2 != users_.end(); ++it2){
        (*it2) -> dump(ofile);
    }
    ofile << "</users>" << endl;
}


/**
* Get a set of usernames
*/
std::set<std::string> MyDataStore::getUsers(){
    // since didn't use a map, way to access private usernames from users set
    std::set<std::string> userNames;
    for(set<User*>::iterator it = users_.begin(); it != users_.end(); ++it){
        userNames.insert((*it) -> getName());
    }
    return userNames;
}

/**
* Add to user's cart
*/
void MyDataStore::addCart(std::string user, Product* product){
    // if user doesn't currently have a cart, add new username, cart queue pair to map
    if (carts_.find(user) == carts_.end()){
        std::queue<Product*> tempQueue;
        tempQueue.push(product);
        carts_.insert(make_pair(user, tempQueue));
    } else {
        // if already has a cart (can be found in map), then just add new product to the back
        carts_[user].push(product);
    }
}

/**
* Print user's cart to terminal
*/
void MyDataStore::viewCart(std::string user){
    std::map<std::string, std::queue<Product*>>::iterator it = carts_.find(user);
    if (it != carts_.end()){
        std::queue<Product*> cartProducts = it -> second;
        int i = 1;
        while (!cartProducts.empty()) {
            Product* currProduct = cartProducts.front();  
            std::cout << "Item " << i << endl; 
            cout << currProduct -> displayString() << "\n" << endl;
            cartProducts.pop(); 
            i++;
        }
    }
}


/**
* Buy user's cart
*/
void MyDataStore::buyCart(std::string user){
    std::map<std::string, std::queue<Product*>>::iterator it = carts_.find(user);
    // checks if user has a cart (username is within carts_ as a key)
    if (it != carts_.end()){
        // reference, so affects actual cart queue
        std::queue<Product*>& cartProducts = it -> second;
        std::set<User*>::iterator currUserPtr;

        // connect users_ to carts_ so we can adjust user data members accordingly
        for(std::set<User*>::iterator it1 = users_.begin(); it1 != users_.end(); ++it1){
            if ((*it1) -> getName() == user){
                currUserPtr = it1;
                break; // get out of loop since found User
            }    
        }

        // can only buy if cart is not empty
        if (!cartProducts.empty()){
            std::queue<Product*> tempCart;
            while (!cartProducts.empty()){
                // get balance of user, price + quantity of current product to determine if you can buy
                double currBalance = (*currUserPtr) -> getBalance();
                Product* currProduct = cartProducts.front();
                double currProductPrice = currProduct -> getPrice();
                if ((currBalance - currProductPrice >= 0) && ((currProduct -> getQty()) > 0)){
                    // valid buy, user balance and product quantity decreases as necessary
                    (*currUserPtr) -> deductAmount(currProductPrice);  
                    currProduct -> subtractQty(1);           
                } else {
                    // temp cart stores products that we cannot currently buy, but may be able to buy later
                    // i.e. product is skipped over, but we don't want to lose track of it
                    Product* temp = cartProducts.front();
                    tempCart.push(temp);    
                }
                // pops product from current cart, no matter if we bought it or not
                cartProducts.pop();
            }
            // replace empty cartProducts queue with tempCart queue, which--if not empty--contains the products we skipped over buying
            std::swap(cartProducts,tempCart);
        }     
    }       
}
