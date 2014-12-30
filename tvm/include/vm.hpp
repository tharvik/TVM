#ifndef VM_HPP
#define VM_HPP

#include <stack>
#include <cstdint>
#include <string>
#include <vector>
#include <map>

#include "clss.hpp"
#include "opcode_h.hpp"

namespace stack_elem
{
	class base
	{
	public:
		virtual ~base() {}
	};

#define macro_val_const(name, type)				\
	class name##_const : public base			\
	{							\
	public:							\
		name##_const(type value) : value(value) {}	\
		type const value;				\
	};
	macro_val_const(int, int)
	macro_val_const(string, std::string)
#undef macro_val_const

	class class_ref : public base
	{
	public:
		class_ref(class clss *cls) : cls(cls) {}
		class clss *cls;
	};

	class print_class : public class_ref
	{
	public:
		print_class() : class_ref(new print_clss()) {}
	};

}

class class_state
{
public:
	class_state(std::string class_name);


};

class class_pool
{
public:
	std::map<std::string, class_state*> map;
};

class vm
{
public:
	void exec(class bc const &bc, std::vector<opcode::base*> ops);

	std::stack<class stack_elem::base*> stack;
	std::vector<class stack_elem::base*> vars;
	class class_pool class_pool;

	void pc_goto(uint32_t index);

private:
	uint32_t pc;
	std::vector<opcode::base*> ops;
};

#endif // VM_HPP
