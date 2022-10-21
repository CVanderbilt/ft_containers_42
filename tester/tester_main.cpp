#include "map_tester_ft.hpp"
#include "map_tester_std.hpp"
#include "vector_tester_ft.hpp"
#include "vector_tester_std.hpp"
#include "stack_tester_ft.hpp"
#include "stack_tester_std.hpp"
#include <fstream>
#include <iostream>

#define TIME_CHECK
#ifdef TIME_CHECK
# include <chrono>
#endif

void s_exec(StackTesterFt& ft, StackTesterStd& std, const std::string& test, int seed)
{
#ifdef TIME_CHECK
	using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;
	auto t1 = high_resolution_clock::now();
	auto t2 = t1;
	auto t3 = t1;
	auto t4 = t1;
	duration<double, std::milli> ms_double1;
	duration<double, std::milli> ms_double2;
#endif
		std::cerr << "_____________________FT___________________________" << std::endl;
		srand(seed);
		ft.execute(test);
#ifdef TIME_CHECK
		t2 = high_resolution_clock::now();
		ms_double1 = t2 - t1;
		std::cerr << "Duration: " << ms_double1.count() << " ms" << std::endl;					
		t3 = high_resolution_clock::now();													
#endif
		std::cerr << "__________________________________________________" << std::endl;
		std::cerr << "_____________________STD__________________________" << std::endl;
		srand(seed);
		std.execute(test);
#ifdef TIME_CHECK
		t4 = high_resolution_clock::now();
		ms_double2 = t4 - t3;
		std::cerr << "Duration: " << ms_double2.count() << " ms" << std::endl;
#endif
		std::cerr << "__________________________________________________" << std::endl;
#ifdef TIME_CHECK
			std::cerr << std::endl << "Time difference: " << (ms_double1 - ms_double2).count() << " ms" << std::endl;
			if (ms_double1 > ms_double2) {
				ms_double1.operator/=(ms_double2.count());
				std::cerr << "FT is " << ms_double1.count() << " times faster" << std::endl << std::endl;
			} else {
				ms_double2.operator/=(ms_double1.count());
				std::cerr << "FT is " << ms_double2.count() << " times slower" << std::endl << std::endl;
			}
#endif
}

void s_interactive_test(StackTesterFt& mapTesterFt, StackTesterStd& mapTesterStd, int seed) {
	int i, c;
	std::cout << "Available tests:" << std::endl;
	//capacity element_access operator_equal iterators erase swap find bound hint

	while (1) {
		std::cout << "0.capacity" << std::endl;
		std::cout << "1.push_pop" << std::endl;
		std::cout << "2.relational" << std::endl;
		std::cout << "3.exit" << std::endl;
		std::cout << "Enter your Choice: ";
		std::cin >> c;
		switch (c) {
			case 0: { s_exec(mapTesterFt, mapTesterStd, "capacity", seed); break ; }
			case 1: { s_exec(mapTesterFt, mapTesterStd, "push_pop", seed); break ; }
			case 2: { s_exec(mapTesterFt, mapTesterStd, "relational", seed); break ; }
			case 3: { return ; }
		}
	}
}

void v_exec(VectorTesterFt& ft, VectorTesterStd& std, const std::string& test, int seed)
{
#ifdef TIME_CHECK
	using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;
	auto t1 = high_resolution_clock::now();
	auto t2 = t1;
	auto t3 = t1;
	auto t4 = t1;
	duration<double, std::milli> ms_double1;
	duration<double, std::milli> ms_double2;
#endif
		std::cerr << "_____________________FT___________________________" << std::endl;
		srand(seed);
		ft.execute(test);
#ifdef TIME_CHECK
		t2 = high_resolution_clock::now();
		ms_double1 = t2 - t1;
		std::cerr << "Duration: " << ms_double1.count() << " ms" << std::endl;					
		t3 = high_resolution_clock::now();													
#endif
		std::cerr << "__________________________________________________" << std::endl;
		std::cerr << "_____________________STD__________________________" << std::endl;
		srand(seed);
		std.execute(test);
#ifdef TIME_CHECK
		t4 = high_resolution_clock::now();
		ms_double2 = t4 - t3;
		std::cerr << "Duration: " << ms_double2.count() << " ms" << std::endl;
#endif
		std::cerr << "__________________________________________________" << std::endl;
#ifdef TIME_CHECK
			std::cerr << std::endl << "Time difference: " << (ms_double1 - ms_double2).count() << " ms" << std::endl;
			if (ms_double1 > ms_double2) {
				ms_double1.operator/=(ms_double2.count());
				std::cerr << "FT is " << ms_double1.count() << " times faster" << std::endl << std::endl;
			} else {
				ms_double2.operator/=(ms_double1.count());
				std::cerr << "FT is " << ms_double2.count() << " times slower" << std::endl << std::endl;
			}
#endif
}

