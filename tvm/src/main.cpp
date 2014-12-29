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
int main(int argc, char *const *const argv)
{
	if (argc != 2) {
		syntax(argv[0]);
		return WRONG_SYNTAX;
	}

	class manager manager(argv[1]);
	manager.run();

	return NO_ERROR;
}
