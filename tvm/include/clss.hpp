#ifndef BC_CLSS_HPP
#define BC_CLSS_HPP

#include "vm_h.hpp"
#include "opcode_h.hpp"
#include "type.hpp"
#include "attribute_h.hpp"
#include "cp_h.hpp"

#include <map>
#include <vector>
#include <list>

class clss
{
public:
	clss(std::string name);
	virtual ~clss();

	virtual void run_func(std::string class_name, std::string name, std::vector<type*> types);

	class stack_elem::base *get_field(std::string name);
	void put_field(std::string name, class stack_elem::base *elem);

protected:
	clss();

private:
	std::map<std::pair<std::string, std::vector<type*>>, Code_attribute*> meths;
	std::map<std::string, class stack_elem::base*> fields;

	class bc* bc;
	class clss *parent;

	std::string name;
};

class print_clss : public clss
{
private:
	void run_func(std::string class_name, std::string name, std::vector<type*> types);
};

class StringBuilder : public clss
{
private:
	void run_func(std::string class_name, std::string name, std::vector<type*> types);

	class stack_elem::class_ref *append(class stack_elem::int_const* elem);
	class stack_elem::class_ref *append(class stack_elem::string_const* elem);
	class stack_elem::string_const *toString();

	std::string str;
};

#endif // CLSS_HPP
