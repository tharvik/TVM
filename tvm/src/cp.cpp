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
#define cp_macro(name, id)					\
	case cp::tag::name:					\
		info = name##_info::parse(file, *this); \
		break;
#include "../macro/cp.m"
#undef cp_macro

	default:
		printf("%x\n", tag);
		std::cerr << "Unknow cp tag: " << tag << std::endl;
	}

	this->elements.push_back(info);
}

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
	CONSTANT_Utf8_info &utf8
		= dynamic_cast<CONSTANT_Utf8_info&>(*cp.get(index));

	return new CONSTANT_Class_info(utf8.value);
};

class CONSTANT_Fieldref_info *CONSTANT_Fieldref_info::parse(class file &file, class cp const &cp)
{
	uint16_t class_index = file.read<uint16_t>();
	uint16_t name_and_type_index = file.read<uint16_t>();

	CONSTANT_Class_info *clss = cp.get<CONSTANT_Class_info*>(class_index);
	CONSTANT_NameAndType_info *name_and_type = cp.get<CONSTANT_NameAndType_info*>(name_and_type_index);

	return new CONSTANT_Fieldref_info(clss, name_and_type);
};

class CONSTANT_Methodref_info *CONSTANT_Methodref_info::parse(class file &file, class cp const &cp)
{
	uint16_t class_index = file.read<uint16_t>();
	uint16_t name_and_type_index = file.read<uint16_t>();

	CONSTANT_Class_info *clss = cp.get<CONSTANT_Class_info*>(class_index);
	CONSTANT_NameAndType_info *name_and_type = cp.get<CONSTANT_NameAndType_info*>(name_and_type_index);

	return new CONSTANT_Methodref_info(clss, name_and_type);
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

