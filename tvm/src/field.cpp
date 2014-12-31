#include "field.hpp"

#include "file.hpp"
#include "cp.hpp"
#include "attribute.hpp"

#include <iostream>

field *field::parse(file& file, cp &cp)
{
	uint16_t count = file.read<uint16_t>();

	std::vector<class field_info*> fields;
	for (; count > 0; count--) {
		class field_info *field = get_element(file, cp);
		fields.push_back(field);
	}

	return new field(fields);
}

field::~field()
{
	for (class field_info *field : fields)
		delete field;
}


field_info::~field_info()
{
	for (class attribute_info* attr : attributes)
		delete attr;
}

field_info *field::get_element(file& file, cp& cp)
{
	uint16_t access_flags;
	std::string name;
	std::string descriptor;
	std::vector<attribute_info*> attributes;

	access_flags = file.read<uint16_t>();
	name = cp.get<CONSTANT_Utf8_info*>(file.read<uint16_t>())->value;
	descriptor = cp.get<CONSTANT_Utf8_info*>(file.read<uint16_t>())->value;

	uint16_t attributes_count = file.read<uint16_t>();

	for (; attributes_count > 0; attributes_count--)
		attributes.push_back(attribute::parse(file, cp));

	return new field_info(access_flags, name, descriptor, attributes);
}
