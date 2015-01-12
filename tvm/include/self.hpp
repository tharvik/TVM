#ifndef BC_THIS_HPP
#define BC_THIS_HPP

#include "file_h.hpp"

#include <cstdint>

class self
{
public:
	static class self parse(class file &file);

	uint16_t const access_flags;
	uint16_t const this_class;
	uint16_t const super_class;

private:
	self(uint16_t access_flags, uint16_t this_class, uint16_t super_class);
};

#endif
