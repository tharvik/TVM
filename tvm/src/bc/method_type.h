#ifndef BC_METHOD_TYPE_H
#define BC_METHOD_TYPE_H

#include <stdint.h>
struct method_info {
	uint16_t access_flags;
	uint16_t name_index;
	uint16_t descriptor_index;
	uint16_t attributes_count;
	struct attribute_info *attributes;
};

#endif
