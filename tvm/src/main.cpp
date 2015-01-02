enum return_code {
	NO_ERROR,
	WRONG_SYNTAX,
	READ_FILE
};

#include <iostream>
void syntax(char const *const argv0)
{
	std::cout << argv0 << " class-file" << std::endl;
}

#include "manager.hpp"
#include "clss.hpp"
int main(int argc, char *const *const argv)
{
	if (argc != 2) {
		syntax(argv[0]);
		return WRONG_SYNTAX;
	}

	std::string file(argv[1]);
//	std::string file("asd_class.class");

	//class manager manager(argv[1]);
	//manager.run();
	file.resize(file.length() - 6);

	manager::get_instance().run(file);
//	delete cls;

	return NO_ERROR;
}
