#pragma once
#include <cstddef>

namespace podreader
{
	namespace json
	{
		struct cstring
		{
			constexpr static std::size_t npos = static_cast<std::size_t>(-1);

			const char* const str = nullptr;
			const std::size_t len = npos;

			constexpr cstring() noexcept = default;

			constexpr std::size_t length() const noexcept
			{
				return len;
			}

			using citerator = decltype(str);

			constexpr citerator begin() const noexcept
			{
				return str;
			}

			constexpr citerator end() const noexcept
			{
				return str + len;
			}

			constexpr const char& operator[](std::size_t index) const noexcept
			{
				return str[index];
			}

			constexpr cstring substr(std::size_t off = 0, std::size_t len = npos) const noexcept
			{
				std::size_t count = (len == npos) ? length() - off : len;

#ifdef __GNUC__
				return cstring
				{
					npos, // wtf g++
					str + off,
					len
				};
#else
				return cstring
				{
					str + off,
					len
				};
#endif
			}
		};
	}
}