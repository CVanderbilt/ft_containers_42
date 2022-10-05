#include "map_tester_ft.hpp"
#include "map_tester_std.hpp"

void ft_test(std::string op, size_t s, bool _headers, bool _stop) {
	MapTesterFt mapTesterFt;

	mapTesterFt
		.withIterators()
		.withSize(s);
	if (_headers) mapTesterFt.withHeaders();
	if (_stop) mapTesterFt.withStop();
	std::cerr << "headers: " << _headers << std::endl,
	mapTesterFt.execute(op);
}

void std_test(std::string op, size_t s, bool _headers, bool _stop) {
	MapTesterStd mapTesterStd;

	mapTesterStd
		.withIterators()
		.withSize(s);
	if (_headers) mapTesterStd.withHeaders();
	if (_stop) mapTesterStd.withStop();
	std::cerr << "headers: " << _headers << std::endl,
	mapTesterStd.execute(op);
}

int main(int argc, char *argv[]) {

	std::string choice = "ft";
	std::string operation = "";
	size_t s = 1000;
	bool _stop = false;
	bool _headers = false;

	for (int i = 1; i < argc; i++)
	{
		std::string str = argv[i];
		if (str == "ft") choice = "ft";
		else if (str == "std") choice = "std";
		else if (std::isdigit(str[0])) s = std::atoi(str.c_str());
		else if (str == "s") _stop = true;
		else if (str == "h") _headers = true;
		else operation.append(str);
	}

	srand(42);
	if (choice == "ft")
		ft_test(operation, s, _headers, _stop);
	else
		std_test(operation, s, _headers, _stop);
	return 0;
}