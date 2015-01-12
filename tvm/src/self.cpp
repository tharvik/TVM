#include "self.hpp"

#include <iostream>

#include "file.hpp"

self::self(uint16_t access_flags, uint16_t this_class, uint16_t super_class)
: access_flags(access_flags), this_class(this_class), super_class(super_class)
{
}

class self self::parse(class file &file)
{
	uint16_t access_flags = file.read<uint16_t>();
	uint16_t this_class = file.read<uint16_t>();
	uint16_t super_class = file.read<uint16_t>();

	return self(access_flags, this_class, super_class);
}
