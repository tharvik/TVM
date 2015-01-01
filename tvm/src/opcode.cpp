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
	throw "Unknow opcode";
}

#define opcode_macro(name, op)							\
void opcode::name::exec(class bc const &bc __attribute__ ((unused))) const	\
{										\
	log_name(#name);							\
										\
	class vm &vm = manager::vms.top();					\
										\
	stack_elem::int_const* b = util::dn<stack_elem::int_const*>(vm.stack.top());\
	vm.stack.pop();								\
	stack_elem::int_const* a = util::dn<stack_elem::int_const*>(vm.stack.top());\
	vm.stack.pop();								\
										\
	vm.stack.push(new stack_elem::int_const(a->value op b->value));		\
}
opcode_macro(iadd, +)
opcode_macro(idiv, /)
opcode_macro(imul, *)
opcode_macro(isub, -)
#undef opcode_macro

void opcode::dup::exec(class bc const &bc __attribute__ ((unused))) const
{
	log_name("dup");

	class vm &vm = manager::vms.top();

	vm.stack.push(vm.stack.top());
}

void opcode::swap::exec(class bc const &bc __attribute__ ((unused))) const
{
	log_name("swap");

	class vm &vm = manager::vms.top();

	stack_elem::base* a = vm.stack.top();
	vm.stack.pop();
	stack_elem::base* b = vm.stack.top();
	vm.stack.pop();

	vm.stack.push(a);
	vm.stack.push(b);
}

void opcode::ldc::exec(class bc const &bc) const
{
	log_name("ldc");

	class vm &vm = manager::vms.top();

	class cp_info *info = bc.cp.get(index);
	class stack_elem::base* elem = nullptr;

	class CONSTANT_String_info *str;
	class CONSTANT_Integer_info *integer;
	if ((str = dynamic_cast<CONSTANT_String_info*>(info)))
		elem = new stack_elem::string_const(str->value);
	else if ((integer = dynamic_cast<CONSTANT_Integer_info*>(info)))
		elem = new stack_elem::int_const(integer->value);

	vm.stack.push(elem);
}

#define opcode_with_index(type, name)			\
class opcode::name *opcode::name::parse(file& file)	\
{							\
	type index = file.read<type>();			\
	return new name(index);				\
}
opcode_with_index(uint16_t, invokespecial)
opcode_with_index(uint16_t, invokevirtual)

opcode_with_index(uint8_t, astore)
opcode_with_index(uint8_t, istore)

opcode_with_index(uint8_t, aload)
opcode_with_index(uint8_t, iload)

opcode_with_index(uint16_t, getstatic)
opcode_with_index(uint16_t, getfield)

opcode_with_index(uint8_t , ldc)
opcode_with_index(uint8_t , newarray)
opcode_with_index(uint16_t, op_new)
opcode_with_index(uint16_t, putfield)
#undef opcode_with_index

#define macro_push(name, type)					\
class opcode::name *opcode::name::parse(file& file)		\
{								\
	type index = file.read<type>();				\
	return new name(index);					\
}								\
								\
void opcode::name::exec(class bc const &bc __attribute__ ((unused))) const \
{								\
	log_name(#name);					\
								\
	manager::vms.top().stack.push(new stack_elem::int_const(byte));\
}
macro_push(bipush, uint8_t)
macro_push(sipush, uint16_t)
#undef macro_push

void opcode::getstatic::exec(class bc const &bc __attribute__ ((unused))) const
{
	log_name("getstatic");

	class vm &vm = manager::vms.top();

	vm.stack.push(new stack_elem::class_ref(new print_clss()));
}

void opcode::getfield::exec(class bc const &bc __attribute__ ((unused))) const
{
	class vm &vm = manager::vms.top();
	class ref_info *info = bc.cp.get<ref_info*>(index);

	log_name("getfield\t\t" + info->name_and_type->name);

	class clss *cls = util::dn<stack_elem::class_ref*>(vm.stack.top())->cls;
	vm.stack.pop();

	vm.stack.push(cls->get_field(info->name_and_type->name)->copy());
}

void opcode::putfield::exec(class bc const &bc __attribute__ ((unused))) const
{
	class vm &vm = manager::vms.top();
	class ref_info *info = bc.cp.get<ref_info*>(index);

	log_name("putfield\t\t" + info->name_and_type->name);

	class stack_elem::base *elem = vm.stack.top();
	vm.stack.pop();
	class clss *cls = util::dn<stack_elem::class_ref*>(vm.stack.top())->cls;
	vm.stack.pop();

	cls->put_field(info->name_and_type->name, elem);
}

void opcode::invokespecial::exec(class bc const &bc __attribute__ ((unused))) const
{
	log_name("invokespecial (nop, just remove obj)");

	manager::vms.top().stack.pop();
}

void opcode::pop::exec(class bc const &bc __attribute__ ((unused))) const
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

	args.reserve(total_args + 1);
	for (; total_args > 0; total_args--) {
		args.push_back(old_stack.top()->copy());
		old_stack.pop();
	}
	args.push_back(old_stack.top());
	std::reverse(args.begin(), args.end());

	class clss *cls = util::dn<stack_elem::class_ref*>(args.at(0))->cls;
	old_stack.pop();

	manager::vms.push(vm());
	class vm &vm = manager::vms.top();
	vm.vars = args;

	cls->run_func(meth);
}

void opcode::op_new::exec(class bc const &bc) const
{
	log_name("op_new");

	CONSTANT_Class_info *info = bc.cp.get<CONSTANT_Class_info*>(index);

	class clss *cls = manager::get_class(info->name);

	manager::vms.top().stack.push(new stack_elem::class_ref(cls));
}

void opcode::newarray::exec(class bc const &bc __attribute__ ((unused))) const
{
	log_name("newarray");

	class vm &vm = manager::get_vm();

	stack_elem::int_const *count = util::dn<stack_elem::int_const*>(vm.stack.top());
	vm.stack.pop();

	manager::get_vm().stack.push(new stack_elem::array_ref(count->value));
}

#define trivial_opcode(name)							\
class opcode::name *opcode::name::parse(file& file __attribute__ ((unused)))	\
{										\
	return new name();							\
}
trivial_opcode(aload_0)
trivial_opcode(aload_1)
trivial_opcode(aload_2)
trivial_opcode(aload_3)

trivial_opcode(astore_0)
trivial_opcode(astore_1)
trivial_opcode(astore_2)
trivial_opcode(astore_3)

trivial_opcode(areturn)
trivial_opcode(arraylength)
trivial_opcode(dup)
trivial_opcode(iadd)
trivial_opcode(imul)
trivial_opcode(idiv)
trivial_opcode(isub)

trivial_opcode(iaload)
trivial_opcode(iastore)

trivial_opcode(iconst_0)
trivial_opcode(iconst_1)
trivial_opcode(iconst_2)
trivial_opcode(iconst_3)
trivial_opcode(iconst_4)
trivial_opcode(iconst_5)

trivial_opcode(iload_0)
trivial_opcode(iload_1)
trivial_opcode(iload_2)
trivial_opcode(iload_3)

trivial_opcode(ireturn)

trivial_opcode(istore_0)
trivial_opcode(istore_1)
trivial_opcode(istore_2)
trivial_opcode(istore_3)

trivial_opcode(op_return)
trivial_opcode(pop)
trivial_opcode(swap)
#undef trivial_opcode

#define load_macro(type, num)								\
void opcode::type##load_##num::exec(class bc const &bc __attribute__ ((unused))) const	\
{											\
	log_name(#type "load_" #num);							\
	class vm &vm = manager::vms.top();						\
	vm.stack.push(vm.vars.at(num));							\
}
load_macro(a, 0)
load_macro(a, 1)
load_macro(a, 2)
load_macro(a, 3)

load_macro(i, 0)
load_macro(i, 1)
load_macro(i, 2)
load_macro(i, 3)
#undef load_macro

#define load_macro(type)								\
void opcode::type##load::exec(class bc const &bc __attribute__ ((unused))) const	\
{											\
	log_name(#type "load");								\
	class vm &vm = manager::vms.top();						\
	vm.stack.push(vm.vars.at(index));							\
}
load_macro(a)
load_macro(i)
#undef load_macro

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

#define store_macro(type, num)								\
void opcode::type##store_##num::exec(class bc const &bc __attribute__ ((unused))) const	\
{											\
	log_name(#type "store_" #num);							\
	class vm &vm = manager::vms.top();						\
											\
	class stack_elem::base *val = vm.stack.top();					\
	vm.vars.at(num) = val->copy();							\
	vm.stack.pop();									\
}
store_macro(a, 0)
store_macro(a, 1)
store_macro(a, 2)
store_macro(a, 3)

store_macro(i, 0)
store_macro(i, 1)
store_macro(i, 2)
store_macro(i, 3)
#undef store_macro

#define macro_store(name)								\
void opcode::name##store::exec(class bc const &bc __attribute__ ((unused))) const	\
{											\
	log_name(#name "store");							\
	class vm &vm = manager::vms.top();						\
											\
	class stack_elem::base *val = vm.stack.top();					\
	vm.vars.at(index) = val->copy();						\
	vm.stack.pop();									\
}
macro_store(a)
macro_store(i)
#undef macro_store

#define macro_return(type)						\
void opcode::type##return::exec(class bc const &bc __attribute__ ((unused))) const\
{									\
	log_name(#type "return");					\
									\
	class stack_elem::base *elem = manager::vms.top().stack.top();	\
									\
	manager::vms.pop();						\
									\
	manager::vms.top().stack.push(elem);				\
}
macro_return(a)
macro_return(i)
#undef macro_return

#define opcode_if(name)				\
opcode::name *opcode::name::parse(file& file)	\
{						\
	int16_t branch;			\
						\
	branch = file.read<uint16_t>();		\
						\
	return new name(branch);		\
}
opcode_if(if_acmpeq)
opcode_if(if_acmpne)

opcode_if(if_icmpeq)
opcode_if(if_icmpne)
opcode_if(if_icmplt)
opcode_if(if_icmpge)
opcode_if(if_icmpgt)
opcode_if(if_icmple)

opcode_if(ifeq)
opcode_if(ifne)
opcode_if(iflt)
opcode_if(ifge)
opcode_if(ifgt)
opcode_if(ifle)

opcode_if(op_goto)
#undef opcode_if

#define opcode_if(name, op)					\
void opcode::if_acmp##name::exec(class bc const& bc __attribute__ ((unused))) const	\
{								\
	log_name("if_acmp" #name);				\
								\
	class vm &vm = manager::vms.top();			\
	class stack_elem::class_ref *value1, *value2;		\
								\
	value2 = util::dn<class stack_elem::class_ref*>(vm.stack.top()); vm.stack.pop();\
	value1 = util::dn<class stack_elem::class_ref*>(vm.stack.top()); vm.stack.pop();\
								\
	if (value1->cls op value2->cls)				\
		vm.pc_goto(branch);				\
}
opcode_if(eq, ==)
opcode_if(ne, !=)
#undef opcode_if

#define opcode_if(name, op)					\
void opcode::if_icmp##name::exec(class bc const& bc __attribute__ ((unused))) const	\
{								\
	log_name("if_icmp" #name);				\
								\
	class vm &vm = manager::vms.top();			\
	class stack_elem::int_const *value1, *value2;		\
								\
	value2 = util::dn<class stack_elem::int_const*>(vm.stack.top()); vm.stack.pop();\
	value1 = util::dn<class stack_elem::int_const*>(vm.stack.top()); vm.stack.pop();\
								\
	if (value1->value op value2->value)			\
		vm.pc_goto(branch);				\
}
opcode_if(eq, ==)
opcode_if(ne, !=)
opcode_if(lt, <)
opcode_if(ge, >=)
opcode_if(gt, >)
opcode_if(le, <=)
#undef opcode_if

#define opcode_if(name, op)								\
void opcode::if##name::exec(class bc const& bc __attribute__ ((unused))) const		\
{											\
	log_name("if" #name);								\
											\
	class vm &vm = manager::vms.top();						\
	class stack_elem::int_const *a;							\
											\
	a = util::dn<class stack_elem::int_const*>(vm.stack.top()); vm.stack.pop();	\
											\
	if (a->value op 0)								\
		vm.pc_goto(branch);							\
}
opcode_if(eq, ==)
opcode_if(ne, !=)
opcode_if(lt, <)
opcode_if(ge, >=)
opcode_if(gt, >)
opcode_if(le, <=)
#undef opcode_if

void opcode::op_return::exec(class bc const& bc __attribute__ ((unused))) const
{
	log_name("op_return");

	manager::vms.pop();
}

void opcode::op_goto::exec(class bc const& bc __attribute__ ((unused))) const
{
	log_name("op_goto");

	manager::vms.top().pc_goto(branch);
}

void opcode::iastore::exec(class bc const& bc __attribute__ ((unused))) const
{
	log_name("iastore");

	class vm &vm = manager::get_vm();

	class stack_elem::int_const *value = util::dn<stack_elem::int_const*>(vm.stack.top());
	vm.stack.pop();
	class stack_elem::int_const *index = util::dn<stack_elem::int_const*>(vm.stack.top());
	vm.stack.pop();
	class stack_elem::array_ref *array = util::dn<stack_elem::array_ref*>(vm.stack.top());
	vm.stack.pop();

	array->vec->at(index->value) = value;
}

void opcode::arraylength::exec(class bc const& bc __attribute__ ((unused))) const
{
	log_name("arraylength");

	class vm &vm = manager::get_vm();

	class stack_elem::array_ref *array = util::dn<stack_elem::array_ref*>(vm.stack.top());
	vm.stack.pop();

	vm.stack.push(new stack_elem::int_const(array->vec->size()));
}

void opcode::iaload::exec(class bc const& bc __attribute__ ((unused))) const
{
	log_name("iaload");

	class vm &vm = manager::get_vm();

	class stack_elem::int_const *index = util::dn<stack_elem::int_const*>(vm.stack.top());
	vm.stack.pop();
	class stack_elem::array_ref *array = util::dn<stack_elem::array_ref*>(vm.stack.top());
	vm.stack.pop();

	class stack_elem::int_const *value = array->vec->at(index->value);
	vm.stack.push(value->copy());
}

#define opcode_macro(name, id, length)					\
opcode::name *opcode::name::parse(file& file __attribute__ ((unused)))	\
{									\
	std::cerr << "Unchecked opcode: " #name << std::endl;		\
	throw "Unchecked opcode";					\
									\
}									\
									\
void opcode::name::exec(						\
		class bc const &bc __attribute__ ((unused))) const	\
{									\
}									\

#include "../macro/opcode_unchecked.m"
#undef opcode_macro
