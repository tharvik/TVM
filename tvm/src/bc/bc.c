#include "bc.h"

/*struct ClassFile {
	u2             constant_pool_count;
	cp_info        constant_pool[constant_pool_count-1];
	u2             access_flags;
	u2             this_class;
	u2             super_class;
	u2             interfaces_count;
	u2
		interfaces[interfaces_count];
	u2             fields_count;
	field_info
		fields[fields_count];
	u2
		methods_count;
	method_info
		methods[methods_count];
	u2
		attributes_count;
	attribute_info
		attributes[attributes_count];
}*/

#define BC_MAGIC_SIZE 4
#define BC_MINOR_VERSION_SIZE 2
#define BC_MAJOR_VERSION_SIZE 2

static uint16_t bc_get_version(FILE * const file);

#include <stdlib.h>
struct bc_file *bc_file_init(char const *const path)
{
	struct bc_file *bc_file = malloc(sizeof(struct bc_file));
	bc_file->file = fopen(path, "r");

	return bc_file;
}

#include <stdlib.h>
void bc_file_free(struct bc_file *bc_file)
{
	if (bc_file == NULL)
		return;

	if (bc_file->file != NULL)
		fclose(bc_file->file);

	free(bc_file);
}

#include "util.h"
uint32_t bc_get_magic(FILE * const file)
{
	uint32_t magic;
	read4(file, &magic);

	return magic;
}

static uint16_t bc_get_version(FILE * const file)
{
	uint16_t version;
	read2(file, &version);

	return version;
}

uint16_t bc_get_minor_version(FILE * const file)
{
	const long pad = BC_MAGIC_SIZE;
	fseek(file, pad, SEEK_SET);

	return bc_get_version(file);
}

uint16_t bc_get_major_version(FILE * const file)
{
	const long pad = BC_MAGIC_SIZE + BC_MINOR_VERSION_SIZE;
	fseek(file, pad, SEEK_SET);

	return bc_get_version(file);
}
