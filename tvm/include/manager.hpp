#ifndef MANAGER_HPP
#define MANAGER_HPP

#include "bc_h.hpp"
#include "vm.hpp"

#include <string>
#include <map>
#include <stack>

class manager
{
public:
	static void init(std::string name);
	static void run();

	static class clss *get_class(std::string name);

	static std::stack<class vm> vms;

private:
	static std::map<std::string,class clss*> classes;
	static std::string class_name;
};

#endif // MANAGER_HPP
