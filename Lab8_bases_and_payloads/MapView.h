#ifndef MAP_VIEW_H
#define MAP_VIEW_H

#include <map>
#include <cstddef>
#include <iterator>
#include <utility>
#include <string>

using std::string;
using std::map;
using std::iterator;
using std::bidirectional_iterator_tag;
using std::ptrdiff_t;

/**
 * @brief View of any map<string, T *> as map<string, const T &>.
 */
template <typename T>
class MapView {
    public:
    using MapType = map<string, T *>;
    private:
    const MapType &m_map;

    public:
    /**
     * @brief Initializes the wrapper class.
     * 
     * @param m Map to view.
     */
    MapView(const MapType &m): m_map(m) {}

    /**
     * @brief An iterator that is just the wrapper of the map's iterator.
     * It mirrors the movement of the map's iterator, but dereferences to
     * const T & instead of pair<string, T *>.
     */
    class const_iterator {
        public:
        using iterator_category = bidirectional_iterator_tag;
        using value_type = const T;
        using difference_type = ptrdiff_t;
        using pointer = const T *;
        using reference = const T &;
        private:
        /**
         * @brief The underlying map iterator that we are mirroring.
         */
        typename MapType::const_iterator m_map_iterator;

        public:
        const_iterator() = default;
        /**
         * @brief Initializes the iterator with the map's one.
         * 
         * @param map_iterator The map's iterator.
         */
        explicit const_iterator(typename MapType::const_iterator map_iterator): m_map_iterator{map_iterator} {} 

        /**
         * @brief Moves the iterator to the next element.
         */
        const_iterator &operator ++() {
            ++m_map_iterator;
            return *this;
        }
        /**
         * @brief Moves the iterator to the next element.
         */
        const_iterator operator ++(int) {
            const_iterator prev{*this};
            ++(*this);
            return prev;
        }
        /**
         * @brief Moves the iterator to the previous element.
         */
        const_iterator &operator --() {
            --m_map_iterator;
            return *this;
        }
        /**
         * @brief Moves the iterator to the previous element.
         */
        const_iterator operator --(int) {
            const_iterator prev{*this};
            --(*this);
            return prev;
        }
        bool operator ==(const_iterator other) const { return m_map_iterator == other.m_map_iterator; }
        bool operator !=(const_iterator other) const { return !((*this) == other); }

        /**
         * @return Const reference to the object the iterator currently points at.
         */
        reference operator *() const;
        /**
         * @return Pointer to the object the iterator currently points at.
         */
        pointer operator ->() const;
    };

    /**
     * @return The iterator that points at the first element of the map.
     */
    const_iterator begin() const { return const_iterator{m_map.begin()}; }
    /**
     * @return The iterator that points at the "element after the last element" of the map.
     */
    const_iterator end() const { return const_iterator{m_map.end()}; }

    /**
     * @brief Tries to find an element in the map.
     * 
     * @param key Key of the element.
     * 
     * @return end() if the element is not found.
     * Otherwise, the iterator of the element found.
     */
    const_iterator find(const string &key) const { return const_iterator{m_map.find(key)}; }

    /**
     * @brief Accesses the element with the given key.
     * 
     * @param key Key of the element.
     * 
     * @return Reference to the element to access.
     * 
     * @throw std::out_of_range Thrown if the element does not exist.
     */
    const T &at(const string &key) const { return *m_map.at(key); }
};

#include "MapView.tpp"

#endif