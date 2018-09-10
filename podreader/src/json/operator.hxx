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
				T value = jsonreader<T>(is).result_of();
				std::memcpy(&out, &value, sizeof out);
				return is;
			}
		}
	}
}