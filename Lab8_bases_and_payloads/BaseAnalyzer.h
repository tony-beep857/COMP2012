#ifndef BASE_ANALYZER_H
#define BASE_ANALYZER_H

#include <string>
#include <map>
#include <utility>
using std::string;
using std::map;
using std::pair;

class Base;
class Payload;
class BaseAnalyzer {
    public:

    using ItemMap = map<string, int>;
    using ItemPair = ItemMap::value_type;
    
    /**
     * @brief Sums up the number of items whose names are inclusively between a lower bound and an upper bound.
     * 
     * @param b Base to analyze.
     * 
     * @param lower_bound Lower bound of the item name.
     * 
     * @param upper_bound Upper bound of the item name.
     * 
     * @returns Total number of items whose names are within the bound.
     */
    static int range_sum(const Base &b, const string &lower_bound, const string &upper_bound);

    // static int substring_sum(const Base &b, const string &substring);

    /**
     * @brief Identifies the most frequent item in a base.
     * 
     * Chooses the lexiologically smallest item in case of ties.
     * 
     * @param b Base to analyze.
     * 
     * @returns Pair with string set to "EMPTY" (int value is ungraded) if the base is empty. Otherwise, the name and quantity of the most frequent item.
     */
    static ItemPair max_item(const Base &b);
};

#endif