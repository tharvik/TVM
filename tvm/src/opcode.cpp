#include "opcode.hpp"

opcode::base *opcode::base::parse(file& file)
{
	uint8_t op = file.read<uint8_t>();

	switch(op) {
#define opcode_macro(name, id, size)		\
	case id:				\
		return name::parse(file);
#include "../macro/opcode.m"
#undef opcode_macro
	}

	std::cerr << "Unknow opcode: " << op << std::endl;

	return nullptr;
}


class opcode::iadd *opcode::iadd::parse(file& file __attribute__ ((unused)))
{
	return new iadd();
}

void opcode::iadd::exec(class bc const &bc __attribute__ ((unused)), class vm &vm) const
{
	stack_elem::int_const* a = dynamic_cast<stack_elem::int_const*>(vm.stack.top());
	vm.stack.pop();
	stack_elem::int_const* b = dynamic_cast<stack_elem::int_const*>(vm.stack.top());
	vm.stack.pop();

	vm.stack.push(new stack_elem::int_const(a->value + b->value));

	delete a;
	delete b;
}

class opcode::dup *opcode::dup::parse(file& file __attribute__ ((unused)))
{
	return new dup();
}

void opcode::dup::exec(class bc const &bc __attribute__ ((unused)), class vm &vm) const
{
	vm.stack.push(vm.stack.top());
}

class opcode::getstatic *opcode::getstatic::parse(file& file)
{
	uint16_t index = file.read<uint16_t>();
	return new getstatic(index);
}

void opcode::getstatic::exec(class bc const &bc, class vm &vm) const
{
	std::cerr << "Patched getstatic" << std::endl;

        vm.stack.push(new stack_elem::print_class());
}

class opcode::invokevirtual *opcode::invokevirtual::parse(file& file)
{
	uint16_t index = file.read<uint16_t>();

	return new invokevirtual(index);
}

void opcode::invokevirtual::exec(class bc const &bc, class vm &vm) const
{
	CONSTANT_Methodref_info * meth = bc.cp.get<CONSTANT_Methodref_info*>(index);
}

#define opcode_macro(name, id, size)					\
	opcode::name *opcode::name::parse(file& file)			\
	{								\
		file.read(size - 1);					\
		return new name();					\
	}								\
									\
	void opcode::name::exec(					\
			class bc const &bc __attribute__ ((unused)),	\
			class vm &vm __attribute__ ((unused))) const	\
	{								\
		std::cerr << "Exec no implemented " #name << std::endl;	\
	}								\

#include "../macro/opcode_unchecked.m"
#undef opcode_macro
