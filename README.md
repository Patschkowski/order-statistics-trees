# Order Statistics Trees
STL-style implementation of in situ min-max heaps and order statistics trees.

## Min-max heaps
Min-max heaps are complete binary trees with the following properties.

- The smallest element of the heap can be found in the root node. Thus guaranteeing `O(1)` access to the minimum.
- The largest element of the heap can be found in one of the child nodes of the root element. Thus the maximum can also be accessed in `O(1)`.
- Every level of the tree is labelled a min (even) or max (odd) level and the root node of a subtree starting at a min (max) level has the minimum (maximum) value of that subtree.
- Insertion has a worst case complexity of `O(log n)`.
- Deletion has a worst case complexity of `O(log n)`. In particular deleting the minimum or the maximum has a worst case complexity of `O(log n)`.

Analoguos a max-min heap starts with the maximum value in the root and the minimum in one of the root node's child nodes. 

This library defines the following operations on min-max heaps similar to the heap operations provided by the C++ Standard Library

| Operation | Min-max heap | Max-min heap | Min heap (C++ Standard Library) |
| --: | :-- | :-- | :-- |
| Create | `order_statistics::make_mm_heap(first, last)` | `order_statistics::make_mm_heap(first, last, std::greater<>{})` | `std::make_heap(first, last)` |
| Insert | `order_statistics::push_mm_heap(first, last)` | `order_statistics::push_mm_heap(first, last, std::greater<>{})` | `std::make_heap(first, last)` |
| Delete | `order_statistics::pop_mm_heap(first, last)` | `order_statistics::pop_mm_heap(first, last, std::greater<>{})` | `std::make_heap(first, last)` |
| Check | `order_statistics::is_mm_heap(first, last)` | `order_statistics::is_mm_heap(first, last, std::greater<>{})` | `std::is_heap(first, last)` |

### Constant time access to the minimum element

```
order_statistics::make_mm_heap(begin(container), end(container));

const auto min{*begin(container)};
```

### Constant time access to the maximum element

```
// Expects that the heap has already been created.
assert(order_statistics::is_mm_heap(begin(container), end(container)));
// Expects that there are 2 child nodes. In case of a single child the comparison is not needed.
assert(std::distance(begin(container), end(container)) > 2);

const auto max{std::max(*(begin(container) + 1), *(begin(container) + 2))};
```

### Logarithmic time insertion of a new element

Append the new element to the container of the heap, then call `push_mm_heap`. 

```
assert(order_statistics::is_mm_heap(begin(container), end(container) - 1));

order_statistics::push_mm_heap(begin(container), end(container));
```

### Logarithmic time deletion of the minimum element

```
assert(order_statistics::is_mm_heap(begin(container), end(container)));

order_statistics::pop_mm_heap(begin(container), end(container));

assert(std::min_element(begin(container), end(container)) == (end(container) - 1));
```

### References

- Min-Max Heaps and Generalized Priority Queues (1986) by M.D. Atkinson, J.-R. Sack, N. Santoro, and T. Strothotte
- Minmaxheaps, Orderstatisticstrees and Their Application to the Coursemarks Problem (1985) by M.D. Atkinson, J.-R. Sack, N. Santoro, and T. Strothotte