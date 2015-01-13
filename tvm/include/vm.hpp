#ifndef VM_HPP
#define VM_HPP

#include <stack>
#include <cstdint>
#include <string>
#include <array>
#include <vector>
#include <map>
#include <cassert>
#include <memory>

#include "clss.hpp"
#include "opcode_h.hpp"
#include "util.hpp"

namespace stack_elem
{
	class base {
	public:
		virtual ~base() {}
	};

	template<typename type>
	class const_val : public base
	{
	public:
		const_val();
		const_val(type value);

		type const value;
	};

	class class_ref : public base
	{
	public:
		class_ref(std::shared_ptr<class clss> cls) : cls(cls) {}
		std::shared_ptr<class clss> cls;
	};

	class array_ref : public class_ref
	{
	public:
		array_ref(uint32_t size);

		std::shared_ptr<std::vector<std::shared_ptr<stack_elem::const_val<int>>>> vec;
	};

	class print_class : public class_ref
	{
	public:
		print_class();
	};
}

class class_state
{
public:
	class_state(std::string class_name);
};

class vm
{
public:
	vm(class bc const &bc, std::vector<std::unique_ptr<opcode::base>> const &ops);
	void exec();

	std::stack<std::shared_ptr<class stack_elem::base>> stack;
	std::vector<std::shared_ptr<class stack_elem::base>> vars;

	void pc_goto(int32_t index);

private:
	uint32_t pc;

	class bc const &bc;
	std::vector<std::unique_ptr<opcode::base>> const &ops;
};

#endif // VM_HPP
