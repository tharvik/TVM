#ifndef BC_OPCODE_HPP
#define BC_OPCODE_HPP

namespace opcode
{
class base;
}

#include "bc.hpp"
#include "file_h.hpp"
#include "vm.hpp"
#include "cp.hpp"

#include <iostream>

namespace opcode
{

class base
{
public:
	static base *parse(file& file);

	short const size;

	virtual void exec(class bc const &bc, class vm &vm) const = 0;
protected:
	base(short size) : size(size) {}
};

// TODO move it to template
#define iconst_macro(num)						\
class iconst_##num : public base					\
{									\
public:									\
	static iconst_##num *parse(file& file __attribute__ ((unused)))	\
	{								\
		return new iconst_##num();				\
	}								\
									\
	void exec(class bc const &bc __attribute__ ((unused)),		\
		  class vm &vm) const					\
	{								\
		vm.stack.push(new stack_elem::int_const(num));		\
	}								\
private:								\
	iconst_##num() : base(1) {}					\
};
iconst_macro(0)
iconst_macro(1)
iconst_macro(2)
iconst_macro(3)
iconst_macro(4)
iconst_macro(5)
#undef iconst_macro

class iadd : public base
{
public:
	static class iadd *parse(file& file);
	void exec(class bc const &bc, class vm &vm) const;
private:
	iadd() : base(1) {}
};


class dup : public base
{
public:
	static class dup *parse(file& file );
	void exec(class bc const &bc, class vm &vm) const;
private:
	dup() : base(1) {}
};


class getstatic : public base
{
public:
	uint16_t const index;

	static class getstatic *parse(file& file);
	void exec(class bc const &bc, class vm &vm) const;
private:
	getstatic(uint16_t index) : base(3), index(index) {}
};


class invokevirtual : public base
{
public:
	uint16_t const index;

	static class invokevirtual *parse(file& file);
	void exec(class bc const &bc, class vm &vm) const;
private:
	invokevirtual(uint16_t index) : base(3), index(index) {}
};

#define opcode_macro(name, id, size)					\
class name : public base {						\
public:									\
	static name *parse(file& file);					\
	void exec(class bc const &bc, class vm &vm) const;		\
									\
private:								\
	name() : base(size) {}						\
};
#include "../macro/opcode_unchecked.m"
#undef opcode_macro

};

#endif
