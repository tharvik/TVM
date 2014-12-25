#ifndef BC_CP_TYPE_H
#define BC_CP_TYPE_H

enum cp_tag {
	CONSTANT_Utf8 = 1,

	CONSTANT_Integer = 3,
	CONSTANT_Float = 4,
	CONSTANT_Long = 5,
	CONSTANT_Double = 6,
	CONSTANT_Class = 7,
	CONSTANT_String = 8,
	CONSTANT_Fieldref = 9,
	CONSTANT_Methodref = 10,
	CONSTANT_InterfaceMethodref = 11,
	CONSTANT_NameAndType = 12,

	CONSTANT_MethodHandle = 15,
	CONSTANT_MethodType = 16,

	CONSTANT_InvokeDynamic = 18,
};

#include <stdbool.h>
bool is_valid_cp_tag(enum cp_tag const cp_tag);

#include <stdint.h>
struct cp_info {
	uint8_t tag;
	uint8_t *info;
};

#include <stdint.h>
struct CONSTANT_Utf8_info {
	uint8_t tag;
	uint16_t length;
	uint8_t *bytes;
};

#include <stdint.h>
struct CONSTANT_Integer_info {
	uint8_t tag;
	uint32_t bytes;
};

#include <stdint.h>
struct CONSTANT_Float_info {
	uint8_t tag;
	uint32_t bytes;
};

#include <stdint.h>
struct CONSTANT_Long_info {
	uint8_t tag;
	uint32_t high_bytes;
	uint32_t low_bytes;
};

#include <stdint.h>
struct CONSTANT_Double_info {
	uint8_t tag;
	uint32_t high_bytes;
	uint32_t low_bytes;
};

#include <stdint.h>
struct CONSTANT_Class_info {
	uint8_t tag;
	uint16_t name_index;
};

#include <stdint.h>
struct CONSTANT_String_info {
	uint8_t tag;
	uint16_t string_index;
};

#include <stdint.h>
struct CONSTANT_Fieldref_info {
	uint8_t tag;
	uint16_t class_index;
	uint16_t name_and_type_index;
};

#include <stdint.h>
struct CONSTANT_Methodref_info {
	uint8_t tag;
	uint16_t class_index;
	uint16_t name_and_type_index;
};

#include <stdint.h>
struct CONSTANT_InterfaceMethodref_info {
	uint8_t tag;
	uint16_t class_index;
	uint16_t name_and_type_index;
};

#include <stdint.h>
struct CONSTANT_NameAndType_info {
	uint8_t tag;
	uint16_t name_index;
	uint16_t descriptor_index;
};

#include <stdint.h>
struct CONSTANT_MethodHandle_info {
	uint8_t tag;
	uint8_t reference_kind;
	uint16_t reference_index;
};

#include <stdint.h>
struct CONSTANT_MethodType_info {
	uint8_t tag;
	uint16_t descriptor_index;
};

#include <stdint.h>
struct CONSTANT_InvokeDynamic_info {
	uint8_t tag;
	uint16_t bootstrap_method_attr_index;
	uint16_t name_and_type_index;
};

#endif
