#include "attribute.h"

#define attribute_info_macro(type)	\
	static struct attribute_info *attribute_parse_##type(FILE * const file);
#include "attribute_checked.m"
#undef attribute_info_macro

#include "cp.h"
#include "cp_type.h"
#include "bc_type.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
struct attribute_info *attribute_init(struct bc_file *const bc_file)
{
	FILE *const file = bc_file->file;

	uint16_t attribute_name_index;
	uint32_t attribute_length;
	read2(file, &attribute_name_index);
	read4(file, &attribute_length);

	size_t const pos = ftell(file);
	struct cp_info *const cp_info = cp_get(bc_file, attribute_name_index);
	fseek(file, pos, SEEK_SET);

	assert(cp_info->tag == CONSTANT_Utf8);
	struct CONSTANT_Utf8_info *const utf8 =
		(struct CONSTANT_Utf8_info*) cp_info;

	short const size_cmp = min(4, utf8->length);
	if (memcmp(utf8->bytes, "Code", size_cmp) == 0) {
		struct attribute_info *attribute_info =
			attribute_parse_Code(file);

		attribute_info->attribute_name_index = attribute_name_index;
		attribute_info->attribute_length = attribute_length;

		cp_free(cp_info);
		return attribute_info;
	}

#define attribute_info_macro(type)				\
	if (memcmp(utf8->bytes, #type, utf8->length) == 0)	\
		return NULL;
#include "attribute.m"
#undef attribute_info_macro

	return NULL;
}

#include "cp.h"
#include "cp_type.h"
#include "util.h"
void attribute_free(struct bc_file *bc_file,
		    struct attribute_info *const attribute_info)
{
	if (attribute_info == NULL) return;

	struct cp_info *const cp_info =
		cp_get(bc_file, attribute_info->attribute_name_index);
	struct CONSTANT_Utf8_info *const utf8 = (struct CONSTANT_Utf8_info*)
		cp_info;

	short const size_cmp = min(4, utf8->length);
	if (memcmp(utf8->bytes, "Code", size_cmp) == 0) {
		struct Code_attribute *attr = (struct Code_attribute*)
			attribute_info;
		free(attr->code);
	}

	cp_free(cp_info);
	free(attribute_info);
}

void attribute_skip(FILE *const file)
{
	fseek(file, 2, SEEK_CUR);

	uint32_t count;
	read4(file, &count);

	fseek(file, count, SEEK_CUR);
}

static struct attribute_info *attribute_parse_Code(FILE * const file)
{
	uint16_t max_stack;
	uint16_t max_locals;
	uint32_t code_length;
	uint8_t *code;
	//uint16_t exception_table_length;
	//struct Exception_Code_attribute *exception_table;
	//uint16_t attributes_count;
	//struct attribute_info *attributes;
	read2(file, &max_stack);
	read2(file, &max_locals);

	read4(file, &code_length);
	code = calloc(code_length, sizeof(*code));
	readn(file, code, code_length);

	//read2(file, &exception_table_length);
	//code = calloc(exception_table_length, sizeof(*code));
	//readn(file, code, code_length);

	//read2(file, &attributes_count);
	//code = calloc(code_length, sizeof(*code));
	//readn(file, code, code_length);

	struct Code_attribute *attr = malloc(sizeof(*attr));
	attr->max_stack = max_stack;
	attr->max_locals = max_locals;
	attr->code_length = code_length;
	attr->code = code;
	//attr->exception_table_length = exception_table_length;
	//attr->exception_table = exception_table;
	//attr->attributes_count = attributes_count;
	//attr->attributes = attributes;

	return (struct attribute_info*) attr;
}

enum attribute attribute_get_type(struct bc_file *const bc_file,
				  struct attribute_info const *const attr)
{
	struct cp_info *const cp_info =
		cp_get(bc_file, attr->attribute_name_index);
	struct CONSTANT_Utf8_info *const utf8 = (struct CONSTANT_Utf8_info*)
		cp_info;

	enum attribute res = -1;

#define attribute_info_macro(type)				\
	if (memcmp(utf8->bytes, #type, utf8->length) == 0)	\
		res = type;
#include "attribute.m"
#undef attribute_info_macro

	cp_free(cp_info);

	return res;
}
