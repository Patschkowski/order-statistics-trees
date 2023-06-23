cmake_minimum_required(VERSION 3.20)

project("Order Statistics")

find_package(Boost COMPONENTS unit_test_framework)
find_package(Doxygen)

add_library(order_statistics_trees "order_statistics.ixx" "order_statistics-minmax_heaps.ixx")
target_compile_features(order_statistics_trees PUBLIC cxx_std_20)
target_compile_options(order_statistics_trees PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/WX /W4 /EHsc>)

if(BOOST_FOUND)
add_executable(order_statistics_tests "order_statistics_tests.cpp")
target_compile_options(order_statistics_tests PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/WX /W4 /EHsc $<$<CONFIG:Debug>:/DEBUG /Zi>>)
target_link_libraries(order_statistics_tests PRIVATE Boost::unit_test_framework order_statistics_trees)

# https://github.com/ekcoh/cpp-coverage/blob/master/cmake/cpp_coverage.cmake
include(CTest)
enable_testing()

add_test(test_minmax_heap_is_mm_heap_succeeds order_statistics_tests -t "order_statistics_tests/minmax_heap_tests/is_mm_heap_succeeds")
add_test(test_minmax_heap_min_element_is_first order_statistics_tests -t "order_statistics_tests/minmax_heap_tests/min_element_is_front")
add_test(test_minmax_heap_max_element_is_2nd_or_3rd order_statistics_tests -t "order_statistics_tests/minmax_heap_tests/max_element_is_2nd_or_3rd")
add_test(test_minmax_heap_is_heap_after_make_heap order_statistics_tests -t "order_statistics_tests/minmax_heap_tests/is_heap_after_make_heap")
add_test(test_minmax_heap_is_heap_after_ppush_heap order_statistics_tests -t "order_statistics_tests/minmax_heap_tests/is_heap_after_push_heap")
add_test(test_minmax_heap_is_heap_after_pop_heap order_statistics_tests -t "order_statistics_tests/minmax_heap_tests/is_heap_after_pop_heap")

add_test(test_order_statistics_tree_median_is_in_correct_place order_statistics_tests -t "order_statistics_tests/order_statistics_tree_tests/median_is_in_correct_place")
add_test(test_order_statistics_tree_q1_q3_are_in_correct_place order_statistics_tests -t "order_statistics_tests/order_statistics_tree_tests/q1_q3_are_in_correct_place")
endif()

if(DOXYGEN_FOUND)
  set(DOXYGEN_EXCLUDE_PATTERNS */out/* */.vs/* *_tests.cpp)
  set(DOXYGEN_PLANTUML_JAR_PATH $ENV{PLANTUML_JAR_PATH})
  doxygen_add_docs(doxygen ${CMAKE_CURRENT_SOURCE_DIR} ALL)
endif()

