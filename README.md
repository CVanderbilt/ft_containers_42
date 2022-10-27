# FT_CONTAINERS

## Description

This project goal is to learn about the STL library, how are the containers implemented, how iterators work and some of the useful wierd structures like std::enable_if and so on.
In order to do so we had to implement some of the containers, specifically std::vector, std::map and std::stack.  

### MAP

This is the hardest one, so I started with this one first. The map class stores the contents in a private container which is a tree. From my experience diving deep in the code, it usually is a red_black_tree, which is a kind of balancing tree similar to the AVL tree but have in mind **IT IS NOT THE AVL TREE*, at first I implemented the tree as AVL tree and it was working fine, however one of the requirements for this project is not to be 10 times slower than the real container. The AVL tree is great but it is not fast, and if implemented with this kind of tree your map will be way slower than 10 times the normal map.  

Once the tree was implenmeted the next complex thing to implement are the iterators, the iterator of a map is the iterator of the tree, and the iterator of the tree is bidirectional. Basically means that it can go forward or backwars, but only one step at a time, it has to traverse the tree in inorder. Why? Because the normal map does so and because this way when traversing the tree you will loop through the components in order from smaller to bigger (kind of, it depends of course how the items are compared)  

Other important things about this container, ```value_compare```is a inner class that implement one operator() accepting two parametes of type ```ft::pair<Key, Value>``` (yes, we have to reimplement std::pair), this operator will return true if the first parameter key is smaller (compares using ```<```) than the second it is implemented something like this:  

```cpp
operator(const ft::pair<key, value>& lhs, const ft::pair<key, value>& rhs) {
    return lhs.first < rhs.first;
}
```

Thats it, this class will be used by the tree to be able to properly compare the values inserted in the tree.  
The last hard thing was the method ```insert_hint``` it takes a value value to be inserted and an itertor that when properly selected it speeds up the process of finding where to insert the value. It may not make much sense but basically that pointer can be found using the methods ```lower_bound```or ```upper_bound```.

### VECTOR

This one is much more simpler, it is just an array inside a class, items can be added in the las position with the method ```push_back``` (super important method used by the stack, I will ellaborate).
The complexity of this class is of course the space, once we fill all the space allocated for this array we have to allocate more space and move the content of the old array to the new one, sounds easy because it is easy. Honestly the hardest part to implement was methods like delte or insert, this is because when doing such operations we have to move part of the array to the left or to the right, but this also sounds easy.  
About iterators, this is actually interesting, the pointer of the array is obviously the iterator, but we have to implent a wrapper class around it in order to be able for the struct ```ft::is_integral``` to be able to identify if a template variable is an iterator or not (a pointer by itself shouldnt be, but I may be wrong on this one).

### STACK

Stack, the easiest of them all, thats why it was the last one implented. Basically a wrapper function aroud a container, it will use the ```push_back``` method of that container to add elements to the top of the stack and the ```pop_back``` to remove the elemnt in the top of the stack.
Thas about it ¯\_(ツ)_/¯.

## Testing

For testing I implemented different classes that go in pairs. For the map I implemented the ```map_tester_ft``` and the ```map_tester_std```, and same for stack and vector. What this classes do is permorm several tests one using the STL container and the other one using my implementation, the tests they do are obviously the same (the classes are almost copy pasted jej).

- How to test?  
Easy, just go to the ```/tester``` folder and compile *.cpp, it is important to have the containers in ```../includes/``` relative to the tests for this to work (if this conditions are met this test should work with the containers of any other project but I haven't tried this). Once it is compiled just run ```./a.out (-map/-vector/-stack) (10000) (h) (i) (s) (any number of operations from the list)```.  
- Which container to test obviously, if none specified by default it will be map.
- A number, is the size of the container, it should be just a number.
- ```h```, basically will add some headers to the output produced by the test, it is actually useless but I dont feel like removing it now.
- ```i```, means interactive, instead of executing the specified operators it creates a prompt for you to decide which operations to test.
- ```s```, when specified the test will stop after destroying the tester class, this is usefull to check leaks after the tests (implemented with a getchar so just press enter to continue).
- Anything that is not one of the previous things will be an operation, operations are specific tests that vary from container to container, if none is specified it will test all of them, wrong typed operations are ignored but count as some specified so it may end up not testing anythin. Available operators are:

| Container   | Operator             |
| ----------- | -------------------- |
| map         | capacity             |
| map         | element_access       |
| map         | operator_equal       |
| map         | iterators            |
| map         | erase                |
| map         | swap                 |
| map         | find                 |
| map         | bound                |
| map         | hint                 |
| map         | insert               |
| map         | relational           |
| vector      | capacity             |
| vector      | element_access       |
| vector      | operator_equal       |
| vector      | iterators            |
| vector      | erase                |
| vector      | swap                 |
| vector      | insert               |
| vector      | relational           |
| stack       | push_pop             |
| stack       | capacity             |
| stack       | relational           |
| ----------- | -------------------- |

When compiling it there is a commented line in ```tester_main.cpp```, if uncommented it will define ```TIME_CHECK``` and this will enable time checks in the tests, compiling this code will result in 12 warnings because using c++11 but ignore them.  
The tests are usefull not only to compare the speed but also to compare the outputs, since they are exactly alike and the randomness is not actually random we can test if our containers is working propperly by calling ```diff ft.txt std.txt```, there should be no differences.  

## Resources

- To check what everything does [cppreference](https://en.cppreference.com/w/) and [cplusplus](https://cplusplus.com/)
- When I was a little bit lost I checked this other students repos [repo1](https://github.com/Glagan/42-ft_containers) and [repo2](https://github.com/rchallie/ft_containers)
- To test things about the red_black_tree, this [simulator](https://www.cs.usfca.edu/~galles/visualization/RedBlack.html) it is visual and can be slowed down up to step by step, pretty useful.
- Explanation of [red black trees with code examples](https://www.programiz.com/dsa/red-black-tree) (avoid copying but if you do notice that there is something wrong which I don't remember, but if you have patiente you can compare the python version with the c++ version to spot the error, probably the error was introduced when translating from python to c++)

## My stats

This are the results of my testers applied to my containers:

- ```./a.out -map    10000000```: Time difference: -13185.6 ms, FT is 1.17461 times slower
- ```./a.out -vector   100000```: Time difference: 9911.41 ms, FT is 2.09737 times faster
- ```./a.out -stack  10000000```: Time difference: 163.025 ms, FT is 1.02804 times faster

So I win :)
