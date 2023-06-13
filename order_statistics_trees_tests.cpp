#include <array>
#include <cmath>
#include <iterator>

#define BOOST_TEST_MODULE Order Statistics Trees Tests
#include <boost/test/unit_test.hpp>

import order_statistics_trees;

using namespace order_statistics_trees;

using heap_type = std::array<int, 31>;

struct minmax_fixture {
  heap_type h{17, 16, 31, 30, 10, 13, 12, 15, 50, 45, 38, 39, 27, 34, 30, 28,
              5,  25, 37, 8,  15, 65, 80, 18, 32, 14, 20, 59, 45, 36, 57};
};

/// Returns @c true if comp returns @c true for all child nodes of it.
///
template<typename RandomIt, typename Predicate>
bool for_all(RandomIt first, RandomIt last, RandomIt it, Predicate pred)
{
  const auto d{std::distance(first, it) * 2};
  const auto s{std::distance(first, last)};
  if (s > d + 1) {
    const auto child0{first + d + 1};
    if (s > d + 2) {
      const auto child1{first + d + 2};
      return pred(*child0) && pred(*child1) && for_all(first, last, child0, pred)
             && for_all(first, last, child1, pred);
    }
    else {
      return pred(*child0) && for_all(first, last, child0, pred);
    }
  }
  else {
    return true;
  }
}

BOOST_TEST_DONT_PRINT_LOG_VALUE(heap_type::iterator)

BOOST_FIXTURE_TEST_SUITE(minmax_heap_tests, minmax_fixture)

BOOST_AUTO_TEST_CASE(min_element_is_front)
{
  make_mm_heap(h.begin(), h.end());

  const auto min_it{std::min_element(h.begin(), h.end())};

  BOOST_REQUIRE(min_it == h.begin());
  BOOST_TEST(*min_it == h.front());
}

BOOST_AUTO_TEST_CASE(max_element_is_2nd_or_3rd)
{
  make_mm_heap(h.begin(), h.end());

  const auto max_it{std::max_element(h.begin(), h.end())};
  const auto max_begin{h.begin() + 1};
  const auto max_end{h.begin() + 3};

  BOOST_REQUIRE(max_begin <= max_it);
  BOOST_REQUIRE(max_it < max_end);
  BOOST_TEST(max_end != std::find(max_begin, max_end, *max_it));
}

BOOST_AUTO_TEST_CASE(is_heap_after_make_heap)
{
  BOOST_REQUIRE(!is_mm_heap(h.begin(), h.end()));

  make_mm_heap(h.begin(), h.end());

  BOOST_TEST(is_mm_heap(h.begin(), h.end()));
}

BOOST_AUTO_TEST_CASE(has_minmax_property)
{
  make_mm_heap(h.begin(), h.end());

  for (auto it{h.begin()}; it != h.end(); ++it) {
    const auto d{std::distance(h.begin(), it)};
    if (static_cast<long long>(std::log2(d + 1)) % 2 == 0) {
      BOOST_TEST(for_all(h.begin(), h.end(), it, [val = *it](auto v) {
        return val < v; 
      }));
    }
    else {
      BOOST_TEST(for_all(h.begin(), h.end(), it, [val = *it](auto v) {
        return val > v;
      }));
    }
  }
}

BOOST_AUTO_TEST_SUITE_END()
