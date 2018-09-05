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
	using namespace podreader;

	json::jsonreader<two> reader("test/data/test2.json");

	two val = reader.result_of();

	return 0;
}