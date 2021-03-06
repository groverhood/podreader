#include <json/jsonreader.hxx>
#include <json/operator.hxx>
#include <fstream>

struct foo
{
	int fst;
	int snd;
};

struct pair_si
{
	podreader::json::cstring str;
	int rating;
};

struct two
{
	foo f1; // 8
	foo f2; // 8

	int foo_rating; // 8
	double tuck; // 8
	int l; // 4
	bool condition; // 4

	pair_si pr; // 12

	long last; // 4

	podreader::json::cstring urmom;
};

#include <iostream>

int main()
{
	using namespace podreader::json::operator_overloads;

	two val;

	constexpr std::string_view tn = podreader::meta::typename_of<int>();

	std::ifstream filestream("./data/test2.json");

	filestream >> val;


	return 0;
}
