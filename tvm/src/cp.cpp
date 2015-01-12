#include "cp.hpp"

#include "bc.hpp"
#include "methods.hpp"

#include <iostream>

cp::cp()
{

}

cp::cp(class file& file)
{
	uint16_t count = file.read<uint16_t>();

	for (uint16_t i = count - 1; i > 0; i--)
		add_element(file);
}

cp::cp(class cp& other)
{
	elements = other.elements;
}

cp::cp(class cp&& other)
{
	elements = other.elements;

	other.elements = std::vector<std::shared_ptr<cp_info>>();
}

void cp::add_element(file& file)
{
	uint8_t tag = file.read<uint8_t>();

	std::shared_ptr<cp_info> info;
	switch(tag) {
	case cp::tag::CONSTANT_Fieldref:
	case cp::tag::CONSTANT_Methodref:
	case cp::tag::CONSTANT_InterfaceMethodref:
		info = ref_info::parse(file, *this);
		break;
#define cp_macro(name, id)					\
	case cp::tag::name:					\
		info = name##_info::parse(file, *this); \
		break;
#include "../macro/cp.m"
#undef cp_macro

	default:
		std::cerr << "Unknow cp tag: " << tag << std::endl;
		throw "Unknow cp tag";
	}

	this->elements.push_back(info);
}

#include "type.hpp"

std::shared_ptr<class CONSTANT_Utf8_info> CONSTANT_Utf8_info::parse(class file &file, class cp const &cp __attribute__ ((unused)))
{
	uint16_t length = file.read<uint16_t>();

	std::vector<uint8_t> bytes = file.read(length);
	std::string str(bytes.begin(), bytes.end());

	return std::make_shared<CONSTANT_Utf8_info>(str);
};

std::shared_ptr<class CONSTANT_Class_info> CONSTANT_Class_info::parse(class file &file, class cp const &cp)
{
	uint16_t index = file.read<uint16_t>();
	auto utf8 = cp.get<CONSTANT_Utf8_info>(index);

	return std::shared_ptr<CONSTANT_Class_info>(new CONSTANT_Class_info(utf8->value));
};

std::shared_ptr<class ref_info> ref_info::parse(class file &file, class cp const &cp)
{
	uint16_t class_index = file.read<uint16_t>();
	uint16_t name_and_type_index = file.read<uint16_t>();

	auto clss = cp.get<CONSTANT_Class_info>(class_index);
	auto name_and_type = cp.get<CONSTANT_NameAndType_info>(name_and_type_index);

	return std::shared_ptr<ref_info>(new ref_info(clss, name_and_type));
};

std::shared_ptr<class CONSTANT_NameAndType_info> CONSTANT_NameAndType_info::parse(class file& file, class cp const& cp)
{
	uint16_t name_index = file.read<uint16_t>();
	uint16_t desciptor_index = file.read<uint16_t>();

	std::string name = cp.get<CONSTANT_Utf8_info>(name_index)->value;
	std::string descriptor = cp.get<CONSTANT_Utf8_info>(desciptor_index)->value;

	std::vector<std::shared_ptr<class type>> types = methods::descriptor_to_type(descriptor);

	return std::shared_ptr<CONSTANT_NameAndType_info>(new CONSTANT_NameAndType_info(name, types));
};

std::shared_ptr<class CONSTANT_String_info> CONSTANT_String_info::parse(class file& file, class cp const& cp)
{
	uint16_t index = file.read<uint16_t>();

	auto utf8 = cp.get<CONSTANT_Utf8_info>(index);

	return std::shared_ptr<CONSTANT_String_info>(new CONSTANT_String_info(utf8->value));
};

std::shared_ptr<class CONSTANT_Integer_info> CONSTANT_Integer_info::parse(class file& file, class cp const& cp __attribute__ ((unused)))
{
	uint32_t value;

	value = file.read<uint32_t>();

	return std::shared_ptr<CONSTANT_Integer_info>(new CONSTANT_Integer_info(value));
};


#define cp_macro(name, id, size)				\
class name##_info * name##_info::parse(				\
		class file &file __attribute__ ((unused)),	\
		class cp const &cp __attribute__ ((unused)))	\
{								\
	std::cerr << "Unchecked add_" #name << std::endl;	\
	throw "unchecked add_" #name;				\
}
#include "../macro/cp_unchecked.m"
#undef cp_macro
