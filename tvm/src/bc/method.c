#include "method.h"

#include "bc_type.h"
#include <stdint.h>
#include <stdio.h>
static FILE *method_get_file(struct bc_file *const bc_file, uint8_t offset);

#include <stdio.h>
static void method_skip(FILE *const file);

#include "method_type.h"
#include <stdio.h>
static struct method_info *method_get_info(FILE *const file);

#include "bc_type.h"
#include <stdint.h>
static void method_get_common(struct bc_file *const bc_file, uint16_t index);

#include "bc_type.h"
#include "util.h"
void method_load(struct bc_file * const bc_file)
{
	uint16_t count;
	uint16_t size;

	load_helper(bc_file, &count, &size, method_get_file, method_skip);

	bc_file->methods_count = count;
	bc_file->methods_size = size;
}

#include <stdlib.h>
void method_free(struct method_info *const method_info)
{
	if (method_info == NULL)
		return;

	free(method_info);
}

#include "util.h"
#include "attribute.h"
static void method_skip(FILE *const file)
{
	fseek(file, 6, SEEK_CUR);

	uint16_t count;
	read2(file, &count);

	for (; count > 0; count--)
		attribute_skip(file);
}

#include "bc_type.h"
#include "this_type.h"
static FILE *method_get_file(struct bc_file *const bc_file, uint8_t offset)
{
	FILE *const file = bc_file->file;

	size_t pad = BC_SIZE + bc_file->cp_size + THIS_SIZE +
		bc_file->interfaces_size + bc_file->fields_size + offset;
	fseek(file, pad, SEEK_SET);

	return file;
}

#include "util.h"
#include <stdlib.h>
static struct method_info *method_get_info(FILE *const file)
{
	uint16_t access_flags;
	uint16_t name_index;
	uint16_t descriptor_index;
	uint16_t attributes_count;
	read2(file, &access_flags);
	read2(file, &name_index);
	read2(file, &descriptor_index);
	read2(file, &attributes_count);

	struct method_info *method_info = malloc(sizeof(*method_info));
	method_info->access_flags = access_flags;
	method_info->name_index = name_index;
	method_info->descriptor_index = descriptor_index;
	method_info->attributes_count = attributes_count;

	return method_info;
}

static void method_get_common(struct bc_file *const bc_file, uint16_t index)
{
	FILE *const file = method_get_file(bc_file, 2);

	for (; index > 0; index--)
		method_skip(file);
}

struct method_info *method_get(struct bc_file *const bc_file, uint16_t index)
{
	method_get_common(bc_file, index);
	return method_get_info(bc_file->file);
}

#include "attribute.h"
struct attribute_info *method_get_attribute(struct bc_file *const bc_file,
					    uint16_t meth_index,
					    uint32_t attr_index)
{
	method_get_common(bc_file, meth_index);

	FILE *const file = bc_file->file;
	fseek(file, 6, SEEK_CUR);

	uint16_t count;
	read2(file, &count);

	if(attr_index > count)
		return NULL;

	for(; attr_index > 0; attr_index--)
		attribute_skip(file);

	return attribute_init(bc_file);
}
