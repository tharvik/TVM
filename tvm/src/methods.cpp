#include "methods.hpp"

#include "file.hpp"
#include "cp.hpp"
#include "attribute.hpp"

#include <iostream>

methods *methods::parse(class file &file, class cp &cp)
{
	std::vector<class method_info*> meths;
	uint16_t size;

	size = file.read<uint16_t>();

	std::cout << "methods: " << size << std::endl;

	for (; size > 0; size--)
		meths.push_back(parse_meth(file, cp));

	return new methods(meths);
}

method_info *methods::parse_meth(class file &file, class cp &cp)
{
	uint16_t access_flags;
	std::string name;
	std::string descriptor;
	uint16_t attributes_count;
	std::vector<class attribute_info*> attributes;

	access_flags = file.read<uint16_t>();
	name = cp.get<class CONSTANT_Utf8_info*>(file.read<uint16_t>())->value;
	descriptor = cp.get<class CONSTANT_Utf8_info*>(file.read<uint16_t>())->value;

	attributes_count = file.read<uint16_t>();
	attributes.reserve(attributes_count);
	std::cerr << "attributes_count: " << attributes_count << std::endl;
	for (; attributes_count > 0; attributes_count--)
		attributes.push_back(attribute::parse(file, cp));

	return new method_info(access_flags, name, descriptor, attributes);
}
