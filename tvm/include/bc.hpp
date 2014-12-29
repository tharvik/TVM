#ifndef BC_BC_HPP
#define BC_BC_HPP

#include "file_h.hpp"
#include "cp.hpp"
#include "self.hpp"
#include "interface.hpp"
#include "field_h.hpp"
#include "methods_h.hpp"
#include "opcode_h.hpp"

#include <cstdint>
#include <string>
#include <vector>

class bc
{
public:
	static bc *parse(std::string path);

	~bc();

	std::vector<opcode::base*> get_main() const;

	uint32_t const magic;
	uint16_t const minor_version;
	uint16_t const major_version;

	class cp const cp;
	class self const self;
	class interface const interface;
	class field const * const field;
	class methods const * const methods;

private:
	bc(uint32_t magic, uint16_t minor_version, uint16_t major_version,
	   class cp &cp, class self self, class interface interface,
	   class field *field, class methods *methods)
		: magic(magic), minor_version(minor_version),
		  major_version(major_version), cp(std::move(cp)), self(self),
		  interface(interface), field(field), methods(methods)
	{}
};

#endif
