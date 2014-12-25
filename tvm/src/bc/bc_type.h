#define BC_SIZE 8

#ifndef BC_BC_TYPE_H
#define BC_BC_TYPE_H

#include <stdio.h>
#include <stdint.h>
struct bc_file {
	FILE *file;

	uint16_t cp_count;
	uint16_t cp_size;

	uint16_t interfaces_count;
	uint16_t interfaces_size;

	uint16_t fields_count;
	uint16_t fields_size;

	uint16_t methods_count;
	uint16_t methods_size;

	uint16_t attributes_count;
	uint16_t attributes_size;
};

#endif
