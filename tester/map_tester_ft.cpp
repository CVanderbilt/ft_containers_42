#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include "../includes/map.hpp"
#include "progressBar.hpp"
#include "map_tester_ft.hpp"

MapTesterFt::MapTesterFt(): method(DEFAULT), size(100), cnt(NULL), headers(false), _stop(false), __out("ft.txt") {}
MapTesterFt::~MapTesterFt() { 
	std::cerr << "deleting..." << std::endl;
	delete cnt;
	std::cerr << "deleted" << std::endl;
	if (_stop) {
		std::cerr << "check for leaks now, press enter to end test" << std::endl;
		getchar();
		getchar();
	}
}

MapTesterFt& MapTesterFt::withIterators() { method = RANGE; return *this; }
MapTesterFt& MapTesterFt::withHeaders() {  headers = true; return *this; }
MapTesterFt& MapTesterFt::withStop() {  _stop = true; return *this; }
MapTesterFt& MapTesterFt::withCopy() { method = COPY; return *this; }
MapTesterFt& MapTesterFt::withSize(size_t s) {
	size = s;
	if (size < 10) {
		__out << "size set to 10 (minimum)" << std::endl;
		size = 10; }
	return *this;
}

void MapTesterFt::build() { construct_map(); }

void MapTesterFt::execute(const std::string& testName) {
	
	printHeader("Construction");
	construct_map();
	printHeader("Construction completed");
	printMap();
	for (iterator it = cnt->begin(); it != cnt->end(); it++)
		__out << "(" << it->first << ":" << it->second << ")" << std::endl;

	if (testName == "" || testName.find("capacity") != std::string::npos) { printHeader("capacity"); test_capacity(); }
	if (testName == "" || testName.find("element_access") != std::string::npos) { printHeader("element_access"); test_element_access(); }
	if (testName == "" || testName.find("operator_equal") != std::string::npos) { printHeader("operator_equal"); test_equal_operator(); }
	if (testName == "" || testName.find("iterators") != std::string::npos) { printHeader("iterators"); test_iterators(); }
	if (testName == "" || testName.find("erase") != std::string::npos) { printHeader("erase"); test_erase(); }
	if (testName == "" || testName.find("swap") != std::string::npos) { printHeader("swap"); test_swap(); }
	if (testName == "" || testName.find("find") != std::string::npos) { printHeader("find"); test_find(); }
	if (testName == "" || testName.find("bound") != std::string::npos) { printHeader("bound"); test_bounds(); }
	if (testName == "" || testName.find("hint") != std::string::npos) { printHeader("hint"); test_hinted_insert(); }
	if (testName == "" || testName.find("insert") != std::string::npos) { printHeader("insert"); test_insert(); }
	if (testName == "" || testName.find("relational") != std::string::npos) { printHeader("relational"); test_relational(); }
}

MapTesterFt::iterator MapTesterFt::getRandomIterator() {
	size_t pos = generateNumber(cnt->size());
	iterator ret = cnt->begin();

	for (size_t i = 0; i < pos; i++)
		ret++;
	return ret;
}
	
void MapTesterFt::error(std::string str) { __out << "[ERROR]: " << str << std::endl; }
void MapTesterFt::printMap() { 
	for (iterator it = cnt->begin(); it != cnt->end(); it++) __out << it->first << it->second; __out << std::endl;
}
void MapTesterFt::printHeader(std::string testName) { if (headers) __out << "[TEST]: " << testName << std::endl; }
int MapTesterFt::generateNumber(size_t mod = 10000) const { int r = rand(); r *= r < 0 ? -1 : 1; return r % mod; }
std::string MapTesterFt::generateWord(size_t s = 1) const {
	std::string abc = "abcdefghijklmnopqrstuvwxyz1234567890";
	std::string ret = "";
	for (size_t i = 0; i < s; i++)
		ret += abc.c_str()[generateNumber(abc.length())];
	return ret;
}

void MapTesterFt::construct_map() {
	if (cnt != NULL) return ;
	switch (method)
	{
	case COPY:
		construct_map_copy();
		break;
	case RANGE:
		construct_map_iterator();
		break;
	default:
		construct_map_default();
		break;
	}
}

