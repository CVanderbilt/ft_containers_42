clang++ -g tester_main.cpp

# capacity element_access operator_equal iterators erase swap find bound hint
time ./a.out 5000000 ft
time ./a.out 5000000 std

diff ft.txt std.txt