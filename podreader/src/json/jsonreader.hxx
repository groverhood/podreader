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

			template <typename T>
			inline std::istream& read_v(std::istream& is, value& val)
			{
				T t;
				is >> t;
				val.assign(t);
				return is;
			}

			template <>
			inline std::istream& read_v<bool>(std::istream& is, value& val)
			{
				std::string s;
				is >> s;
				val.assign(s == "true");
				return is;
			}

			inline std::istream& read_T(std::istream& is, value& val)
			{
				const type_data& type = val.type_of();

				if (type == typeof(bool))
				{
					return read_v<bool>(is, val);
				}

				if (type == typeof(signed char))
				{
					return read_v<signed char>(is, val);
				}

				if (type == typeof(unsigned char))
				{
					return read_v<unsigned char>(is, val);
				}

				if (type == typeof(char))
				{
					return read_v<char>(is, val);
				}

				if (type == typeof(int))
				{
					return read_v<int>(is, val);
				}

				if (type == typeof(unsigned int))
				{
					return read_v<unsigned int>(is, val);
				}

				if (type == typeof(signed int))
				{
					return read_v<signed int>(is, val);
				}

				if (type == typeof(short int))
				{
					return read_v<short int>(is, val);
				}

				if (type == typeof(unsigned short int))
				{
					return read_v<unsigned short int>(is, val);
				}

				if (type == typeof(signed short int))
				{
					return read_v<signed short int>(is, val);
				}

				if (type == typeof(long int))
				{
					return read_v<long int>(is, val);
				}

				if (type == typeof(unsigned long int))
				{
					return read_v<unsigned long int>(is, val);
				}

				if (type == typeof(signed long int))
				{
					return read_v<signed long int>(is, val);
				}

				if (type == typeof(long long int))
				{
					return read_v<long long int>(is, val);
				}

				if (type == typeof(unsigned long long int))
				{
					return read_v<unsigned long long int>(is, val);
				}

				if (type == typeof(signed long long int))
				{
					return read_v<signed long long int>(is, val);
				}

				if (type == typeof(float))
				{
					return read_v<float>(is, val);
				}

				if (type == typeof(double))
				{
					return read_v<double>(is, val);
				}

				if (type == typeof(long double))
				{
					return read_v<long double>(is, val);
				}

				return is;
			}

		}


		template <typename T, typename Enable = typename std::enable_if<std::is_aggregate<T>::value and std::is_class<T>::value>::type>
		class jsonreader
		{
		public:

			static constexpr const meta::type_data& type = typeof(T);

		private:

			std::istream &stream;
			value result;
			std::vector<std::string> strs;

			bool set;

		public:

			explicit jsonreader(std::istream &stream)
				: stream(stream),
				result(type),
				set(false)
			{}

		private:

			jsonreader(const jsonreader<T>& other) = delete;

			void evaluate_raw(value &val)
			{
				detail::read_T(stream, val);
			}

			void evaluate_intern(value &val)
			{
				if (!val.type_of().is_struct) evaluate_raw(val);

				else
				{
					const type_data& typeinfo = val.type_of();

					for (std::size_t n = 0; n < typeinfo.num_members; ++n)
					{
						std::string s;
						std::getline(stream, s, ':');
						value v = val[n];
						evaluate_intern(v);
					}
				}
			}

		public:

			void evaluate()
			{
				result.zeroset();

				evaluate_intern(result);

				set = true;
			}

			T result_of() noexcept
			{
				if (!set) evaluate();

				T res = result;

				return res;
			}
		};

	}
}