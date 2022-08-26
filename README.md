# FT_CONTAINERS

## Description

(...)

## Testing

(...)

## Resources

(...)

## TODO

- avl_tree.hpp:
  - Update to add private methods (ex rotation methods should only be called internally and thus they should be private)
  - Finish implementation for erase method
  - Update to remove the need of reassig the variable r internally each time we call method like insert, balance, etc... Instead it should be updated always by that method
  - Fix indentation
  - Update to make insertion iterative instead of recursive
  - Add support for allocator instead of calling new (usually in insert method, marked with a comment)
  - method ```get```, check NULL return and decide if it would be better using exception instead
  - method ```get```, remove msg input variable
  - remove print methods (printbt, show, inorder...)
  - Add operator= support using deep copy and update copy constructor to use the new operator=
  - Properly finish max_size method
  - Clean iterator implementation and update todo list with todo comments in there
