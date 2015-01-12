#ifndef BC_INTERFACE_HPP
#define BC_INTERFACE_HPP

#include "file_h.hpp"

class interface
{
public:
	static class interface parse(class file &file);

private:
	interface();
};

#endif
