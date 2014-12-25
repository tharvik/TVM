#include <stdint.h>
#define BC_MAGIC_SIZE 4
#define BC_MAGIC { 0xCA, 0xFE, 0xBA, 0xBE }

#include <stdint.h>
#define BC_MINOR_VERSION 0x0000
#define BC_MAJOR_VERSION 0x0031

#include <stdint.h>
#define BC_HEADER_SIZE	\
	(BC_MAGIC_SIZE + sizeof(BC_MINOR_VERSION) + sizeof(BC_MAJOR_VERSION))
