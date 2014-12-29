#include "file.hpp"

file::file(std::string path) : input(path, std::ifstream::binary) {}

template <typename type, size_t length>
type file::read()
{
	type acc = 0;

	for (size_t i = length; i > 0; i--)
		acc = (acc << 8) | input.get();

	return acc;
}

template <>
uint8_t file::read<uint8_t>()
{
	return input.get();
}

template <>
uint16_t file::read<uint16_t>()
{
	return read<uint16_t, 2>();
}

template <>
uint32_t file::read<uint32_t>()
{
	return read<uint32_t, 4>();
}

std::vector<uint8_t> file::read(size_t length)
{
	std::vector<uint8_t> vec;
	vec.resize(length);

	input.read(reinterpret_cast<char*>(vec.data()), length);

	return vec;
}
