#include "cp.hpp"

#include "bc.hpp"

#include <iostream>

cp::cp(class file& file)
{
	uint16_t count = file.read<uint16_t>();

	for (uint16_t i = count - 1; i > 0; i--)
		add_element(file);
}

cp::cp(class cp&& other)
{
	elements = other.elements;

	other.elements = std::vector<cp_info*>();
}

cp::~cp()
{
	for (cp_info *info : elements) {
		if (info == nullptr) continue;
		delete info;
	}
}

void cp::add_element(file& file)
{
	uint8_t tag = file.read<uint8_t>();

	cp_info *info;
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

class CONSTANT_Utf8_info *CONSTANT_Utf8_info::parse(class file &file, class cp const &cp __attribute__ ((unused)))
{
	uint16_t length = file.read<uint16_t>();

	std::vector<uint8_t> bytes = file.read(length);
	std::string str(bytes.begin(), bytes.end());

	return new CONSTANT_Utf8_info(str);
};

class CONSTANT_Class_info *CONSTANT_Class_info::parse(class file &file, class cp const &cp)
{
	uint16_t index = file.read<uint16_t>();
	CONSTANT_Utf8_info *utf8
		= util::dn<CONSTANT_Utf8_info*>(cp.get(index));

	return new CONSTANT_Class_info(utf8->value);
};

class ref_info *ref_info::parse(class file &file, class cp const &cp)
{
	uint16_t class_index = file.read<uint16_t>();
	uint16_t name_and_type_index = file.read<uint16_t>();

	CONSTANT_Class_info *clss = cp.get<CONSTANT_Class_info*>(class_index);
	CONSTANT_NameAndType_info *name_and_type = cp.get<CONSTANT_NameAndType_info*>(name_and_type_index);

	return new ref_info(clss, name_and_type);
};

class CONSTANT_NameAndType_info *CONSTANT_NameAndType_info::parse(class file& file, class cp const& cp)
{
	uint16_t name_index = file.read<uint16_t>();
	uint16_t desciptor_index = file.read<uint16_t>();

	std::string name = cp.get<CONSTANT_Utf8_info*>(name_index)->value;
	std::string descriptor = cp.get<CONSTANT_Utf8_info*>(desciptor_index)->value;

	std::vector<type*> types;

	for (auto i = descriptor.begin(); i != descriptor.end(); i++) {
		class type *type;
		switch(*i) {
		case 'L': {
			std::string class_name;
			while (*i != ';') class_name += *i++;
			type = new type_class(class_name);
			break;
		}

		case 'V': type = new type_void(); break;
		case 'I': type = new type_int(); break;
		case 'Z': type = new type_int(); break;

		case '(': continue;
		case ')': continue;

		default:
			std::cerr << "Unknow type: " << *i << std::endl;
		}

		types.push_back(type);
	}

	return new CONSTANT_NameAndType_info(name, types);
};

class CONSTANT_String_info *CONSTANT_String_info::parse(class file& file, class cp const& cp)
{
	uint16_t index = file.read<uint16_t>();

	CONSTANT_Utf8_info *utf8 = cp.get<CONSTANT_Utf8_info*>(index);

	return new CONSTANT_String_info(utf8->value);
};


#define cp_macro(name, id, size)				\
class name##_info * name##_info::parse(				\
		class file &file __attribute__ ((unused)),	\
		class cp const &cp __attribute__ ((unused)))	\
{								\
	std::cerr << "Unchecked add_" #name << std::endl;	\
								\
	file.read(size - 1);					\
								\
	return nullptr;						\
}
#include "../macro/cp_unchecked.m"
#undef cp_macro