void MapTesterFt::construct_map_default() {
	__out << "default map constructor" << std::endl;
	std::cerr << "default map constructor" << std::endl;
	cnt = new t_map();

	ProgressBar *bar = new ProgressBar();
	for (size_t i = 0; i < size; i++) {
		bar->setProgress(i, size);
		(*cnt)[generateNumber()] = generateWord();
	}
	delete bar; std::cerr << std::endl;
}

void MapTesterFt::construct_map_iterator() {
	__out << "range map constructor" << std::endl;
	std::cerr << "range map constructor" << std::endl;
	std::vector<t_pair> v;
	std::cerr << "filling vector" << std::endl;
	ProgressBar *bar = new ProgressBar();
	for (size_t i = 0; i < size; i++) {
		bar->setProgress(i, size);
		v.push_back(t_pair(generateNumber(), generateWord()));
	}
	delete bar; std::cerr << std::endl;
	std::cerr << "calling range iterator (without loading bar)" << std::endl;
	cnt = new t_map(v.begin(), v.end());
}

void MapTesterFt::construct_map_copy() {
	__out << "copy map constructor" << std::endl;
	std::cerr << "copy map constructor" << std::endl;
	std::cerr << "calling default map constructor (to be copied)" << std::endl;
	construct_map_default();
	t_map *aux = cnt;
	std::cerr << "calling copy constructor" << std::endl;
	cnt = new t_map(*aux);
	std::cerr << "destroying temporary map (to be copied)" << std::endl;
	delete aux;
}

void MapTesterFt::test_equal_operator() {
	__out << "operator= test" << std::endl;
	std::cerr << "operator= test" << std::endl;
	t_map newMap = *cnt;
	
	const_iterator it1 = newMap.begin();
	const_iterator it2 = cnt->begin();
	for (; it1 != newMap.end(); it1++)
	{
		__out << it1->first << it2->first << "|" << std::endl;
		if (it1->first != it2->first || it1->second != it2->second)
			error("after using operator= iterator contents doesnt match");
		it2++;
	}
	//if (it1 != it2)
	if (it2 != cnt->end())
		error("after looping both maps, both iterators are not equal to map.end()");
}

void MapTesterFt::test_iterators() {
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
		__out << cit->first << ":" << cit->second << std::endl;
	}
	delete bar; std::cerr << std::endl;
	
	it->second = diff;
	it++;
	it++;
	it++;
	it->second = diff;
	it = cnt->end();
	it--;
	it--;
	it->second = diff;

	std::cerr << "reverse iterating through map" << std::endl;
	bar = new ProgressBar();
	i = 0;
	for (const_reverse_iterator rit = cnt->rbegin(); rit != cnt->rend(); rit++) {
		bar->setProgress(i++, s);
		__out << rit->first << ":" << rit->second << std::endl;
	}
	delete bar; std::cerr << std::endl;
}

void MapTesterFt::test_capacity() {
	t_map emptyMap;
	__out << "Capacity test" << std::endl;
	std::cerr << "Capacity test" << std::endl;

	__out << emptyMap.empty() << ", " << emptyMap.size() << std::endl;
	__out << cnt->empty() << ", " << cnt->size() << std::endl;
}

void MapTesterFt::test_element_access() {
	size_t max_extra_elements = cnt->size() / 10;
	size_t extra_elements = 0;
	__out << "test element access" << std::endl;
	std::cerr << "test element access" << std::endl;
	ProgressBar *bar = new ProgressBar();
	std::string str = "+++++";
	for (size_t i = 0; i < size; i++) {
		bar->setProgress((float)i / (float)size);
		int n = generateNumber(size);

		try { (*cnt).at(n).append("foundWithAt"); }
		catch(const std::exception& e) {
			if (extra_elements < max_extra_elements) {
				(*cnt)[n] = str;
				(*cnt).at(n).append("notFoundWithAt");
				extra_elements++;
			}
		}
	}
	delete bar; std::cerr << std::endl;
}

