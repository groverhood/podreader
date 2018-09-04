#pragma once

#include <meta/meta.hxx>
#include <json/jsonreader.hxx>

namespace podreader
{

	namespace json
	{


		namespace operator_overloads
		{
			template <typename T,
					  typename Enable = typename ::std::enable_if<::std::is_pod<T>::value && ::std::is_class<T>::value>::type>
			STL istream& operator>>(::std::istream& in, T& out)
			{
				podreader::json::jsonreader<T> r = podreader::json::jsonreader<T>(in);
				out = r.result_of();

				return in;
			}


			template <typename T,
					  typename Enable = typename ::std::enable_if<::std::is_pod<T>::value && ::std::is_class<T>::value>::type>
			STL ostream& operator<<(::std::ostream& os, T& out)
			{
				return os;
			}
		}


	}

}