void v_interactive_test(VectorTesterFt& mapTesterFt, VectorTesterStd& mapTesterStd, int seed) {
	int i, c;
	std::cout << "Available tests:" << std::endl;
	//capacity element_access operator_equal iterators erase swap find bound hint

	while (1) {
		std::cout << "0.capacity" << std::endl;
		std::cout << "1.element_access" << std::endl;
		std::cout << "2.operator_equal" << std::endl;
		std::cout << "3.iterators" << std::endl;
		std::cout << "4.erase" << std::endl;
		std::cout << "5.swap" << std::endl;
		std::cout << "6.insert" << std::endl;
		std::cout << "7.relational" << std::endl;
		std::cout << "8.exit" << std::endl;
		std::cout << "Enter your Choice: ";
		std::cin >> c;
		switch (c) {
			case 0: { v_exec(mapTesterFt, mapTesterStd, "capacity", seed); break ; }
			case 1: { v_exec(mapTesterFt, mapTesterStd, "element_access", seed); break ; }
			case 2: { v_exec(mapTesterFt, mapTesterStd, "operator_equal", seed); break ; }
			case 3: { v_exec(mapTesterFt, mapTesterStd, "iterators", seed); break ; }
			case 4: { v_exec(mapTesterFt, mapTesterStd, "erase", seed); break ; }
			case 5: { v_exec(mapTesterFt, mapTesterStd, "swap", seed); break ; }
			case 9: { v_exec(mapTesterFt, mapTesterStd, "insert", seed); break ; }
			case 10: { v_exec(mapTesterFt, mapTesterStd, "relational", seed); break ; }
			case 11: { return ; }
		}
	}
}

void m_exec(MapTesterFt& ft, MapTesterStd& std, const std::string& test, int seed)
{
#ifdef TIME_CHECK
	using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;
	auto t1 = high_resolution_clock::now();
	auto t2 = t1;
	auto t3 = t1;
	auto t4 = t1;
	duration<double, std::milli> ms_double1;
	duration<double, std::milli> ms_double2;
#endif
		std::cerr << "_____________________FT___________________________" << std::endl;
		srand(seed);
		ft.execute(test);
#ifdef TIME_CHECK
		t2 = high_resolution_clock::now();
		ms_double1 = t2 - t1;
		std::cerr << "Duration: " << ms_double1.count() << " ms" << std::endl;					
		t3 = high_resolution_clock::now();													
#endif
		std::cerr << "__________________________________________________" << std::endl;
		std::cerr << "_____________________STD__________________________" << std::endl;
		srand(seed);
		std.execute(test);
#ifdef TIME_CHECK
		t4 = high_resolution_clock::now();
		ms_double2 = t4 - t3;
		std::cerr << "Duration: " << ms_double2.count() << " ms" << std::endl;
#endif
		std::cerr << "__________________________________________________" << std::endl;
#ifdef TIME_CHECK
			std::cerr << std::endl << "Time difference: " << (ms_double1 - ms_double2).count() << " ms" << std::endl;
			if (ms_double1 > ms_double2) {
				ms_double1.operator/=(ms_double2.count());
				std::cerr << "FT is " << ms_double1.count() << " times faster" << std::endl << std::endl;
			} else {
				ms_double2.operator/=(ms_double1.count());
				std::cerr << "FT is " << ms_double2.count() << " times slower" << std::endl << std::endl;
			}
#endif
}

