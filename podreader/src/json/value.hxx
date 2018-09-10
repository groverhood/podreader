#pragma once

#include <meta/meta.hxx>
#include <json/string.hxx>
#include <utility>
#include <cstring>
#include <vector>

namespace podreader
{
	using meta::type_data;
	using meta::get_type_data;

	namespace json
	{

		namespace detail
		{
			template <typename T, STL size_t ...Ns, typename Enable = typename STL enable_if<sizeof(T) == sizeof...(Ns)>::type>
			unsigned char* value_to_bytes(const T& value, STL index_sequence<Ns...>)
			{
				return new unsigned char[sizeof value]{ reinterpret_cast<const unsigned char*>(&value)[Ns]... };
			}
		}

		constexpr auto get_default(const type_data& type) noexcept
		{

		}

		struct value
		{
		private:

			union member
			{
				bool bool_v;

				char char_v;
				signed char schar_v;
				unsigned char byte_v;

				short short_v;
				unsigned short ushort_v;

				int int_v;
				unsigned int uint_v;

				long long long_v;
				unsigned long long ulong_v;

				float float_v;
				double double_v;
				long double ldouble_v;

				cstring string_v;
				value *object_v;
			};

			std::vector<member> members;

			const type_data& type;

		public:

			template <typename T>
			inline value(const T& val)
				: type(typeof(T))
			{

			}

			inline value(const type_data& type)
				: type(type)
			{

			}

			template <typename T>
			inline operator T() const
			{

			}

		};

	}
}