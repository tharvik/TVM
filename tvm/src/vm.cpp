#include "vm.hpp"

#include "opcode.hpp"

void vm::exec(class bc const &bc, std::vector<opcode::base*> ops)
{
	pc = 0;

	while (pc < ops.size()) {
		ops.at(pc++)->exec(bc, *this);
	}
}


void vm::pc_goto(std::vector<opcode::base*> const ops, uint32_t index)
{
	uint32_t count = 0;
	uint32_t i;

	for (i = 0; i < ops.size() && count < index; i++) {
		count += ops.at(i)->size;
	}

	pc = i;
}
