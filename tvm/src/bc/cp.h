#ifndef BC_CP_H
#define BC_CP_H

// setup the constant pool for the given bc_file
#include "bc_type.h"
#include <stdbool.h>
void cp_load(struct bc_file *const bc_file);

// free a generated cp_info
#include "cp_type.h"
void cp_free(struct cp_info *const cp_info);

// get the corresponding cp_info at the given index in the given bc_file
#include "bc_type.h"
#include "cp_type.h"
#include <stdint.h>
struct cp_info *cp_get(struct bc_file *const bc_file, uint16_t const index);

// TODO move it somewhere more relevant
// return a string version of the given tag
#include "cp_type.h"
char *cp_tag_to_string(enum cp_tag cp_tag);

#endif
