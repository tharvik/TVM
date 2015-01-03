#include "opcode.hpp"

#include "manager.hpp"

static void log_name(std::string name)
{
	std::cerr << manager::get_instance().vms.size() << ":" << manager::get_instance().get_vm().stack.size() << " ~ " << name << std::endl;
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
	class vm &vm = manager::get_instance().get_vm();					\
										\
	stack_elem::int_const* b = util::dn<stack_elem::int_const*>(vm.stack.top());\
	vm.stack.pop();								\
	stack_elem::int_const* a = util::dn<stack_elem::int_const*>(vm.stack.top());\
	vm.stack.pop();								\
										\
	vm.stack.push(new stack_elem::int_const(a->value op b->value));		\
										\
	delete a;								\
	delete b;								\
}
opcode_macro(iadd, +)
opcode_macro(idiv, /)
opcode_macro(imul, *)
opcode_macro(isub, -)
#undef opcode_macro

void opcode::dup::exec(class bc const &bc __attribute__ ((unused))) const
{
	log_name("dup");

	class vm &vm = manager::get_instance().get_vm();

	vm.stack.push(vm.stack.top()->copy());
}

void opcode::swap::exec(class bc const &bc __attribute__ ((unused))) const
{
	log_name("swap");

	class vm &vm = manager::get_instance().get_vm();

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

	class vm &vm = manager::get_instance().get_vm();

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
	manager::get_instance().get_vm().stack.push(new stack_elem::int_const(byte));\
}
macro_push(bipush, uint8_t)
macro_push(sipush, uint16_t)
#undef macro_push

void opcode::getstatic::exec(class bc const &bc __attribute__ ((unused))) const
{
	log_name("getstatic");

	class vm &vm = manager::get_instance().get_vm();

	vm.stack.push(new stack_elem::class_ref(std::shared_ptr<class clss>(new print_clss())));
}

void opcode::getfield::exec(class bc const &bc __attribute__ ((unused))) const
{
	class vm &vm = manager::get_instance().get_vm();
	class ref_info *info = bc.cp.get<ref_info*>(index);

	log_name("getfield\t\t" + info->name_and_type->name);

	stack_elem::class_ref *ref = util::dn<stack_elem::class_ref*>(vm.stack.top());
	vm.stack.pop();

	vm.stack.push(ref->cls->get_field(info->name_and_type->name)->copy());

	delete ref;
}

void opcode::putfield::exec(class bc const &bc __attribute__ ((unused))) const
{
	class vm &vm = manager::get_instance().get_vm();
	class ref_info *info = bc.cp.get<ref_info*>(index);

	log_name("putfield\t\t" + info->name_and_type->name);

	class stack_elem::base *elem = vm.stack.top();
	vm.stack.pop();
	stack_elem::class_ref *ref = util::dn<stack_elem::class_ref*>(vm.stack.top());
	vm.stack.pop();

	ref->cls->put_field(info->name_and_type->name, elem->copy());

	delete ref;
	delete elem;
}

void opcode::invokespecial::exec(class bc const &bc __attribute__ ((unused))) const
{
	log_name("invokespecial (nop, just remove obj)");

	auto &stack = manager::get_instance().get_vm().stack;
	delete stack.top();
	stack.pop();
}

void opcode::pop::exec(class bc const &bc __attribute__ ((unused))) const
{
	log_name("pop");

	auto &stack = manager::get_instance().get_vm().stack;
	delete stack.top();
	stack.pop();
}

void opcode::invokevirtual::exec(class bc const &bc) const
{
	ref_info * meth = bc.cp.get<ref_info*>(index);

	log_name("invokevirtual\t\t" + meth->name_and_type->name);

	std::stack<class stack_elem::base*> &old_stack = manager::get_instance().get_vm().stack;

	std::vector<class stack_elem::base*> args;
	short total_args = meth->name_and_type->types.size() - 1;

	args.reserve(total_args + 1);
	for (; total_args > 0; total_args--) {
		args.push_back(old_stack.top());
		old_stack.pop();
	}
	args.push_back(old_stack.top());
	std::reverse(args.begin(), args.end());

	class stack_elem::class_ref *ref = util::dn<stack_elem::class_ref*>(args.at(0));
	old_stack.pop();

	manager::get_instance().vms.push(vm());
	class vm &vm = manager::get_instance().get_vm();
	vm.vars = args;

	ref->cls->run_func(meth->clss->name, meth->name_and_type->name, meth->name_and_type->types);
}

void opcode::op_new::exec(class bc const &bc) const
{
	log_name("op_new");

	CONSTANT_Class_info *info = bc.cp.get<CONSTANT_Class_info*>(index);

	auto cls = manager::get_instance().get_class(info->name);

	manager::get_instance().get_vm().stack.push(new stack_elem::class_ref(cls));
}

void opcode::newarray::exec(class bc const &bc __attribute__ ((unused))) const
{
	log_name("newarray");

	class vm &vm = manager::get_instance().get_vm();

	stack_elem::int_const *count = util::dn<stack_elem::int_const*>(vm.stack.top());
	vm.stack.pop();

	manager::get_instance().get_vm().stack.push(new stack_elem::array_ref(count->value));

	delete count;
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
	class vm &vm = manager::get_instance().get_vm();				\
	vm.stack.push(vm.vars.at(num)->copy());						\
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
	class vm &vm = manager::get_instance().get_vm();				\
	vm.stack.push(vm.vars.at(index)->copy());					\
}
load_macro(a)
load_macro(i)
#undef load_macro

