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
