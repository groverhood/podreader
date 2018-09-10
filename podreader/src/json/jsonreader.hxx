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

			template <typename T, typename Enable = STL enable_if_t<STL is_fundamental_v<T>>>
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

				value &char_ptr = val[0];
				value &len = val[1];

				if (auto ptr = dynamic_cast<value_impl<const char *>*>(&val))
				{

				}

				if (auto ptr = dynamic_cast<value_impl<std::size_t>*>(&len))
				{

				}

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
			inline static std::vector<std::string> strs;

			bool set;

		public:

			explicit jsonreader(std::istream &stream)
				: stream(stream),
				result(new value_impl<T>(type)),
				set(false)
			{}

			~jsonreader()
			{
				delete result;
			}

		private:

			jsonreader(const jsonreader<T>& other) = delete;

			void evaluate_raw(value *val)
			{
				if (val->type_of() == typeof(bool))
				{

				}
			}

			void evaluate_intern(value *val)
			{
				if (!val->type_of().is_struct || val->type_of() == typeof(cstring)) evaluate_raw(val);

				else
				{
					const type_data& typeinfo = val->type_of();
					value &this_val = *val;

					for (std::size_t n = 0; n < typeinfo.num_members; ++n)
					{
						evaluate_intern(&this_val[n]);
					}
				}
			}

		public:

			void evaluate()
			{

				evaluate_intern(result);

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