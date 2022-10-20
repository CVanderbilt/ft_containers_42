#ifndef VECTOR_TESTER_FT
# define VECTOR_TESTER_FT
# include <map>
# include <vector>
# include <iostream>
# include <string>
# include <fstream>
# include "../includes/vector.hpp"
# include "progressBar.hpp"

class VectorTesterFt {
public:

	typedef ft::vector<int, std::string>		t_vector;
	typedef t_vector::iterator					iterator;
	typedef t_vector::const_iterator			const_iterator;
	typedef t_vector::const_reverse_iterator	const_reverse_iterator;

	enum constructor { DEFAULT, COPY, RANGE };

	VectorTesterFt();
	~VectorTesterFt();

	VectorTesterFt& withIterators();
	VectorTesterFt& withHeaders();
	VectorTesterFt& withStop();
	VectorTesterFt& withCopy();
	VectorTesterFt& withSize(size_t s);

	void build();
	void execute(const std::string& testName);


private:

	constructor method;
	size_t size;
	t_vector *cnt;
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
	void test_insert();
	void test_relational();

};

#endif