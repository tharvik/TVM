#ifndef BC_FP_H
#define BC_FP_H

#include "bc_type.h"
void field_load(struct bc_file *const bc_file);

#include "field_type.h"
void field_free(struct field_info *const field_info);

#include "bc_type.h"
#include <stdint.h>
struct field_info *field_get(struct bc_file *const bc_file, uint16_t index);

#endif
