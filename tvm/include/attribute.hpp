#ifndef BC_ATTRIBUTE_HPP
#define BC_ATTRIBUTE_HPP

#include "file_h.hpp"
#include "cp_h.hpp"
#include "opcode_h.hpp"

#include <cstdint>
#include <string>
#include <vector>

class attribute_info;
class attribute
{
public:
	static attribute_info *parse(class file& file, class cp& cp);

	enum tag {
#define attribute_macro(name)	\
		name,
#include "../macro/attribute.m"
#undef attribute_macro
	};
private:
	static attribute_info *get_element(class file& file, class cp& cp,
					   std::string name);

#define attribute_macro(name)	\
	static attribute_info *get_element_##name(class file& file,	\
						  class cp& cp);
#include "../macro/attribute.m"
#undef attribute_macro
};

class attribute_info
{
public:
	attribute::tag tag;

	virtual ~attribute_info() {}
protected:
	attribute_info(attribute::tag tag) : tag(tag) {}
};

class Code_attribute : public attribute_info
{
public:
	Code_attribute(uint16_t max_stack, uint16_t max_locals,
		       std::vector<opcode::base*> code)
		: attribute_info(attribute::tag::Code), max_stack(max_stack),
		  max_locals(max_locals), code(code) {}

	uint16_t const max_stack;
	uint16_t const max_locals;
	std::vector<opcode::base*> code;



	/*uint16_t exception_table_length;
	{   uint16_t start_pc;
		uint16_t end_pc;
		uint16_t handler_pc;
		uint16_t catch_type;
	}
	exception_table[exception_table_length];
	uint16_t attributes_count;
	attribute_info
		attributes[attributes_count];*/
};

#endif
