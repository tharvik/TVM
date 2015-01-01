#ifndef VM_HPP
#define VM_HPP

#include <stack>
#include <cstdint>
#include <string>
#include <array>
#include <vector>
#include <map>
#include <cassert>

#include "clss.hpp"
#include "opcode_h.hpp"
#include "util.hpp"

namespace stack_elem
{
	class base
	{
	public:
		virtual ~base() {}
		virtual stack_elem::base *copy() = 0;
	};

#define macro_val_const(name, type, by_default)			\
	class name##_const : public base			\
	{							\
	public:							\
		name##_const() : value(by_default) {}		\
		name##_const(type value) : value(value) {}	\
		type const value;				\
		stack_elem::base *copy() {return new name##_const(value);}\
	};
	macro_val_const(int, int, 0)
	macro_val_const(bool, bool, 0)
	macro_val_const(string, std::string, "")
#undef macro_val_const

	class array_ref : public base
	{
	public:
		array_ref(uint32_t size) {
			vec = new std::vector<stack_elem::int_const*>();
			vec->reserve(size);
			for(; size > 0; size--)
				vec->push_back(new stack_elem::int_const(0));
		}

		std::vector<stack_elem::int_const*> *vec;

		stack_elem::base *copy() {
			class array_ref *a = new array_ref(vec->size());
			a->vec = vec;
			return a;
		}
	};

	class class_ref : public base
	{
	public:
		class_ref(class clss *cls) : cls(cls) {}
		class clss *cls;

		stack_elem::base *copy() {return new class_ref(cls);}
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

	void pc_goto(int32_t index);

private:
	uint32_t pc;
	std::vector<opcode::base*> ops;
};

#endif // VM_HPP
