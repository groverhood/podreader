#pragma once

#include <json/value.hxx>
#include <istream>
#include <ostream>
#include <fstream>

namespace podreader
{
	namespace json
	{
		namespace detail
		{

#define templates_xD(T, stream, val)\
			T t;\
			is >> t;\
			\
			val = t;\
			return is\

			inline std::istream& read_T(std::istream& is, value& val)
			{
				const type_data& type = val.type_of();

				if (type == typeof(bool))
				{
					templates_xD(bool, is, val);
				}

				if (type == typeof(signed char))
				{
					templates_xD(signed char, is, val);
				}

				if (type == typeof(unsigned char))
				{
					templates_xD(unsigned char, is, val);
				}

				if (type == typeof(char))
				{
					templates_xD(char, is, val);
				}

				if (type == typeof(int))
				{
					templates_xD(int, is, val);
				}

				if (type == typeof(unsigned int))
				{
					templates_xD(unsigned int, is, val);
				}

				if (type == typeof(signed int))
				{
					templates_xD(signed int, is, val);
				}

				if (type == typeof(short int))
				{
					templates_xD(short int, is, val);
				}

				if (type == typeof(unsigned short int))
				{
					templates_xD(unsigned short int, is, val);
				}

				if (type == typeof(signed short int))
				{
					templates_xD(signed short int, is, val);
				}

				if (type == typeof(long int))
				{
					templates_xD(long int, is, val);
				}

				if (type == typeof(unsigned long int))
				{
					templates_xD(unsigned long int, is, val);
				}

				if (type == typeof(signed long int))
				{
					templates_xD(signed long int, is, val);
				}

				if (type == typeof(long long int))
				{
					templates_xD(long long int, is, val);
				}

				if (type == typeof(unsigned long long int))
				{
					templates_xD(unsigned long long int, is, val);
				}

				if (type == typeof(signed long long int))
				{
					templates_xD(signed long long int, is, val);
				}

				if (type == typeof(float))
				{
					templates_xD(float, is, val);
				}

				if (type == typeof(double))
				{
					templates_xD(double, is, val);
				}

				if (type == typeof(long double))
				{
					templates_xD(long double, is, val);
				}

				return is;
			}

#undef templates_xD

		}

		template <typename T, typename Enable = typename std::enable_if<std::is_pod<T>::value && std::is_class<T>::value>::type>
		class jsonreader
		{
		public:

			static constexpr const meta::type_data& type = typeof(T);

		private:

			std::istream stream;
			value result;

			bool set;

		public:

			explicit jsonreader(std::istream &stream)
				: stream(stream.rdbuf()),
				  result(type),
				  set(false)
			{
			}

			explicit jsonreader(const std::string &filename)
				: stream(std::ifstream(filename).rdbuf()),
				  result(type),
				  set(false)
			{
			}

		private:

			jsonreader(const jsonreader<T>& other) = delete;

			void evaluate_raw(value &val)
			{

			}

			void evaluate_intern(value &val)
			{
				if (!val.type_of().is_struct) evaluate_raw(val);

				else
				{
					const type_data& typeinfo = val.type_of();

					
				}
			}

		public:

			void evaluate()
			{
				evaluate_intern(result);
			}

			T result_of() noexcept
			{
				if (!set) evaluate();

				return result;
			}
		};

	}
}