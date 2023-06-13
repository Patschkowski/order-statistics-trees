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

export module order_statistics_trees;

export namespace order_statistics_trees {

template<typename RandomIt>
bool is_mm_heap(RandomIt first, RandomIt last);

template<typename RandomIt, typename Compare>
void make_mm_heap(RandomIt first, RandomIt last, Compare comp);

template<typename RandomIt>
void make_mm_heap(RandomIt first, RandomIt last);

template<typename RandomIt>
void pop_heap(RandomIt first, RandomIt last);

template<typename RandomIt, typename Compare>
void push_mm_heap(RandomIt first, RandomIt last, Compare comp);

template<typename RandomIt>
void push_mm_heap(RandomIt first, RandomIt last);

template<typename RandomIt1, typename RandomIt2, typename Compare>
void make_order_statistics_tree(RandomIt1 first,
                                RandomIt1 last,
                                RandomIt2 ranks_first,
                                RandomIt2 ranks_last,
                                Compare   comp);

template<typename RandomIt1, typename RandomIt2>
void make_order_statistics_tree(RandomIt1 first,
                                RandomIt1 last,
                                RandomIt2 ranks_first,
                                RandomIt2 ranks_last);

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

// module :private;

namespace order_statistics_trees {
template<typename RandomIt>
bool is_mm_heap(RandomIt /*first*/, RandomIt /*last*/)
{
  return true;
}

template<typename RandomIt, typename Compare>
void push_mm_heap_impl(RandomIt first, RandomIt last, Compare comp)
{
  using std::swap;
  auto it{last - 1};
  for (;;) {
    auto i{std::distance(first, it)};
    if (i <= 2)
      break;
    auto grandparent_it{first + ((i - 1) / 2 - 1) / 2};
    if (!comp(*it, *grandparent_it))
      break;
    swap(*it, *grandparent_it);
    it = grandparent_it;
  }
}

template<typename RandomIt, typename Compare>
void heapify(RandomIt first, RandomIt it, RandomIt last, Compare comp)
{
  using diff_type = std::iterator_traits<RandomIt>::difference_type;
  const std::array<std::array<diff_type, 2>, 6> offsets{
    std::array<diff_type, 2>{2, 1},
    std::array<diff_type, 2>{2, 2},
    std::array<diff_type, 2>{4, 3},
    std::array<diff_type, 2>{4, 4},
    std::array<diff_type, 2>{4, 5},
    std::array<diff_type, 2>{4, 6}};
  using std::swap;
  for (;;) {
    const auto i{std::distance(first, it)};
    auto       offs{offsets.begin()};
    auto       descendent{first + i * (*offs)[0] + (*offs)[1]};
    if (descendent >= last)
      break;
    if (static_cast<long long>(std::log2(i + 1)) % 2 == 0) {
      for (auto o{++offs}; o < offsets.end(); ++o) {
        const diff_type d{i * (*o)[0] + (*o)[1]};
        if (d >= std::distance(first, last))
          break;
        if (comp(*(first + d), *descendent)) {
          descendent = first + d;
          offs       = o;
        }
      }
      if (!comp(*descendent, *it))
        break;
      swap(*descendent, *it);
      auto dif{std::distance(offs, offsets.begin())};
      if (dif < 2)
        break;
      auto parent{first + i * offsets[dif / 4][0] + offsets[dif / 4][1]};
      if (comp(*parent, *descendent)) {
        swap(*descendent, *parent);
      }
    }
    else {
      for (auto o{++offs}; o < offsets.end(); ++o) {
        const diff_type d{i * (*o)[0] + (*o)[1]};
        if (d >= std::distance(first, last))
          break;
        if (comp(*descendent, *(first + d))) {
          descendent = first + d;
          offs       = o;
        }
      }
      if (!comp(*it, *descendent))
        break;
      swap(*descendent, *it);
      auto dif{std::distance(offs, offsets.begin())};
      if (dif < 2)
        break;
      auto parent{first + i * offsets[dif / 4][0] + offsets[dif / 4][1]};
      if (comp(*descendent, *parent)) {
        swap(*descendent, *parent);
      }
    }
    it = descendent;
  }
}

template<typename RandomIt, typename Compare>
void push_mm_heap(RandomIt first, RandomIt last, Compare comp)
{
  Expects(is_mm_heap(first, last - 1));

  using std::swap;
  using namespace std::placeholders;

  auto it{last - 1};
  auto i{std::distance(first, it)};
  if (i > 0) {
    auto parent_it{first + (i - 1) / 2};
    if (static_cast<long long>(std::log2(i + 1)) % 2 == 0) {
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

  Ensures(is_mm_heap(first, last));
}

template<typename RandomIt, typename Compare>
void make_mm_heap(RandomIt first, RandomIt last, Compare comp)
{
  auto it{first + std::distance(first, last) / 2};
  for (;;) {
    heapify(first, it, last, comp);
    if (it == first)
      break;
    --it;
  }

  Ensures(is_mm_heap(first, last));
}

template<typename RandomIt>
void push_mm_heap(RandomIt first, RandomIt last)
{
  push_mm_heap(first, last, std::less<>{});
}

template<typename RandomIt>
void make_mm_heap(RandomIt first, RandomIt last)
{
  make_mm_heap(first, last, std::less<>{});
}

template<typename RandomIt1, typename RandomIt2, typename Compare>
void make_order_statistics_tree(RandomIt1 first,
                                RandomIt1 last,
                                RandomIt2 ranks_first,
                                RandomIt2 ranks_last,
                                Compare   comp)
{
  auto nth{first};
  for (auto rank{ranks_first}; rank != ranks_last; ++rank) {
    const auto prev_nth{nth};
    std::advance(nth, *rank);
    std::nth_element(first, nth, last, comp);
    make_mm_heap(prev_nth, nth);
  }
}

template<typename RandomIt1, typename RandomIt2>
void make_order_statistics_tree(RandomIt1 first,
                                RandomIt1 last,
                                RandomIt2 ranks_first,
                                RandomIt2 ranks_last)
{
  make_order_statistics_tree(first,
                             last,
                             ranks_first,
                             ranks_last,
                             std::less<>{});
}

}
