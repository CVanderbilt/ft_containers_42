clang++ -g tester_main.cpp
./a.out ft erase 100000 > ft.txt ; ./a.out std erase 100000 > std.txt

diff ft.txt std.txt