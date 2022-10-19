clang++ -g -fsanitize=address *.cpp

# capacity element_access operator_equal iterators erase swap
./a.out ft 10000000 h erase
