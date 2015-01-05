#include "interface.hpp"

#include "file.hpp"

#include <cstdlib>
#include <cstdint>

interface::interface()
{

}

interface::interface(file& file)
{
	uint16_t count = file.read<uint16_t>();

	for (; count > 0; count--)
		file.read(3);
}
