#include "field.h"

#include "bc_type.h"
#include "this_type.h"
static FILE *field_get_file(struct bc_file *const bc_file, uint8_t offset);

#include <stdio.h>
static void field_skip(FILE * const file);

#include "field_type.h"
#include <stdio.h>
static struct field_info *field_get_info(FILE * const file);

#include "util.h"
void field_load(struct bc_file *const bc_file)
{
	uint16_t count;
	uint16_t size;

	load_helper(bc_file, &count, &size, field_get_file, field_skip);

	bc_file->fields_count = count;
	bc_file->fields_size = size;
}

#include <stdlib.h>
void field_free(struct field_info *const field_info)
{
	if (field_info == NULL)
		return;

	free(field_info->attributes);
	free(field_info);
}

#include <stdio.h>
static FILE *field_get_file(struct bc_file *const bc_file, uint8_t offset)
{
	FILE *const file = bc_file->file;

	size_t pad = BC_SIZE + bc_file->cp_size + THIS_SIZE +
		bc_file->interfaces_size + offset;

	fseek(file, pad, SEEK_SET);

	return file;
}

#include "util.h"
static void field_skip(FILE * const file)
{
	skip_helper(file, 6);
}

#include "util.h"
#include <stdlib.h>
static struct field_info *field_get_info(FILE * const file)
{
	uint16_t access_flags;
	uint16_t name_index;
	uint16_t descriptor_index;
	uint16_t attributes_count;
	read2(file, &access_flags);
	read2(file, &name_index);
	read2(file, &descriptor_index);
	read2(file, &attributes_count);

	struct attribute_info *const attributes =
		calloc(attributes_count, sizeof(*attributes));
	readn(file, attributes, attributes_count);

	struct field_info *const field_info = malloc(sizeof(*field_info));
	field_info->access_flags = access_flags;
	field_info->name_index = name_index;
	field_info->descriptor_index = descriptor_index;
	field_info->attributes_count = attributes_count;
	field_info->attributes = attributes;

	return field_info;
}

struct field_info *field_get(struct bc_file *const bc_file,
			     uint16_t index)
{
	if (index > bc_file->fields_count - 1)
		return NULL;

	FILE *const file = field_get_file(bc_file, 2);

	for (; index > 0; index--)
		field_skip(file);

	return field_get_info(file);
}
