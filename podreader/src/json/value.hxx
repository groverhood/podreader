#pragma once

#include <meta/meta.hxx>
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
				return new unsigned char[sizeof value]{ reinterpret_cast<const unsigned char*>(&value)[Ns]... };
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
				if (!moved && !reference && bytes) delete[] bytes;
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
			inline value operator[](std::size_t index)
			{
				if (bytes == nullptr)
				{
					bytes = new unsigned char[type.size_of];
				}

				value val = value(type.children[index]);

				// view only
				val.reference = true;

				std::size_t offset = type.align_of * index;

				val.bytes = bytes + offset;

				return val;
			}

			inline value& operator=(const value& other)
			{
				if (type == other.type)
				{
					std::copy_n(other.bytes, other.type.size_of, bytes);
				}

				return *this;
			}
		};

	}
}


#include <json/operator.hxx>