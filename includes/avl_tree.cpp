#include <iostream>
#include <cstdio>
#include <sstream>
#include <algorithm>
#include <utility>
#include <limits.h>
#include "avl_tree.hpp"

#define pow2(n) (1 << (n))

void announce(const std::string& msg) {
	const std::string a = "*--------------------------------------------------------*";
	
	std::cout << a << std::endl;
	std::cout << msg << std::endl;
	std::cout << a << std::endl;
}

void findAndErase(int n, ft::avl_tree<int> *tree) {
	ft::avl_tree<int>::Iterator it = tree->begin();
	std::cout << "iterator search" << std::endl;
	size_t maxsize = tree->size();
	size_t actualsize = 0;
	while (it != tree->end()) {
		std::cout << "actual value: " << *it << std::endl;
		if (*it == n) {
			std::cout << "node with value " << n << " found in tree" << std::endl;
			std::cout << "tree before deletion" << std::endl;
			tree->printBT();
			tree->erase(it);
			std::cout << "tree after deletion" << std::endl;
			tree->printBT();
			return ;
		}
		actualsize++;
		it++;
		if (actualsize > maxsize) { //prevent for loops in malformed tree
			std::cout << "size check!!!" << std::endl;
			break ;
		}
		//TODO: add test de erase(it++)
	}
	std::cout << "value not found!!" << std::endl;
}

bool iteratorTest(ft::avl_tree<int> *tree) {
	bool result = true;
	ft::avl_tree<int>::Iterator it = tree->begin();
	size_t calculatedSize = 0;
	size_t expectedSize = tree->size();
	int prevValue = INT_MIN;
	while (it != tree->end()) {
		std::cout << *it << " ";
		if (*it < prevValue) {
			std::cout << "(not sorted properly)" << std::endl;
			result = false;
		}
		prevValue = *it++;
		calculatedSize++;
		if (calculatedSize > expectedSize) { //this check will prevent loops
			std::cout << "size failed!!!" << std::endl;
			result = false;
			break ;
		}
	}
	std::cout << std::endl;
	std::cout << "Expected size (tree size): " << expectedSize;
	if (calculatedSize != expectedSize) {
		result = false;
		std::cout << " but calculated: " << calculatedSize;
	}
	std::cout << std::endl;
	return result;
}

void autoTest() {
	ft::avl_tree<int> avl;
	bool result = true;
	
	announce("Inserting...");
	//*
	avl.insert(6);
	if (!iteratorTest(&avl)) result = false;
	avl.insert(7);
	if (!iteratorTest(&avl)) result = false;
	avl.insert(8);
	if (!iteratorTest(&avl)) result = false;
	avl.insert(5);
	if (!iteratorTest(&avl)) result = false;
	avl.insert(3);
	if (!iteratorTest(&avl)) result = false;
	avl.insert(2);
	if (!iteratorTest(&avl)) result = false;
	avl.insert(100);
	if (!iteratorTest(&avl)) result = false;
	avl.insert(4);
	if (!iteratorTest(&avl)) result = false;
	avl.insert(10);
	if (!iteratorTest(&avl)) result = false;
	avl.insert(9);
	if (!iteratorTest(&avl)) result = false;
	avl.insert(1);
	if (!iteratorTest(&avl)) result = false;
	avl.insert(11);
	if (!iteratorTest(&avl)) result = false;
	avl.insert(-2);
	if (!iteratorTest(&avl)) result = false;
	avl.insert(-6);
	if (!iteratorTest(&avl)) result = false;
	avl.insert(-7);
	if (!iteratorTest(&avl)) result = false;
	avl.insert(-8);
	if (!iteratorTest(&avl)) result = false;
	avl.insert(-5);
	if (!iteratorTest(&avl)) result = false;
	avl.insert(-3);
	if (!iteratorTest(&avl)) result = false;
	avl.insert(20);
	if (!iteratorTest(&avl)) result = false;
	avl.insert(-100);
	if (!iteratorTest(&avl)) result = false;
	avl.insert(-4);
	if (!iteratorTest(&avl)) result = false;
	avl.insert(-10);
	if (!iteratorTest(&avl)) result = false;
	avl.insert(-9);
	if (!iteratorTest(&avl)) result = false;
	avl.insert(-1);
	if (!iteratorTest(&avl)) result = false;
	avl.insert(-11);
	if (!iteratorTest(&avl)) result = false;
	avl.insert(-20);
	if (!iteratorTest(&avl)) result = false;
	//*/

	std::cout << "printing tree, check balances" << std::endl;
	avl.printBT();

	announce("Iterator (should be ordered)");
	if (!iteratorTest(&avl)) result = false;

	announce("Erasing some values...");
	if (!iteratorTest(&avl)) result = false;
	findAndErase(-100, &avl);
	if (!iteratorTest(&avl)) result = false;
	findAndErase(-3, &avl);
	if (!iteratorTest(&avl)) result = false;
	findAndErase(100, &avl);
	if (!iteratorTest(&avl)) result = false;
	findAndErase(9, &avl);
	if (!iteratorTest(&avl)) result = false;
	findAndErase(-100, &avl);
	if (!iteratorTest(&avl)) result = false;
	findAndErase(-9, &avl);
	if (!iteratorTest(&avl)) result = false;
	findAndErase(-4, &avl);
	if (!iteratorTest(&avl)) result = false;
	findAndErase(1, &avl);
	if (!iteratorTest(&avl)) result = false;

	if (!result)
		std::cout << "KO: Some tests failed" << std::endl;
	else
		std::cout << "OK: No tests failed" << std::endl;
}

