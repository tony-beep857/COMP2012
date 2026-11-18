#include "Base.h"
#include "Payload.h"
#include <stdexcept>
using std::runtime_error;

void Base::add_item(ItemPair arg) {
    if (arg.second > 0)
        m_items[arg.first] += arg.second;
}

Base &Base::operator +=(const Payload &payload) {
    for (const ItemPair &pair: payload.get_items())
        add_item(pair); // it is okay to do this because map[nonexistent_key] will make the map create a new key value pair with value default-initialized
    return *this;
}

Base &Base::operator -=(const Payload &payload) {
    for (const ItemPair &pair: payload.get_items()) {
        m_items[pair.first] -= pair.second;
        if (m_items[pair.first] == 0)
            m_items.erase(pair.first);
        else if (m_items[pair.first] < 0) {
            throw runtime_error("Base " + m_id + " could not afford the content of payload " + payload.get_id() + ".");
        }
    }
    return *this;
}

int Base::operator [](const string &s) const {
    ItemMap::const_iterator iterator = m_items.find(s);
    return iterator != m_items.end()? iterator->second: 0;
}