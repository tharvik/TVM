#include "cp_type.h"

bool is_valid_cp_tag(enum cp_tag const cp_tag)
{
	return cp_tag == 1 || (cp_tag >= 3 && cp_tag <= 12) || cp_tag == 15 ||
	    cp_tag == 16 || cp_tag == 18;
}
