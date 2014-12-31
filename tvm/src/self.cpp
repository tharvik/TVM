#include "self.hpp"

#include <iostream>

#include "file.hpp"

self::self(file& file)
{
	access_flags = file.read<uint16_t>();
	this_class = file.read<uint16_t>();
	super_class = file.read<uint16_t>();
}
