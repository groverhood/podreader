#include <json/jsonreader.hxx>
#include <json/operator.hxx>
#include <fstream>

struct foo
{
	int fst;
	int snd;
};

struct two
{
	foo f1;
	foo f2;

	int foo_rating;
	double tuck;
	short nice;
	bool condition;
};

#include <iostream>

int main()
{
	using namespace podreader::json::operator_overloads;

	two val;

	std::ifstream filestream("./data/test2.json");

	filestream >> val;

	return 0;
}
