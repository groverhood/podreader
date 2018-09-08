#pragma once

#include <cstddef>
#include <type_traits>

namespace podreader
{
	namespace json
	{
		// ramshackle string
		struct string
		{
			void* string_impl;

			string() = default;

			inline std::size_t length() const
			{
				return *reinterpret_cast<std::size_t*>(string_impl);
			}

			inline const char* chars() const
			{
				return reinterpret_cast<const char*>(string_impl) + sizeof(std::size_t);
			}

			inline char& operator[](std::size_t n)
			{
				return const_cast<char*>(chars())[n];
			}

			inline const char& operator[](std::size_t n) const
			{
				return chars()[n];
			}

			using iterator = char*;
			using citerator = const char*;

			inline citerator cbegin() const
			{
				return chars();
			}

			inline citerator cend() const
			{
				return chars() + length();
			}
		};





	}
}