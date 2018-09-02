#include <json/json.hxx>
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
};

int main()
{
	using namespace podreader::json::operator_overloads;

	std::ifstream test2json("test2.json");
	two pod_struct;

	test2json >> pod_struct;

	return 0;
}