#ifndef LINKEDHMAP_HPP
#define LINKEDHMAP_HPP

#include "Order.hpp"
#include <boost/unordered/unordered_flat_map.hpp>
#include <list>

using id = int;
using fast_vec_index = unsigned int;

template <class T>
class LinkedHashMap {
  private:
    std::list<T> orders;                                                                      // Linked List of orders
    boost::unordered::unordered_flat_map<OrderId, typename std::list<T>::iterator> ordersMap; // Random access to orders to remove in O1, key is order ID.

  public:
    LinkedHashMap(unsigned int init_capacity);
    void push_back(T item);
    void erase(id itemId);
    bool contains(id itemId);
    T get(id itemId);
    void clear();

    typename std::vector<T>::iterator begin() { return orders.begin(); }
    typename std::vector<T>::iterator end() { return orders.end(); }
    typename std::vector<T>::const_iterator begin() const { return orders.begin(); }
    typename std::vector<T>::const_iterator end() const { return orders.end(); }
};

template <class T>
LinkedHashMap<T>::LinkedHashMap(unsigned int init_capacity) {
}

template <class T>
void LinkedHashMap<T>::push_back(T item) {
    this->orders.push_back(item);
    this->ordersMap[item->getId()] = --this->orders.end();
}

template <class T>
void LinkedHashMap<T>::erase(id itemId) {
    auto entry = this->ordersMap.find(itemId);
    if (entry == this->ordersMap.end()) {
        throw std::invalid_argument("Remove - id does not exist in hashmap");
    }

    std::list<Order *>::iterator it = entry->second;

    this->orders.erase(it);
    this->ordersMap.erase(itemId);

    delete *it;
}

template <class T>
bool LinkedHashMap<T>::contains(id itemId) {
    return this->ordersMap.contains(itemId);
}

template <class T>
T LinkedHashMap<T>::get(id itemId) {
    auto it = this->ordersMap.find(itemId);
    if (it == this->ordersMap.end()) {
        throw std::invalid_argument("Get - id does not exist in hashmap");
    }
    return *it->second;
}

template <class T>
void LinkedHashMap<T>::clear() {
    this->orders.clear();
    this->ordersMap.clear();
}
#endif