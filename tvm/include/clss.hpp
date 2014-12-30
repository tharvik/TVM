#ifndef BC_CLSS_HPP
#define BC_CLSS_HPP

#include "vm_h.hpp"
#include "opcode_h.hpp"
#include "val.hpp"

#include "attribute_h.hpp"

#include <map>
#include <vector>

class clss
{
public:
	clss(std::string name);

	virtual void run_func(std::string name);

	class val get_field(std::string name);
	void set_field(std::string name, class val val);

protected:
	clss();

private:
	std::map<std::string, Code_attribute*> meths;
	std::map<std::string, class val> fields;

	class bc *bc;
};

class print_clss : public clss
{
public:
	print_clss() {}

	void run_func(std::string name);
};

#endif // CLSS_HPP
