#include "field.hpp"

#include "file.hpp"
#include "cp.hpp"
#include "attribute.hpp"
#include "methods.hpp"

#include <iostream>

class field field::parse(file& file, cp &cp)
{
	uint16_t count = file.read<uint16_t>();

	std::vector<class field_info> fields;
	for (; count > 0; count--) {
		class field_info const &field = get_element(file, cp);
		fields.push_back(field);
	}

	return field(std::move(fields));
}

field_info const field::get_element(file& file, cp& cp)
{
	uint16_t const access_flags = file.read<uint16_t>();
	std::string const name = cp.get<CONSTANT_Utf8_info>(file.read<uint16_t>())->value;
	std::string const descriptor = cp.get<CONSTANT_Utf8_info>(file.read<uint16_t>())->value;

	std::vector<std::shared_ptr<attribute_info>> attributes;
	uint16_t attributes_count = file.read<uint16_t>();
	attributes.reserve(attributes_count);
	for (; attributes_count > 0; attributes_count--)
		attributes.push_back(attribute::parse(file, cp));

	std::shared_ptr<class type> type = methods::descriptor_to_type(descriptor).at(0);

	return field_info(access_flags, name, std::move(type), attributes);
}
