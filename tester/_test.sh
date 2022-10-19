clang++ -g *.cpp

# capacity element_access operator_equal iterators erase swap find bound hint insert
time ./a.out 500000

diff ft.txt std.txt