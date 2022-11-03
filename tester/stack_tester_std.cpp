#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include "progressBar.hpp"
#include "vector_tester_ft.hpp"
#include "../includes/vector.hpp"
#include "stack_tester_std.hpp"

StackTesterStd::StackTesterStd(): method(DEFAULT), size(100), cnt(NULL), headers(false), _stop(false), __out("std.txt") {}
StackTesterStd::~StackTesterStd() { 
	std::cerr << "deleting..." << std::endl;
	delete cnt;
	std::cerr << "deleted" << std::endl;
	if (_stop) {
		std::cerr << "check for leaks now, press enter to end test" << std::endl;
		getchar();
		getchar();
	}
}

StackTesterStd& StackTesterStd::withContainer() { method = CONTAINER; return *this; }
StackTesterStd& StackTesterStd::withHeaders() {  headers = true; return *this; }
StackTesterStd& StackTesterStd::withStop() {  _stop = true; return *this; }
StackTesterStd& StackTesterStd::withCopy() { method = COPY; return *this; }
StackTesterStd& StackTesterStd::withSize(size_t s) {
	size = s;
	if (size < 10) {
		__out << "size set to 10 (minimum)" << std::endl;
		size = 10; }
	return *this;
}

void StackTesterStd::build() { construct_map(); }

void StackTesterStd::execute(const std::string& testName) {
	
	printHeader("Construction");
	construct_map();
	printHeader("Construction completed");

	if (testName == "" || testName.find("push_pop") != std::string::npos) { printHeader("push_pop"); test_push_pop(); }
	if (testName == "" || testName.find("capacity") != std::string::npos) { printHeader("capacity"); test_capacity(); }
	if (testName == "" || testName.find("relational") != std::string::npos) { printHeader("relational"); test_relational(); }
}

void StackTesterStd::error(std::string str) { __out << "[ERROR]: " << str << std::endl; }
void StackTesterStd::printHeader(std::string testName) { if (headers) __out << "[TEST]: " << testName << std::endl; }
int StackTesterStd::generateNumber(size_t mod = 10000) const { int r = rand(); r *= r < 0 ? -1 : 1; return r % mod; }
std::string StackTesterStd::generateWord(size_t s = 1) const {
	std::string abc = "abcdefghijklmnopqrstuvwxyz1234567890";
	std::string ret = "";
	for (size_t i = 0; i < s; i++)
		ret += abc.c_str()[generateNumber(abc.length())];
	return ret;
}

void StackTesterStd::construct_map() {
	if (cnt != NULL) return ;
	switch (method)
	{
	case COPY:
		construct_copy();
		break;
	case CONTAINER:
		construct_container();
		break;
	default:
		construct_default();
		break;
	}
}

void StackTesterStd::construct_default() {
	__out << "default map constructor" << std::endl;
	std::cerr << "default map constructor" << std::endl;
	cnt = new t_stack();

	ProgressBar *bar = new ProgressBar();
	for (size_t i = 0; i < size; i++) {
		bar->setProgress(i, size);
		cnt->push(generateWord());
	}
	delete bar; std::cerr << std::endl;
}

void StackTesterStd::construct_container() {
	__out << "container map constructor" << std::endl;
	std::cerr << "container map constructor" << std::endl;
	std::cerr << "filling vector" << std::endl;
	std::vector<std::string> v;
	ProgressBar *bar = new ProgressBar();
	for (size_t i = 0; i < size; i++) {
		bar->setProgress(i, size);
		v.push_back(generateWord());
	}
	delete bar; std::cerr << std::endl;
	std::cerr << "calling range iterator (without loading bar)" << std::endl;
	cnt = new t_stack(v);
}

void StackTesterStd::construct_copy() {
	__out << "copy map constructor" << std::endl;
	std::cerr << "copy map constructor" << std::endl;
	std::cerr << "calling default map constructor (to be copied)" << std::endl;
	construct_default();
	t_stack *aux = cnt;
	std::cerr << "calling copy constructor" << std::endl;
	cnt = new t_stack(*aux);
	std::cerr << "destroying temporary map (to be copied)" << std::endl;
	delete aux;
}

void StackTesterStd::test_capacity() {
	t_stack emptyMap;
	__out << "Capacity test" << std::endl;
	std::cerr << "Capacity test" << std::endl;

	__out << cnt->top() << std::endl;

	__out << emptyMap.empty() << ", " << emptyMap.size() << std::endl;
	__out << cnt->empty() << ", " << cnt->size() << std::endl;
}

void StackTesterStd::test_relational() {
	__out << "Test relational" << std::endl;
	std::cerr << "Test relational" << std::endl;
	t_stack map1;
	map1 = *cnt;

	__out << cnt->top() << std::endl;

	__out << (map1 == *cnt)
	<< (map1 < *cnt)
	<< (map1 > *cnt)
	<< (map1 <= *cnt)
	<< (map1 >= *cnt) << std::endl;

	//map1[cnt->size() / 2].append("42");
	map1.push(map1.top());
	cnt->pop();

	__out << cnt->top() << std::endl;

	__out << (map1 == *cnt)
	<< (map1 < *cnt)
	<< (map1 > *cnt)
	<< (map1 <= *cnt)
	<< (map1 >= *cnt) << std::endl;

	__out << cnt->top() << std::endl;
}

void StackTesterStd::test_push_pop() {
	__out << "Test push pop" << std::endl;
	std::cerr << "Test push pop" << std::endl;

	while (cnt->size() > 2)
	{
		__out << cnt->top();
		cnt->pop();
		cnt->pop();
		__out << cnt->top();
		cnt->push(generateWord());
	}
}