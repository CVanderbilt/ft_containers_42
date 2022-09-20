# FT_CONTAINERS

## Description

(...)

## Testing

(...)

## Resources

(...)

## TODO

- General:
  - Check if we need to free/destroy allocators and comparators
  - iterators_traits, reverse_iterator, enable_if, is_integral, equal/lexicographical com-
pare, std::pair, std::make_pair, must be reimplemented.

- avl_tree.hpp:
  - Properly finish max_size method

- map.hpp:
  - Check explicit keyword in constructor
  - Check if I need to add a default constructor map() (right now using default values)
  - Add logic for constructor with iterators once we have logic for insertion with iterators
  - Add logic containing duplicated code in both at methods and operator[] method
