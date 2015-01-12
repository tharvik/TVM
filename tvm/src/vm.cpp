#include "vm.hpp"

#include "opcode.hpp"

vm::vm(class bc const &bc, std::vector<std::unique_ptr<opcode::base>> const &ops) : pc(0), bc(bc), ops(ops)
{
}

void vm::exec()
{
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

stack_elem::array_ref::array_ref(uint32_t size) : vec(new std::vector<std::shared_ptr<stack_elem::int_const>>())
{
	vec->reserve(size);
	for(; size > 0; size--)
		vec->push_back(std::make_shared<class stack_elem::int_const>(0));
}
