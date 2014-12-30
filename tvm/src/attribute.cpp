#include "attribute.hpp"

#include "cp.hpp"
#include "opcode.hpp"
#include "file.hpp"

#include <iostream>

attribute_info *attribute::parse(class file& file, class cp& cp)
{
	uint16_t index = file.read<uint16_t>();
	std::string name = cp.get<CONSTANT_Utf8_info*>(index)->value;

	file.read<uint32_t>();

	return get_element(file, cp, name);
}

attribute_info *attribute::get_element(class file& file, class cp& cp,
				       std::string name)
{
#define attribute_macro(type)				\
	if (name == #type) {				\
		return get_element_##type(file, cp);	\
	}
#include "../macro/attribute.m"
#undef attribute_macro

	std::cerr << "Unknow attribute: " << name << std::endl;
	throw "Unknow attribute: ";
}

attribute_info *attribute::get_element_Code(class file& file, class cp& cp)
{
	uint16_t max_stack;
	uint16_t max_locals;
	std::vector<opcode::base*> code;

	max_stack = file.read<uint16_t>();
	max_locals = file.read<uint16_t>();

	uint32_t size = file.read<uint32_t>();
	code.reserve(size);

	while (size > 0) {
		class opcode::base *opcode = opcode::base::parse(file);
		code.push_back(opcode);

		size -= opcode->size;
	}

	// drop it
	uint16_t exception_table_length = file.read<uint16_t>();
	for (; exception_table_length > 0; exception_table_length--)
		file.read(8);

	// drop it
	uint16_t attributes_count = file.read<uint16_t>();
	for (; attributes_count > 0; attributes_count--)
		delete attribute::parse(file, cp);

	return new Code_attribute(max_stack, max_locals, code);
}

attribute_info *attribute::get_element_LineNumberTable(class file& file __attribute__((unused)), class cp& cp __attribute__((unused)))
{
	// drop it
	uint16_t size = file.read<uint16_t>();
	for (; size > 0; size--)
		file.read(4);

	return new LineNumberTable_attribute();
}

#define attribute_macro(name)						\
	attribute_info *attribute::get_element_##name(			\
			class file& file __attribute__((unused)),	\
			class cp& cp __attribute__((unused)))	\
{									\
	std::cerr << "Unchecked get_element_" #name << std::endl;	\
	return nullptr;							\
}
#include "../macro/attribute_unchecked.m"
#undef attribute_macro
