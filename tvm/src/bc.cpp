#include "bc.hpp"

#include "field.hpp"
#include "methods.hpp"
#include "attribute.hpp"

bc *bc::parse(std::string path)
{
	class file file(path);

	uint32_t const magic = file.read<uint32_t>();
	uint16_t const minor_version = file.read<uint16_t>();
	uint16_t const major_version = file.read<uint16_t>();

	if (magic != 0xCAFEBABE)
		throw "Not a valid java class file";

	class cp cp(file);
	class self self(file);
	class interface interface(file);
	class field *field = field::parse(file, cp);
	class methods *methods = methods::parse(file, cp);

	return new bc(magic, minor_version, major_version, cp,
		      self, interface, field, methods);
}

bc::~bc()
{
	delete field;
	delete methods;
}

std::vector<opcode::base*> bc::get_main() const
{
	std::vector<class attribute_info*> attributes;
	for (method_info const * const m : methods->meths)
		if (m->name == "main") {
			attributes = m->attributes;
			break;
		}

 	class Code_attribute * code = dynamic_cast<class Code_attribute* >(attributes.at(0));

	return code->code;
}
