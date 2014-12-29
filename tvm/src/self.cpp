#include "self.hpp"

#include <iostream>

#include "file.hpp"

self::self(file& file)
{
	access_flags = file.read<uint16_t>();
	this_class = file.read<uint16_t>();
	super_class = file.read<uint16_t>();

	std::cout << "self: " << access_flags << std::endl;
	std::cout << "self: " << this_class << std::endl;
	std::cout << "self: " << super_class << std::endl;
}
