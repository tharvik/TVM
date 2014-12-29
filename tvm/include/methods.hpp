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
	static methods *parse(class file &file, class cp &cp);

	std::vector<class method_info*> const meths;

private:
	static method_info *parse_meth(class file &file, class cp &cp);

	methods(std::vector<class method_info*> meths) : meths(meths) {}

};

class method_info
{
public:
	method_info(uint16_t access_flags, std::string name,
		    std::string descriptor,
		    std::vector<class attribute_info*> attributes)
		: access_flags(access_flags), name(name),
		  descriptor(descriptor), attributes(attributes)
	{}

	uint16_t const access_flags;
	std::string const name;
	std::string const descriptor;

	std::vector<class attribute_info*> const attributes;
};

#endif
