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

clss::clss(std::string name) : clss(name, true)
{

}

clss::clss(std::string name, bool load_bc) : name(name)
{
	if (!load_bc)
		return;

	bc = bc::parse(name + ".class");

	std::cerr << "--> load new class: " << name << std::endl;

	for (auto info : bc->methods.meths) {
		auto code = util::dpc<Code_attribute>(info->attributes.at(0));

		meths.insert(std::make_pair(std::make_pair(info->name, info->types), code));
	}

	for (class field_info const &info : bc->field.fields) {
		fields.insert(std::make_pair(info.name, nullptr));
	}

	auto info = bc->cp.get<class CONSTANT_Class_info>(bc->self.super_class);
	if (info->name == "java/lang/Object")
		parent = nullptr;
	else
		parent = std::make_shared<class clss>(info->name);
}


void clss::run_func(
		std::string const class_name,
		std::string const name,
		std::vector<std::shared_ptr<class type>> const &types,
		std::vector<std::shared_ptr<class stack_elem::base>> args)
{
	auto i = meths.find(std::make_pair(name, types));

	if (i == meths.end()) {
		parent->run_func(class_name, name, types, args);
	} else {
		auto attr = i->second;

		class vm vma(*bc, attr->code);
		manager::get_instance().vms.push(std::move(vma));
		class vm &vm = manager::get_instance().get_vm();
		vm.vars = args;
		vm.vars.resize(attr->max_locals);
		vm.exec();
	}
}

String::String(std::string value) : clss("Ljava/lang/String", false), value(value)
{

}

void print_clss::run_func(
			std::string const class_name __attribute__ ((unused)),
			std::string const name,
			std::vector<std::shared_ptr<class type>> const &types,
			std::vector<std::shared_ptr<class stack_elem::base>> args)
{
	if(name != "println")
		throw "Unimplemented printing func";

	auto elem = args.at(1);

	std::shared_ptr<class type_class> resolved_class;
	if ((resolved_class = std::dynamic_pointer_cast<type_class>(types.at(0))) != nullptr
	    && resolved_class->name == "Ljava/lang/String") {
	    	auto ref = util::dpc<stack_elem::class_ref>(elem);
		auto val = util::dpc<String>(ref->cls);
		std::cout << val->value << std::endl;
	} else {
		auto val = util::dpc<stack_elem::const_val<int>>(elem);
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

void StringBuilder::run_func(
			std::string const class_name __attribute__ ((unused)),
			std::string const name,
			std::vector<std::shared_ptr<class type>> const &types __attribute__ ((unused)),
			std::vector<std::shared_ptr<class stack_elem::base>> args)
{
	std::shared_ptr<stack_elem::base> elem;
	if (name == "append") {
		elem = args.at(1);

		auto int_val = std::dynamic_pointer_cast<stack_elem::const_val<int>>(elem);
		auto string_val = std::dynamic_pointer_cast<stack_elem::class_ref>(elem);

		if (int_val != nullptr)
			elem = append(int_val);
		else if (string_val != nullptr && string_val->cls->name == "Ljava/lang/String")
			elem = append(string_val);
		else {
			std::cerr << string_val->cls->name << std::endl;
			throw "unimplemented StringBuilder.append func";
		}

	} else if (name == "toString") {
		elem = toString();
	} else {
		throw "unimplemented StringBuilder func";
	}

	if (elem == nullptr)
		throw "elem not set";

	manager::get_instance().get_vm().stack.push(elem);
}


std::shared_ptr<class stack_elem::class_ref> StringBuilder::append(std::shared_ptr<class stack_elem::const_val<int>> elem)
{
	auto n = std::shared_ptr<class StringBuilder>(new StringBuilder());
	n->str = str + std::to_string(elem->value);

	return std::shared_ptr<class stack_elem::class_ref>(new stack_elem::class_ref(n));
}

std::shared_ptr<class stack_elem::class_ref> StringBuilder::append(std::shared_ptr<class stack_elem::class_ref> elem)
{
	auto n = std::shared_ptr<class StringBuilder>(new StringBuilder());
	auto cls = util::dpc<String>(elem->cls);
	n->str = str + cls->value;

	return std::shared_ptr<class stack_elem::class_ref>(new stack_elem::class_ref(n));
}

std::shared_ptr<class stack_elem::class_ref> StringBuilder::toString()
{
	auto s = std::make_shared<String>(str);
	return std::make_shared<stack_elem::class_ref>(s);
}
