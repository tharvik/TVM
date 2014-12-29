#ifndef VM_HPP
#define VM_HPP

#include <stack>
#include <cstdint>
#include <string>
#include <vector>
#include <map>

#include "opcode_h.hpp"

namespace stack_elem
{
class base
{
public:
	virtual ~base() {}
};

class int_const : public base
{
public:
	int_const(int value) : value(value) {}

	int const value;
};

class class_ref : public base {
public:
	class_ref(std::map<std::string,std::vector<opcode::base*>> map) : map(map) {}

	void invokevirtual(class vm &vm, std::string name);

private:
	std::map<std::string,std::vector<opcode::base*>> map;
};

class print_class : public class_ref {
public:
	print_class() : class_ref(std::map<std::string,std::vector<opcode::base*>>()) {}
};

}

class vm
{
public:
	void exec(class bc const &bc, std::vector<opcode::base*> ops);

	std::stack<class stack_elem::base*> stack;

	void pc_goto(std::vector<opcode::base*> const ops, uint32_t index);

private:
	uint32_t pc;
};

#endif // VM_HPP
