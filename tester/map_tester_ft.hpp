#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include "../includes/map.hpp"
#include "progressBar.hpp"

class MapTesterFt {
public:

	typedef ft::map<int, std::string>		t_map;
	typedef ft::pair<int, std::string>		t_pair;
	typedef t_map::iterator					iterator;
	typedef t_map::const_iterator			const_iterator;
	typedef t_map::const_reverse_iterator	const_reverse_iterator;

	enum constructor { DEFAULT, COPY, RANGE };

	MapTesterFt(): method(DEFAULT), size(100), headers(false), _stop(false), __out("ft.txt") {}
	~MapTesterFt() { 
		std::cerr << "deleting..." << std::endl;
		delete cnt;
		std::cerr << "deleted" << std::endl;
		if (_stop) {
			//__out << "check for leaks now, press enter to end test" << std::endl;
			getchar();
		}
	}

	MapTesterFt& withIterators() { method = RANGE; return *this; }
	MapTesterFt& withHeaders() {  headers = true; return *this; }
	MapTesterFt& withStop() {  _stop = true; return *this; }
	MapTesterFt& withCopy() { method = COPY; return *this; }
	MapTesterFt& withSize(size_t s) {
		size = s;
		if (size < 10) {
			__out << "size set to 10 (minimum)" << std::endl;
			size = 10; }
		return *this;
	}

	void execute(std::string testName) {
		
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
	}

	iterator getRandomIterator(int chance = 1) {
		size_t pos = generateNumber(cnt->size());
		iterator ret = cnt->begin();

		for (int i = 0; i < pos; i++)
			ret++;
		return ret;
	}

private:

	constructor method;
	size_t size;
	t_map *cnt;
	bool headers;
	bool _stop;
	std::ofstream __out;
	
	void error(std::string str) { __out << "[ERROR]: " << str << std::endl; }
	void printMap() { 
		for (iterator it = cnt->begin(); it != cnt->end(); it++) __out << it->first << it->second; __out << std::endl;
	}
	void printHeader(std::string testName) { if (headers) __out << "[TEST]: " << testName << std::endl; }
	int generateNumber(size_t mod = 10000) const { int r = rand(); r *= r < 0 ? -1 : 1; return r % mod; }
	std::string generateWord(size_t s = 50) const {
		std::string abc = "abcdefghijklmnopqrstuvwxyz1234567890";
		std::string ret = "";
		for (int i = 0; i < s; i++)
			ret += abc.c_str()[generateNumber(abc.length())];
		return ret;
	}

	void construct_map() {
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
	
	void construct_map_default() {
		__out << "default map constructor" << std::endl;
		std::cerr << "default map constructor" << std::endl;
		cnt = new t_map();

		ProgressBar *bar = new ProgressBar();
		for (int i = 0; i < size; i++) {
			bar->setProgress(i, size);
			(*cnt)[generateNumber()] = generateWord();
		}
		delete bar; std::cerr << std::endl;
	}

	void construct_map_iterator() {
		__out << "range map constructor" << std::endl;
		std::cerr << "range map constructor" << std::endl;
		std::vector<t_pair> v;
		std::cerr << "filling vector" << std::endl;
		ProgressBar *bar = new ProgressBar();
		for (int i = 0; i < size; i++) {
			bar->setProgress(i, size);
			v.push_back(t_pair(generateNumber(), generateWord()));
		}
		delete bar; std::cerr << std::endl;
		std::cerr << "calling range iterator (without loading bar)" << std::endl;
		cnt = new t_map(v.begin(), v.end());
	}

	void construct_map_copy() {
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

	void test_equal_operator() {
		t_map newMap = *cnt;
		
		__out << "operator= test" << std::endl;
		std::cerr << "operator= test" << std::endl;
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

	void test_iterators() {
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

	void test_capacity() {
		t_map emptyMap;
		__out << "Capacity test" << std::endl;
		std::cerr << "Capacity test" << std::endl;

		__out << emptyMap.empty() << ", " << emptyMap.size() << std::endl;
		__out << cnt->empty() << ", " << cnt->size() << std::endl;
	}

	void test_element_access() {
		size_t max_extra_elements = cnt->size() / 10;
		size_t extra_elements = 0;
		__out << "test element access" << std::endl;
		std::cerr << "test element access" << std::endl;
		ProgressBar *bar = new ProgressBar();
		std::string str = "+++++";
		for (int i = 0; i < size; i++) {
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

	//modifiers tested implicitly
	void test_erase() {
		size_t mapSize = cnt->size();
		__out << "Test erase: (" << mapSize << ")" << std::endl;
		std::cerr << "Test erase: (" << mapSize << ")" << std::endl;
		size_t n = cnt->size() / 4;

		std::cerr << "Erase single elements" << std::endl;
		ProgressBar *bar = new ProgressBar();
		for (int i = 0; i < n; i++) {
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

	void test_swap() {
		__out << "Test swap" << std::endl;
		std::cerr << "Test swap" << std::endl;
		t_map mapa;
		for (int i = 0; i < size / 2; i++)
			mapa[generateNumber()] = generateWord();

		cnt->swap(mapa);
		void *ptr = cnt;
		printMap();
		if (ptr != cnt)
			error("swap modified the memory pointer of the map somehow!!!");
		mapa.swap(*cnt);
		printMap();
	}
	
	/*
	// observers:
	//allocator_type get_allocator() const
	//key_compare    key_comp() const
	//value_compare  value_comp()    const

	// map operations:
	//iterator find(const key_type& k)
	//const_iterator find(const key_type& k) const

	//size_type      count(const key_type& k)
	
	//iterator lower_bound(const key_type& k)
	//const_iterator lower_bound(const key_type& k) const

	//iterator upper_bound(const key_type& k)
	//const_iterator upper_bound(const key_type& k) const

	//pair<iterator,iterator>             equal_range(const key_type& k)
	//pair<const_iterator,const_iterator> equal_range(const key_type& k) const

	*/


};