#ifndef BC_CLSS_HPP
#define BC_CLSS_HPP

#include "vm_h.hpp"
#include "opcode_h.hpp"

#include "attribute_h.hpp"

#include <map>
#include <vector>

class clss
{
public:
	clss(std::string name);

	virtual void run_func(std::string name);

	class stack_elem::base *get_field(std::string name);
	void put_field(std::string name, class stack_elem::base *elem);

protected:
	clss();

private:
	std::map<std::string, Code_attribute*> meths;
	std::map<std::string, class stack_elem::base*> fields;

	class bc *bc;
};

class print_clss : public clss
{
public:
	void run_func(std::string name);
};

class StringBuilder : public clss
{
public:
	void run_func(std::string name);

private:
	class stack_elem::class_ref *append(class stack_elem::int_const* elem);
	class stack_elem::class_ref *append(class stack_elem::string_const* elem);
	class stack_elem::string_const *toString();

	std::string str;
};

#endif // CLSS_HPP
