#ifndef BC_MP_H
#define BC_MP_H

#include "bc_type.h"
void method_load(struct bc_file *const bc_file);

#include "method_type.h"
void method_free(struct method_info *const method_info);

#include "bc_type.h"
#include "method_type.h"
struct method_info *method_get(struct bc_file *const bc_file, uint16_t index);

#include "attribute_type.h"
#include "bc_type.h"
struct attribute_info *method_get_attribute(struct bc_file *const bc_file,
					    uint16_t meth_index,
					    uint32_t attr_index);

#endif