void m_interactive_test(MapTesterFt& mapTesterFt, MapTesterStd& mapTesterStd, int seed) {
	int i, c;
	std::cout << "Available tests:" << std::endl;
	//capacity element_access operator_equal iterators erase swap find bound hint

	while (1) {
		std::cout << "0.capacity" << std::endl;
		std::cout << "1.element_access" << std::endl;
		std::cout << "2.operator_equal" << std::endl;
		std::cout << "3.iterators" << std::endl;
		std::cout << "4.erase" << std::endl;
		std::cout << "5.swap" << std::endl;
		std::cout << "6.find" << std::endl;
		std::cout << "7.bound" << std::endl;
		std::cout << "8.hint" << std::endl;
		std::cout << "9.insert" << std::endl;
		std::cout << "10.relational" << std::endl;
		std::cout << "11.exit" << std::endl;
		std::cout << "Enter your Choice: ";
		std::cin >> c;
		switch (c) {
			case 0: { m_exec(mapTesterFt, mapTesterStd, "capacity", seed); break ; }
			case 1: { m_exec(mapTesterFt, mapTesterStd, "element_access", seed); break ; }
			case 2: { m_exec(mapTesterFt, mapTesterStd, "operator_equal", seed); break ; }
			case 3: { m_exec(mapTesterFt, mapTesterStd, "iterators", seed); break ; }
			case 4: { m_exec(mapTesterFt, mapTesterStd, "erase", seed); break ; }
			case 5: { m_exec(mapTesterFt, mapTesterStd, "swap", seed); break ; }
			case 6: { m_exec(mapTesterFt, mapTesterStd, "find", seed); break ; }
			case 7: { m_exec(mapTesterFt, mapTesterStd, "bound", seed); break ; }
			case 8: { m_exec(mapTesterFt, mapTesterStd, "hint", seed); break ; }
			case 9: { m_exec(mapTesterFt, mapTesterStd, "insert", seed); break ; }
			case 10: { m_exec(mapTesterFt, mapTesterStd, "relational", seed); break ; }
			case 11: { return ; }
		}
	}
}

int main(int argc, char *argv[]) {

	std::string operation = "";
	//size_t s = 5000000;
	size_t s = 150569;
	//size_t s = 5000000;
	bool _stop = false;
	bool _headers = false;
	bool interactive = false;
	int seed = 42;
	std::string testChoice = "-map";

	for (int i = 1; i < argc; i++)
	{
		std::string str = argv[i];
		if (std::isdigit(str[0])) s = std::atoi(str.c_str());
		else if (str == "s") _stop = true;
		else if (str == "h") _headers = true;
		else if (str == "i") interactive = true;
		else if (str[0] == '-') testChoice = str;
		else operation.append(str);
	}

	if (testChoice == "-map") {
		std::cerr << "Initializing std tester:" << std::endl;
		MapTesterStd mapTesterStd;
		srand(seed); mapTesterStd.withSize(s).build();
		std::cerr << "Initializing ft tester:" << std::endl;
		MapTesterFt mapTesterFt;
		srand(seed); mapTesterFt.withSize(s);
		if (_stop) mapTesterFt.withStop();
		mapTesterFt.build();

		if (interactive) m_interactive_test(mapTesterFt, mapTesterStd, seed);
		else m_exec(mapTesterFt, mapTesterStd, operation, seed);
	} else if (testChoice == "-vector") {
		std::cerr << "Initializing std tester:" << std::endl;
		VectorTesterStd mapTesterStd;
		srand(seed); mapTesterStd.withSize(s).build();
		std::cerr << "Initializing ft tester:" << std::endl;
		VectorTesterFt mapTesterFt;
		srand(seed); mapTesterFt.withSize(s);
		if (_stop) mapTesterFt.withStop();
		mapTesterFt.build();

		if (interactive) v_interactive_test(mapTesterFt, mapTesterStd, seed);
		else v_exec(mapTesterFt, mapTesterStd, operation, seed);
	} else if (testChoice == "-stack") {
		std::cerr << "Initializing std tester:" << std::endl;
		StackTesterStd mapTesterStd;
		srand(seed); mapTesterStd.withSize(s).build();
		std::cerr << "Initializing ft tester:" << std::endl;
		StackTesterFt mapTesterFt;
		srand(seed); mapTesterFt.withSize(s);
		if (_stop) mapTesterFt.withStop();
		mapTesterFt.build();

		if (interactive) s_interactive_test(mapTesterFt, mapTesterStd, seed);
		else s_exec(mapTesterFt, mapTesterStd, operation, seed);
	} else
		std::cerr << "Wrong choice; received " << testChoice << ", expecing it to be one of the following: -map, -vector, -stack" << std::endl;

	return 0;
}