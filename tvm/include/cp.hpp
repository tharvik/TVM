#ifndef BC_CP_HPP
#define BC_CP_HPP

#include "file.hpp"
#include "util.hpp"

#include <vector>
#include <cstdint>

class cp_info;

class cp
{
public:
	cp(class file& file);
	cp(class cp&& other);
	~cp();

	template<typename type = class cp_info*>
	type get(uint16_t index) const;

	enum tag {
		CONSTANT_Fieldref = 9,
		CONSTANT_Methodref = 10,
		CONSTANT_InterfaceMethodref = 11,
#define cp_macro(name, id)	\
	name = id,
#include "../macro/cp.m"
#undef cp_macro
	};

private:
	std::vector<cp_info*> elements;
	void add_element(file& file);
};

template<typename type>
type cp::get(uint16_t index) const
{
	cp_info *elem = elements.at(index - 1);
	return util::dn<type>(elem);
}


#include <cstdint>
class cp_info
{
public:
	virtual ~cp_info() {}
};

#include "type.hpp"

#include <cstdint>
class CONSTANT_Utf8_info : public cp_info
{
public:
	static class CONSTANT_Utf8_info * parse(class file &file, class cp const &cp);

	CONSTANT_Utf8_info(std::string value)
		: value(value) {}

	const std::string value;
};

#define cp_macro(name, id, size)						\
class name##_info : public cp_info {						\
public:										\
	static class name##_info * parse(class file &file, class cp const &cp);	\
};

#include "../macro/cp_unchecked.m"
#undef cp_macro

class CONSTANT_Class_info : public cp_info
{
public:
	static class CONSTANT_Class_info * parse(class file &file, class cp const &cp);
	std::string const name;

private:
	CONSTANT_Class_info(std::string name)
		: name(name) {};
};

class CONSTANT_NameAndType_info : public cp_info
{
public:

	static class CONSTANT_NameAndType_info * parse(class file &file, class cp const &cp);
	std::string const name;
	std::vector<type*> const types;

private:
	CONSTANT_NameAndType_info(std::string name, std::vector<type*> types)
		: name(name), types(types) {};
};

class ref_info : public cp_info
{
public:
	static class ref_info * parse(class file &file, class cp const &cp);
	CONSTANT_Class_info const * const clss;
	CONSTANT_NameAndType_info const * const name_and_type;

private:
	ref_info(CONSTANT_Class_info *clss, CONSTANT_NameAndType_info *name_and_type)
		: clss(clss), name_and_type(name_and_type)
	{}
};

class CONSTANT_String_info : public cp_info
{
public:
	static class CONSTANT_String_info * parse(class file &file, class cp const &cp);
	std::string const value;

private:
	CONSTANT_String_info(std::string value)
		: value(value) {};
};

#endif
