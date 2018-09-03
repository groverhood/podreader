#pragma once

#include <meta/meta.hxx>
#include <istream>
#include <ostream>
#include <utility>



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
				return new unsigned char[sizeof value] { reinterpret_cast<const unsigned char*>(&value)[Ns]... };
			}
		}

		struct value
		{
		private:

			bool moved;
			bool reference = false;

			unsigned char *bytes;

			const type_data& type;

		public:

			template <typename T>
			value(const T& val)
				: moved(false),
				  bytes(detail::value_to_bytes(val, STL make_index_sequence<sizeof val>{})),
				  type(get_type_data<T>::value)

			{
			}

			inline value(const value& other)
				: moved(false),
				  reference(other.reference),
				  bytes(reinterpret_cast<unsigned char*>(std::memcpy(new unsigned char[other.type.size_of], other.bytes, other.type.size_of))),
				  type(other.type)
			{

			}

			inline
				value(value &&other) noexcept
				: moved(false),
				  reference(other.reference),
				  bytes(other.bytes),
				  type(other.type)
			{
				other.moved = true;
			}

			inline explicit
				value(const type_data& type)
				: moved(false),
				  bytes(nullptr),
				  type(type)
			{

			}

			inline ~value()
			{
				if (!moved && !reference) delete[] bytes;
			}

			template <typename T>
			operator const T&() const
			{
				if (type == get_type_data<T>::value)
				{
					return *reinterpret_cast<const T*>(bytes);
				}

				constexpr T* bad_ptr = nullptr;

				return *bad_ptr;
			}

			template <typename T>
			operator T&()
			{
				if (type == get_type_data<T>::value)
				{
					return *reinterpret_cast<T*>(bytes);
				}

				constexpr T* bad_ptr = nullptr;

				return *bad_ptr;
			}

			inline const type_data& type_of() const noexcept
			{
				return type;
			}
		
			template <typename T>
			void assign(const T& val)
			{
				if (bytes == nullptr)
				{
					bytes = new unsigned char[type.size_of];
				}

				if (type == typeof(T))
				{
					auto ptr = reinterpret_cast<const unsigned char*>(&val);
					std::copy_n(ptr, sizeof val, bytes);
				}
			}

			// attribute getter
			value operator[](std::size_t index)
			{
				value val = value(type.children[index]);

				// view only
				val.reference = true;

				std::size_t offset = type.align_of * index;

				val.bytes = bytes + offset;

				return val;
			}

			value& operator=(const value& other)
			{
				if (type == other.type)
				{
					std::copy_n(other.bytes, other.type.size_of, bytes);
				}

				return *this;
			}
		};

		constexpr bool is_whitespace(unsigned char c) noexcept
		{
			return
				c == ' '
			||  c == '\t'
			||  c == '\n'
			||  c == '\r';
		}

		constexpr bool is_brace(unsigned char c) noexcept
		{
			return
				c == '{' || c == '}';
		}

		constexpr bool is_bracket(unsigned char c) noexcept
		{
			return
				c == '[' || c == ']';
		}

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

			inline value read_value(std::istream &is, const type_data& type)
			{
				if (!type.is_struct)
				{
					value val = value(type);

					read_T(is, val);

					return val;
				}

				value retval = value(type);

				for (std::size_t n = 0; n < type.num_members; ++n)
				{
					unsigned char tmp;

					is >> tmp;

					if (tmp == '"')
					{
						do {
							is >> tmp;
						} while (tmp != ':');

						retval[n] = read_value(is, type.children[n]);
					}
				}
			}
		}

		template <typename T>
		std::istream& read_struct(std::istream &is, T& obj)
		{
			static constexpr type_data& type = get_type_data<T>::value;

			unsigned char tmp;

			is >> tmp;

			if (tmp != '{') return is;

			obj = detail::read_value(is, type);

			return is;
		}
	}

}



#include <json/operator.hxx>