#ifndef BC_METHOD_HPP
#define BC_METHOD_HPP

#include "methods_h.hpp"

#include "file_h.hpp"
#include "attribute_h.hpp"

#include <cstdint>
#include <vector>
#include <string>
#include <memory>

class methods
{
public:
	static class methods parse(class file &file, class cp &cp);
	methods() {}

	std::vector<std::shared_ptr<class method_info>> const meths;

	static std::vector<std::shared_ptr<class type>> descriptor_to_type(std::string descriptor);

private:
	static std::shared_ptr<class method_info> parse_meth(class file &file, class cp &cp);

	methods(std::vector<std::shared_ptr<class method_info>> meths) : meths(meths) {}

};

class method_info
{
public:
	method_info(uint16_t access_flags, std::string name,
		    std::vector<std::shared_ptr<class type>> types,
		    std::vector<std::shared_ptr<class attribute_info>> attributes)
		: access_flags(access_flags), name(name),
		  types(types), attributes(attributes)
	{}

	uint16_t const access_flags;
	std::string const name;
	std::vector<std::shared_ptr<class type>> types;

	std::vector<std::shared_ptr<class attribute_info>> const attributes;
};

#endif
