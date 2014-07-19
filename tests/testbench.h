// Initialize the test bench by calling TESTBENCH(),
// then define the tests and, finally,
// name the TESTFIXTURE.

// This file is inspired by http://rxcpp.codeplex.com/SourceControl/latest#Ix/CPP/unittest/testbench.hpp, 
// released under Apache Licence v2, http://www.apache.org/licenses/LICENSE-2.0
//   - the testcase is a struct templatized on the line number in the file
//   - the testrange executes all the testcases between two lines
// I changed the following:
//   - the empty_testcase has been removed
//   - the test bench declares the tests in an anonyous namespace
//   - the fixture (there should be only one by file) auto registers itself
//   - the main allow to run all fixtures or only the one specified in the argv list
//   - reformat the messages

#include <algorithm>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct fixture {
	const std::string name;

	virtual ~fixture() {}

	void operator()(size_t& pass, size_t& fail) const {
		std::cout << "> Fixture " << name << std::endl;
		run(pass, fail);
		std::cout << std::endl;
	}

protected:
	fixture(const char* name);

private:
	virtual void run(size_t& pass, size_t& fail) const = 0;
};

class testing_bench {
	friend struct fixture;
	std::vector<typename std::reference_wrapper<const fixture>> fixtures;

	static void run(const fixture& f, size_t& pass, size_t& fail) {
		f(pass, fail);
	}
	static testing_bench instance;

public:

	static void run(const char* fixture_name, size_t& pass, size_t& fail) {
		std::string name = fixture_name;
		auto first = instance.fixtures.cbegin();
		auto last = instance.fixtures.cend();
		auto found = std::find_if(first, last, [&name](const fixture& x) { return x.name == name; });
		if (found != last)
			run(*found, pass, fail);
	}

	static void run_all(size_t& pass, size_t& fail) {
		for (auto& f : instance.fixtures)
			run(f, pass, fail);
	}

};

inline fixture::fixture(const char* name) : name(name) {
	testing_bench::instance.fixtures.push_back(std::cref(*this));
}

#define TESTNAMESPACE namespace

#define TESTBENCH() TESTNAMESPACE {\
template <size_t offset> struct testcase { const char* name() { return nullptr; } void run() {} }; \
template <size_t begin, size_t end> \
struct testrange {\
	void run(size_t& pass, size_t& fail) {\
		testcase<begin> c;\
		if(c.name()) {\
			std::cout << "Running " << c.name() << std::endl;\
			try {\
				c.run();\
				++pass; \
			}\
			catch(std::logic_error& e) {\
				std::cerr << e.what() << std::endl;\
				++fail; \
			}\
		}\
		const size_t mid = (begin+end+1)/2;\
		testrange<begin+1, mid>().run(pass, fail);\
		testrange<mid, end>().run(pass, fail);\
	}\
};\
template <size_t begin> struct testrange<begin, begin> { void run(size_t& pass, size_t& fail) {}; };

#define TEST(fun_name) template <> \
struct testcase<__LINE__> { \
	const char* name() { return(#fun_name); } \
	void run(); \
}; \
inline void testcase<__LINE__>::run()

#define Q_(e) #e
#define Q(e)  Q_(e)
#define POS   __FILE__ "(" Q(__LINE__) "): "
#define VERIFY(expr) \
{ auto e = (expr); if(!e) { throw std::logic_error(POS "VERIFY("#expr")"); }  }
#define VERIFY_EQ(expected, actual) \
{ auto e = (expected); auto a = (actual); \
if(!(e == a)) { \
	std::stringstream msg; \
	msg << POS "VERIFY_EQ("#expected","#actual") => (" << e << ")!=(" << a << ")"; \
	throw std::logic_error(msg.str()); \
}}
#define SKIP(expr) \
{ auto e = (expr); if(e) { cout << "SKIPPED" << endl; return; }  }

#define TESTFIXTURE(b) struct b##_fixture : public fixture { b##_fixture() : fixture(Q(b)) {} \
private: virtual void run(size_t& pass, size_t& fail) const { testrange<0, __LINE__>().run(pass, fail); } \
} registered_##b##_fixture; \
/* unnamed namespace { */ }
