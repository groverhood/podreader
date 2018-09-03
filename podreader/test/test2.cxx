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

	podreader::json::jsonreader<two> reader("test2.json");

	return 0;
}