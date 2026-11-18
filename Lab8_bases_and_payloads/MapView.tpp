template <typename T>
typename MapView<T>::const_iterator::reference MapView<T>::const_iterator::operator *() const {
    return *(m_map_iterator->second);
}

template <typename T>
typename MapView<T>::const_iterator::pointer MapView<T>::const_iterator::operator ->() const {
    return m_map_iterator->second;
}