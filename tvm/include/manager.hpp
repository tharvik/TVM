#ifndef MANAGER_HPP
#define MANAGER_HPP

#include "bc_h.hpp"
#include "vm.hpp"
#include "opcode.hpp"

#include <string>
#include <map>
#include <stack>

class manager
{
public:
	static class manager &get_instance();

	void run(std::string name);

	std::shared_ptr<class clss> get_class(std::string name);

	class vm& get_vm();

	std::stack<class vm> vms;

private:
	manager() {}

	std::string class_name;

	static manager instance;
};

#endif // MANAGER_HPP
