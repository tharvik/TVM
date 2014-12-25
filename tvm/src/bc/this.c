#include "bc_type.h"
static FILE *this_get_file(struct bc_file *const bc_file, uint8_t const offset);

#include "util.h"
uint16_t this_get_access_flags(struct bc_file *const bc_file)
{
	FILE *const file = this_get_file(bc_file, 0);

	uint16_t flags;
	read2(file, &flags);

	return flags;
}

#include "util.h"
uint16_t this_get_this_class(struct bc_file * const bc_file)
{
	FILE *const file = this_get_file(bc_file, 2);

	uint16_t this;
	read2(file, &this);

	return this;
}

#include "util.h"
uint16_t this_get_super_class(struct bc_file * const bc_file)
{
	FILE *const file = this_get_file(bc_file, 4);

	uint16_t super;
	read2(file, &super);

	return super;
}

static FILE *this_get_file(struct bc_file *const bc_file, uint8_t const offset)
{
	FILE *const file = bc_file->file;
	fseek(file, BC_SIZE + bc_file->cp_size + offset, SEEK_SET);

	return file;
}