#define iconst_macro(num)								\
void opcode::iconst_##num::exec(class bc const &bc __attribute__ ((unused))) const	\
{											\
	log_name("iconst_" #num);							\
	class vm &vm = manager::get_instance().get_vm();				\
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
	class vm &vm = manager::get_instance().get_vm();				\
											\
	class stack_elem::base *val = vm.stack.top();					\
	delete vm.vars.at(num);								\
	vm.vars.at(num) = val->copy();							\
	vm.stack.pop();									\
											\
	delete val;									\
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
	class vm &vm = manager::get_instance().get_vm();				\
											\
	class stack_elem::base *val = vm.stack.top();					\
	delete vm.vars.at(index);							\
	vm.vars.at(index) = val->copy();						\
	vm.stack.pop();									\
											\
	delete val;									\
}
macro_store(a)
macro_store(i)
#undef macro_store

#define macro_return(type)								\
void opcode::type##return::exec(class bc const &bc __attribute__ ((unused))) const	\
{											\
	log_name(#type "return");							\
											\
	auto &stack = manager::get_instance().get_vm().stack;				\
	class stack_elem::base *elem = stack.top()->copy();				\
	delete stack.top();								\
	stack.pop();									\
											\
	manager::get_instance().vms.pop();						\
											\
	manager::get_instance().get_vm().stack.push(elem);				\
}
macro_return(a)
macro_return(i)
#undef macro_return

#define opcode_if(name)				\
opcode::name *opcode::name::parse(file& file)	\
{						\
	int16_t branch;				\
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
void opcode::if_acmp##name::exec(class bc const& bc __attribute__ ((unused))) const\
{								\
	log_name("if_acmp" #name);				\
								\
	class vm &vm = manager::get_instance().get_vm();	\
								\
	bool comp = false;					\
	if (dynamic_cast<class stack_elem::class_ref*>(vm.stack.top()) != nullptr) {\
		class stack_elem::class_ref *value1, *value2;	\
		value2 = util::dn<class stack_elem::class_ref*>(vm.stack.top()); vm.stack.pop();\
		value1 = util::dn<class stack_elem::class_ref*>(vm.stack.top()); vm.stack.pop();\
								\
		comp = value1->cls op value2->cls;		\
								\
		delete value2;					\
		delete value1;					\
	} else if (dynamic_cast<class stack_elem::string_const*>(vm.stack.top()) != nullptr) {\
		class stack_elem::string_const *value1, *value2;\
		value2 = util::dn<class stack_elem::string_const*>(vm.stack.top()); vm.stack.pop();\
		value1 = util::dn<class stack_elem::string_const*>(vm.stack.top()); vm.stack.pop();\
								\
		comp = &value1 op &value2;			\
								\
		delete value2;					\
		delete value1;					\
	} else							\
		throw "Unknow value to cast";			\
								\
	if(comp)						\
		vm.pc_goto(branch);				\
}
opcode_if(eq, ==)
opcode_if(ne, !=)
#undef opcode_if

#define opcode_if(name, op)					\
void opcode::if_icmp##name::exec(class bc const& bc __attribute__ ((unused))) const\
{								\
	log_name("if_icmp" #name);				\
								\
	class vm &vm = manager::get_instance().get_vm();	\
	class stack_elem::int_const *value1, *value2;		\
								\
	value2 = util::dn<class stack_elem::int_const*>(vm.stack.top()); vm.stack.pop();\
	value1 = util::dn<class stack_elem::int_const*>(vm.stack.top()); vm.stack.pop();\
								\
	if (value1->value op value2->value)			\
		vm.pc_goto(branch);				\
								\
	delete value2;						\
	delete value1;						\
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
	class vm &vm = manager::get_instance().get_vm();						\
	class stack_elem::int_const *a;							\
											\
	a = util::dn<class stack_elem::int_const*>(vm.stack.top()); vm.stack.pop();	\
	int val = a->value;								\
	delete a;									\
											\
	if (val op 0)									\
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

	manager::get_instance().vms.pop();
}

void opcode::op_goto::exec(class bc const& bc __attribute__ ((unused))) const
{
	log_name("op_goto");

	manager::get_instance().get_vm().pc_goto(branch);
}

void opcode::iastore::exec(class bc const& bc __attribute__ ((unused))) const
{
	log_name("iastore");

	class vm &vm = manager::get_instance().get_vm();

	class stack_elem::int_const *value = util::dn<stack_elem::int_const*>(vm.stack.top()->copy());
	vm.stack.pop();
	class stack_elem::int_const *index = util::dn<stack_elem::int_const*>(vm.stack.top()->copy());
	vm.stack.pop();
	class stack_elem::array_ref *array = util::dn<stack_elem::array_ref*>(vm.stack.top()->copy());
	vm.stack.pop();

	array->vec->at(index->value) = util::dn<stack_elem::int_const*>(value->copy());

	delete value;
	delete index;
	delete array;
}

void opcode::arraylength::exec(class bc const& bc __attribute__ ((unused))) const
{
	log_name("arraylength");

	class vm &vm = manager::get_instance().get_vm();

	class stack_elem::array_ref *array = util::dn<stack_elem::array_ref*>(vm.stack.top()->copy());
	vm.stack.pop();

	vm.stack.push(new stack_elem::int_const(array->vec->size()));

	delete array;
}

void opcode::iaload::exec(class bc const& bc __attribute__ ((unused))) const
{
	log_name("iaload");

	class vm &vm = manager::get_instance().get_vm();

	class stack_elem::int_const *index = util::dn<stack_elem::int_const*>(vm.stack.top()->copy());
	vm.stack.pop();
	class stack_elem::array_ref *array = util::dn<stack_elem::array_ref*>(vm.stack.top()->copy());
	vm.stack.pop();

	class stack_elem::int_const *value = array->vec->at(index->value);
	vm.stack.push(value->copy());

	delete index;
	delete array;
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
