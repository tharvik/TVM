#ifndef BC_THIS_H
#define BC_THIS_H

#include "bc_type.h"
#include "field_type.h"
uint16_t this_get_access_flags(struct bc_file * const bc_file);
uint16_t this_get_this_class(struct bc_file *const bc_file);
uint16_t this_get_super_class(struct bc_file *const bc_file);

#endif
