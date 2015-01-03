#ifndef BC_METHOD_HPP
#define BC_METHOD_HPP

#include "methods_h.hpp"

#include "file_h.hpp"
#include "attribute_h.hpp"

#include <cstdint>
#include <vector>
#include <string>

class methods
{
public:
	static class methods *parse(class file &file, class cp &cp);
	~methods();

	std::vector<class method_info*> const meths;

	static std::vector<class type*> descriptor_to_type(std::string descriptor);

private:
	static method_info *parse_meth(class file &file, class cp &cp);

	methods(std::vector<class method_info*> meths) : meths(meths) {}

};

class method_info
{
public:
	method_info(uint16_t access_flags, std::string name,
		    std::vector<class type*> types,
		    std::vector<class attribute_info*> attributes)
		: access_flags(access_flags), name(name),
		  types(types), attributes(attributes)
	{}

	~method_info();

	uint16_t const access_flags;
	std::string const name;
	std::vector<class type*> types;

	std::vector<class attribute_info*> const attributes;
};

#endif
