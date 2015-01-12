#include "clss.hpp"

#include "bc.hpp"
#include "vm.hpp"
#include "methods.hpp"
#include "attribute.hpp"
#include "manager.hpp"
#include "field.hpp"
#include "cp.hpp"
#include "type.hpp"

#include <iostream>

clss::clss()
{

}

clss::clss(std::string name)
	: bc(bc::parse(name + ".class"))
{
	this->name = name;

	std::cerr << "--> load new class: " << name << std::endl;

	for (auto info : bc->methods.meths) {
		auto code = util::dpc<Code_attribute>(info->attributes.at(0));

		meths.insert(std::make_pair(std::make_pair(info->name, info->types), code));
	}

	for (class field_info const &info : bc->field->fields) {
		fields.insert(std::make_pair(info.name, nullptr));
	}

	auto info = bc->cp.get<class CONSTANT_Class_info>(bc->self.super_class);
	if (info->name == "java/lang/Object")
		parent = nullptr;
	else
		parent = std::make_shared<class clss>(info->name);
}

void clss::run_func(std::string const class_name, std::string const name, std::vector<std::shared_ptr<class type>> const &types)
{
	class vm &vm = manager::get_instance().get_vm();

	auto i = meths.find(std::make_pair(name, types));

	if (i == meths.end()) {
		parent->run_func(class_name, name, types);
	} else {
		auto attr = i->second;
		vm.vars.resize(attr->max_locals);

		vm.exec(*bc, attr->code);
	}
}

void print_clss::run_func(std::string const class_name, std::string const name __attribute__ ((unused)), std::vector<std::shared_ptr<class type>> const &types)
{
	if(name != "println")
		throw "Unimplemented printing func";

	auto elem = manager::get_instance().get_vm().vars.at(1);

	std::shared_ptr<class type_class> resolved_class;
	if ((resolved_class = std::dynamic_pointer_cast<type_class>(types.at(0))) != nullptr
	    && resolved_class->name == "Ljava/lang/String") {
		auto val = util::dpc<stack_elem::string_const>(elem);
		std::cout << val->value << std::endl;
	} else {
		auto val = util::dpc<stack_elem::int_const>(elem);
		std::shared_ptr<class type_elem> resolved_type;
		if ((resolved_type = std::dynamic_pointer_cast<type_elem>(types.at(0))) != nullptr
		    && resolved_type->elm == type::INT)
			std::cout << val->value << std::endl;

		else if ((resolved_type = std::dynamic_pointer_cast<type_elem>(types.at(0))) != nullptr
			 && resolved_type->elm == type::BOOL) {
			if(val->value) {
				std::cout << "true" << std::endl;
			} else {
				std::cout << "false" << std::endl;
			}
		}
	}

	manager::get_instance().vms.pop();
}

void clss::put_field(std::string name, std::shared_ptr<class stack_elem::base> elem)
{
	auto i = fields.find(name);
	if (i != fields.end())
		fields.erase(i);

	fields.insert(std::make_pair(name, elem));
}

std::shared_ptr<class stack_elem::base> clss::get_field(std::string name)
{
	return fields.at(name);
}

void StringBuilder::run_func(std::string const class_name __attribute__ ((unused)), std::string const name,
			std::vector<std::shared_ptr<class type>> const &types __attribute__ ((unused)))
{
	std::shared_ptr<stack_elem::base> elem;
	if (name == "append") {
		elem = manager::get_instance().get_vm().vars.at(1);

		std::dynamic_pointer_cast<stack_elem::int_const>(elem);

#define macro_append(type)				\
		auto val_##type = std::dynamic_pointer_cast<stack_elem::type>(elem);\
		if (val_##type != nullptr)		\
			elem = append(val_##type);
		macro_append(int_const)
		macro_append(string_const)
#undef macro_append
	} else if (name == "toString") {
		elem = toString();
	} else {
		throw "Unimplemented printing func";
	}

	if (elem == nullptr)
		throw "elem not set";

	manager::get_instance().vms.pop();
	manager::get_instance().get_vm().stack.push(elem);
}


std::shared_ptr<class stack_elem::class_ref> StringBuilder::append(std::shared_ptr<class stack_elem::int_const> elem)
{
	auto n = std::shared_ptr<class StringBuilder>(new StringBuilder());
	n->str = str + std::to_string(elem->value);

	return std::shared_ptr<class stack_elem::class_ref>(new stack_elem::class_ref(n));
}

std::shared_ptr<class stack_elem::class_ref> StringBuilder::append(std::shared_ptr<class stack_elem::string_const> elem)
{
	auto n = std::shared_ptr<class StringBuilder>(new StringBuilder());
	n->str = str + elem->value;

	return std::shared_ptr<class stack_elem::class_ref>(new stack_elem::class_ref(n));
}

std::shared_ptr<class stack_elem::string_const> StringBuilder::toString()
{
	return std::shared_ptr<class stack_elem::string_const>(new stack_elem::string_const(str));
}
