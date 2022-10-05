clang++ -g tester_main.cpp

# capacity element_access operator_equal iterators erase swap
time ./a.out ft element_access 50000 > ft.txt ; time ./a.out element_access std 100000 > std.txt

diff ft.txt std.txt