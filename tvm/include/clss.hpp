#ifndef BC_CLSS_HPP
#define BC_CLSS_HPP

#include "vm_h.hpp"
#include "opcode_h.hpp"
#include "type.hpp"
#include "attribute_h.hpp"
#include "cp_h.hpp"

#include <map>
#include <vector>

class clss
{
public:
	clss(std::string name);

	void run_main();
	virtual void run_func(class method_info const * const meth);
	virtual void run_func(class ref_info const * const meth);

	class stack_elem::base *get_field(std::string name);
	void put_field(std::string name, class stack_elem::base *elem);

protected:
	clss();

private:
	std::map<class method_info const * const, Code_attribute*> meths;
	std::map<std::string, class stack_elem::base*> fields;

	class bc *bc;

	std::string name;
};

class optimized_clss : public clss
{
public:
	void run_func(class method_info const * const meth);
	void run_func(class ref_info const * const meth);

protected:
	virtual void run_func(std::string name, std::vector<type*> types) = 0;
};

class print_clss : public optimized_clss
{
private:
	void run_func(std::string name, std::vector<type*> types);
};

class StringBuilder : public optimized_clss
{
private:
	void run_func(std::string name, std::vector<type*> types);

	class stack_elem::class_ref *append(class stack_elem::int_const* elem);
	class stack_elem::class_ref *append(class stack_elem::string_const* elem);
	class stack_elem::string_const *toString();

	std::string str;
};

#endif // CLSS_HPP
