#include "methods.hpp"

#include "file.hpp"
#include "cp.hpp"
#include "attribute.hpp"

#include <iostream>

class methods methods::parse(class file &file, class cp &cp)
{
	std::vector<std::shared_ptr<class method_info>> meths;
	uint16_t size;

	size = file.read<uint16_t>();

	for (; size > 0; size--)
		meths.push_back(parse_meth(file, cp));

	return methods(meths);
}

static std::shared_ptr<class type> char_to_type(std::string::iterator &iter)
{
	std::shared_ptr<class type> type;

	switch(*iter) {
	case 'L': {
		std::string class_name;
		while (*iter != ';') class_name += *iter++;
		type = type::get(class_name);
		break;
	}

	case 'V':
		type = type::get(type::VOID);
		break;
	case 'I':
		type = type::get(type::INT);
		break;
	case 'Z':
		type = type::get(type::BOOL);
		break;

	case '[':
		type = type::get(char_to_type(++iter));
		break;

	case '(':
		break;
	case ')':
		break;

	default:
		std::cerr << "Unknow type: " << *iter << std::endl;
		throw "unknow type";
	}

	return type;
}

std::vector<std::shared_ptr<class type>> methods::descriptor_to_type(std::string descriptor)
{
	std::vector<std::shared_ptr<class type>> types;

	for (auto i = descriptor.begin(); i != descriptor.end(); i++) {
		auto tpe = char_to_type(i);
		if (tpe == nullptr) continue;
		types.push_back(tpe);
	}

	return types;
}

std::shared_ptr<class method_info> methods::parse_meth(class file &file, class cp &cp)
{
	uint16_t access_flags;
	std::string name;
	std::string descriptor;
	uint16_t attributes_count;
	std::vector<std::shared_ptr<class attribute_info>> attributes;

	access_flags = file.read<uint16_t>();
	name = cp.get<class CONSTANT_Utf8_info>(file.read<uint16_t>())->value;
	descriptor = cp.get<class CONSTANT_Utf8_info>(file.read<uint16_t>())->value;

	attributes_count = file.read<uint16_t>();
	attributes.reserve(attributes_count);
	for (; attributes_count > 0; attributes_count--)
		attributes.push_back(attribute::parse(file, cp));

	auto const &types = descriptor_to_type(descriptor);

	return std::shared_ptr<method_info>(new method_info(access_flags, name, types, attributes));
}
