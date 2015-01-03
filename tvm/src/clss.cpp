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
		fields.insert(std::make_pair(info->name, elem));
	}

	class CONSTANT_Class_info *info = bc->cp.get<class CONSTANT_Class_info*>(bc->self.super_class);
	if (info->name == "java/lang/Object")
		parent = nullptr;
	else
		parent = std::shared_ptr<class clss>(new clss(info->name));
}

clss::~clss()
{
	delete bc;

	for(auto i : fields)
		delete i.second;
}

void clss::run_func(std::string class_name, std::string name, std::vector<type*> types)
{
	class vm &vm = manager::get_instance().get_vm();

	auto i = meths.find(std::make_pair(name, types));

	if (i == meths.end()) {
		parent->run_func(class_name, name, types);
	} else {
		class Code_attribute *attr = i->second;
		vm.vars.resize(attr->max_locals);

		vm.exec(*bc, attr->code);
	}
}

void print_clss::run_func(std::string class_name __attribute__((unused)), std::string name, std::vector<type*> types)
{
	if(name != "println")
		throw "Unimplemented printing func";

	stack_elem::base *elem = manager::get_instance().get_vm().vars.at(1);

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

	manager::get_instance().vms.pop();
}

void clss::put_field(std::string name, class stack_elem::base *elem)
{
	auto i = fields.find(name);
	if (i != fields.end()) {
		delete i->second;
		fields.erase(i);
	}

	fields.insert(std::make_pair(name, elem));
}

class stack_elem::base *clss::get_field(std::string name)
{
	return fields.at(name);
}

void StringBuilder::run_func(std::string class_name __attribute__((unused)), std::string name, std::vector<type*> types __attribute__((unused)))
{
	stack_elem::base *elem = nullptr;
	if (name == "append") {
		elem = manager::get_instance().get_vm().vars.at(1);
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

	manager::get_instance().vms.pop();
	manager::get_instance().get_vm().stack.push(elem);
}


class stack_elem::class_ref *StringBuilder::append(class stack_elem::int_const* elem)
{
	auto n = std::shared_ptr<class StringBuilder>(new StringBuilder());
	n->str = str + std::to_string(elem->value);

	return new stack_elem::class_ref(n);
}

class stack_elem::class_ref *StringBuilder::append(class stack_elem::string_const* elem)
{
	auto n = std::shared_ptr<class StringBuilder>(new StringBuilder());
	n->str = str + elem->value;

	return new stack_elem::class_ref(n);
}

class stack_elem::string_const *StringBuilder::toString()
{
	return new stack_elem::string_const(str);
}
