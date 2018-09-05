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
};

int main()
{
	using namespace podreader::json::;

	

	two val = reader.result_of();

	return 0;
}