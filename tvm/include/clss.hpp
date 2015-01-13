#ifndef BC_CLSS_HPP
#define BC_CLSS_HPP

#include "vm_h.hpp"
#include "opcode_h.hpp"
#include "type.hpp"
#include "attribute_h.hpp"
#include "cp_h.hpp"
#include "bc.hpp"

#include <map>
#include <vector>
#include <list>
#include <memory>

class clss
{
public:
	clss();
	clss(std::string name);

	virtual void run_func(
			std::string const class_name,
			std::string const name,
			std::vector<std::shared_ptr<class type>> const &types,
			std::vector<std::shared_ptr<class stack_elem::base>> args);

	std::shared_ptr<class stack_elem::base> get_field(std::string name);
	void put_field(std::string name, std::shared_ptr<class stack_elem::base> elem);

	std::string const name;

protected:
	clss(std::string name, bool load_bc);

private:
	std::map<std::pair<std::string, std::vector<std::shared_ptr<class type>>>, std::shared_ptr<Code_attribute>> meths;
	std::map<std::string, std::shared_ptr<class stack_elem::base>> fields;

	std::shared_ptr<class bc> bc;
	std::shared_ptr<class clss> parent;

};

class String : public clss
{
public:
	String(std::string value);

	std::string const value;
};

class print_clss : public clss
{
public:
	void run_func(
		std::string const class_name,
		std::string const name,
		std::vector<std::shared_ptr<class type>> const &types,
		std::vector<std::shared_ptr<class stack_elem::base>> args);
};

class StringBuilder : public clss
{
public:
	void run_func(
		std::string const class_name,
		std::string const name,
		std::vector<std::shared_ptr<class type>> const &types,
		std::vector<std::shared_ptr<class stack_elem::base>> args);

	std::shared_ptr<class stack_elem::class_ref> append(std::shared_ptr<class stack_elem::const_val<int>> elem);
	std::shared_ptr<class stack_elem::class_ref> append(std::shared_ptr<class stack_elem::class_ref> elem);
	std::shared_ptr<class stack_elem::class_ref> toString();
private:
	std::string str;
};

#endif // CLSS_HPP
