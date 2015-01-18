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
		static std::unique_ptr<base> parse(class file& file);

		unsigned short const size;

		virtual void exec(class bc const &bc) const = 0;
	protected:
		base(short size) : size(size + 1) {}
	};

	template<typename type>
	class with_index : public base
	{
	public:
		type const index;

	protected:
		with_index(type index) : base(sizeof(index)), index(index) {}
	};

#define opcode_with_index(name, type)			\
	class name : public with_index<type> {				\
	public:							\
		static std::unique_ptr<class name> parse(file& file);\
		void exec(class bc const &bc) const;		\
	private:						\
		name(type index) : with_index(index) {}	\
	};
	opcode_with_index(invokespecial, uint16_t)
	opcode_with_index(invokevirtual, uint16_t)

	opcode_with_index(astore, uint8_t)
	opcode_with_index(istore, uint8_t)

	opcode_with_index(aload, uint8_t)
	opcode_with_index(iload, uint8_t)

	opcode_with_index(getfield, uint16_t)
	opcode_with_index(getstatic, uint16_t)

	opcode_with_index(ldc, uint8_t)
	opcode_with_index(op_new, uint16_t)
	opcode_with_index(newarray, uint8_t)
	opcode_with_index(putfield, uint16_t)
#undef opcode_with_index

	class trivial : public base
	{
	protected:
		trivial() : base(0) {}
	};

#define opcode_trivial(name)				\
	class name : public trivial {			\
	public:						\
		static std::unique_ptr<class name> parse(file& file);\
		void exec(class bc const &bc) const;	\
							\
		name() {}				\
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

	class jump : public base
	{
	protected:
		int16_t const branch;

		jump(int16_t branch) : base(sizeof(branch)), branch(branch) {}
	};

#define opcode_if(name)					\
	class name : public jump			\
	{						\
	public:						\
		static std::unique_ptr<class name> parse(file& file);\
		void exec(class bc const &bc) const;	\
							\
		name(int16_t branch) : jump(branch) {}	\
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





	template<short num>
	class iconst : public trivial
	{
	public:
		static std::unique_ptr<class base> parse(class file &file);
		void exec(class bc const &bc) const;
	private:
		iconst<num>() {}
	};

	template<typename type>
	class push : public base
	{
	public:
		static std::unique_ptr<class base> parse(class file &file);
		void exec(class bc const &bc) const;
	private:
		type const data;

	protected:
		push(type data) : base(sizeof(type)), data(data) {}
	};



	template<typename type, short num>
	class load : public trivial
	{
	public:
		static std::unique_ptr<class base> parse(class file &file);
		void exec(class bc const &bc) const;
	private:
		load<type,num>() {}
	};
};

#endif
