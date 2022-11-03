#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include "progressBar.hpp"
#include "vector_tester_ft.hpp"
#include"../includes/vector.hpp"

VectorTesterFt::VectorTesterFt(): method(DEFAULT), size(100), cnt(NULL), headers(false), _stop(false), __out("ft.txt") {}
VectorTesterFt::~VectorTesterFt() { 
	std::cerr << "deleting..." << std::endl;
	delete cnt;
	std::cerr << "deleted" << std::endl;
	if (_stop) {
		std::cerr << "check for leaks now, press enter to end test" << std::endl;
		getchar();
		getchar();
	}
}

VectorTesterFt& VectorTesterFt::withIterators() { method = RANGE; return *this; }
VectorTesterFt& VectorTesterFt::withHeaders() {  headers = true; return *this; }
VectorTesterFt& VectorTesterFt::withStop() {  _stop = true; return *this; }
VectorTesterFt& VectorTesterFt::withCopy() { method = COPY; return *this; }
VectorTesterFt& VectorTesterFt::withSize(size_t s) {
	size = s;
	if (size < 10) {
		__out << "size set to 10 (minimum)" << std::endl;
		size = 10; }
	return *this;
}

void VectorTesterFt::build() { construct_map(); }

void VectorTesterFt::execute(const std::string& testName) {
	
	printHeader("Construction");
	construct_map();
	printHeader("Construction completed");
	printMap();
	for (iterator it = cnt->begin(); it != cnt->end(); it++)
		__out << "(" << *it << ")" << std::endl;

	if (testName == "" || testName.find("capacity") != std::string::npos) { printHeader("capacity"); test_capacity(); }
	if (testName == "" || testName.find("element_access") != std::string::npos) { printHeader("element_access"); test_element_access(); }
	if (testName == "" || testName.find("operator_equal") != std::string::npos) { printHeader("operator_equal"); test_equal_operator(); }
	if (testName == "" || testName.find("iterators") != std::string::npos) { printHeader("iterators"); test_iterators(); }
	if (testName == "" || testName.find("erase") != std::string::npos) { printHeader("erase"); test_erase(); }
	if (testName == "" || testName.find("swap") != std::string::npos) { printHeader("swap"); test_swap(); }
	if (testName == "" || testName.find("insert") != std::string::npos) { printHeader("insert"); test_insert(); }
	if (testName == "" || testName.find("relational") != std::string::npos) { printHeader("relational"); test_relational(); }
}

VectorTesterFt::iterator VectorTesterFt::getRandomIterator() {
	size_t pos = generateNumber(cnt->size());
	iterator ret = cnt->begin();

	for (size_t i = 0; i < pos; i++)
		ret++;
	return ret;
}
	
void VectorTesterFt::error(std::string str) { __out << "[ERROR]: " << str << std::endl; }
void VectorTesterFt::printMap() { 
	for (iterator it = cnt->begin(); it != cnt->end(); it++) __out << *it; __out << std::endl;
}
void VectorTesterFt::printHeader(std::string testName) { if (headers) __out << "[TEST]: " << testName << std::endl; }
int VectorTesterFt::generateNumber(size_t mod = 10000) const { int r = rand(); r *= r < 0 ? -1 : 1; return r % mod; }
std::string VectorTesterFt::generateWord(size_t s = 1) const {
	std::string abc = "abcdefghijklmnopqrstuvwxyz1234567890";
	std::string ret = "";
	for (size_t i = 0; i < s; i++)
		ret += abc.c_str()[generateNumber(abc.length())];
	return ret;
}

void VectorTesterFt::construct_map() {
	if (cnt != NULL) return ;
	switch (method)
	{
	case COPY:
		construct_copy();
		break;
	case RANGE:
		construct_iterator();
		break;
	default:
		construct_default();
		break;
	}
}

void VectorTesterFt::construct_default() {
	__out << "default map constructor" << std::endl;
	std::cerr << "default map constructor" << std::endl;
	cnt = new t_vector();

	ProgressBar *bar = new ProgressBar();
	for (size_t i = 0; i < size; i++) {
		bar->setProgress(i, size);
		cnt->push_back(generateWord());
	}
	delete bar; std::cerr << std::endl;
}

void VectorTesterFt::construct_iterator() {
	__out << "range map constructor" << std::endl;
	std::cerr << "range map constructor" << std::endl;
	std::cerr << "filling vector" << std::endl;
	std::vector<std::string> v;
	ProgressBar *bar = new ProgressBar();
	for (size_t i = 0; i < size; i++) {
		bar->setProgress(i, size);
		v.push_back(generateWord());
	}
	delete bar; std::cerr << std::endl;
	std::cerr << "calling range iterator (without loading bar)" << std::endl;
	cnt = new t_vector(v.begin(), v.end());
}

void VectorTesterFt::construct_copy() {
	__out << "copy map constructor" << std::endl;
	std::cerr << "copy map constructor" << std::endl;
	std::cerr << "calling default map constructor (to be copied)" << std::endl;
	construct_default();
	t_vector *aux = cnt;
	std::cerr << "calling copy constructor" << std::endl;
	cnt = new t_vector(*aux);
	std::cerr << "destroying temporary map (to be copied)" << std::endl;
	delete aux;
}

