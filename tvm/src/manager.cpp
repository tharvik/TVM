#include "manager.hpp"

#include "bc.hpp"
#include "clss.hpp"

std::map<std::string,class clss*> manager::classes;
std::stack<class vm> manager::vms;
std::string manager::class_name;

void manager::init(std::string name)
{
	class clss *cls = new clss(name);
	classes.insert(std::make_pair(name, cls));

	cls = new print_clss();
	classes.insert(std::make_pair("java/io/PrintStream", cls));

	class_name = name;

	vms.push(vm());
}

void manager::run()
{
	classes.at(class_name)->run_func("main");
}

class clss *manager::get_class(std::string name)
{
	class clss *cls;
	if (name == "java/lang/StringBuilder")
		cls = new StringBuilder();
	else
		cls = new clss(name);

	return cls;
};
