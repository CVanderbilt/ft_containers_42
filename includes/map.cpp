#include "map.hpp"
#include <map>

#define TEST_HEADER_SIZE 50

//constructor and destructor automatically tested by using them
	/*
	required tests
	operator=
	get allocator (not needed)
	begin end (iterator tests):
		- traverse the tree
		- modify a item through the iterator
		- test also const iterators (?)
		- test reverse iterators once implemented
	empty
	size
	max_size (not needed)
	clear
	insert (1, 4, 7)
		- simple insert (1)
		- insert with hint (4): compare times with valid and invalid hint
		- insert with iterators (7): using iterators to different containers to insert in this one
	erase (1, 2, 3)
		- simple erase (1)
		- range erase (2)
		- key erase (3)
	swap
	count
	find
	equal_range
	lower_bound
	upper_bound
	key_comp
	value_comp
	//non-member functions
	operator==
	operator!=
	operator<
	operator>
	operator<=
	operator>=
	ft::swap (if needed to implement) but should be tested in a separate test
	*/

//enum testResult { UNTESTED, OK, KO }

class mapTester {
	private:

	ft::map<std::string, bool> testResults;

	bool verbose;


	void testHeader(const std::string& testName) {
		this->testResults[testName] = true;

		char delimiter = '=';
		size_t headerLenght = TEST_HEADER_SIZE;
		size_t testNameMaxSize = 30;
		size_t testNameSize = testName.length();
		bool testNameResized = false;

		if (testNameSize > testNameMaxSize) {
			testNameResized = true;
			testNameSize = testNameMaxSize;
		}

		size_t extraSpace = headerLenght - testNameSize;
		size_t headerPreffixLength = extraSpace / 2;
		size_t headerSuffixLength = headerLenght - testNameSize - headerPreffixLength;

		for (int i = 0; i < headerPreffixLength; i++)
			std::cout << delimiter;
		for (int i = 0; i < testNameSize; i++) {
			if (i + 1 == testNameSize && testNameResized)
				std::cout << '.';
			else
				std::cout << testName.c_str()[i];
		}
		for (int i = 0; i < headerSuffixLength; i++)
			std::cout << delimiter;
		std::cout << std::endl;
	}

	void testEnd(bool result) {
		char delimiter = '=';
		std::string msg = result ? "PASS" : "FAIL";
		size_t prefix_size = (TEST_HEADER_SIZE - msg.length()) / 2;
		size_t sufix_size = TEST_HEADER_SIZE - msg.length() - prefix_size;
		for (int i = 0; i < prefix_size; i++)
			std::cout << delimiter;
		std::cout << msg;
		for (int i = 0; i < sufix_size; i++)
			std::cout << delimiter;
		std::cout << std::endl;
	}
	void log(std::string msg, bool override = false) { if (this->verbose || override) std::cout << msg << std::endl; }
	void logError(std::string msg, bool override = false) { if (this->verbose || override) std::cerr << msg << std::endl; }

	public:

	mapTester(bool v = false):
		verbose(v)
	{}

	void doTests() {
		ft::map<int, std::string> testMap;

		testMap.insert(ft::make_pair(5, "*****"));
		testMap.insert(ft::make_pair(3, "***"));
		testMap.insert(ft::make_pair(4, "****"));
		testMap.insert(ft::make_pair(7, "*******"));
		testMap.insert(ft::make_pair(9, "*********"));
		testMap.insert(ft::make_pair(1, "*"));
		testMap.insert(ft::make_pair(6, "******"));
		testMap.insert(ft::make_pair(2, "**"));
		testMap.insert(ft::make_pair(8, "********"));

		for (ft::map<int, std::string>::iterator it = testMap.begin(); it != testMap.end(); it++)
			std::cout << "(" << (*it).first << ": " << (*it).second << ")" << std::endl;

		assignmentOperatorTest(testMap);
		beginEndTest(testMap);
	}

