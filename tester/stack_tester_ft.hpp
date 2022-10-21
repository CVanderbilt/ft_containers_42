#ifndef STACK_TESTER_FT
# define STACK_TESTER_FT

# include <map>
# include <vector>
# include <stack>
# include <iostream>
# include <string>
# include <fstream>
# include "../includes/vector.hpp"
# include "../includes/stack.hpp"
# include "progressBar.hpp"

class StackTesterFt {
public:

	typedef ft::stack<std::string, std::vector<std::string> >	t_stack;

	enum constructor { DEFAULT, COPY, CONTAINER };

	StackTesterFt();
	~StackTesterFt();

	StackTesterFt& withContainer();
	StackTesterFt& withHeaders();
	StackTesterFt& withStop();
	StackTesterFt& withCopy();
	StackTesterFt& withSize(size_t s);

	void build();
	void execute(const std::string& testName);


private:

	constructor method;
	size_t size;
	t_stack *cnt;
	bool headers;
	bool _stop;
	std::ofstream __out;
	
	void error(std::string str);
	void printHeader(std::string testName);
	int generateNumber(size_t mod) const; // maybe sobra
	std::string generateWord(size_t s) const;

	void construct_map();
	
	void construct_default();
	void construct_copy();
	void construct_container();

	void test_capacity();
	void test_relational();
	void test_push_pop();
};

#endif