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

template<>
Code_attribute *attribute::get_element<Code_attribute>(class file& file, class cp& cp)
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

template<>
LineNumberTable_attribute *attribute::get_element<LineNumberTable_attribute>(class file& file, class cp& cp __attribute__ ((unused)))
{
	// drop it
	uint16_t size = file.read<uint16_t>();
	for (; size > 0; size--)
		file.read(4);

	return new LineNumberTable_attribute();
}

attribute_info *attribute::get_element(class file& file, class cp& cp,
				       std::string name)
{
	if (name == "Code")
		return get_element<Code_attribute>(file, cp);
	if (name == "LineNumberTable")
		return get_element<LineNumberTable_attribute>(file, cp);

	std::cerr << "Unknow attribute: " << name << std::endl;
	throw "Unknow attribute: ";
}

Code_attribute::~Code_attribute()
{
	for (opcode::base * elem : code)
		delete elem;
}
