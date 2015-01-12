#include "manager.hpp"

#include "bc.hpp"
#include "clss.hpp"

manager manager::instance;
class manager &manager::get_instance()
{
	return instance;
};

class vm& manager::get_vm()
{
	return vms.top();
};

void manager::run(std::string name)
{
	auto cls = clss(name);

	class_name = name;

	std::vector<std::shared_ptr<class type>> types;

	auto str = type::get("Ljava/lang/String");
	auto arr = type::get(str);
	types.push_back(arr);

	auto vod = type::get(type::VOID);
	types.push_back(vod);

	cls.run_func(class_name, "main", types, std::vector<std::shared_ptr<class stack_elem::base>>());
}

std::shared_ptr<class clss> manager::get_class(std::string name)
{
	std::shared_ptr<class clss> cls;

	if (name == "java/io/PrintStream")
		cls = std::make_shared<class print_clss>();
	else if (name == "java/lang/StringBuilder")
		cls = std::make_shared<class StringBuilder>();
	else
		cls = std::shared_ptr<class clss>(new clss(name));

	return cls;
};
