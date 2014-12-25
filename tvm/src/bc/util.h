#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

bool read1(FILE * const file, uint8_t * const arg);

#define util_macro(num, width) \
	bool read##num(FILE *const file, uint##width##_t *const arg);
#include "util.m"
#undef util_macro

bool readn(FILE * const file, void *const arg, size_t size);

#include "bc_type.h"
#include <stdint.h>
#include <stdio.h>
void load_helper(struct bc_file *const bc_file,
		 uint16_t *count,
		 uint16_t *size,
		 FILE *(* get)(struct bc_file *const bc_file, uint8_t offset),
		 void(* skip)(FILE * const file)
		 );

#include <stdint.h>
#include <stdio.h>
void skip_helper(FILE *const file, uint8_t offset);

#define min(x, y) (x < y) ? (x) : (y)
