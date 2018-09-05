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
	double fuck;
	short nice;

	const char* s;
};

int main()
{
	using namespace podreader::json::operator_overloads;

	two val;

	std::ifstream filestream("test/data/test2.json");

	filestream >> val;

	return 0;
}