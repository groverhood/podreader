#pragma once

#include <meta/meta.hxx>
#include <json/string.hxx>
#include <utility>
#include <cstring>
#include <vector>
#include <array>

namespace podreader
{
	using meta::type_data;
	using meta::get_type_data;

	namespace json
	{

		struct value
		{
		protected:

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
			operator T() const
			{
				return get_value();
			}

			const type_data& type_of() const noexcept
			{
				return type;
			}

		private:

			template <typename T>
			T get_value() const;

		public:

			virtual value& operator[](std::size_t index) = 0;

		};

		template <typename T>
		struct value_impl : public value
		{
			using namespace podreader::meta

			detail::either_t<std::is_class_v<T>, std::array<value, num_members<T>::value>, T> members;

			value_impl()
				: value(typeof(T))
			{
			}

			value_impl(std::enable_if_t<std::is_fundamental_v<T> || std::is_same_v<T, cstring>, const T&> other)
				: value(other)
			{
				members[0] = other;
			}

			T get_value_impl() const
			{
				if constexpr (std::is_class_v<T>)
				{
					return make_value(std::make_index_sequence<num_members<T>::value>{});
				}
				else
				{
					return members;
				}
			}

		private:

			template <std::size_t ...Ns>
			T make_value(std::index_sequence<Ns...>) const noexcept
			{
				if constexpr (std::is_class_v<T>)
				{
					return T { members[Ns]... };
				}
				else
				{
					return members;
				}
			}

		public:

			value& operator[](std::size_t index) override
			{
				if constexpr (std::is_class_v<T>)
				{
					return members[index];
				}
				else
				{
					return members;
				}
			}

		};

		template<typename T>
		T value::get_value() const
		{
			if (type == typeof(T))
			{
				return dynamic_cast<const value_impl<T>*>(this)->get_value_impl();
			}

			return *reinterpret_cast<T*>(nullptr);
		}

	}
}