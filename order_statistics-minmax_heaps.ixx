/// @file
/// A generic, in situ implementation of min-max heaps.
///
/// This implementation provides functionality similar to the Heap operations of
/// the algorithm library of C++. The heap (tree) is stored implicitly in a
/// contiguous sequence of memory. Consequently, for each function there is a
/// variant that accepts a functor to specify what smaller is.
///
/// A min-max heap is a binary tree with the following properties.
/// @arg The tree is complete. @arg It contains alternating min and max levels
/// (sometimes called even and odd levels). @arg The root node contains the
/// smallest element. @arg One of the child nodes of the root contains the
/// greatest element. @arg A node of a min level contains the smallest element
/// in its subtree and a node on a max level is greater than all its ancestors.

module;

// C++ Standard Library.
#include <cmath>
#include <functional>
#include <iterator>

export module order_statistics:minmax_heaps;

namespace order_statistics {

/// @brief Returns @c true if @p it is on a min (even) level of the min-max heap
/// starting at @p first.
template<typename RandomIt>
bool is_min_level(RandomIt first, RandomIt it)
{
  return static_cast<unsigned long long>(
           std::log2(std::distance(first, it) + 1))
           % 2
         == 0;
}

/// @brief Returns @c true if @p it still has children inside the heap [@p
/// first, @p last).
template<typename RandomIt>
bool has_children(RandomIt first, RandomIt it, RandomIt last)
{
  // return std::distance(first, it) + 1 < std::distance(it, last);
  return std::distance(first, it) * 2 + 1 < std::distance(first, last);
}

/// @brief Returns the iterator to the parent of @p it in the min-max heap
/// starting at @c first.
template<typename RandomIt>
RandomIt parent(RandomIt first, RandomIt it)
{
  // Expects(std::distance(first, it) > 0);
  auto p{first};
  std::advance(p, (std::distance(first, it) - 1) / 2);
  return p;
}

/// @brief Returns @c true if @p it is a grandchild of @p it2.
template<typename RandomIt>
bool is_grandchild(RandomIt first, RandomIt it, RandomIt it2)
{
  return parent(first, parent(first, it)) == it2;
}

/// @brief Checks if a predicate @pred holds for all child nodes of @p it in the
/// min-max heap [@c first, @c last).
template<typename RandomIt, typename Predicate>
bool for_all_children(RandomIt  first,
                      RandomIt  it,
                      RandomIt  last,
                      Predicate pred)
{
  const auto d0{std::distance(first, it)};
  const auto d1{std::distance(first, last)};
  if (d0 * 2 + 2 < d1) {
    auto left_child{first};
    std::advance(left_child, d0 * 2 + 1);
    auto right_child{left_child};
    std::advance(right_child, 1);
    return pred(*left_child) && pred(*right_child)
           && for_all_children(first, left_child, last, pred)
           && for_all_children(first, right_child, last, pred);
  }
  else if (d0 * 2 + 1 < d1) {
    auto left_child{first};
    std::advance(left_child, d0 * 2 + 1);
    return pred(*left_child) && for_all_children(first, left_child, last, pred);
  }
  else {
    return true;
  }
}

/// @brief Returns an iterator to the grandparent node of @p it in the min-max
/// heap starting at @p first.
template<typename RandomIt>
RandomIt grandparent(RandomIt first, RandomIt it)
{
  return parent(first, parent(first, it));
}

/// @brief Pushes an item up the tree defined by [@p first, @p last) maintaining
/// the min-max property using the caller-supplied ordering functor @c comp.
template<typename RandomIt, typename Compare>
void push_mm_heap_impl(RandomIt first, RandomIt last, Compare comp)
{
  using std::swap;

  if (std::distance(first, last) > 2) {
    auto it{last - 1};
    while (std::distance(first, it) > 2 && comp(*it, *grandparent(first, it))) {
      swap(*it, *grandparent(first, it));
      it = grandparent(first, it);
    }
  }
}

/// @brief Finds the smallest ancestor according to the caller-supplied ordering
/// functor @c comp among the child and grand-child nodes of the node @p it in
/// the min-max heap [@c first, @c last).
template<typename RandomIt, typename Compare>
RandomIt
smallest_ancestor(RandomIt first, RandomIt it, RandomIt last, Compare comp)
{
  // Expects(has_children(first, it, last));
  const std::iterator_traits<RandomIt>::difference_type children[6][2] =
    {{2, 1}, {2, 2}, {4, 3}, {4, 4}, {4, 5}, {4, 6}};
  const auto size{std::distance(first, last)};
  auto       smallest_it{first};
  auto       d{std::distance(first, it)};

  std::advance(smallest_it, d * children[0][0] + children[0][1]);

  for (auto i{1}; i < 6; ++i) {
    const auto offset{d * children[i][0] + children[i][1]};
    if (offset < size) {
      auto ancestor_it{first};
      std::advance(ancestor_it, offset);
      if (comp(*ancestor_it, *smallest_it)) {
        smallest_it = ancestor_it;
      }
    }
    else {
      break;
    }
  }

  return smallest_it;
}

/// @brief Push the item at @c it down into the heap [@c first, @c last) using
/// the ordering functor @c comp.
template<typename RandomIt, typename Compare>
void heapify(RandomIt first, RandomIt it, RandomIt last, Compare comp)
{
  using namespace std::placeholders;
  using std::swap;

  while (has_children(first, it, last)) {
    const auto it2{it};
    if (is_min_level(first, it2)) {
      it = smallest_ancestor(first, it2, last, comp);
      if (comp(*it, *it2)) {
        swap(*it, *it2);
        if (is_grandchild(first, it, it2)) {
          const auto parent_it{parent(first, it)};
          if (comp(*parent_it, *it)) {
            swap(*it, *parent_it);
          }
        }
        else {
          break;
        }
      }
      else {
        break;
      }
    }
    else {
      it = smallest_ancestor(first, it2, last, std::bind(comp, _2, _1));
      if (comp(*it2, *it)) {
        swap(*it, *it2);
        if (is_grandchild(first, it, it2)) {
          const auto parent_it{parent(first, it)};
          if (comp(*it, *parent_it)) {
            swap(*it, *parent_it);
          }
        }
        else {
          break;
        }
      }
      else {
        break;
      }
    }
  }
}

}

