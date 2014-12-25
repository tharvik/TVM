#ifndef SM_SM_TYPE_H
#define SM_SM_TYPE_H

#include "../bc/bc_type.h"
#include <stdint.h>
struct sm {
	struct bc_file *bc_file;
	struct sm_head *head;
	struct sm_element *registers;
	uint32_t code_length;
	uint8_t *code_base;
	uint8_t *code;
};

enum sm_entry_type {
	INT,
	CLASS
};

#include "../bc/cp_type.h"
struct sm_element {
	enum sm_entry_type type;
	union {
		int integer;
		struct CONSTANT_Class_info *class;
	} data;
};

#include <stdlib.h>
#include <sys/queue.h>
SLIST_HEAD(sm_head, sm_entry);
struct sm_entry {
	struct sm_element *element;

	SLIST_ENTRY(sm_entry) entries;
};

#endif
