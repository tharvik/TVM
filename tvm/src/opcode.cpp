#include "opcode.hpp"

#include "manager.hpp"

static void log_name(std::string name)
{
	std::cerr << manager::vms.size() << ":" << manager::vms.top().stack.size() << " ~ " << name << std::endl;
}

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

void opcode::iadd::exec(class bc const &bc __attribute__ ((unused))) const
{
	log_name("iadd");

	class vm &vm = manager::vms.top();

	stack_elem::int_const* a = dynamic_cast<stack_elem::int_const*>(vm.stack.top());
	vm.stack.pop();
	stack_elem::int_const* b = dynamic_cast<stack_elem::int_const*>(vm.stack.top());
	vm.stack.pop();

	vm.stack.push(new stack_elem::int_const(a->value + b->value));

	delete a;
	delete b;
}

void opcode::dup::exec(class bc const &bc __attribute__ ((unused))) const
{
	log_name("dup");

	class vm &vm = manager::vms.top();

	vm.stack.push(vm.stack.top());
}

void opcode::ldc::exec(class bc const &bc) const
{
	log_name("ldc");

	class vm &vm = manager::vms.top();
	class CONSTANT_String_info *info = bc.cp.get<CONSTANT_String_info*>(index);

	vm.stack.push(new stack_elem::string_const(info->value));
}

#define opcode_with_index(type, name)			\
class opcode::name *opcode::name::parse(file& file)	\
{							\
	type index = file.read<type>();			\
	return new name(index);				\
}
opcode_with_index(uint16_t, invokespecial)
opcode_with_index(uint16_t, invokevirtual)
opcode_with_index(uint16_t, getstatic)
opcode_with_index(uint8_t , ldc)
opcode_with_index(uint16_t, op_new)
#undef opcode_with_index

class opcode::bipush *opcode::bipush::parse(file& file)
{
	uint8_t index = file.read<uint8_t>();
	return new bipush(index);
}

void opcode::bipush::exec(class bc const &bc __attribute__ ((unused))) const
{
	log_name("bipush");

	manager::vms.top().stack.push(new stack_elem::int_const(byte));
}

void opcode::getstatic::exec(class bc const &bc __attribute__ ((unused))) const
{
	log_name("getstatic");

	class vm &vm = manager::vms.top();

	vm.stack.push(new stack_elem::class_ref(new print_clss()));
}

void opcode::invokespecial::exec(class bc const &bc) const
{
	log_name("invokespecial (nop, just remove obj)");

	manager::vms.top().stack.pop();
}

void opcode::pop::exec(class bc const &bc) const
{
	log_name("pop");

	manager::vms.top().stack.pop();
}

void opcode::invokevirtual::exec(class bc const &bc) const
{
	log_name("invokevirtual");

	ref_info * meth = bc.cp.get<ref_info*>(index);

	std::stack<class stack_elem::base*> &old_stack = manager::vms.top().stack;

	std::vector<class stack_elem::base*> args;
	short total_args = meth->name_and_type->types.size() - 1;

	args.reserve(total_args);
	for (; total_args > 0; total_args--) {
		args.push_back(old_stack.top());
		old_stack.pop();
	}

	std::reverse(args.begin(), args.end());

	class clss *cls = dynamic_cast<stack_elem::class_ref*>(old_stack.top())->cls;
	old_stack.pop();

	manager::vms.push(vm());
	class vm &vm = manager::vms.top();
	vm.vars = args;

	cls->run_func(meth->name_and_type->name);
}

void opcode::op_new::exec(class bc const &bc) const
{
	log_name("op_new");

	CONSTANT_Class_info *info = bc.cp.get<CONSTANT_Class_info*>(index);

	class clss *cls = manager::get_class(info->name);

	manager::vms.top().stack.push(new stack_elem::class_ref(cls));
}

#define trivial_opcode(name)							\
class opcode::name *opcode::name::parse(file& file __attribute__ ((unused)))	\
{										\
	return new name();							\
}
trivial_opcode(dup)
trivial_opcode(iadd)

trivial_opcode(iconst_0)
trivial_opcode(iconst_1)
trivial_opcode(iconst_2)
trivial_opcode(iconst_3)
trivial_opcode(iconst_4)
trivial_opcode(iconst_5)

trivial_opcode(ireturn)

trivial_opcode(istore_0)
trivial_opcode(istore_1)
trivial_opcode(istore_2)
trivial_opcode(istore_3)

trivial_opcode(op_return)
trivial_opcode(pop)
#undef trivial_opcode

#define iconst_macro(num)								\
void opcode::iconst_##num::exec(class bc const &bc __attribute__ ((unused))) const	\
{											\
	log_name("iconst_" #num);							\
	class vm &vm = manager::vms.top();						\
	vm.stack.push(new stack_elem::int_const(num));					\
}
iconst_macro(0)
iconst_macro(1)
iconst_macro(2)
iconst_macro(3)
iconst_macro(4)
iconst_macro(5)
#undef iconst_macro

#define istore_macro(num)								\
void opcode::istore_##num::exec(class bc const &bc __attribute__ ((unused))) const	\
{											\
	log_name("istore_" #num);							\
	class vm &vm = manager::vms.top();						\
											\
	class stack_elem::int_const *val = dynamic_cast<class stack_elem::int_const*>(vm.stack.top());\
	vm.vars.at(num) = val;								\
}
istore_macro(0)
istore_macro(1)
istore_macro(2)
istore_macro(3)
#undef istore_macro

void opcode::ireturn::exec(class bc const &bc __attribute__ ((unused))) const
{
	log_name("ireturn");

	class stack_elem::base *elem = manager::vms.top().stack.top();

	manager::vms.pop();

	manager::vms.top().stack.push(elem);
}

#define opcode_if(name)				\
opcode::name *opcode::name::parse(file& file)	\
{						\
	uint16_t branch;			\
						\
	branch = file.read<uint16_t>();		\
						\
	return new name(branch);		\
}
opcode_if(if_icmpeq)
opcode_if(ifeq)
#undef opcode_if

void opcode::if_icmpeq::exec(class bc const& bc) const
{
	log_name("if_icmpeq");

	class vm &vm = manager::vms.top();
	class stack_elem::int_const *a, *b;

	a = dynamic_cast<class stack_elem::int_const*>(vm.stack.top()); vm.stack.pop();
	b = dynamic_cast<class stack_elem::int_const*>(vm.stack.top()); vm.stack.pop();

	if (a->value == b->value)
		vm.pc_goto(branch);
}

void opcode::ifeq::exec(class bc const& bc) const
{
	log_name("ifeq");

	class vm &vm = manager::vms.top();
	class stack_elem::int_const *a;

	a = dynamic_cast<class stack_elem::int_const*>(vm.stack.top()); vm.stack.pop();

	if (a->value == 0)
		vm.pc_goto(branch);
}

void opcode::op_return::exec(class bc const& bc) const
{
	log_name("op_return");

	manager::vms.pop();
}

#define opcode_macro(name, id, length)					\
opcode::name *opcode::name::parse(file& file)				\
{									\
	file.read(length - 1);						\
	return new name();						\
}									\
									\
void opcode::name::exec(						\
		class bc const &bc __attribute__ ((unused))) const	\
{									\
	log_name("unimplemented : " #name);				\
}									\

#include "../macro/opcode_unchecked.m"
#undef opcode_macro
