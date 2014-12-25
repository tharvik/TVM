#ifndef BC_BC_H
#define BC_BC_H

#include "bc_type.h"

// initialize a new bc_file with the given path.
// return NULL in case of error.
struct bc_file *bc_file_init(char const *const path);

// free a given bc_file.
// if bc_file is NULL, nop.
void bc_file_free(struct bc_file *bc_file);

uint32_t bc_get_magic(FILE * const file);
uint16_t bc_get_minor_version(FILE * const file);
uint16_t bc_get_major_version(FILE * const file);

#endif
