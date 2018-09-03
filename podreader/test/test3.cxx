#include <json/json.hxx>


int main()
{
	struct foo
	{
		int i;
		double d;
	} bar = { 1, 2.5 };

	using namespace podreader::json;

	value val = bar;

	val[1] = 12.0;

	foo f = val;
}