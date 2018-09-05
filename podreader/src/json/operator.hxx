#pragma once

#include <fstream>
#include <json/jsonreader.hxx>

namespace podreader
{
	namespace json
	{
		namespace operator_overloads
		{
			template <typename T>
			std::istream& operator>>(std::istream &is, T &out)
			{
				out = jsonreader<T>(is).result_of();
				return is;
			}
		}
	}
}