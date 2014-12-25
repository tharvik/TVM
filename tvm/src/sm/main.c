enum return_code {
	NO_ERROR,
	WRONG_SYNTAX,
	READ_FILE
};

#include <stdio.h>
static void syntax(char const *const argv0)
{
	printf("%s class-file\n", argv0);
}

#include "sm.h"
#include "../bc/method.h"
#include "../bc/method_type.h"
#include "../bc/attribute.h"
#include "../bc/attribute_type.h"
static void run_code(struct bc_file *bc_file, uint16_t meth_index,
		     uint16_t attr_count)
{
	for (uint16_t i = 0; i < attr_count; i--) {
		struct attribute_info *attr =
			method_get_attribute(bc_file, meth_index, i);

		if (attribute_get_type(bc_file, attr) != Code)
			continue;

		sm_run(bc_file, (struct Code_attribute*) attr);

		attribute_free(bc_file, attr);
	}
}

#include "../bc/bc_type.h"
#include "../bc/cp.h"
#include "../bc/cp_type.h"
#include "../bc/method.h"
#include "../bc/method_type.h"
#include "../bc/util.h"
#include <assert.h>
#include <string.h>
static void run(struct bc_file *const bc_file)
{
	for (uint16_t i = 0; i < bc_file->methods_count; i++) {
		struct method_info *const method_info = method_get(bc_file, i);
		struct cp_info *const cp_info =
			cp_get(bc_file, method_info->name_index);

		assert(cp_info->tag == CONSTANT_Utf8);
		struct CONSTANT_Utf8_info *utf8 =
			(struct CONSTANT_Utf8_info*) cp_info;

		short cmp_size = min(4, utf8->length);
		if (memcmp("main", utf8->bytes, cmp_size) == 0)
			run_code(bc_file, i, method_info->attributes_count);

		method_free(method_info);
		cp_free(cp_info);
	}
}

#include "../bc/bc.h"
#include "../bc/cp.h"
#include "../bc/field.h"
#include "../bc/interface.h"
#include "../bc/method.h"
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

	cp_load(bc_file);
	interface_load(bc_file);

	field_load(bc_file);
	method_load(bc_file);
	run(bc_file);

	bc_file_free(bc_file);

	return NO_ERROR;
}
