#include <exception>
#include <iostream>

#include "./testbench.h"

using namespace std;

testing_bench testing_bench::instance;

int main(int argc, char* argv[])
try {
	size_t pass = 0, fail = 0;
	if (argc > 1) {
		// run the specified fixtures in the requested order
		for (auto first = argv + 1, last = argv + argc; first != last; ++first) {
			testing_bench::run(*first, pass, fail);
		}
	} else {
		// run all fixtures
		testing_bench::run_all(pass, fail);
	}

	cout << "pass: " << pass << ", fail: " << fail << endl;
	if (fail) {
		cerr << "ERRORS PRESENT." << endl;
	} else if (!pass) {
		cerr << "ERROR, no test to run" << endl;
	}

	cin.get();
}
catch (const exception& e) {
	cerr << "Exception \"" << e.what() << "\" caught!" << endl;
	return -1;
}
catch (...) {
	cerr << "Unknown exception caught!" << endl;
	return -1;
}
