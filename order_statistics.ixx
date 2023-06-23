/// @file
/// A generic, in situ implementation of order statistics trees.
///
/// This implementation builds on min-max trees as well as std::nth_element to
/// realize a flat data structure called order statistics trees that allows
/// quick access to its elements based on the rank of the element. This way
/// queries important for statistics like getting the median or certain
/// percentiles / quantiles can be performed very efficiently.

/// @cond
module;
/// @endcond

// C++ Standard Library.
#include <algorithm>
#include <iterator>

/// @cond
export module order_statistics;

export import :minmax_heaps;
/// @endcond

/// @brief Order statistics operations.
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
    std::nth_element(prev_nth, *rank, last, comp);
    make_mm_heap(prev_nth, *rank);
    prev_nth = *rank;
  }
  make_mm_heap(prev_nth, last);
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
                               RandomIt2 ranks_last)
{
}

template<typename RandomIt>
void push_order_statistics_tree(
  typename std::iterator_traits<RandomIt>::value_type first,
  typename std::iterator_traits<RandomIt>::value_type last,
  RandomIt                                            ranks_first,
  RandomIt                                            ranks_last)
{
  if (first != last) {
    for (auto rank{ranks_first}; rank != ranks_last; ++rank) {
      if (std::next(rank) != ranks_last) {
        if (**rank <= *(last - 1) && *(last - 1) < **std::next(rank)) {
        }
      }
      else {
        push_mm_heap(*rank, last);
      }
    }
  }
}

}
