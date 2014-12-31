#ifndef BC_FIELD_HPP
#define BC_FIELD_HPP

#include "file_h.hpp"
#include "attribute_h.hpp"
#include "cp_h.hpp"

#include <cstdint>
#include <string>
#include <vector>

class field_info;

class field
{
public:
	static field *parse(class file& file, class cp &cp);

	~field();

	std::vector<class field_info*> const fields;

private:
	static field_info *get_element(class file& file, class cp &cp);

	field(std::vector<class field_info*> fields) : fields(fields) {}
};

class field_info
{
public:
	field_info(uint16_t access_flags, std::string name,
		   std::string descriptor,
		   std::vector<attribute_info*> attributes)
		: access_flags(access_flags), name(name),
		  descriptor(descriptor), attributes(attributes) {}
	~field_info();

	uint16_t const access_flags;
	std::string const name;
	std::string const descriptor;
	std::vector<attribute_info*> const attributes;
};

#endif
