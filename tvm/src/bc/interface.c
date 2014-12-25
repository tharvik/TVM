#include "interface.h"

#include "bc_type.h"
#include "this_type.h"
#include "util.h"
void interface_load(struct bc_file *const bc_file)
{
	FILE * const file = bc_file->file;

	fseek(file, BC_SIZE + bc_file->cp_size + THIS_SIZE, SEEK_SET);
	read2(file, &bc_file->interfaces_count);

	bc_file->interfaces_size = 2 + bc_file->interfaces_count * 2;
}