void MapTesterFt::test_erase() {
	size_t mapSize = cnt->size();
	__out << "Test erase: (" << mapSize << ")" << std::endl;
	std::cerr << "Test erase: (" << mapSize << ")" << std::endl;
	size_t n = cnt->size() / 4;

	std::cerr << "Erase single elements" << std::endl;
	ProgressBar *bar = new ProgressBar();
	for (size_t i = 0; i < n; i++) {
		bar->setProgress(i, n);
		iterator it = getRandomIterator();
		cnt->erase(it);
		int key = generateNumber();
		__out << cnt->erase(key);
	}
	delete bar; std::cerr << std::endl;
	__out << std::endl;

	iterator it1 = getRandomIterator();
	iterator it2 = getRandomIterator();
	iterator first, last;
	if (it1->first < it2->first)
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

void MapTesterFt::test_swap() {
	__out << "Test swap" << std::endl;
	std::cerr << "Test swap" << std::endl;
	t_map mapa;
	for (int i = 0; i < 100; i++)
		mapa[generateNumber()] = generateWord();

	cnt->swap(mapa);
	void *ptr = cnt;
	printMap();
	if (ptr != cnt)
		error("swap modified the memory pointer of the map somehow!!!");
	mapa.swap(*cnt);
	printMap();
}

void MapTesterFt::test_find() {
	__out << "Test find" << std::endl;
	std::cerr << "Test find" << std::endl;
	iterator it = getRandomIterator();

	if (cnt->find(it->first) != it)
		error("iterator returend doesnt match with expected iterator (operator==)");
	for (size_t i = 0; i < cnt->size(); i++) {
		int n = generateNumber();
		it = cnt->find(n);
		if (it != cnt->end()) __out << '(' << it->first << ':' << it->second << ')';
		else __out << "(-:-)";
		__out << " -> count: " << cnt->count(n) << std::endl;
	}
}

void MapTesterFt::test_bounds() {
	__out << "Test bounds" << std::endl;
	std::cerr << "Test bounds" << std::endl;
	for (size_t i = 0; i < cnt->size(); i++) {
		int n = generateNumber();
		iterator lit = cnt->lower_bound(n);
		iterator uit = cnt->upper_bound(n);
		tt_pair p = cnt->equal_range(n);
		if (lit != cnt->end()) __out << "l:" << lit->first << lit->second << ';';
		if (uit != cnt->end()) __out << "r:" << uit->first << uit->second << ';';
		if (p.first != lit || p.second != uit) error("equal range expected to be (lower_bound, upper_bound)");
	}
}

void MapTesterFt::test_hinted_insert() {
	__out << "Test insert with hint" << std::endl;
	std::cerr << "Test insert with hint" << std::endl;
	t_map newMap;

	iterator hint = newMap.end();
	ProgressBar *bar = new ProgressBar;
	size_t i = 0;
	size_t cntsize = cnt->size();
	for (const_iterator cit = cnt->begin(); cit != cnt->end(); cit++) {
		hint = newMap.insert(hint, t_pair(cit->first, cit->second));
		bar->setProgress(i++, cntsize);
	}
	delete bar; std::cerr << std::endl;
	for (const_iterator cit = newMap.begin(); cit != newMap.end(); cit++)
		__out << cit->first << cit->second;
	__out << std::endl;
}

void MapTesterFt::test_insert() {
	__out << "Test insert without hint" << std::endl;
	std::cerr << "Test insert without hint" << std::endl;
	t_map newMap;

	ProgressBar *bar = new ProgressBar;
	size_t i = 0;
	size_t cntsize = cnt->size();
	for (const_iterator cit = cnt->begin(); cit != cnt->end(); cit++) {
		newMap.insert(t_pair(cit->first, cit->second));
		bar->setProgress(i++, cntsize);
	}
	delete bar; std::cerr << std::endl;
	for (const_iterator cit = newMap.begin(); cit != newMap.end(); cit++)
		__out << cit->first << cit->second;
	__out << std::endl;
}

void MapTesterFt::test_relational() {
	__out << "Test relational" << std::endl;
	std::cerr << "Test relational" << std::endl;
	t_map map1;
	map1 = *cnt;

	__out << (map1 == *cnt)
	<< (map1 < *cnt)
	<< (map1 > *cnt)
	<< (map1 <= *cnt)
	<< (map1 >= *cnt) << std::endl;

	size_t i = 0;
	while (i < cnt->size())
		i++;
	map1[i].append("42");

	__out << (map1 == *cnt)
	<< (map1 < *cnt)
	<< (map1 > *cnt)
	<< (map1 <= *cnt)
	<< (map1 >= *cnt) << std::endl;
}