int main() {
	int c, i;
   
	ft::avl_tree<int> avl;

	//*
	avl.insert(6);
	avl.insert(7);
	avl.insert(8);
	avl.insert(5);
	avl.insert(3);
	avl.insert(2);
	avl.insert(100);
	avl.insert(4);
	avl.insert(10);
	avl.insert(9);
	avl.insert(1);
	avl.insert(11);
	avl.insert(-2);

	avl.insert(-6);
	avl.insert(-7);
	avl.insert(-8);
	avl.insert(-5);
	avl.insert(-3);
	avl.insert(20);
	avl.insert(-100);
	avl.insert(-4);
	avl.insert(-10);
	avl.insert(-9);
	avl.insert(-1);
	avl.insert(-11);
	avl.insert(-20);
	//*/

	avl.printBT();
		//std::cout << *it++ << ", ";

	std::cout << std::endl;
	///*
	while (1) {
		std::cout << "0.Show size" << std::endl;
		std::cout << "1.Insert Element into the tree" << std::endl;
		std::cout << "2.show Balanced AVL Tree" << std::endl;
		std::cout << "3.InOrder traversal" << std::endl;
		std::cout << "4.PreOrder traversal" << std::endl;
		std::cout << "5.PostOrder traversal" << std::endl;
		std::cout << "6.Show as a triangle" << std::endl;
		std::cout << "7.Search value" << std::endl;
		std::cout << "8.Erase node with specific value" << std::endl;
		std::cout << "9.Loop with iterators" << std::endl;
		std::cout << "10.Automatic tests" << std::endl;
		std::cout << "11.Exit" << std::endl;
		std::cout << "Enter your Choice: ";
		std::cin >> c;
    	switch (c) {
			case 0: {
				std::cout << "Size: " << avl.size() << std::endl;
				break;
			}
        	case 1: {
        		std::cout << "Enter value to be inserted: ";
        		std::cin >> i;
				avl.insert(i);
        		break;
			}
			case 2: {
				std::cout << "Balanced AVL Tree:" << std::endl;
				avl.show();
				std::cout<< std::endl;
				break;
			}
			case 3: {
				std::cout << "Inorder Traversal:" << std::endl;
				avl.inorder();
				std::cout << std::endl;
				break;
			}
			case 4: {
				std::cout << "Preorder Traversal:" << std::endl;
				avl.preorder();
				std::cout << std::endl;
				break;
			}
			case 5: {
				std::cout << "Postorder Traversal:" << std::endl;
				avl.postorder();
				std::cout << std::endl;
				break;
			}
			case 6: {
				std::cout << "Showing as a triangle" << std::endl;
				avl.printBT();
				break;
			}
			case 7: {
				std::cout << "Enter value to search: ";
				std::cin >> i;
				avl.printBT();
				avl.get(i);
				break;
			}
			case 8: {
				std::cout << "Enter value to erase: ";
				std::cin >> i;
				std::cout << "entered: " << i << std::endl;
				findAndErase(i, &avl);
				avl.printBT();
				break;
			}
			case 9: {
				std::cout << "Looping with iterators: " << std::endl;
				iteratorTest(&avl);
				break ;
			}
			case 10: {
				std::cout << "Running automatic test" << std::endl;
				autoTest();
				break ;
			}
        	case 11: {
            	exit(1);
			}
	    	default: {
            	std::cout << "Wrong Choice" << std::endl;
			}
      }
   }//*/
   return 0;
}