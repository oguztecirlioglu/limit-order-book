#ifndef FASTVEC_HPP
#define FASTVEC_HPP

#include <boost/unordered/unordered_flat_map.hpp>
#include <sparsehash/dense_hash_map>
#include <vector>

using id = int;
using fast_vec_index = unsigned int;

template <class T>
class FastVec {
  private:
    std::vector<T> orders;                                // Linked List of orders
    google::dense_hash_map<id, fast_vec_index> ordersMap; // Random access to orders to remove in O1, key is order ID

  public:
    FastVec(unsigned int init_capacity);
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

#include "FastVec.hpp"

template <class T>
FastVec<T>::FastVec(unsigned int init_capacity) {
    this->orders.reserve(init_capacity);
    this->ordersMap.set_empty_key(-1);
    this->ordersMap.set_deleted_key(-1);
}

template <class T>
void FastVec<T>::push_back(T item) {
    this->orders.push_back(item);
    this->ordersMap[item->getId()] = this->orders.size() - 1;
}

template <class T>
void FastVec<T>::erase(id itemId) {
    auto it = this->ordersMap.find(itemId);
    if (it == this->ordersMap.end()) {
        throw std::invalid_argument("Remove - id does not exist in hashmap");
    }

    fast_vec_index i = it->second;
    T elem = std::move(this->orders[i]);

    fast_vec_index lastIdx = this->orders.size() - 1;
    if (i != lastIdx) {
        this->orders[i] = std::move(this->orders[lastIdx]);
        this->ordersMap[this->orders[i]->getId()] = i;
    }

    this->orders.pop_back();
    this->ordersMap.erase(it);

    delete elem;
}

template <class T>
bool FastVec<T>::contains(id itemId) {
    return this->ordersMap.find(itemId) != this->ordersMap.end();
}

template <class T>
T FastVec<T>::get(id itemId) {
    auto it = this->ordersMap.find(itemId); // Single lookup
    if (it == this->ordersMap.end()) {
        throw std::invalid_argument("Get - id does not exist in hashmap");
    }
    return this->orders[it->second]; // Use iterator result for direct access
}

template <class T>
void FastVec<T>::clear() {
    this->orders.clear();
    this->ordersMap.clear();
}
#endif