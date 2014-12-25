enum return_code {
	NO_ERROR,
	WRONG_SYNTAX,
	READ_FILE
};

#include <stdio.h>
void syntax(char const *const argv0)
{
	printf("%s class-file\n", argv0);
}

#include "bc.h"
#include "cp.h"
#include <stdio.h>
void show_constant_pool(struct bc_file *const bc_file)
{
	puts("Constant pool:");
	cp_load(bc_file);
	for (uint16_t i = 1; i < bc_file->cp_count; i++) {
		struct cp_info *cp_info = cp_get(bc_file, i);
		printf(" #%hd = %s\n", i, cp_tag_to_string(cp_info->tag));
		cp_free(cp_info);
	}
}

#include "bc.h"
#include "this.h"
void show_this(struct bc_file *const bc_file)
{
	printf("access_flags: %d\n", this_get_access_flags(bc_file));
	printf("this class  : %d\n", this_get_this_class(bc_file));
	printf("super class : %d\n", this_get_super_class(bc_file));
};

#include "bc.h"
#include "interface.h"
void show_interfaces(struct bc_file *const bc_file)
{
	interface_load(bc_file);
	for (uint16_t i = 1; i < bc_file->interfaces_count; i++) {
		// TODO find how javap show it
	}
};

#include "bc.h"
#include "field.h"
void show_fields(struct bc_file *const bc_file)
{
	field_load(bc_file);
	for (uint16_t i = 0; i < bc_file->fields_count; i++) {
		struct field_info *const field_info = field_get(bc_file, i);

		printf(" field %d: %d\n", i, field_info->name_index);

		field_free(field_info);
	}
};

#include "attribute.h"
#include "bc.h"
#include "method.h"
#include "method_type.h"
#include <stdint.h>
void show_attributes(struct bc_file *const bc_file, uint16_t meth_index)
{
	struct method_info *const method_info = method_get(bc_file, meth_index);
	for (uint16_t i = 0; i < method_info->attributes_count; i++) {
		struct attribute_info *attr =
			method_get_attribute(bc_file, meth_index, i);

		printf("  attr %d: %d\n", i, attr->attribute_name_index);

		attribute_free(bc_file, attr);
	}

	method_free(method_info);
}

#include "bc.h"
#include "method.h"
void show_methods(struct bc_file *const bc_file)
{
	method_load(bc_file);
	for (uint16_t i = 0; i < bc_file->methods_count; i++) {
		struct method_info *const method_info = method_get(bc_file, i);
		printf(" method %d: %d\n", i, method_info->name_index);

		show_attributes(bc_file, i);

		method_free(method_info);
	}
};

#include "bc.h"
#include <stdio.h>
int main(int argc, char *const *const argv)
{
	if (argc != 2) {
		syntax(argv[0]);
		return WRONG_SYNTAX;
	}

	struct bc_file *bc_file = bc_file_init(argv[1]);
	if (bc_file == NULL) {
		return READ_FILE;
	}

	show_constant_pool(bc_file);
	show_this(bc_file);
	show_interfaces(bc_file);
	puts("{");
	show_fields(bc_file);
	show_methods(bc_file);
	puts("}");

	bc_file_free(bc_file);

	return NO_ERROR;
}
