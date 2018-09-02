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
			__forceinline unsigned char* value_to_bytes(const T& value, STL index_sequence<Ns...>)
			{
				return new unsigned char[sizeof value] { reinterpret_cast<const unsigned char*>(&value)[Ns]... };
			}
		}

		struct value
		{
		private:

			bool moved;

			unsigned char *bytes;

			const type_data& type;

		public:

			template <typename T>
			value(const T& val)
				: moved(false),
				  bytes(value_to_bytes(val, STL make_index_sequence<sizeof val>{})),
				  type(get_type_data<T>::value)

			{
			}

			inline value(const value& other)
				: moved(false),
				  bytes(reinterpret_cast<unsigned char*>(std::memcpy(new unsigned char[other.type.size_of], other.bytes, other.type.size_of))),
				  type(other.type)
			{

			}

			inline
				value(value &&other) noexcept
				: moved(false),
				  bytes(other.bytes),
				  type(other.type)
			{
				other.moved = true;
			}

			inline ~value()
			{
				if (!moved) delete[] bytes;
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
		};

		constexpr bool is_whitespace(unsigned char c) noexcept
		{
			return
				c == ' '
			||  c == '\t'
			||  c == '\n'
			||  c == '\r';
		}

		template <typename T>
		std::istream& read_struct(std::istream &is, T& obj)
		{
			static constexpr type_data& type = get_type_data<T>::value;

			std::size_t index = 0;

			unsigned char c;
			while (is && index < type.num_members)
			{
				is >> c;

				switch (c)
				{

				case '{':
				case '}': break;



				}
			}
		}
	}

}



#include <json/operator.hxx>