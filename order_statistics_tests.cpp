#include <array>
#include <cmath>
#include <iterator>

#define BOOST_TEST_MODULE Order Statistics Tests
#include <boost/test/unit_test.hpp>

import order_statistics;

using namespace order_statistics;

using heap_type = std::array<int, 31>;

struct minmax_fixture {
  heap_type h{17, 16, 31, 30, 10, 13, 12, 15, 50, 45, 38, 39, 27, 34, 30, 28,
              5,  25, 37, 8,  15, 65, 80, 18, 32, 14, 20, 59, 45, 36, 57};
};

struct order_statistics_tree_fixture {
  heap_type h{17, 16, 31, 30, 10, 13, 12, 15, 50, 45, 38, 39, 27, 34, 30, 28,
              5,  25, 37, 8,  15, 65, 80, 18, 32, 14, 20, 59, 45, 36, 57};
};

BOOST_TEST_DONT_PRINT_LOG_VALUE(heap_type::iterator)

BOOST_AUTO_TEST_SUITE(order_statistics_tests)

BOOST_FIXTURE_TEST_SUITE(minmax_heap_tests, minmax_fixture)

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

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(order_statistics_tree_tests,
                         order_statistics_tree_fixture)

BOOST_AUTO_TEST_CASE(median_is_in_correct_place)
{
  std::array<std::array<int, 31>::iterator, 1> ranks{h.begin() + h.size() / 2};
  make_order_statistics_tree(h.begin(), h.end(), ranks.begin(), ranks.end());

  BOOST_TEST(*ranks[0] == 30);
}

BOOST_AUTO_TEST_CASE(q1_q3_are_in_correct_place)
{
  std::array<std::array<int, 31>::iterator, 3> ranks{h.begin() + h.size() / 4,
                                                     h.begin() + h.size() / 2,
                                                     h.begin() + h.size() * 3 / 4};
  make_order_statistics_tree(h.begin(), h.end(), ranks.begin(), ranks.end());

  BOOST_TEST(*ranks[0] == 15);
  BOOST_TEST(*ranks[1] == 30);
  BOOST_TEST(*ranks[2] == 39);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
