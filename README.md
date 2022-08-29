# FT_CONTAINERS

## Description

(...)

## Testing

(...)

## Resources

(...)

## TODO

- avl_tree.hpp:
  - Update to remove the need of reassig the variable r internally each time we call method like balance, etc... Instead it should be updated always by that method
  - Add support for allocator instead of calling new (usually in insert method, marked with a comment)
  - method ```get```, check NULL return and decide if it would be better using exception instead
  - method ```get```, remove msg input variable
  - Add operator= support using deep copy and update copy constructor to use the new operator=
  - Properly finish max_size method
  - Clean iterator implementation and update todo list with todo comments in there
- avl_tree.cpp
  - Add leaks test when exiting
