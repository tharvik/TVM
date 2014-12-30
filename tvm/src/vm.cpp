#include "vm.hpp"

#include "opcode.hpp"

void vm::exec(class bc const &bc, std::vector<opcode::base*> ops)
{
	this->ops = ops;

	pc = 0;

	while (pc < ops.size())
		ops.at(pc++)->exec(bc);
}


void vm::pc_goto(uint32_t index)
{
	uint32_t i = pc - 1;

	for (; index > 0; i++)
		index -= ops.at(i)->size;

	pc = i;
}
