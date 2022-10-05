#include <map>
#include <vector>
#include <iostream>
#include "progressBar.hpp"

class MapTesterStd {
public:

	typedef std::map<int, std::string>		t_map;
	typedef std::pair<int, std::string>		t_pair;
	typedef std::map<int, std::string>::iterator					iterator;
	typedef t_map::const_iterator			const_iterator;
	typedef t_map::const_reverse_iterator	const_reverse_iterator;

	enum constructor { DEFAULT, COPY, RANGE };

	MapTesterStd(): method(DEFAULT), size(100), headers(false), _stop(false) {}
	~MapTesterStd() { 
		std::cerr << "deleting..." << std::endl;
		delete cnt;
		std::cerr << "deleted" << std::endl;
		if (_stop) {
			std::cout << "check for leaks now, press enter to end test" << std::endl;
			getchar();
		}
	}

	MapTesterStd& withIterators() { method = RANGE; return *this; }
	MapTesterStd& withHeaders() {  headers = true; return *this; }
	MapTesterStd& withStop() {  _stop = true; return *this; }
	MapTesterStd& withCopy() { method = COPY; return *this; }
	MapTesterStd& withSize(size_t s) {
		size = s;
		if (size < 10) {
			std::cout << "size set to 10 (minimum)" << std::endl;
			size = 10; }
		return *this;
	}

	void execute(std::string testName) {
		printHeader("Construction");
		construct_map();
		printHeader("Construction completed");
		printMap();
		for (iterator it = cnt->begin(); it != cnt->end(); it++)
			std::cout << "(" << it->first << ":" << it->second << ")" << std::endl;

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
	
	void error(std::string str) { std::cout << "[ERROR]: " << str << std::endl; }
	void printMap() { for (iterator it = cnt->begin(); it != cnt->end(); it++) std::cout << it->first << it->second; std::cout << std::endl; }
	void printHeader(std::string testName) { if (headers) std::cout << "[TEST]: " << testName << std::endl; }
	int generateNumber(size_t mod = 10000) const { int r = rand(); r *= r < 0 ? -1 : 1; return r % mod; }
	std::string generateWord(size_t s = 1) const {
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
		std::cout << "default map constructed" << std::endl;
		cnt = new t_map();

		for (int i = 0; i < size; i++)
			(*cnt)[generateNumber()] = generateWord();
	}

	void construct_map_iterator() {
		std::cout << "range map constructed" << std::endl;
		std::vector<t_pair> v;
		for (int i = 0; i < size; i++)
			v.push_back(t_pair(generateNumber(), generateWord()));
		cnt = new t_map(v.begin(), v.end());
		/*cnt = new t_map();
		for (std::vector<t_pair>::iterator it = v.begin(); it != v.end(); it++)
			cnt->insert(ft::make_pair(it->first, it->second));
		//*/
	}

	void construct_map_copy() {
		std::cout << "copy map constructed" << std::endl;
		construct_map_default();
		t_map *aux = cnt;
		cnt = new t_map(*aux);
		delete aux;
	}

	void test_equal_operator() {
		t_map newMap = *cnt;
		
		const_iterator it1 = newMap.begin();
		const_iterator it2 = cnt->begin();
		for (; it1 != newMap.end(); it1++)
		{
			std::cout << it1->first << it2->first << "|" << std::endl;
			if (it1->first != it2->first || it1->second != it2->second)
				error("after using operator= iterator contents doesnt match");
			it2++;
		}
		if (it2 != cnt->end())
			error("after looping both maps, both iterators are not equal to map.end()");
		/*if (it1 != it2) {
			error("after looping both maps, both iterators are not equal to map.end()");
			std::cout << it2->first << std::endl;
			std::cout << (it2 == cnt->end()) << std::endl;
			//Check why is this happening it1 == end(), it2 == end(), it1 != it2
		}*/
	}

	void test_iterators() {
		iterator it = cnt->begin();
		std::string diff = "-----";

		for (const_iterator cit = cnt->begin(); cit != cnt->end(); cit++)
			std::cout << cit->first << ":" << cit->second << std::endl;
		
		it->second = diff;
		it++;
		it++;
		it++;
		it->second = diff;
		it = cnt->end();
		it--;
		it--;
		it->second = diff;

		for (const_reverse_iterator rit = cnt->rbegin(); rit != cnt->rend(); rit++)
			std::cout << rit->first << ":" << rit->second << std::endl;
	}

	void test_capacity() {
		t_map emptyMap;

		std::cout << emptyMap.empty() << ", " << emptyMap.size() << std::endl;
		std::cout << cnt->empty() << ", " << cnt->size() << std::endl;
	}

	void test_element_access() {
		ProgressBar *bar = new ProgressBar();
		std::string str = "+++++";
		for (int i = 0; i < size; i++) {
			bar->setProgress((float)i / (float)size);
			int n = generateNumber(size);

			try { (*cnt).at(n).append("foundWithAt"); }
			catch(const std::exception& e) {
				(*cnt)[n] = str;
				(*cnt).at(n).append("notFoundWithAt");
			}
		}
		delete bar;
		std::cerr << std::endl << "element access test ended" << std::endl;
	}

	//modifiers tested implicitly
	void test_erase() {
		size_t n = cnt->size() / 4;

		for (int i = 0; i < n; i++) { cnt->erase(getRandomIterator()); std::cout << cnt->erase(generateNumber()); } std::cout << std::endl;

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
		printMap();
		cnt->erase(first, last);
		printMap();
	}

	void test_swap() {
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