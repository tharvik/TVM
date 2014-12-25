#include "sm.h"

#include "sm_type.h"
static void sm_exec(struct sm *sm);

#include "sm_type.h"
#define sm_macro(name,op,size)				\
	static void sm_exec_##name(struct sm *sm);
#include "sm.m"
#undef sm_macro

#include "sm_type.h"
#define sm_macro(name,op,size)				\
	static void sm_skip_##name(struct sm *sm);
#undef sm_macro

#include "sm_type.h"
#include <assert.h>
#include <sys/queue.h>
void sm_run(struct bc_file *bc_file, struct Code_attribute *attr)
{
	struct sm_head sm_head = SLIST_HEAD_INITIALIZER(sm_head);
	SLIST_INIT(&sm_head);

	struct sm_element *registers =
		calloc(attr->max_locals, sizeof(*registers));

	struct sm *sm = malloc(sizeof(*sm));
	sm->bc_file = bc_file;
	sm->head = &sm_head;
	sm->registers = registers;
	sm->code_length = attr->code_length;
	sm->code_base = attr->code;
	sm->code = attr->code;
	sm_exec(sm);

	while (!SLIST_EMPTY(&sm_head)) {
		struct sm_entry *entry = SLIST_FIRST(&sm_head);
		SLIST_REMOVE_HEAD(&sm_head, entries);

		if (entry->element->type == CLASS)
			free(entry->element->data.class);

		free(entry->element);
		free(entry);
	}

	free(registers);
	free(sm);
}

#include <stdio.h>
static void sm_exec(struct sm *sm)
{
	switch(*sm->code) {
#define sm_macro(name,op,size)						\
	case op:							\
		puts("got " #name);					\
		sm_exec_##name(sm);					\
		sm->code += size;					\
		if (sm->code >= sm->code_base + sm->code_length)	\
			return;						\
		break;
#include "sm.m"
#undef sm_macro
	}

	sm_exec(sm);
}

#include <assert.h>
static void sm_exec_nop(struct sm *sm)
{
	assert(sm != NULL);
}

#include "../bc/cp.h"
#include <assert.h>
#include <stdlib.h>
static void sm_exec_new(struct sm *sm)
{
	assert(sm != NULL);

	uint16_t index = (*(sm->code + 1) << 8) | *(sm->code + 2);

	struct cp_info *cp_info = cp_get(sm->bc_file, index);
	assert(cp_info->tag == CONSTANT_Class);

	struct CONSTANT_Class_info *class =
		(struct CONSTANT_Class_info*) cp_info;

	struct sm_entry *entry = malloc(sizeof(*entry));
	entry->element = malloc(sizeof(*entry->element));
	entry->element->type = CLASS;
	entry->element->data.class = class;

	SLIST_INSERT_HEAD(sm->head, entry, entries);
}

#include <stdlib.h>
static void sm_exec_aload_0(struct sm *sm)
{
	struct sm_entry *entry = malloc(sizeof(*entry));
	entry->element = malloc(sizeof(*entry->element));
	entry->element->type = sm->registers->type;
	entry->element->data = sm->registers->data;

	SLIST_INSERT_HEAD(sm->head, entry, entries);
}

static void sm_exec_dup(struct sm *sm)
{
	struct sm_entry *old = SLIST_FIRST(sm->head);

	struct sm_entry *entry = malloc(sizeof(*entry));
	entry->element = malloc(sizeof(*entry->element));
	entry->element->type = old->element->type;
	entry->element->data = old->element->data;

	SLIST_INSERT_HEAD(sm->head, entry, entries);
}

#define sm_macro(name,op,size)						\
	static void sm_exec_##name(struct sm *sm)			\
	{								\
		if (sm == NULL) return;					\
	}
#include "sm_unchecked.m"
#undef sm_macro

#define sm_macro(name,op,size)				\
	static void sm_skip_##name(struct sm *sm)	\
	{						\
		sm->code += size;			\
	}
#undef sm_macro
