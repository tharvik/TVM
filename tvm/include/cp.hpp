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
	static class cp parse(class file &file);

	template<typename type = class cp_info>
	std::shared_ptr<type> get(uint16_t index) const;

private:
	cp(std::vector<std::shared_ptr<class cp_info>> &&elements);
	static void add_element(file& file, std::vector<std::shared_ptr<class cp_info>> &elements);

	std::vector<std::shared_ptr<class cp_info>> elements;
};

template<typename type>
std::shared_ptr<type> cp::get(uint16_t index) const
{
	auto elem = elements.at(index - 1);
	return util::dpc<type>(elem);
}

class cp_info
{
public:
	virtual ~cp_info() {} // only for vtable
};

class CONSTANT_Utf8_info : public cp_info
{
public:
	static std::shared_ptr<class CONSTANT_Utf8_info> parse(class file &file, std::vector<std::shared_ptr<class cp_info>> &elements);

	const std::string value;

private:
	CONSTANT_Utf8_info(std::string value)
		: value(value) {}
};

class CONSTANT_Class_info : public cp_info
{
public:
	static std::shared_ptr<class CONSTANT_Class_info> parse(class file &file, std::vector<std::shared_ptr<class cp_info>> &elements);

	std::string const name;

private:
	CONSTANT_Class_info(std::string name)
		: name(name) {};
};

class CONSTANT_NameAndType_info : public cp_info
{
public:
	static std::shared_ptr<class CONSTANT_NameAndType_info> parse(class file &file, std::vector<std::shared_ptr<class cp_info>> &elements);

	std::string const name;
	std::vector<std::shared_ptr<class type>> const types;

private:
	CONSTANT_NameAndType_info(std::string name, std::vector<std::shared_ptr<class type>> types)
		: name(name), types(types) {};
};

class ref_info : public cp_info
{
public:
	static std::shared_ptr<class ref_info> parse(class file &file, std::vector<std::shared_ptr<class cp_info>> &elements);

	std::shared_ptr<CONSTANT_Class_info> const clss;
	std::shared_ptr<CONSTANT_NameAndType_info> const name_and_type;

private:
	ref_info(std::shared_ptr<CONSTANT_Class_info> clss, std::shared_ptr<CONSTANT_NameAndType_info> name_and_type)
		: clss(clss), name_and_type(name_and_type)
	{}
};

class CONSTANT_String_info : public cp_info
{
public:
	static std::shared_ptr<class CONSTANT_String_info> parse(class file &file, std::vector<std::shared_ptr<class cp_info>> &elements);

	std::string const value;

private:
	CONSTANT_String_info(std::string value)
		: value(value) {};
};

class CONSTANT_Integer_info : public cp_info
{
public:
	static std::shared_ptr<class CONSTANT_Integer_info> parse(class file &file, std::vector<std::shared_ptr<class cp_info>> &elements);

	uint32_t const value;

private:
	CONSTANT_Integer_info(uint32_t value)
		: value(value) {};
};

#endif
