#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include "progressBar.hpp"
#include "vector_tester_ft.hpp"
#include "../includes/vector.hpp"
#include "stack_tester_ft.hpp"

StackTesterFt::StackTesterFt(): method(DEFAULT), size(100), cnt(NULL), headers(false), _stop(false), __out("ft.txt") {}
StackTesterFt::~StackTesterFt() { 
	std::cerr << "deleting..." << std::endl;
	delete cnt;
	std::cerr << "deleted" << std::endl;
	if (_stop) {
		std::cerr << "check for leaks now, press enter to end test" << std::endl;
		getchar();
		getchar();
	}
}

StackTesterFt& StackTesterFt::withContainer() { method = CONTAINER; return *this; }
StackTesterFt& StackTesterFt::withHeaders() {  headers = true; return *this; }
StackTesterFt& StackTesterFt::withStop() {  _stop = true; return *this; }
StackTesterFt& StackTesterFt::withCopy() { method = COPY; return *this; }
StackTesterFt& StackTesterFt::withSize(size_t s) {
	size = s;
	if (size < 10) {
		__out << "size set to 10 (minimum)" << std::endl;
		size = 10; }
	return *this;
}

void StackTesterFt::build() { construct_map(); }

void StackTesterFt::execute(const std::string& testName) {
	
	printHeader("Construction");
	construct_map();
	printHeader("Construction completed");

	if (testName == "" || testName.find("push_pop") != std::string::npos) { printHeader("push_pop"); test_push_pop(); }
	if (testName == "" || testName.find("capacity") != std::string::npos) { printHeader("capacity"); test_capacity(); }
	if (testName == "" || testName.find("relational") != std::string::npos) { printHeader("relational"); test_relational(); }
}

void StackTesterFt::error(std::string str) { __out << "[ERROR]: " << str << std::endl; }
void StackTesterFt::printHeader(std::string testName) { if (headers) __out << "[TEST]: " << testName << std::endl; }
int StackTesterFt::generateNumber(size_t mod = 10000) const { int r = rand(); r *= r < 0 ? -1 : 1; return r % mod; }
std::string StackTesterFt::generateWord(size_t s = 1) const {
	std::string abc = "abcdefghijklmnopqrstuvwxyz1234567890";
	std::string ret = "";
	for (size_t i = 0; i < s; i++)
		ret += abc.c_str()[generateNumber(abc.length())];
	return ret;
}

void StackTesterFt::construct_map() {
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

void StackTesterFt::construct_default() {
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

void StackTesterFt::construct_container() {
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

void StackTesterFt::construct_copy() {
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

void StackTesterFt::test_capacity() {
	t_stack emptyMap;
	__out << "Capacity test" << std::endl;
	std::cerr << "Capacity test" << std::endl;

	__out << cnt->top() << std::endl;

	__out << emptyMap.empty() << ", " << emptyMap.size() << std::endl;
	__out << cnt->empty() << ", " << cnt->size() << std::endl;
}

void StackTesterFt::test_relational() {
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

void StackTesterFt::test_push_pop() {
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