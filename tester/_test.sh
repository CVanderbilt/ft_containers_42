clang++ -g tester_main.cpp

# capacity element_access operator_equal iterators erase swap
time ./a.out ft  5000000 > ft.txt
time ./a.out std 5000000 > std.txt

diff ft.txt std.txt