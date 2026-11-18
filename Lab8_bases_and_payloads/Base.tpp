template <class Container>
Base::Base(const string &id, const Container &initial_items): m_id{id} {
    for (ItemPair p: initial_items)
        add_item(p);
}
template <class Container>
bool Base::can_afford(const Container &items) const {
    for (const ItemPair &pair: items) {
        ItemMap::const_iterator iterator = m_items.find(pair.first); // has to be done like this because operator [] has no const overload in map
        if (iterator == m_items.end())
            return false; // an item in payload is not in m_items 
        if (iterator->second < pair.second)
            return false; // item count not sufficient
    }
    return true;
}