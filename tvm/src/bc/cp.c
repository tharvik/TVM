#include "cp.h"

#include "bc_type.h"
static void cp_parse_size(struct bc_file *const bc_file);

#include "cp_type.h"
#include <stdio.h>
static struct cp_info *cp_parse_tag(FILE * const file);

#include "cp_type.h"
#include <stdio.h>
static void cp_skip_tag(FILE * const file);

#include <stdio.h>
#define cp_tag_macro(type, size)					\
	static struct cp_info *cp_parse_##type(FILE * const file);
#include "cp_tag_macro.m"
#undef cp_tag_macro

void cp_load(struct bc_file *const bc_file)
{
	cp_parse_size(bc_file);

	FILE * const file = bc_file->file;

	for(uint16_t i = bc_file->cp_count; i > 0; i--)
		cp_skip_tag(file);

	bc_file->cp_size = ftell(file) - BC_SIZE - 1;
}

#include "bc.h"
#include "util.h"
static void cp_parse_size(struct bc_file *const bc_file)
{
	FILE *const file = bc_file->file;
	fseek(file, BC_SIZE, SEEK_SET);

	read2(file, &bc_file->cp_count);
}

#include <stdlib.h>
void cp_free(struct cp_info *const cp_info)
{
	if (cp_info->tag == CONSTANT_Utf8) {
		struct CONSTANT_Utf8_info *inst =
		    (struct CONSTANT_Utf8_info *)cp_info;
		free(inst->bytes);
	}

	free(cp_info);
}

#include "util.h"
static struct cp_info *cp_parse_tag(FILE * const file)
{
	uint8_t int_tag;
	read1(file, &int_tag);
	if (!is_valid_cp_tag(int_tag))
		return NULL;

	enum cp_tag tag = int_tag;

	struct cp_info *info;
	switch (tag) {
#define cp_tag_macro(type, size)			\
	case type:					\
		info = cp_parse_##type(file);		\
		break;
#include "cp_tag_macro.m"
#undef cp_tag_macro
	}

	info->tag = tag;

	return info;
}

#include "util.h"
static struct cp_info *cp_parse_CONSTANT_Utf8(FILE * const file)
{
	struct CONSTANT_Utf8_info *const info = malloc(sizeof(*info));
	if (info == NULL)
		return NULL;

	read2(file, &info->length);

	info->bytes = calloc(info->length, sizeof(*info->bytes));
	readn(file, info->bytes, info->length);

	return (struct cp_info *)info;
}

#include <stdlib.h>
#define cp_tag_macro(type, size)					\
	static struct cp_info *cp_parse_##type(FILE * const file)	\
	{								\
		struct type##_info * const info = malloc(sizeof(*info));\
		if (info == NULL)					\
			return NULL;					\
									\
		size_t pad = sizeof(info->tag);				\
		size_t remain = size - pad;				\
		void *ptr = info;					\
		ptr += pad;						\
		size_t read = fread(ptr, remain, 1, file);		\
		if (read != 1) {					\
			free(info);					\
			return NULL;					\
		}							\
									\
		return (struct cp_info *) info;				\
	}
#include "cp_tag_no_utf8.m"
#undef cp_tag_macro

#include <stdio.h>
static void cp_skip_tag(FILE * const file)
{
	uint8_t int_tag;
	read1(file, &int_tag);
	enum cp_tag tag = int_tag;

	if (!is_valid_cp_tag(tag))
		return;

	uint16_t skip;
	switch (tag) {
	case CONSTANT_Utf8:
		read2(file, &skip);
		skip += sizeof(int_tag);
		break;

#define cp_tag_macro(type, size)					\
	case type:							\
		skip = size;						\
		break;
#include "cp_tag_no_utf8.m"
#undef cp_tag_macro
	}

	skip -= sizeof(int_tag);
	fseek(file, skip, SEEK_CUR);
}

#include "config.h"
struct cp_info *cp_get(struct bc_file *const bc_file, uint16_t index)
{
	uint16_t const cp_count = bc_file->cp_count;
	if (index > cp_count - 1)
		return NULL;

	FILE *const file = bc_file->file;

	fseek(file, BC_SIZE + 2, SEEK_SET);

	for (; index > 1; index--)
		cp_skip_tag(file);

	return cp_parse_tag(file);
}

char *cp_tag_to_string(enum cp_tag cp_tag)
{
	switch (cp_tag) {
#define cp_tag_macro(type, size)					\
	case type:							\
		return #type + 9;					\
		break;
#include "cp_tag_macro.m"
#undef cp_tag_macro
	default:
		return NULL;
	}
}
