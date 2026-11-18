#include "Base.h"
#include "BaseAnalyzer.h"

using ItemMap = Base::ItemMap;
using ItemPair = Base::ItemPair;

int BaseAnalyzer::range_sum(const Base &b, const string &lower_bound, const string &upper_bound) {
    const ItemMap &items = b.get_items();
    ItemMap::const_iterator start = items.lower_bound(lower_bound);
    ItemMap::const_iterator end = items.upper_bound(upper_bound);

    int sum = 0;
    for(ItemMap::const_iterator it = start; it != end; ++it){
        sum += it->second;
    }
    return sum;
}

ItemPair BaseAnalyzer::max_item(const Base &b) {
    const ItemMap &items = b.get_items();
    if (items.empty()){
        return ItemPair("EMPTY", 0);
    }

    ItemMap::const_iterator max_it = items.begin();
    for (ItemMap::const_iterator it = items.begin(); it != items.end(); ++it){
        if (max_it->second < it->second)
            max_it = it;
    }

    return *max_it;
}