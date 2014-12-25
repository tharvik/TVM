#ifndef BC_AP_H
#define BC_AP_H

#include "attribute_type.h"
#include "bc_type.h"
struct attribute_info *attribute_init(struct bc_file *const bc_file);

#include "attribute_type.h"
void attribute_free(struct bc_file *bc_file,
		    struct attribute_info *attribute_info);

#include <stdio.h>
void attribute_skip(FILE *const file);

// TODO find a better place to put nicer functions
#include "attribute_type.h"
enum attribute attribute_get_type(struct bc_file *const bc_file,
				  struct attribute_info const *const attr);

#endif
