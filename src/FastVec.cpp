#include "FastVec.hpp"

template <class T>
FastVec<T>::FastVec(unsigned int init_capacity) {
    this->orders.reserve(init_capacity);
}

template <class T>
void FastVec<T>::push_back(T item) {
    this->orders.push_back(item);
    this->ordersMap[item->getId()] = m_insertionIndex;
}

template <class T>
void FastVec<T>::erase(id itemId) {
    if (not this->ordersMap.contains(itemId)) {
        throw std::invalid_argument("Remove - id does not exist in hashmap");
    }

    index i = this->ordersMap[itemId];
    T elem = this->orders[i];

    if (i != this->orders.size() - 1) {
        this->orders[i] = std::move(this->orders.back());
        this->ordersMap[this->orders[i]->getId()] = i;
    }

    this->ordersMap.erase(itemId);
    this->orders.pop_back();
    delete elem;
}

template <class T>
bool FastVec<T>::contains(id itemId) {
    return this->ordersMap.contains(itemId);
}

template <class T>
T FastVec<T>::get(id itemId) {
    if (not this->ordersMap.contains(itemId)) {
        throw std::invalid_argument("Remove - id does not exist in hashmap");
    }
    return ordersMap[itemId];
}