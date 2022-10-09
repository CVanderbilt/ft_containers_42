#include <iostream>
#include "red_black_tree.hpp"
#include "../tester/progressBar.hpp"

void showNode(ft::_red_black_tree_node<int> *n, size_t spaces = 0) {
	return ;/*
	std::string prefix = ""; for (int i = 0; i < spaces; i++) prefix += " ";
	if (!n) { std::cout << prefix << "NULL" << std::endl; return ;}
	std::cout << prefix << "Value: " << *n->d << std::endl;
	//std::cout << prefix << "Parent: "; showNode(n->p);
	std::cout << prefix << "Left: "; showNode(n->l);
	std::cout << prefix << "Right: "; showNode(n->r);
	std::cout << std::endl;*/
}

void show(ft::_red_black_tree<int, std::less<int>, std::allocator<int> >& t) {
	int black = 0;
	int red = 0;
	for (ft::_red_black_tree<int>::iterator it = t.begin();
		it._node != t.end()._node;
		it++)
		if (it._node->isRed) red++; else black++;
	//std::cout << "red: " << red << std::endl;
	//std::cout << "blackk: " << black << std::endl;
}

int main () {

	typedef ft::_red_black_tree<int, std::less<int>, std::allocator<int> > _tree;

	_tree *t = new _tree();

	srand(42);
	
	ProgressBar *bar = new ProgressBar();
	size_t loops = 1000000;
	for (int i = 0; i < 1000000; i++){
		std::cout << "VUELTA BUCLE" << std::endl;
		bar->setProgress(i, 1000000);
		int n = rand();
		n *= n < 0 ? -1 : 1;
		n %= 100000;
		bool b = false;
		t->insert(n, &b);
		//show(t);
	}
	delete bar;
	//std::cout << std::endl;

	show(*t);

	delete t;
	//std::cout << "enter to end, check leaks now" << std::endl;
	getchar();
	//show(*t);
	return (0);
}