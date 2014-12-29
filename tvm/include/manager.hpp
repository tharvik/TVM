#ifndef MANAGER_HPP
#define MANAGER_HPP

#include "bc_h.hpp"
#include "vm.hpp"

#include <string>

class manager
{
public:
	manager(std::string main_class);

	void run();
private:
	class bc const * const bc;
	class vm vm;
};

#endif // MANAGER_HPP
