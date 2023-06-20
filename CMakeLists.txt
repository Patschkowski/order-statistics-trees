cmake_minimum_required(VERSION 3.20)

project(OrderStatistics)

find_package(Microsoft.GSL CONFIG REQUIRED)
find_package(Boost COMPONENTS unit_test_framework)
find_package(Doxygen)

#add_library(order_statistics_trees "order_statistics_trees.ixx")
#target_compile_features(order_statistics_trees PUBLIC cxx_std_20)
#target_compile_options(order_statistics_trees PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/WX /W4 /EHsc>)
#target_compile_definitions(order_statistics_trees PRIVATE $<$<CONFIG:Debug>:DEBUG>)
#target_link_libraries(order_statistics_trees PRIVATE Microsoft.GSL::GSL)

if(BOOST_FOUND)
add_executable(order_statistics_tests "order_statistics_tests.cpp" "order_statistics.ixx" "order_statistics-minmax_heaps.ixx")
target_compile_features(order_statistics_tests PUBLIC cxx_std_20)
target_compile_options(order_statistics_tests PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/WX /W4 /EHsc $<$<CONFIG:Debug>:/DEBUG /Zi>>)
target_compile_definitions(order_statistics_tests PRIVATE BOOST_TEST_DYN_LINK $<$<CONFIG:Debug>:DEBUG>)
target_link_options(order_statistics_tests PRIVATE $<$<CXX_COMPILER_ID:MSVC>:$<$<CONFIG:Debug>:/PROFILE>>)
target_link_libraries(order_statistics_tests PRIVATE Boost::unit_test_framework)

# https://github.com/ekcoh/cpp-coverage/blob/master/cmake/cpp_coverage.cmake
include(CTest)
enable_testing()

add_test(test_minmax_heap_min_element_is_first order_statistics_tests -t "minmax_heap_tests/min_element_is_front")
add_test(test_minmax_heap_max_element_is_2nd_or_3rd order_statistics_tests -t "minmax_heap_tests/max_element_is_2nd_or_3rd")
add_test(test_minmax_heap_is_heap_after_make_heap order_statistics_tests -t "minmax_heap_tests/is_heap_after_make_heap")
add_test(test_minmax_heap_has_minmax_property order_statistics_tests -t "minmax_heap_tests/has_minmax_property")
add_test(test_minmax_heap_is_min_level_succeeds order_statistics_tests -t "minmax_heaps_tests/is_min_level_succeeds")
add_test(test_minmax_heap_has_children_succeeds order_statistics_tests -t "minmax_heaps_tests/has_children_succeeds")
add_test(test_minmax_heap_is_parent_succeeds order_statistics_tests -t "minmax_heaps_tests/is_parent_succeeds")
add_test(test_minmax_heap_is_grandchild_succeeds order_statistics_tests -t "minmax_heaps_tests/is_grandchild_succeeds")
add_test(test_minmax_heap_is_mm_heap_succeeds order_statistics_tests -t "minmax_heaps_tests/is_mm_heap_succeeds")
add_test(test_minmax_heap_is_heap_after_pop_heap order_statistics_tests -t "minmax_heaps_tests/is_heap_after_pop_heap")
endif()

if(DOXYGEN_FOUND)
  set(DOXYGEN_EXCLUDE_PATTERNS */out/* */.vs/* *_tests.cpp)
  set(DOXYGEN_PLANTUML_JAR_PATH $ENV{PLANTUML_JAR_PATH})
  doxygen_add_docs(doxygen ${CMAKE_CURRENT_SOURCE_DIR} ALL)
endif()

