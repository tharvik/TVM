#ifndef BC_ATTRIBUTE_TYPE_H
#define BC_ATTRIBUTE_TYPE_H

#include <stdint.h>
struct attribute_info {
	uint16_t attribute_name_index;
	uint32_t attribute_length;
	uint8_t *info;
};

enum attribute {
#define attribute_info_macro(type)	\
	type,
#include "attribute.m"
#undef attribute_info_macro
};

#include <stdint.h>
struct ConstantValue_attribute {
	uint16_t attribute_name_index;
	uint32_t attribute_length;
	uint16_t constantvalue_index;
};

#include <stdint.h>
struct Code_attribute {
	uint16_t attribute_name_index;
	uint32_t attribute_length;
	uint16_t max_stack;
	uint16_t max_locals;
	uint32_t code_length;
	uint8_t *code;
	uint16_t exception_table_length;
	struct Exception_Code_attribute *exception_table;
	uint16_t attributes_count;
	struct attribute_info *attributes;
};

#include <stdint.h>
struct Exception_Code_attribute {
	uint16_t start_pc;
	uint16_t end_pc;
	uint16_t handler_pc;
	uint16_t catch_type;
};

#endif
