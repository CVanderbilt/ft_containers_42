clang++ -g tester_main.cpp

# capacity element_access operator_equal iterators erase swap
time ./a.out ft  5000000
time ./a.out std 5000000

diff ft.txt std.txt