#include "clss.hpp"

#include "bc.hpp"
#include "vm.hpp"
#include "methods.hpp"
#include "attribute.hpp"
#include "manager.hpp"

#include <iostream>

clss::clss()
{
	// empty
}

clss::clss(std::string name)
	: bc(bc::parse(name + ".class"))
{
	std::cerr << "--> load new class: " << name << std::endl;

	for (class method_info *info : bc->methods->meths) {
		Code_attribute *code = dynamic_cast<Code_attribute*>(info->attributes.at(0));

		meths.insert(std::make_pair(info->name, code));
	}
}

void clss::run_func(std::string name)
{
	class vm &vm = manager::vms.top();
	class Code_attribute *attr = meths.at(name);
	vm.vars.resize(attr->max_locals);
	vm.exec(*bc, attr->code);
}

void print_clss::run_func(std::string name)
{
	if(name != "println")
		throw "Unimplemented printing func";

	stack_elem::base *elem = manager::vms.top().vars.at(0);

#define macro_print(type, member)						\
	if (stack_elem::type *val = dynamic_cast<stack_elem::type*>(elem))	\
		std::cerr << ">>>>" << val->member << "<<<<<" << std::endl;
macro_print(int_const, value)
macro_print(string_const, value)
#undef macro_print

	manager::vms.pop();
}
