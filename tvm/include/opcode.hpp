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
#include "manager.hpp"

#include <iostream>

namespace opcode
{
	class base
	{
	public:
		virtual ~base() {} // only for vtable

		static std::unique_ptr<base> parse(class file& file);

		short const size;

		virtual void exec(class bc const &bc) const = 0;
	protected:
		base(short size) : size(size) {}
	};

	template<short num>
	class iconst : public base
	{
	public:
		static std::unique_ptr<class base> parse(class file &file);
		void exec(class bc const &bc) const;

	private:
		iconst<num>() : base(1) {}
	};

#define opcode_with_index(name, size, type)			\
	class name : public base {				\
	public:							\
		type const index;				\
								\
		static std::unique_ptr<class name> parse(file& file);\
		void exec(class bc const &bc) const;		\
	private:						\
		name(type index) : base(size), index(index) {}	\
	};
	opcode_with_index(invokespecial, 3, uint16_t)
	opcode_with_index(invokevirtual, 3, uint16_t)

	opcode_with_index(astore, 2, uint8_t)
	opcode_with_index(istore, 2, uint8_t)

	opcode_with_index(aload, 2, uint8_t)
	opcode_with_index(iload, 2, uint8_t)

	opcode_with_index(getfield, 3, uint16_t)
	opcode_with_index(getstatic, 3, uint16_t)

	opcode_with_index(ldc, 2, uint8_t)
	opcode_with_index(op_new, 3, uint16_t)
	opcode_with_index(newarray, 2, uint8_t)
	opcode_with_index(putfield, 3, uint16_t)
#undef opcode_with_index


#define opcode_trivial(name)			\
	class name : public base {			\
	public:						\
		static std::unique_ptr<class name> parse(file& file);\
		void exec(class bc const &bc) const;	\
							\
	private:					\
		name() : base(1) {}			\
	};
	opcode_trivial(aload_0)
	opcode_trivial(aload_1)
	opcode_trivial(aload_2)
	opcode_trivial(aload_3)

	opcode_trivial(astore_0)
	opcode_trivial(astore_1)
	opcode_trivial(astore_2)
	opcode_trivial(astore_3)

	opcode_trivial(areturn)
	opcode_trivial(arraylength)
	opcode_trivial(dup)
	opcode_trivial(iadd)
	opcode_trivial(idiv)
	opcode_trivial(imul)
	opcode_trivial(ireturn)
	opcode_trivial(isub)

	opcode_trivial(iaload)
	opcode_trivial(iastore)

	opcode_trivial(iload_0)
	opcode_trivial(iload_1)
	opcode_trivial(iload_2)
	opcode_trivial(iload_3)

	opcode_trivial(istore_0)
	opcode_trivial(istore_1)
	opcode_trivial(istore_2)
	opcode_trivial(istore_3)

	opcode_trivial(op_return)
	opcode_trivial(pop)
	opcode_trivial(swap)
#undef opcode_trivial

#define opcode_if(name)					\
	class name : public base			\
	{						\
	public:						\
		static std::unique_ptr<class name> parse(file& file);\
		void exec(class bc const &bc) const;	\
	private:					\
		int16_t const branch;			\
							\
		name(int16_t branch)			\
		: base(3), branch(branch)		\
		{}					\
	};
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

#define opcode_macro(name, id, size)		\
	class name : public base {			\
	public:						\
		static std::unique_ptr<class name>parse(file& file);		\
		void exec(class bc const &bc) const;	\
							\
	private:					\
		name() : base(size) {}			\
	};
#include "../macro/opcode_unchecked.m"
#undef opcode_macro

#define macro_push(name, size, type)				\
	class name : public base				\
	{							\
	public:							\
		type const byte;				\
								\
		static std::unique_ptr<class name> parse(file& file);		\
		void exec(class bc const &bc) const;		\
	private:						\
		name(type byte) : base(size), byte(byte) {}	\
	};
	macro_push(bipush, 2, uint8_t)
	macro_push(sipush, 3, uint16_t)
#undef macro_push
};

#endif
