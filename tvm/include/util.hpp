#ifndef BC_UTIL_HPP
#define BC_UTIL_HPP

#include <array>
#include <cstdint>

namespace util
{
template<typename type, int size>
type array_to(std::array<uint8_t, size> array);
}

template<typename type, int size>
type util::array_to(std::array<uint8_t, size> array)
{
	type res = 0;

	for (uint8_t byte : array)
		res = (res << 8) | byte;

	return res;
}

#endif
