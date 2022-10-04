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

	std::string randStr(size_t size = 10) {
		std::string alphabet = "abcdefghijklmnopqrstuvwxyz1234567890!?";
		size_t alphabet_size = alphabet.length();
		std::string ret;
		for (int i = 0; i < 10; i++){
			int n = rand();
			n = n > 0 ? n : n * -1;
			ret += alphabet.c_str()[n % alphabet_size];
		}
		return ret;
	}

	void testHeader(const std::string& testName) {
		testResults[testName] = true;

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
	void log(std::string msg, bool override = false) { if (verbose || override) std::cout << msg << std::endl; }
	void logError(std::string msg, bool override = false) { if (verbose || override) std::cerr << msg << std::endl; }

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
		atTest();
		swapTest();
		comparersTest();
		findTest();
		boundsTest();
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
				logError("found different values in iterators in the same position", true);
				logError("In pos (" + std::to_string(pos) + "), found: (" + std::to_string((*copiedIt).first) + "," + (*copiedIt).second + ") and expected: (" + std::to_string((*toCopyIt).first) + "," + (*toCopyIt).second);
				testResults[testName] = false;
			}
			pos++;
			(*copiedIt).second = "different";
			copiedIt++;
			toCopyIt++;
		}

		if (copiedIt != copiedEnd || toCopyIt != toCopyEnd)
		{
			logError("Something failed because both iteators didn't finished at the same time", true);
			testResults[testName] = false;
		}

		if ((*newMap.begin()).second != "different")
		{
			logError("Updating the new map through the iterator didn't work", true);
			testResults[testName] = false;
		}
		if ((*mapToCopy.begin()).second == (*newMap.begin()).second)
		{
			logError("The new map was updated through its first iterator but it still equals the second map first iteratr");
			testResults[testName] = false;
		}
		testEnd(testResults[testName]);
	}

	void beginEndTest(ft::map<int, std::string> mapToTraverse) {
		size_t expectedSize = mapToTraverse.size();
		std::string different = "different";
		std::string iteratorTestName = "IteratorTest";
		std::string reverseIteratorTestName = "ReverseIteratorTest";

		testHeader(iteratorTestName);

		size_t calculatedSize = 0;
		size_t modPos = 3;
		for (ft::map<int, std::string>::iterator cit = mapToTraverse.begin(); cit != mapToTraverse.end(); cit++) {
			if (verbose) std::cout << '(' << cit->first << ": " << cit->second << ')' << std::endl;
			if (calculatedSize == modPos)
				cit->second = different;
			calculatedSize++;
		}
		if (calculatedSize != expectedSize) {
			logError("The map size is expected to be " + std::to_string(expectedSize) + " but counted " + std::to_string(calculatedSize) + " iterators", true);
			testResults[iteratorTestName] = false;
		}

		calculatedSize = 0;
		for (ft::map<int, std::string>::const_iterator cit = mapToTraverse.begin(); cit != mapToTraverse.end(); cit++) {
			if (verbose) std::cout << '(' << cit->first << ": " << cit->second << ')' << std::endl;
			if (calculatedSize == modPos)
				if ((*cit).second != different)  {
					logError("The map was modified through an iterator but couldn't find the modification in the expected position", true);
					testResults[iteratorTestName] = false;
				}
			calculatedSize++;
			//cit->second = "kk"; // Uncommenting this line will result in compilation error (can't assign when using const_iterator)
		}
		if (calculatedSize != expectedSize) {
			logError("(after modifying the map through an iterator)The map size is expected to be " + std::to_string(expectedSize) + " but counted " + std::to_string(calculatedSize) + " iterators", true);
			testResults[iteratorTestName] = false;
		}

		const ft::map<int, int> constMap;
		const ft::map<int, int>::const_iterator it = constMap.begin();
		//const ft::map<int, int>::iterator it = constMap.begin(); //Uncommenting this line will result in compilation error (if const map, begin and end methods will return only const_iterator)

		testEnd(testResults[iteratorTestName]);

		testHeader(reverseIteratorTestName);

		ft::map<int, std::string>::reverse_iterator rit = mapToTraverse.rbegin();
		ft::map<int, std::string>::iterator mit = mapToTraverse.end();

		rit--;
		if (rit.base() != mapToTraverse.end()) {
			testResults[reverseIteratorTestName] = false;
			logError("The base iterator of the reverse iterator after incrementing its begin is different than the regular iterator end()", true);
		}
		rit = mapToTraverse.rbegin();
		mit--;
		calculatedSize = 0;
		if (verbose) std::cout << "reverse:regular" << std::endl;
		do {
			if (verbose) 
				std::cout << "(" << rit->first << "," << rit->second << "):(" << mit->first << "," << mit->second << ")" << std::endl;
			if (rit->first != mit->first) {
				logError("The reverse iterator sequence doesnt match with the regular iterator sequence reverted", true);
				testResults[reverseIteratorTestName] = false;
			}
			if (calculatedSize == modPos)
				rit->second = different;
			rit++;
			mit--;
			calculatedSize++;
		} while (rit.base() != mapToTraverse.begin());
		if (calculatedSize != expectedSize) {
			logError("(reverse_iterator)The map size is expected to be " + std::to_string(expectedSize) + " but counted " + std::to_string(calculatedSize) + " iterators", true);
			testResults[iteratorTestName] = false;
		}

		rit = mapToTraverse.rbegin();
		mit = --mapToTraverse.end();

		calculatedSize = 0;
		do {
			if (verbose) 
				std::cout << "(" << rit->first << "," << rit->second << "):("
				<< mit->first << "," << mit->second << ")" << std::endl;
			if (rit->first != mit->first) {
				logError("The reverse iterator sequence doesnt match with the regular iterator sequence reverted", true);
				testResults[reverseIteratorTestName] = false;
			}
			if (calculatedSize == modPos)
				if (rit->second != different) {
					logError("The map was modified through an iterator but couldn't find the modification in the expected position", true);
					testResults[iteratorTestName] = false;
				}
			rit++;
			mit--;
			calculatedSize++;
		} while (rit.base() != mapToTraverse.begin());
		if (calculatedSize != expectedSize) {
			logError("(reverse_iterator)The map size is expected to be " + std::to_string(expectedSize) + " but counted " + std::to_string(calculatedSize) + " iterators", true);
			testResults[iteratorTestName] = false;
		}
	}

	void atTest() {
		std::string atTestName = "map::at";
		testHeader(atTestName);
		ft::map<int, int> mapa;
		mapa.insert(ft::make_pair(3, 7));

		try
		{
			if (mapa.at(3) != 7) {
				logError("at returning something different than the expected value", true);
				testResults[atTestName] = false;
			}
		}
		catch(const std::exception& e)
		{
			logError("at trhew exception when looking for a value that should be found in the map", true);
			testResults[atTestName] = false;
		}

		try
		{
			mapa.at(5);
			logError("at didn't threw exception when lookig for a value that shouldn't be found in the map", true);
			testResults[atTestName] = false;
		}
		catch(const std::exception& e) {
			if(verbose) {
				std::cout << "next line is the expected exception" << std::endl;
				std::cout << e.what() << std::endl;
			}
		}

		testEnd(testResults[atTestName]);
	}

	void swapTest() {
		std::string swapTestName = "SwapTest";
		testHeader(swapTestName);

		ft::map<int, bool> m1;
		ft::map<int, bool> m2;

		m1.insert(ft::make_pair(1, true));
		m1.insert(ft::make_pair(2, true));
		m1.insert(ft::make_pair(3, true));

		m2.insert(ft::make_pair(-1, false));
		m2.insert(ft::make_pair(-2, false));
		m2.insert(ft::make_pair(-3, false));

		for (ft::map<int, bool>::iterator it = m1.begin(); it != m1.end(); it++)
			std::cout << "m1: (" << it->first << "," << it->second << ")" << std::endl;
		for (ft::map<int, bool>::iterator it = m2.begin(); it != m2.end(); it++)
			std::cout << "m2: (" << it->first << "," << it->second << ")" << std::endl;
		log("m1.swap(m2)");
		m1.swap(m2);
		for (ft::map<int, bool>::iterator it = m1.begin(); it != m1.end(); it++) {
			if (verbose) std::cout << "m1: (" << it->first << "," << it->second << ")" << std::endl;
			if (it->first > 0 || it->second != false) {
				logError("not all contents were swapped correctly", 1);
				testResults[swapTestName] = false;
			}
		}
		for (ft::map<int, bool>::iterator it = m2.begin(); it != m2.end(); it++) {
			if (verbose) std::cout << "m2: (" << it->first << "," << it->second << ")" << std::endl;
			if (it->first < 0 || it->second != true) {
				logError("not all contents were swapped correctly", 1);
				testResults[swapTestName] = false;
			}
		}
		m2[5] = true;
		m2.swap(m1);

		if (m2[5] == true || m1[5] != true) {
			logError("new content added to the second tree wont be swapped to the first tree", true);
			testResults[swapTestName] = false;
		}

		testEnd(testResults[swapTestName]);
	}

	void comparersTest() {
		ft::map<int, std::string> mapa;
		std::string comparersTestName = "comparersTest";
		testHeader(comparersTestName);
		ft::map<int, std::string>::key_compare keycmp = mapa.key_comp();
		ft::map<int, std::string>::value_compare valcmp = mapa.value_comp();

		std::less<int> lesscmp;

		ft::pair<int, std::string> p1(1, "a");
		ft::pair<int, std::string> p2(2, "a");
		ft::pair<int, std::string> p3(2, "b");

		if (keycmp(p1.first, p2.first) != lesscmp(p1.first, p2.first) ||
			keycmp(p3.first, p2.first) != lesscmp(p3.first, p2.first) ||
			keycmp(p3.first, p1.first) != lesscmp(p3.first, p1.first)) {
			logError("keycompare() returned comparator returns different than std::less");
			testResults[comparersTestName] = false;
		}

		if (valcmp(p1, p2) != lesscmp(p1.first, p2.first) ||
			valcmp(p3, p2) != lesscmp(p3.first, p2.first) ||
			valcmp(p3, p1) != lesscmp(p3.first, p1.first)) {
			logError("keycompare() returned comparator returns different than std::less");
			testResults[comparersTestName] = false;
		}

		testEnd(testResults[comparersTestName]);
	}

	void findTest() {
		std::string findTestName = "findTest";
		testHeader(findTestName);

		ft::map<int, std::string> mapa;
		ft::map<int, std::string>::iterator it;

		mapa[0] = "msg";
		mapa[9] = "msg9";
		mapa[6] = "12345";
		mapa[-1] = findTestName;

		it = mapa.find(-1);

		if (it == mapa.end()) {
			logError("find returned end() instead of iterator pointing at the requested value", true);
			testResults[findTestName] = false;
		} else if (it->first != -1 || it->second != findTestName) {
			logError("find returned an iterator but either the key or the value doesn't match with the expected value", true);
			testResults[findTestName] = false;
		}

		it = mapa.find(100);
		if (it != mapa.end()) {
			logError("find returned an iterator different than end() when searching for a value that wasn't inserted", true);
		}

		testEnd(testResults[findTestName]);

		std::string countTestName = "countTest";
		testHeader(countTestName);

		if (mapa.count(5) != 0 || mapa.count(9) != 1){
			logError("count failed", 1);
			testResults[countTestName] = false;
		}

		testEnd(testResults[countTestName]);
	}

	void boundsTest() {
		std::string boundsTestName = "boundsTest";
		testHeader(boundsTestName);
		size_t mapSize = 100;
		ft::map<int, std::string> mapa;

		
		int bound = rand();

		mapa.insert(ft::make_pair(rand(), randStr()));
		for (int i = 0; i < mapSize; i++) {
			int to_insert = rand();
			mapa.insert(ft::make_pair(to_insert, randStr()));
		}

		ft::map<int, std::string>::iterator it = mapa.begin();
		while (it != mapa.end() && it->first < bound)
			it++;
		if (it != mapa.lower_bound(bound)) {
			logError("manually calculated lower bound doesnt match the returned by the method lower_bound", true);
			testResults[boundsTestName] = false;
		}

		while (it != mapa.end() && it->first < bound)
			it++;
		if (it != mapa.lower_bound(bound)) {
			logError("manually calculated lower bound doesnt match the returned by the method lower_bound", true);
			testResults[boundsTestName] = false;
		}

		testEnd(testResults[boundsTestName]);
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
		for (ft::map<std::string, bool>::iterator it = testResults.begin(); it != testResults.end(); it++) {
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
	srand(42);

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