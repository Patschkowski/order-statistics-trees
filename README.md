# Order Statistics Trees
STL-style implementation of in situ min-max heaps and order statistics trees.

## Min-Max Heaps
Min-max heaps are complete binary trees with the following properties.

- The smallest element of the heap can be found in the root node. Thus guaranteeing `O(1)` access to the minimum.
- The largest element of the heap can be found in one of the child nodes of the root element. Thus the maximum can also be accessed in `O(1)`.
- Every level of the tree is labelled a min (even) or max (odd) level and the root node of a subtree starting at a min (max) level has the minimum (maximum) value of that subtree.
- Insertion has a worst case complexity of `O(log n)`.
- Deletion has a worst case complexity of `O(log n)`. In particular deleting the minimum or the maximum has a worst case complexity of `O(log n)`.

Analoguos a max-min heap starts with the maximum value in the root and the minimum in one of the root node's child nodes. 

This library defines the following operations on min-max heaps similar to the heap operations provided by the C++ Standard Library

| Operation | Min-Max Heap | Max-Min Heap | Min Heap (C++ Standard Library) |
| --: | :-- | :-- | :-- |
| Create | `order_statistics::make_mm_heap(first, last)` | `order_statistics::make_mm_heap(first, last, std::greater<>{})` | `std::make_heap(first, last)` |
| Insert | `order_statistics::push_mm_heap(first, last)` | `order_statistics::push_mm_heap(first, last, std::greater<>{})` | `std::make_heap(first, last)` |
| Delete | `order_statistics::pop_mm_heap(first, last)` | `order_statistics::pop_mm_heap(first, last, std::greater<>{})` | `std::make_heap(first, last)` |
| Check | `order_statistics::is_mm_heap(first, last)` | `order_statistics::is_mm_heap(first, last, std::greater<>{})` | `std::is_heap(first, last)` |

### Constant Time Access to the Minimum Element

```
order_statistics::make_mm_heap(begin(container), end(container));

const auto min{*begin(container)};
```

### Constant Time Access to the Maximum Element

```
// Expects that the heap has already been created.
assert(order_statistics::is_mm_heap(begin(container), end(container)));
// Expects that there are 2 child nodes. In case of a single child the comparison is not needed.
assert(std::distance(begin(container), end(container)) > 2);

const auto max{std::max(*(begin(container) + 1), *(begin(container) + 2))};
```

### Logarithmic Time Insertion of a New Element

Append the new element to the container of the heap, then call `order_statistics::push_mm_heap`. 

```
assert(order_statistics::is_mm_heap(begin(container), end(container) - 1));

order_statistics::push_mm_heap(begin(container), end(container));
```

### Logarithmic Time Deletion of the Minimum Element

```
assert(order_statistics::is_mm_heap(begin(container), end(container)));

order_statistics::pop_mm_heap(begin(container), end(container));

assert(std::min_element(begin(container), end(container)) == (end(container) - 1));
```
---

## Order Statistics Trees

An Order Statistics Tree is a data type that supports finding the element at rank *k_i* from a set of predefined ranks *K = {k_i, i in 1 .. m | k_i < k_(i+1)}* in `O(1)` (rank-query). It also supports finding all elements (unsorted) in the range *[k_i, k_j], i < j* in `O(outputsize)` (range-query).

This library provides the following operations to create, maintain and query order statistics trees in flat memory (in situ).

| Operation | Order Statistics Tree |
| --: | :-- |
| Create | `order_statistics::make_order_statistics_tree(first, last, ranks_first, ranks_last)` |
| Insert | `order_statistics::push_order_statistics_tree(first, last, ranks_first, ranks_last)` |
| Delete | `order_statistics::pop_order_statistics_tree(first, last, ranks_first, ranks_last)` |
| rank-query | *implicitly defined* |
| range-query | *implicitly defined* |

In statistics there is also the concept of quantiles that divide the set of samples into equal intervals. Some of those quantiles are of special interest, e.g., Q1 (25th percentile, *k_i = n/4*), Q2 (median, 50th percentile, *k_i = n/2*), Q3 (75th percentile, *k_i = n\*3/4*) to describe a distribution.

### Rank-Query the Quantiles Q1, Q2 (Median), Q3 in Constant Time

```
const auto size{std::distance(begin(container), end(container))};
const typename container_type::iterator ranks[3]{
  begin(container) + size / 4, // Q1
  begin(container) + size / 2, // Q2
  begin(container) + size * 3 / 4 // Q3
};

order_statistics::make_order_statistics_tree(begin(container), end(container), begin(ranks), end(ranks));

// *ranks[0] is Q1
// *ranks[1] is Q2
// *ranks[2] is Q3
```

### Range-Query all Elements Smaller than the Median in Linear Time

```
const auto size{std::distance(begin(container), end(container))};
const typename container_type::iterator ranks[1]{
  begin(container) + size / 2, // Median
};

order_statistics::make_order_statistics_tree(begin(container), end(container), begin(ranks), end(ranks));

std::for_each(begin(container), ranks[0], ...);
```

### Implementation Details

Conceptually, an order statistics tree is a hybrid data structure that features one vector *V\[1..m]* holding the elements of rank *k_i* and multiple min-max heaps *H_0*, ..., *H_m* where *H_i* holds the elements *V\[i]* and *V\[i+1]* with *V\[0] = -inf* and *V\[m+1] = +inf*.

To construct the min-max heaps, the `order_statistics::make_mm_heap` functions are used. To find the *V\[i]* the function `std::nth_element` is used. The min-max heaps are already flat data structures, the elements *V\[i]* are just placed in between those memory sequences.

---

### References

- Min-Max Heaps and Generalized Priority Queues (1986) by M.D. Atkinson, J.-R. Sack, N. Santoro, and T. Strothotte
- Minmaxheaps, Orderstatisticstrees and Their Application to the Coursemarks Problem (1985) by M.D. Atkinson, J.-R. Sack, N. Santoro, and T. Strothotte