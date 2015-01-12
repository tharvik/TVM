#include "cp.hpp"

#include "bc.hpp"
#include "methods.hpp"
#include "type.hpp"

#include <iostream>

class cp cp::parse(class file &file)
{
	std::vector<std::shared_ptr<class cp_info>> elements;

	uint16_t count = file.read<uint16_t>();
	elements.reserve(count);
	for (uint16_t i = count - 1; i > 0; i--)
		add_element(file, elements);

	return std::move(cp(std::move(elements)));
}

cp::cp(std::vector<std::shared_ptr<class cp_info>> &&elements) : elements(std::move(elements))
{
}

void cp::add_element(file& file, std::vector<std::shared_ptr<class cp_info>> &elements)
{
	uint8_t tag = file.read<uint8_t>();

	enum tag {
	#define cp_macro(name, type, id)	\
		name = id,
	#include "../macro/cp.m"
	#undef cp_macro
	};

	std::shared_ptr<cp_info> info;
	switch(tag) {
#define cp_macro(name, type, id)				\
	case name:					\
		info = type##_info::parse(file, elements); 	\
		break;
#include "../macro/cp.m"
#undef cp_macro

	default:
		std::cerr << "Unknow cp tag: " << tag << std::endl;
		throw "unknow cp tag";
	}

	elements.push_back(info);
}

std::shared_ptr<class CONSTANT_Utf8_info> CONSTANT_Utf8_info::parse(class file &file,
								std::vector<std::shared_ptr<class cp_info>> &elements __attribute__ ((unused)))
{
	uint16_t length = file.read<uint16_t>();

	std::vector<uint8_t> bytes = file.read(length);
	std::string str(bytes.begin(), bytes.end());

	return std::shared_ptr<CONSTANT_Utf8_info>(new CONSTANT_Utf8_info(str));
};

std::shared_ptr<class CONSTANT_Class_info> CONSTANT_Class_info::parse(class file &file, std::vector<std::shared_ptr<class cp_info>> &elements)
{
	uint16_t index = file.read<uint16_t>();
	auto utf8 = util::dpc<CONSTANT_Utf8_info>(elements.at(index - 1));

	return std::shared_ptr<CONSTANT_Class_info>(new CONSTANT_Class_info(utf8->value));
};

std::shared_ptr<class ref_info> ref_info::parse(class file &file, std::vector<std::shared_ptr<class cp_info>> &elements)
{
	uint16_t class_index = file.read<uint16_t>();
	uint16_t name_and_type_index = file.read<uint16_t>();

	auto clss = util::dpc<CONSTANT_Class_info>(elements.at(class_index - 1));
	auto name_and_type = util::dpc<CONSTANT_NameAndType_info>(elements.at(name_and_type_index - 1));

	return std::shared_ptr<ref_info>(new ref_info(clss, name_and_type));
};

std::shared_ptr<class CONSTANT_NameAndType_info> CONSTANT_NameAndType_info::parse(class file& file, std::vector<std::shared_ptr<class cp_info>> &elements)
{
	uint16_t name_index = file.read<uint16_t>();
	uint16_t desciptor_index = file.read<uint16_t>();

	std::string name = util::dpc<CONSTANT_Utf8_info>(elements.at(name_index - 1))->value;
	std::string descriptor = util::dpc<CONSTANT_Utf8_info>(elements.at(desciptor_index - 1))->value;

	std::vector<std::shared_ptr<class type>> types = methods::descriptor_to_type(descriptor);

	return std::shared_ptr<CONSTANT_NameAndType_info>(new CONSTANT_NameAndType_info(name, types));
};

std::shared_ptr<class CONSTANT_String_info> CONSTANT_String_info::parse(class file& file, std::vector<std::shared_ptr<class cp_info>> &elements)
{
	uint16_t index = file.read<uint16_t>();

	auto utf8 = util::dpc<CONSTANT_Utf8_info>(elements.at(index - 1));

	return std::shared_ptr<CONSTANT_String_info>(new CONSTANT_String_info(utf8->value));
};

std::shared_ptr<class CONSTANT_Integer_info> CONSTANT_Integer_info::parse(class file& file,
									std::vector<std::shared_ptr<class cp_info>> &elements __attribute__ ((unused)))
{
	uint32_t value = file.read<uint32_t>();

	return std::shared_ptr<CONSTANT_Integer_info>(new CONSTANT_Integer_info(value));
};
