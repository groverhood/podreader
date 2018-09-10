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
			auto read_v(std::istream& is, value& val)
			{
				T t;
				is >> t;
				val.assign(t);
			}

			template <>
			auto read_v<bool>(std::istream& is, value& val)
			{
				std::string s;
				unsigned char c;
				is >> c;
				is.unget();
				std::getline(is, s, ',');
				val.assign(s == "true");
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
			value result;
			inline static std::vector<std::string> strs;

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
			}

			void evaluate_intern(value &val)
			{
				if (!val.type_of().is_struct || val.type_of() == typeof(cstring)) evaluate_raw(val);

				else
				{
					const type_data& typeinfo = val.type_of();

					for (std::size_t n = 0; n < typeinfo.num_members; ++n)
					{

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