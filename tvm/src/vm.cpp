#include "vm.hpp"

#include "opcode.hpp"

void vm::exec(class bc const &bc, std::vector<opcode::base*> ops)
{
	this->ops = ops;

	pc = 0;

	while (pc < ops.size()) {
		ops.at(pc)->exec(bc);
		pc++;
	}
}

void vm::pc_goto(int32_t index)
{
	uint32_t i = pc;

	if (index > 0) {
		for (; index > 0; i++)
			index -= ops.at(i)->size;
	} else {
		i--;
		for(; index < 0; i--)
			index += ops.at(i)->size;
		i++;
	}
	i--;

	pc = i;
}

vm::~vm()
{
	while(!stack.empty()) {
		class stack_elem::base* elem = stack.top();
		stack.pop();

		delete elem;
	}

	for(class stack_elem::base* elem : vars)
		delete elem;
}

stack_elem::array_ref::array_ref(uint32_t size)
{
	vec = new std::vector<stack_elem::int_const*>();
	vec->reserve(size);
	for(; size > 0; size--)
		vec->push_back(new stack_elem::int_const(0));
}

stack_elem::base *stack_elem::array_ref::copy()
{
	class array_ref *a = new array_ref(vec->size());
	a->vec = vec;
	return a;
}
