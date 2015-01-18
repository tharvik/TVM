enum return_code {
	NO_ERROR,
	WRONG_SYNTAX
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

	std::string const ext = ".class";

	std::string file(argv[1]);
	if(file.length() > ext.length()
	   && file.substr(file.length() - ext.length()) == ext)
		file.resize(file.length() - ext.length());

	manager::get_instance().run(file);

	return NO_ERROR;
}
