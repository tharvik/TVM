#ifndef BC_FILE_HPP
#define BC_FILE_HPP

#include "util.hpp"

#include <algorithm>
#include <fstream>
#include <array>
#include <string>

class file
{
public:
	file(std::string path);

	template <typename type>
	type read();

	std::vector<uint8_t> read(size_t length);

private:
	std::ifstream input;

	template <typename type, size_t length>
	type read();
};

#endif
