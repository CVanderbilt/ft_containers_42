clang++ -g tester_main.cpp

# capacity element_access operator_equal iterators erase swap find bound hint insert
time ./a.out 50000000

diff ft.txt std.txt