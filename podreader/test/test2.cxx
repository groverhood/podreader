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

	constexpr auto td = typeof(two);

	constexpr auto tdf1 = td.children[0];
	constexpr auto tdf2 = td.children[1];

	constexpr bool same = tdf1 == tdf2;

	constexpr auto urmom = podreader::meta::get_member_info<two>::values;

	constexpr std::string_view tn = podreader::meta::typename_of<int>();

	std::ifstream filestream("./data/test2.json");

	filestream >> val;

	return 0;
}
