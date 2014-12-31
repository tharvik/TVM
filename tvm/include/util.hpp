#ifndef UTIL_HPP
#define UTIL_HPP

#include <typeinfo>

namespace util
{
	template<typename to, typename from = void*>
	to dn(from obj);
};

template<typename to, typename from>
to util::dn(from obj)
{
	to ptr = dynamic_cast<to>(obj);

	if (ptr == nullptr)
		throw std::bad_cast();

	return ptr;
}

#endif // UTIL_HPP
