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
	// empty
}

clss::clss(std::string name)
	: bc(bc::parse(name + ".class"))
{
	this->name = name;

	std::cerr << "--> load new class: " << name << std::endl;

	for (class method_info *info : bc->methods->meths) {
		Code_attribute *code = util::dn<Code_attribute*>(info->attributes.at(0));

		meths.insert(std::make_pair(std::make_pair(info->name, info->types), code));
	}

	for (class field_info *info : bc->field->fields) {

		stack_elem::base *elem;

		elem = nullptr;

//		if(dynamic_cast<type_int*>(info->type) != nullptr)
//			elem = new stack_elem::int_const(0);
//		else if(dynamic_cast<type_array*>(info->type) != nullptr)
//			elem = new stack_elem::array_ref(0);
//		else if(dynamic_cast<type_class*>(info->type) != nullptr)
//			elem = new stack_elem::array_ref(0);
//		else {
//			std::cerr << "field unknow type for return" << std::endl;
//			throw "field unknow type";
//		}

		fields.insert(std::make_pair(info->name, elem));
	}

	class CONSTANT_Class_info *info = bc->cp.get<class CONSTANT_Class_info*>(bc->self.super_class);
	if (info->name == "java/lang/Object")
		return;

	clss *cls = new clss(info->name);
	for (auto p : cls->meths)
		meths.insert(p);
	for (auto p : cls->fields)
		fields.insert(p);
}

void clss::run_func(std::string name, std::vector<type*> types)
{
	class vm &vm = manager::vms.top();
	class Code_attribute *attr = meths.at(std::make_pair(name, types));
	vm.vars.resize(attr->max_locals);
	vm.exec(*bc, attr->code);
}

void print_clss::run_func(std::string name, std::vector<type*> types)
{
	if(name != "println")
		throw "Unimplemented printing func";

	stack_elem::base *elem = manager::vms.top().vars.at(1);

	class type_class *resolved_class;
	if ((resolved_class = dynamic_cast<type_class*>(types.at(0))) != nullptr
	    && resolved_class->name == "Ljava/lang/String") {
		stack_elem::string_const *val = dynamic_cast<stack_elem::string_const*>(elem);
		std::cout << val->value << std::endl;
	}

	stack_elem::int_const *val = dynamic_cast<stack_elem::int_const*>(elem);
	class type_elem *resolved_type;
	if ((resolved_type = dynamic_cast<type_elem*>(types.at(0))) != nullptr
		&& resolved_type->elm == type::INT)
		std::cout << val->value << std::endl;

	else if ((resolved_type = dynamic_cast<type_elem*>(types.at(0))) != nullptr
		&& resolved_type->elm == type::BOOL) {
		if(val->value) {
			std::cout << "true" << std::endl;
		} else {
			std::cout << "false" << std::endl;
		}
	}

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

void StringBuilder::run_func(std::string name, std::vector<type*> types)
{
	stack_elem::base *elem = nullptr;
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

	if (elem == nullptr)
		throw "elem not set";

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