	void assignmentOperatorTest(ft::map<int, std::string> mapToCopy) {
		const std::string testName = "AssignmentOperatorTest";
		const std::string different = "different";
		testHeader(testName);
		ft::map<int, std::string> newMap;

		newMap = mapToCopy;

		ft::map<int, std::string>::iterator copiedIt = newMap.begin();
		ft::map<int, std::string>::iterator toCopyIt = mapToCopy.begin();
		size_t pos = 0;

		ft::map<int, std::string>::iterator copiedEnd = newMap.end();
		ft::map<int, std::string>::iterator toCopyEnd = mapToCopy.end();
		while (42) {
			if (copiedIt == copiedEnd || toCopyIt == toCopyEnd)
				break ;
			if (*copiedIt != *toCopyIt) {
				this->logError("found different values in iterators in the same position", true);
				this->logError("In pos (" + std::to_string(pos) + "), found: (" + std::to_string((*copiedIt).first) + "," + (*copiedIt).second + ") and expected: (" + std::to_string((*toCopyIt).first) + "," + (*toCopyIt).second);
				this->testResults[testName] = false;
			}
			pos++;
			(*copiedIt).second = "different";
			copiedIt++;
			toCopyIt++;
		}

		if (copiedIt != copiedEnd || toCopyIt != toCopyEnd)
		{
			this->logError("Something failed because both iteators didn't finished at the same time", true);
			this->testResults[testName] = false;
		}

		if ((*newMap.begin()).second != "different")
		{
			this->logError("Updating the new map through the iterator didn't work", true);
			this->testResults[testName] = false;
		}
		if ((*mapToCopy.begin()).second == (*newMap.begin()).second)
		{
			this->logError("The new map was updated through its first iterator but it still equals the second map first iteratr");
			this->testResults[testName] = false;
		}
		testEnd(this->testResults[testName]);
	}

	void beginEndTest(ft::map<int, std::string> mapToTraverse) {
		size_t expectedSize = mapToTraverse.size();
		std::string different = "different";
		std::string iteratorTestName = "IteratorTest";
		std::string revrseIteratorTestName = "ReverseIteratorTest";

		testHeader(iteratorTestName);

		size_t calculatedSize = 0;
		size_t modPos = 3;
		for (ft::map<int, std::string>::iterator cit = mapToTraverse.begin(); cit != mapToTraverse.end(); cit++) {
			if (this->verbose) std::cout << '(' << cit->first << ": " << cit->second << ')' << std::endl;
			if (calculatedSize == modPos)
				cit->second = different;
			calculatedSize++;
		}
		if (calculatedSize != expectedSize) {
			this->logError("The map size is expected to be " + std::to_string(expectedSize) + " but counted " + std::to_string(calculatedSize) + " iterators", true);
			this->testResults[iteratorTestName] = false;
		}

		calculatedSize = 0;
		for (ft::map<int, std::string>::const_iterator cit = mapToTraverse.begin(); cit != mapToTraverse.end(); cit++) {
			if (this->verbose) std::cout << '(' << cit->first << ": " << cit->second << ')' << std::endl;
			if (calculatedSize == modPos)
				if ((*cit).second != different)  {
					this->logError("The map was modified through an iterator but couldn't find the modification in the expected position", true);
					this->testResults[iteratorTestName] = false;
				}
			calculatedSize++;
			//cit->second = "kk"; // Uncommenting this line will result in compilation error (can't assign when using const_iterator)
		}
		if (calculatedSize != expectedSize) {
			this->logError("(after modifying the map through an iterator)The map size is expected to be " + std::to_string(expectedSize) + " but counted " + std::to_string(calculatedSize) + " iterators", true);
			this->testResults[iteratorTestName] = false;
		}

		const ft::map<int, int> constMap;
		const ft::map<int, int>::const_iterator it = constMap.begin();
		//const ft::map<int, int>::iterator it = constMap.begin(); //Uncommenting this line will result in compilation error (if const map, begin and end methods will return only const_iterator)

		testEnd(this->testResults[iteratorTestName]);


		//check for const iterators once they are implemented
		//check for reverse iterators once they are implemented
		//check for const reverse iterators once they are implemented
	}

	void emptyTest(ft::map<int, std::string> emptyMap, ft::map<int, std::string> nonEmptyMap) {
	}

	void sizeTest(ft::map<int, std::string> map, size_t expectedSize) {
	}

	void clearTest(ft::map<int, std::string> map) {
	}

	void insertTest(ft::map<int, std::string> map) {
		//also implicity tested by usage in the rest of the tests
	}

	void eraseTest(ft::map<int, std::string> map) {
	}

	bool showResults() {
		bool ret = true;
		for (ft::map<std::string, bool>::iterator it = this->testResults.begin(); it != this->testResults.end(); it++) {
			std::string testName = (*it).first;
			bool result = (*it).second;
			std::cout << "[" << testName << "]:" << (result ? "OK" : "KO") << std::endl;
			ret = ret && result;
		}
		return ret;
	}

};
/*
	swap
	count
	find
	equal_range
	lower_bound
	upper_bound
	key_comp
	value_comp
	//non-member functions*/

bool execute_tests(bool v) {
	ft::map<int, std::string> testMap;

	mapTester tester(v);
	std::map<int, int> kk;

	tester.doTests();
	std::cout << std::endl;
	return tester.showResults() ? 0 : 1;
}

int main() {
	bool result = execute_tests(true);

	std::cout << std::endl << "Check now for leaks" << std::endl;
	getchar();

	return result;
}