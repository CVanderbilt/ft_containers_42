#include "map_tester_ft.hpp"
#include "map_tester_std.hpp"

void ft_test(std::string op, size_t s) {
	MapTesterFt mapTesterFt;

	mapTesterFt
		.withIterators()
		.withSize(s)
		//.withStop()
		//.withHeaders()
		.execute(op);
}

void std_test(std::string op, size_t s) {
	MapTesterStd mapTesterStd;

	mapTesterStd
		.withIterators()
		.withSize(s)
		//.withStop()
		//.withHeaders()
		.execute(op);
}

int main(int argc, char *argv[]) {
	std::string choice = argc > 1 ? argv[1] : "ft";
	std::string operation = argc > 2 ? argv[2]: "";
	size_t s = argc > 3 ? atoi(argv[3]) : 1000;
	srand(42);
	if (choice == "ft")
		ft_test(operation, s);
	else
		std_test(operation, s);
	return 0;
}