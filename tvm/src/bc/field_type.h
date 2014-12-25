#ifndef BC_FIELD_H
#define BC_FIELD_H

enum access_flag {
	ACC_PUBLIC = 0x0001,
	ACC_PRIVATE = 0x0002,
	ACC_PROTECTED = 0x0004,
	ACC_STATIC = 0x0008,
	ACC_FINAL = 0x0010,
	ACC_VOLATILE = 0x0040,
	ACC_TRANSIENT = 0x0080,
	ACC_SYNTHETIC = 0x1000,
	ACC_ENUM = 0x4000,
};

#include "attribute_type.h"
#include <stdint.h>
struct field_info {
	uint16_t access_flags;
	uint16_t name_index;
	uint16_t descriptor_index;
	uint16_t attributes_count;
	struct attribute_info *attributes;
};

#endif
