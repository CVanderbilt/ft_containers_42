#ifndef MAP_TESTER_FT_HPP
# define MAP_TESTER_FT_HPP

# include <map>
# include <vector>
# include <iostream>
# include <string>
# include <fstream>
# include "../includes/map.hpp"
# include "progressBar.hpp"

class MapTesterFt {
public:

	typedef ft::map<int, std::string>		t_map;
	typedef ft::pair<int, std::string>		t_pair;
	typedef t_map::iterator					iterator;
	typedef ft::pair<iterator, iterator>	tt_pair;
	typedef t_map::const_iterator			const_iterator;
	typedef t_map::const_reverse_iterator	const_reverse_iterator;

	enum constructor { DEFAULT, COPY, RANGE };

	MapTesterFt();
	~MapTesterFt();

	MapTesterFt& withIterators();
	MapTesterFt& withHeaders();
	MapTesterFt& withStop();
	MapTesterFt& withCopy();
	MapTesterFt& withSize(size_t s);

	void build();
	void execute(const std::string& testName);


private:

	constructor method;
	size_t size;
	t_map *cnt;
	bool headers;
	bool _stop;
	std::ofstream __out;
	
	iterator getRandomIterator(int chance);
	void error(std::string str);
	void printMap();
	void printHeader(std::string testName);
	int generateNumber(size_t mod) const;
	std::string generateWord(size_t s) const;

	void construct_map();
	
	void construct_map_default();
	void construct_map_iterator();
	void construct_map_copy();

	void test_equal_operator();
	void test_iterators();
	void test_capacity();
	void test_element_access();
	void test_erase();
	void test_swap();
	void test_find();
	void test_bounds();
	void test_hinted_insert();
	void test_insert();
	void test_relational();

};

#endif