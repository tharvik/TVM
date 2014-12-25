#include "util.h"

bool read1(FILE * const file, uint8_t * const arg)
{
	int c = getc(file);
	*arg = c;

	return c != EOF;
}

// FIXME why not _BSD_SOURCE?
#define __USE_BSD
#include <endian.h>

#define util_macro(num, width)					\
bool read##num(FILE * const file, uint##width##_t *const arg)	\
{								\
	uint##width##_t got;					\
	size_t read = fread(&got, sizeof(got), 1, file);	\
	*arg = be##width##toh(got);				\
								\
	return read == 1;					\
}
#include "util.m"
#undef util_macro

bool readn(FILE * const file, void *const arg, size_t size)
{
	size_t read = fread(arg, 1, size, file);
	return read == size;
}

#include "bc_type.h"
void load_helper(struct bc_file *const bc_file,
		 uint16_t *count,
		 uint16_t *size,
		 FILE *(* get)(struct bc_file *const bc_file, uint8_t offset),
		 void(* skip)(FILE * const file)
		 )
{
	FILE *const file = get(bc_file, 0);
	size_t sze = ftell(file);

	uint16_t length;
	read2(file, &length);
	*count = length;

	for(; length > 0; length--) {
		skip(file);
	}

	*size = ftell(file) - sze;
}

#include <stdint.h>
#include <stdio.h>
void skip_helper(FILE *const file, uint8_t offset)
{
	fseek(file, offset, SEEK_CUR);

	uint16_t length;
	read2(file, &length);

	fseek(file, length, SEEK_CUR);
}
