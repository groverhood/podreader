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

			virtual inline ~value()
			{}

			inline value(const type_data& type)
				: type(type)
			{
			}

			template <typename T>
			operator T() const
			{
				return get_value<T>();
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

		template <typename T, typename Enable = std::enable_if_t<std::is_class_v<T>>>
		std::array<value *, podreader::meta::num_members<T>::value> get_member_ptrs();

		template <typename T>
		struct value_impl : public value
		{
			podreader::meta::detail::either_t<std::is_class_v<T>, std::array<value*, podreader::meta::num_members<T>::value>, T> members;

			value_impl()
				: value(typeof(T))
			{
				if constexpr (std::is_class_v<T>)
				{
					members = get_member_ptrs<T>();
				}
			}

			virtual inline ~value_impl()
			{
				if constexpr (std::is_class_v<T>)
				{
					for (auto &ptr : members)
					{
						delete ptr;
					}
				}
			}

			value_impl(const T& other)
				: value(other)
			{
				if constexpr (std::is_fundamental_v<T> or std::is_same_v<T, cstring>)
				{
					members = other;
				}
			}

			T get_value_impl() const
			{
				if constexpr (std::is_class_v<T>)
				{
					return make_value(std::make_index_sequence<podreader::meta::num_members<T>::value>{});
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
					return T { *members[Ns]... };
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
					return *members[index];
				}
				else
				{
					return *this;
				}
			}

		};

		template <STL size_t N>
		struct get_member_ptr
		{
			value *& ptr;

			template <typename U>
			operator U()
			{
				ptr = new value_impl<U>;

				if constexpr (std::is_pointer<U>::value)
				{
					return nullptr;
				}
				else
				{
					return U{};
				}
			}
		};

		template <typename T, typename Enable, std::size_t ...Ns>
		std::array<value *, podreader::meta::num_members<T>::value> get_member_ptrs(std::index_sequence<Ns...>)
		{
			std::array<value *, podreader::meta::num_members<T>::value> values = {};
			T{ get_member_ptr<Ns>{ values[Ns] }... };
			return values;
		}

		template <typename T, typename Enable>
		std::array<value *, podreader::meta::num_members<T>::value> get_member_ptrs()
		{
			return get_member_ptrs<T, Enable>(std::make_index_sequence<podreader::meta::num_members<T>::value>{});
		}

		template<typename T>
		T value::get_value() const
		{
			const type_data& to = typeof(T);
			if (type == typeof(T))
			{
				return dynamic_cast<const value_impl<T>*>(this)->get_value_impl();
			}

			T* bad_result_ptr = nullptr;
			return *bad_result_ptr;
		}

	}
}