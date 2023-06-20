module;

// C++ Standard Library.
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <functional>
#include <iterator>

// Guideline Support Library.
#include <gsl/gsl>

export module order_statistics;

export import :minmax_heaps;

export namespace order_statistics {

template<typename RandomIt, typename Compare>
void make_order_statistics_tree(
  typename std::iterator_traits<RandomIt>::value_type first,
  typename std::iterator_traits<RandomIt>::value_type last,
  RandomIt                                            ranks_first,
  RandomIt                                            ranks_last,
  Compare                                             comp)
{
  auto prev_nth{first};
  for (auto rank{ranks_first}; rank != ranks_last; ++rank) {
    std::nth_element(first, *rank, last, comp);
    make_mm_heap(prev_nth, *rank);
    prev_nth = *rank;
  }
}

template<typename RandomIt>
void make_order_statistics_tree(
  typename std::iterator_traits<RandomIt>::value_type first,
  typename std::iterator_traits<RandomIt>::value_type last,
  RandomIt                                            ranks_first,
  RandomIt                                            ranks_last)
{
  make_order_statistics_tree(first,
                             last,
                             ranks_first,
                             ranks_last,
                             std::less<>{});
}

template<typename RandomIt1, typename RandomIt2>
void pop_order_statistics_tree(RandomIt1 first,
                               RandomIt1 last,
                               RandomIt2 ranks_first,
                               RandomIt2 ranks_last);

template<typename RandomIt1, typename RandomIt2>
void push_order_statistics_tree(RandomIt1 first,
                                RandomIt1 last,
                                RandomIt2 ranks_first,
                                RandomIt2 ranks_last);

}
