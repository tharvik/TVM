#ifndef BC_ATTRIBUTE_HPP
#define BC_ATTRIBUTE_HPP

#include "file_h.hpp"
#include "cp_h.hpp"
#include "opcode_h.hpp"

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

class attribute_info;
class attribute
{
public:
	static std::shared_ptr<class attribute_info> parse(class file& file, class cp& cp);
private:
	static std::shared_ptr<class attribute_info> get_element(class file& file, class cp& cp,
					   std::string name);

	template <typename type>
	static std::shared_ptr<type> get_element(class file& file, class cp& cp);
};

class attribute_info
{
public:
	virtual ~attribute_info() {}
protected:
	attribute_info() {}
};

class Code_attribute : public attribute_info
{
public:
	Code_attribute(uint16_t max_stack, uint16_t max_locals,
		       std::vector<opcode::base*> code)
		: max_stack(max_stack),
		  max_locals(max_locals), code(code) {}

	~Code_attribute();

	uint16_t const max_stack;
	uint16_t const max_locals;
	std::vector<opcode::base*> code;
};

class LineNumberTable_attribute : public attribute_info
{
};

#endif
