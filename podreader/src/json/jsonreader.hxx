#pragma once

#include <json/value.hxx>
#include <json/string.hxx>
#include <istream>
#include <ostream>
#include <fstream>
#include <string>
#include <vector>

namespace podreader
{
	namespace json
	{
		namespace detail
		{

			template <typename T, typename Enable = STL enable_if_t<STL is_fundamental_v<T> or STL is_same_v<T, cstring>>>
			auto read_v(std::istream& is, value& val)
			{
				T t;
				is >> t;

				if (auto ptr = dynamic_cast<value_impl<T>*>(&val))
				{
					ptr->members = t;
				}
			}

			template <>
			auto read_v<bool>(std::istream& is, value& val)
			{
				std::string s;
				unsigned char c;
				is >> c;
				is.unget();
				std::getline(is, s, ',');

				if (auto ptr = dynamic_cast<value_impl<bool>*>(&val))
				{
					ptr->members = (s == "true");
				}
			}

			template <>
			auto read_v<cstring>(std::istream& is, value& val)
			{
				std::string s;
				unsigned char c;
				is >> c;
				std::getline(is, s, '"');
				return s;
			}

		}


		template <typename T, typename Enable = typename std::enable_if<std::is_aggregate<T>::value and std::is_class<T>::value>::type>
		class jsonreader
		{
		public:

			static constexpr const meta::type_data& type = typeof(T);

		private:

			std::istream &stream;
			value *result;
			inline static std::vector<std::string *> strs;

			bool set;

		public:

			explicit jsonreader(std::istream &stream)
				: stream(stream),
				result(new value_impl<T>()),
				set(false)
			{}

			~jsonreader()
			{
				delete result;
			}

		private:

			jsonreader(const jsonreader<T>& other) = delete;

			void evaluate_raw(value &val)
			{
				using namespace detail;

				if (val.type_of() == typeof(bool))
				{
					read_v<bool>(stream, val);
				}
				else if (val.type_of() == typeof(char))
				{
					read_v<char>(stream, val);
				}
				else if (val.type_of() == typeof(signed char))
				{
					read_v<signed char>(stream, val);
				}
				else if (val.type_of() == typeof(unsigned char))
				{
					read_v<unsigned char>(stream, val);
				}
				else if (val.type_of() == typeof(short))
				{
					read_v<short>(stream, val);
				}
				else if (val.type_of() == typeof(unsigned short))
				{
					read_v<unsigned short>(stream, val);
				}
				else if (val.type_of() == typeof(int))
				{
					read_v<int>(stream, val);
				}
				else if (val.type_of() == typeof(unsigned int))
				{
					read_v<unsigned int>(stream, val);
				}
				else if (val.type_of() == typeof(long))
				{
					read_v<long>(stream, val);
				}
				else if (val.type_of() == typeof(unsigned long))
				{
					read_v<unsigned long>(stream, val);
				}
				else if (val.type_of() == typeof(long long))
				{
					read_v<long long>(stream, val);
				}
				else if (val.type_of() == typeof(unsigned long long))
				{
					read_v<unsigned long long>(stream, val);
				}
				else if (val.type_of() == typeof(float))
				{
					read_v<float>(stream, val);
				}
				else if (val.type_of() == typeof(double))
				{
					read_v<double>(stream, val);
				}
				else if (val.type_of() == typeof(long double))
				{
					read_v<long double>(stream, val);
				}
				else if (val.type_of() == typeof(cstring))
				{
					const std::size_t index = strs.size();
					strs.push_back(new std::string(read_v<cstring>(stream, val)));
					
					const std::string& str = *strs[index]; 
					auto &c_str = dynamic_cast<value_impl<const char *>&>(val[0]);
					auto &len = dynamic_cast<value_impl<std::size_t>&>(val[1]);

					c_str.members = str.c_str();
					len.members = str.length();

				}
			}

			void evaluate_intern(value &val)
			{
				if (!val.type_of().is_struct || val.type_of() == typeof(cstring)) evaluate_raw(val);

				else
				{
					const type_data& typeinfo = val.type_of();

					for (std::size_t n = 0; n < typeinfo.num_members; ++n)
					{
						std::string s;
						std::getline(stream, s, ':');
						evaluate_intern(val[n]);
					}
				}
			}

		public:

			void evaluate()
			{
				evaluate_intern(*result);

				set = true;
			}

			T result_of() noexcept
			{
				if (!set) evaluate();

				T res = *result;

				return res;
			}
		};

	}
}