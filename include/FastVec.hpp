#ifndef LIMIT_HPP
#define LIMIT_HPP

#include <boost/unordered/unordered_flat_map.hpp>
#include <vector>

using id = int;
using index = unsigned int;

template <class T>
class FastVec {
  private:
    std::vector<T> orders;                                     // Linked List of orders
    boost::unordered::unordered_flat_map<id, index> ordersMap; // Random access to orders to remove in O1, key is order ID.

  public:
    FastVec(unsigned int init_capacity);
    void push_back(T item);
    void erase(id itemId);
    bool contains(id itemId);
    T get(id itemId);
};

#endif