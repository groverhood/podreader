
#include <iostream>
#include <cstddef>
#include <meta/meta.hxx>

struct baz
{
	int j;
};

struct foo
{
	baz lame;
	int b;
	double d;
	baz cool;
};

static std::ostream& operator<<(std::ostream& out, const podreader::meta::type_data &data);

static std::ostream& log_struct(std::size_t depth, std::ostream& out, const podreader::meta::type_data &data)
{
	out << data.type_name;

	if (data.is_struct)
	{
		out << "\n";
		for (std::size_t j = 0; j < depth - 1; ++j)
		{
			out << "    ";
		}
		out << "{\n";

		for (std::size_t i = 0; i < data.num_members; ++i)
		{
			for (std::size_t j = 0; j < depth; ++j)
			{
				out << "    ";
			}
			log_struct(depth + 1, out, data.children[i]) << "\n";
		}

		for (std::size_t j = 0; j < depth - 1; ++j)
		{
			out << "    ";
		}
		out << "}";
	}

	return out << ";";
}

static std::ostream& operator<<(std::ostream& out, const podreader::meta::type_data &data)
{
	return log_struct(1, out, data);
}

int main()
{
	using namespace podreader::meta;

	auto str = typename_of<int>();

	type_data td = get_type_data<foo>::value;

	std::cout << td << std::endl;

	return 0;
}