void VectorTesterFt::test_equal_operator() {
	__out << "operator= test" << std::endl;
	std::cerr << "operator= test" << std::endl;
	t_vector newMap = *cnt;
	
	const_iterator it1 = newMap.begin();
	const_iterator it2 = cnt->begin();
	for (; it1 != newMap.end(); it1++)
	{
		__out << *it1 << *it2 << "|" << std::endl;
		if (*it1 != *it2)
			error("after using operator= iterator contents doesnt match");
		it2++;
	}
	//if (it1 != it2)
	if (it2 != cnt->end())
		error("after looping both maps, both iterators are not equal to map.end()");
}

void VectorTesterFt::test_iterators() {
	__out << "iterators test" << std::endl;
	std::cerr << "iterators test" << std::endl;
	iterator it = cnt->begin();
	std::string diff = "-----";
	size_t i = 0;
	size_t s = cnt->size();

	std::cerr << "iterating through map" << std::endl;
	ProgressBar *bar = new ProgressBar();
	for (const_iterator cit = cnt->begin(); cit != cnt->end(); cit++) {
		bar->setProgress(i++, s);
		__out << *cit << std::endl;
	}
	delete bar; std::cerr << std::endl;
	
	*it = diff;
	it++;
	it++;
	it++;
	*it = diff;
	it = cnt->end();
	it--;
	it--;
	*it = diff;

	std::cerr << "reverse iterating through map" << std::endl;
	bar = new ProgressBar();
	i = 0;
	for (const_reverse_iterator rit = cnt->rbegin(); rit != cnt->rend(); rit++) {
		bar->setProgress(i++, s);
		__out << *rit << std::endl;
	}
	delete bar; std::cerr << std::endl;
}

void VectorTesterFt::test_capacity() {
	t_vector emptyMap;
	__out << "Capacity test" << std::endl;
	std::cerr << "Capacity test" << std::endl;

	__out << emptyMap.empty() << ", " << emptyMap.size() << std::endl;
	__out << cnt->empty() << ", " << cnt->size() << std::endl;
}

void VectorTesterFt::test_element_access() {
	__out << "test element access" << std::endl;
	std::cerr << "test element access" << std::endl;
	ProgressBar *bar = new ProgressBar();
	size_t iterations = cnt->size() / 4;
	for (size_t i = 0; i < iterations; i++) {
		bar->setProgress((float)i / (float)size);
		try
		{
			std::string& foundStr = cnt->at(generateNumber(cnt->size() + cnt->size() / 10));
			__out << foundStr;
		}
		catch(const std::exception& e)
		{
			__out << "nf";
		}
		__out << '|';
	}
	__out << std::endl;
	delete bar; std::cerr << std::endl;
}

void VectorTesterFt::test_erase() {
	size_t mapSize = cnt->size();
	__out << "Test erase: (" << mapSize << ")" << std::endl;
	std::cerr << "Test erase: (" << mapSize << ")" << std::endl;
	size_t n = cnt->size() / 20;

	std::cerr << "Erase single elements" << std::endl;
	ProgressBar *bar = new ProgressBar();
	for (size_t i = 0; i < n; i++) {
		bar->setProgress(i, n);
		iterator it = getRandomIterator();
		cnt->erase(it);
	}
	delete bar; std::cerr << std::endl;
	__out << std::endl;

	iterator it1 = getRandomIterator();
	iterator it2 = getRandomIterator();
	iterator first, last;
	if (it1 < it2)
	{
		first = it1;
		last = it2;
	} else {
		first = it2;
		last = it1;
	}
	std::cerr << "print map" << std::endl;
	printMap();
	std::cerr << "iterator erase" << std::endl;
	cnt->erase(first, last);
	std::cerr << "print map" << std::endl;
	printMap();
	std::cerr << std::endl;
}

void VectorTesterFt::test_swap() {
	__out << "Test swap" << std::endl;
	std::cerr << "Test swap" << std::endl;
	t_vector mapa;
	for (int i = 0; i < 100; i++)
		mapa.push_back(generateWord());

	cnt->swap(mapa);
	void *ptr = cnt;
	printMap();
	if (ptr != cnt)
		error("swap modified the memory pointer of the map somehow!!!");
	mapa.swap(*cnt);
	printMap();
}

void VectorTesterFt::test_insert() {
	__out << "Test insert" << std::endl;
	std::cerr << "Test insert" << std::endl;
	t_vector newMap;

	for (int i = 0; i < 1000; i++)
		newMap.push_back("insertC");

	ProgressBar *bar = new ProgressBar;
	size_t i = 0;
	size_t cntsize = cnt->size() / 20;
	for (const_iterator cit = cnt->begin(); cit != cnt->end() && i < cntsize; cit++) {
		bar->setProgress(i++, cntsize);
		cnt->insert(getRandomIterator(), "insertA");
	}
	delete bar; std::cerr << std::endl;
	cnt->insert(getRandomIterator(), cnt->size()/10, "insertB");
	cnt->insert(getRandomIterator(), newMap.begin() + 10, newMap.end() - 10);
	printMap();
}

void VectorTesterFt::test_relational() {
	__out << "Test relational" << std::endl;
	std::cerr << "Test relational" << std::endl;
	t_vector map1;
	map1 = *cnt;

	__out << (map1 == *cnt)
	<< (map1 < *cnt)
	<< (map1 > *cnt)
	<< (map1 <= *cnt)
	<< (map1 >= *cnt) << std::endl;

	map1[cnt->size() / 2].append("42");

	__out << (map1 == *cnt)
	<< (map1 < *cnt)
	<< (map1 > *cnt)
	<< (map1 <= *cnt)
	<< (map1 >= *cnt) << std::endl;
}