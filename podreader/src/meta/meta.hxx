
#include <type_traits>
#include <utility>
#include <tuple>
#include <type_traits>
#include <array>
#include <string_view>
#include <vector>
#include <functional>

#define STL ::std::

namespace podreader
{

namespace meta
{

	/**
	  *  THIS CODE WAS TAKEN FROM ANTONY POLUKHIN'S EXCELLENT PRESENTATION AT CPPCON 2016.
	  *	 I WOULD HIGHLY RECOMMEND VIEWING IT AT https://www.youtube.com/watch?v=abdeAew3gmQ
	  */
	namespace detail
	{
		template <STL size_t N>
		struct auto_cast_t
		{
			template <typename U>
			constexpr operator U() const noexcept
			{
				return U{};
			}
		};

		template <typename T, STL size_t N, STL size_t ...Ns>
		constexpr auto find_num_members_pod(STL size_t &out, STL index_sequence<N, Ns...>) noexcept
			-> decltype(static_cast<void>(T{ auto_cast_t<N>{}, auto_cast_t<Ns>{}... }))
		{
			out = sizeof...(Ns) + 1;
		}

		template <typename T, STL size_t ...Ns>
		constexpr void find_num_members_pod(STL size_t &out, STL index_sequence<Ns...>) noexcept
		{
			find_num_members_pod<T>(out, STL make_index_sequence<sizeof...(Ns) - 1>{});
		}

		template <typename T>
		constexpr STL size_t find_num_members_pod() noexcept
		{
			STL size_t num_members = 0;

			find_num_members_pod<T>(num_members, STL make_index_sequence<sizeof(T)>{});

			return num_members;
		}
	}
	

	template <typename T, typename Enable = typename STL enable_if<STL is_pod<T>::value>::type>
	struct num_members
	{
		constexpr static STL size_t value = detail::find_num_members_pod<T>();
	};

	template <typename T>
	constexpr STL string_view typename_of() noexcept
	{
		constexpr STL string_view function_name = __FUNCSIG__;
		constexpr STL size_t fst = function_name.find_first_of('<', 75);
		constexpr STL size_t snd = function_name.find_last_of('>');
		constexpr STL string_view result = function_name.substr(fst, snd - fst);

		return result;
	}

	// CONSTEXPR AGGREGATE TYPE STRUCT TYPE_DATA
	struct type_data
	{
		STL string_view type_name = "";

		bool is_pod = false;

		bool is_struct = false;

		STL size_t size_of = 0;

		STL size_t num_members = 0;

		const type_data *children = nullptr;

		constexpr type_data() noexcept = default;

		constexpr bool operator==(const type_data& other) const noexcept
		{
			return type_name == other.type_name;
		}

		constexpr bool operator!=(const type_data& other) const noexcept
		{
			return type_name != other.type_name;
		}
	};

	namespace detail
	{

		// TYPE_DATA CONSTRUCTOR STRUCT TYPE_DATA_IMPL
		template <STL size_t N>
		struct type_data_impl
		{
			type_data& type_data_ref = {};

			template <typename U>
			constexpr operator U() const noexcept;
		};

		template <typename T, STL size_t ...Ns>
		constexpr static void get_member_info_pod(STL array<type_data, num_members<T>::value> &members, STL index_sequence<Ns...>) noexcept
		{
			T{ type_data_impl<Ns>{ members[Ns] }... };
		}

		template <typename T>
		constexpr STL array<type_data, num_members<T>::value> get_member_info_pod() noexcept
		{
			STL array<type_data, num_members<T>::value> members = {};
			get_member_info_pod<T>(members, STL make_index_sequence<num_members<T>::value>{});
			return members;
		}
	}

	// UNDEFINED
	template <typename T, bool Struct>
	struct member_info
	{
	};

	// RECURSIVE CASE STRUCT MEMBER_INFO
	template <typename T>
	struct member_info<T, true>
	{
		constexpr static STL array<type_data, num_members<T>::value> values = detail::get_member_info_pod<T>();
	};

	// BASE CASE STRUCT MEMBER_INFO
	template <typename T>
	struct member_info<T, false>
	{
		constexpr static STL array<type_data, num_members<T>::value> values = 
		{
			type_data 
			{
				typename_of<T>(),
				true,
				false,
				0
			}
		};
	};

	template <typename T, typename Enable = typename STL enable_if<STL is_pod<T>::value>::type>
	struct get_member_info
	{
		constexpr static STL array<type_data, num_members<T>::value> values = member_info<T, STL is_class<T>::value>::values;
	};

	namespace detail
	{
		template <STL size_t N>
		template <typename U>
		inline constexpr type_data_impl<N>::operator U() const noexcept
		{
			type_data_ref.type_name = typename_of<U>();
			type_data_ref.is_pod = STL is_pod<U>::value;
			type_data_ref.is_struct = STL is_class<U>::value;
			type_data_ref.num_members = num_members<U>::value;
			type_data_ref.children = get_member_info<U>::values.data();

			return U{};
		}
	}

	template <typename T, typename Enable = typename STL enable_if<STL is_pod<T>::value>::type>
	struct get_type_data
	{
		static constexpr type_data value =
		{
			typename_of<T>(),
			STL is_pod<T>::value,
			STL is_class<T>::value,
			sizeof (T),
			num_members<T>::value,
			get_member_info<T>::values.data()
		};
	};
}

}