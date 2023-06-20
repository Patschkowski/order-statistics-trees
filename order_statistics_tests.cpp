#include <array>
#include <cmath>
#include <iterator>

#define BOOST_TEST_MODULE Order Statistics Trees Tests
#include <boost/test/unit_test.hpp>

import order_statistics;

using namespace order_statistics;

using heap_type = std::array<int, 31>;

struct minmax_fixture {
  heap_type h{17, 16, 31, 30, 10, 13, 12, 15, 50, 45, 38, 39, 27, 34, 30, 28,
              5,  25, 37, 8,  15, 65, 80, 18, 32, 14, 20, 59, 45, 36, 57};
};

BOOST_TEST_DONT_PRINT_LOG_VALUE(heap_type::iterator)

BOOST_FIXTURE_TEST_SUITE(minmax_heap_tests, minmax_fixture)

/* BOOST_AUTO_TEST_CASE(is_min_level_succeeds)
{
  std::array<bool, 31> expected_is_min{
    true, // 1
    false, false, // 2
    true, true, true, true, // 4
    false, false, false, false, false, false, false, false, // 8
    true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true // 16
  };

  std::array<bool, 31> actual_is_min;
  for (auto it{ h.begin() }; it != h.end(); ++it) {
    actual_is_min[std::distance(h.begin(), it)] = is_min_level(h.begin(), it);
  }

  BOOST_TEST(expected_is_min == actual_is_min,
             boost::test_tools::per_element{});
}

BOOST_AUTO_TEST_CASE(has_children_succeeds)
{
  // Empty heap.
  BOOST_TEST(!has_children(h.begin(), h.begin(), h.begin()));

  // Root without children.
  BOOST_TEST(!has_children(h.begin(), h.begin(), h.begin() + 1));

  // Root with one child.
  BOOST_TEST(has_children(h.begin(), h.begin(), h.begin() + 2));

  // Root with two children.
  BOOST_TEST(has_children(h.begin(), h.begin(), h.begin() + 3));
}

BOOST_AUTO_TEST_CASE(is_parent_succeeds)
{
  BOOST_TEST(h.begin() == parent(h.begin(), h.begin() + 1));
  BOOST_TEST(h.begin() == parent(h.begin(), h.begin() + 2));
  BOOST_TEST(h.begin() + 1 == parent(h.begin(), h.begin() + 4));
  BOOST_TEST(h.begin() + 2 == parent(h.begin(), h.begin() + 5));
}

BOOST_AUTO_TEST_CASE(is_grandchild_succeeds)
{
  BOOST_TEST(is_grandchild(h.begin(), h.begin() + 3, h.begin()));
  BOOST_TEST(is_grandchild(h.begin(), h.begin() + 4, h.begin()));
  BOOST_TEST(is_grandchild(h.begin(), h.begin() + 5, h.begin()));
  BOOST_TEST(is_grandchild(h.begin(), h.begin() + 6, h.begin()));
}*/

BOOST_AUTO_TEST_CASE(is_mm_heap_succeeds)
{
  BOOST_TEST(!is_mm_heap(h.begin(), h.end()));

  std::array<int, 12> h2{8, 71, 41, 31, 10, 11, 16, 46, 51, 31, 21, 13};
  BOOST_TEST(is_mm_heap(h2.begin(), h2.end()));
}

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

BOOST_AUTO_TEST_CASE(is_heap_after_push_heap)
{
  for (auto it{h.begin()}; it != h.end(); ++it) {
    push_mm_heap(h.begin(), it);
    BOOST_TEST(is_mm_heap(h.begin(), it));
  }
}

BOOST_AUTO_TEST_CASE(is_heap_after_pop_heap)
{
  make_mm_heap(h.begin(), h.end());
  pop_mm_heap(h.begin(), h.end());

  BOOST_TEST(is_mm_heap(h.begin(), h.end() - 1));
  BOOST_TEST((h.end() - 1) == std::min_element(h.begin(), h.end()));
}

/* BOOST_AUTO_TEST_CASE(has_minmax_property)
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
}*/

BOOST_AUTO_TEST_SUITE_END()

