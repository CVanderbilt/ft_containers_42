# FT_CONTAINERS

## Description

(...)

## Testing

(...)

## Resources

(...)

## TODO

- NextThings:
  - add map tests
  - add ft::enable_if
  - add const_iterators

- General:
  - Create a test framework to test each feature of my container and compare its speed against the official container
  - Check for usages of ```*.d``` and ```*->d``` see if they are being used properly (not assigning ```a.d = b.d``` but ```*(a.d)=*(b.d))```
  - Check if we need to free/destroy allocators and comparators
  - iterators_traits, reverse_iterator, enable_if, is_integral, equal/lexicographical com-
pare, std::pair, std::make_pair, must be reimplemented.

- avl_tree.hpp:
  - Properly finish max_size method
  - Check tests for hinted inserts

- map.hpp:
  - Check explicit keyword in constructor
  - Check if I need to add a default constructor map() (right now using default values)
  - Add logic for constructor with iterators once we have logic for insertion with iterators
  - Add logic containing duplicated code in both at methods and operator[] method
  - Add logic for const iterators (also in tree)
