#include "clss.hpp"

#include "bc.hpp"
#include "vm.hpp"
#include "methods.hpp"
#include "attribute.hpp"
#include "manager.hpp"
#include "field.hpp"

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
		Code_attribute *code = util::dn<Code_attribute*>(info->attributes.at(0));

		meths.insert(std::make_pair(info->name, code));
	}

	for (class field_info *info : bc->field->fields) {

		std::cerr << "adding field: " << info->name << std::endl;

		stack_elem::base *elem;
		switch (info->descriptor.at(0)) {
		case 'I':
			elem = new stack_elem::int_const(0);
			break;
		default:
			std::cerr << "field unknow type: " << info->descriptor << std::endl;
			throw "field unknow type";
		}

		fields.insert(std::make_pair(info->name, elem));
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

	stack_elem::base *elem = manager::vms.top().vars.at(1);

#define macro_print(type, member)						\
	if (stack_elem::type *val = dynamic_cast<stack_elem::type*>(elem))	\
		std::cout <<  val->member << std::endl;
	macro_print(int_const, value)
	macro_print(string_const, value)
#undef macro_print

	manager::vms.pop();
}

void clss::put_field(std::string name, class stack_elem::base *elem)
{
	fields.erase(name);
	fields.insert(std::make_pair(name, elem));
}

class stack_elem::base *clss::get_field(std::string name)
{
	return fields.at(name);
}

void StringBuilder::run_func(std::string name)
{
	stack_elem::base *elem;
	if (name == "append") {
		elem = manager::vms.top().vars.at(1);
#define macro_append(type)							\
		if (stack_elem::type *val = dynamic_cast<stack_elem::type*>(elem))	\
			elem = append(val);
		macro_append(int_const)
		macro_append(string_const)
#undef macro_append
	} else if (name == "toString") {
		elem = toString();
	} else {
		throw "Unimplemented printing func";
	}

	manager::vms.pop();
	manager::vms.top().stack.push(elem);
}

class stack_elem::class_ref *StringBuilder::append(class stack_elem::int_const* elem)
{
	str += std::to_string(elem->value);

	return new stack_elem::class_ref(this);
}

class stack_elem::class_ref *StringBuilder::append(class stack_elem::string_const* elem)
{
	str += elem->value;

	return new stack_elem::class_ref(this);
}

class stack_elem::string_const *StringBuilder::toString()
{
	return new stack_elem::string_const(str);
}
