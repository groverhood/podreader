#include <json/jsonreader.hxx>

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


};

int main()
{
	using namespace podreader::json::operator_overloads;

	std::ifstream inf("test2.json");

	foo f = {};

	inf >> f;

	return 0;
}