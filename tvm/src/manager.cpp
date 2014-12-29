#include "manager.hpp"

#include "bc.hpp"

manager::manager(std::string main_class)
	: bc(bc::parse(main_class))
{}


void manager::run()
{
	std::vector<opcode::base*> code(bc->get_main());

	vm.exec(*bc, code);
}