export namespace order_statistics {

/// @brief Returns @c true if the heap [@c first, @c last) meets the min-max
/// properties.
/// @tparam RandomIt A @c RandomAccessIterator type.
/// @tparam Compare Type of a binary functor to compare two elements in the
/// heap.
/// @param [in] first Iterator to the first element of the heap.
/// @param [in] last Iterator to the element past the last element of the heap.
/// @param [in] comp Functor to determine which of two items in the heap is
/// considered smaller.
/// @return @c true if [@c first, @c last) is a min-max heap, @c false
/// otherwise.
template<typename RandomIt, typename Compare>
bool is_mm_heap(RandomIt first, RandomIt last, Compare comp)
{
  using namespace std::placeholders;

  for (auto it{first}; it != last; ++it) {
    if (is_min_level(first, it)) {
      if (!for_all_children(first, it, last, std::bind(comp, *it, _1))) {
        return false;
      }
    }
    else {
      if (!for_all_children(first, it, last, std::bind(comp, _1, *it))) {
        return false;
      }
    }
  }
  return true;
}

/// @brief Returns @c true if the heap [@c first, @c last) meets the min-max
/// properties.
///
/// Uses std::less to determine the order of elements.
///
/// @tparam RandomIt A @c RandomAccessIterator type.
/// @param [in] first Iterator to the first element of the heap.
/// @param [in] last Iterator to the element past the last element of the heap.
/// @return @c true if [@c first, @c last) is a min-max heap, @c false
/// otherwise.
template<typename RandomIt>
bool is_mm_heap(RandomIt first, RandomIt last)
{
  return is_mm_heap(first, last, std::less<>{});
}

/// @brief Inserts the element at (@c last - 1) into the min-max heap [@c first,
/// @c last - 1)
/// @tparam RandomIt A @c RandomAccessIterator type.
/// @tparam Compare Type of a binary functor to compare two elements in the
/// heap.
/// @param first Iterator to the first element of the heap.
/// @param last Iterator to the element past the last element of the heap.
/// @param comp Functor to determine which of two items in the heap is
/// considered smaller.
template<typename RandomIt, typename Compare>
void push_mm_heap(RandomIt first, RandomIt last, Compare comp)
{
  // Expects(first == last || is_mm_heap(first, last - 1));

  using std::swap;
  using namespace std::placeholders;

  if (std::distance(first, last) > 1) {
    auto it{last - 1};
    auto parent_it{parent(first, it)};
    if (is_min_level(first, it)) {
      if (comp(*parent_it, *it)) {
        swap(*it, *parent_it);
        push_mm_heap_impl(first, parent_it + 1, std::bind(comp, _2, _1));
      }
      else {
        push_mm_heap_impl(first, it + 1, comp);
      }
    }
    else {
      if (comp(*it, *parent_it)) {
        swap(*it, *parent_it);
        push_mm_heap_impl(first, parent_it + 1, comp);
      }
      else {
        push_mm_heap_impl(first, it + 1, std::bind(comp, _2, _1));
      }
    }
  }

  // Ensures(first == last || is_mm_heap(first, last));
}

/// @brief Inserts the element at (@c last - 1) into the min-max heap [@c first,
/// @c last - 1)
/// @tparam RandomIt A @c RandomAccessIterator type.
/// @param first Iterator to the first element of the heap.
/// @param last Iterator to the element past the last element of the heap.
template<typename RandomIt>
void push_mm_heap(RandomIt first, RandomIt last)
{
  push_mm_heap(first, last, std::less<>{});
}

/// @brief Swaps the items at @c first and (@c last - 1) and turns [@c first, @c last - 1) into a min-max heap.
/// 
/// Effectively, this function removes the minimum or the root of the min-max heap.
/// 
/// @tparam RandomIt A @c RandomAccessIterator type.
/// @tparam Compare Functor to determine which of two items in the heap is
/// considered smaller.
/// @param first Iterator to the first element of the heap.
/// @param last Iterator to the element past the last element of the heap.
/// @param comp Functor to determine which of two items in the heap is
/// considered smaller.
template<typename RandomIt, typename Compare>
void pop_mm_heap(RandomIt first, RandomIt last, Compare comp)
{
  using std::swap;

  // Expects(first == last || is_mm_heap(first, last));

  if (std::distance(first, last) > 1) {
    swap(*first, *(last - 1));
    heapify(first, first, last - 1, comp);
  }

  // Ensures(first == last || is_mm_heap(first, last - 1));
}

/// @brief Swaps the items at @c first and (@c last - 1) and turns [@c first, @c last - 1) into a min-max heap.
/// 
/// Effectively, this function removes the minimum or the root of the min-max heap.
/// 
/// @tparam RandomIt A @c RandomAccessIterator type.
/// @param first Iterator to the first element of the heap.
/// @param last Iterator to the element past the last element of the heap.
template<typename RandomIt>
void pop_mm_heap(RandomIt first, RandomIt last)
{
  pop_mm_heap(first, last, std::less<>{});
}

/// @brief Turn the sequence [@c first, @c last) into a min-max heap.
/// @tparam RandomIt A @c RandomAccessIterator type.
/// @tparam Compare Type of a binary functor to compare two elements in the
/// heap.
/// @param first Iterator to the first element of the heap. This is where the
/// smallest element will reside after the function returns.
/// @param last Iterator to the element past the last element of the heap.
/// @param comp Functor to determine which of two items in the heap is
/// considered smaller.
template<typename RandomIt, typename Compare>
void make_mm_heap(RandomIt first, RandomIt last, Compare comp)
{
  auto it{first};
  std::advance(it, std::distance(first, last) / 2);
  heapify(first, it, last, comp);
  while (first != it) {
    --it;
    heapify(first, it, last, comp);
  }

  // Ensures(is_mm_heap(first, last));
}

/// @brief Turn the sequence [@c first, @c last) into a min-max heap.
/// @tparam RandomIt A @c RandomAccessIterator type.
/// @param first Iterator to the first element of the heap. This is where the
/// smallest element will reside after the function returns.
/// @param last Iterator to the element past the last element of the heap.
template<typename RandomIt>
void make_mm_heap(RandomIt first, RandomIt last)
{
  make_mm_heap(first, last, std::less<>{});
}

}
