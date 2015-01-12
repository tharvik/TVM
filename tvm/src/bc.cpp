#include "bc.hpp"

#include "field.hpp"
#include "methods.hpp"
#include "attribute.hpp"

std::map<std::string, std::shared_ptr<class bc>> bc::bcs;

std::shared_ptr<class bc> bc::parse(std::string const &path)
{
	auto i = bcs.find(path);
	if(i != bcs.end())
		return i->second;

	class file file(path);

	uint32_t const magic = file.read<uint32_t>();
	uint16_t const minor_version = file.read<uint16_t>();
	uint16_t const major_version = file.read<uint16_t>();

	if (magic != 0xCAFEBABE)
		throw "Not a valid java class file";

	class cp cp(file);
	class self self(file);
	class interface interface(file);
	class field field(field::parse(file, cp));
	class methods methods(methods::parse(file, cp));

	auto inst = std::shared_ptr<class bc>(new bc(magic, minor_version, major_version, std::move(cp),
		      self, interface, field, methods));

	bcs.insert(std::make_pair(path, inst));

	return bcs.find(path)->second;
}

bc::bc() : magic(0), minor_version(0), major_version(0)
{

}

bc::bc(class bc const &other) : magic(other.magic), minor_version(other.minor_version), major_version(other.major_version),
field(other.field), methods(other.methods)
{

}

std::vector<std::unique_ptr<opcode::base>> bc::get_main() const
{
	std::vector<std::shared_ptr<class attribute_info>> attributes;
	for (auto m : methods.meths)
		if (m->name == "main") {
			attributes = m->attributes;
			break;
		}

 	std::shared_ptr<class Code_attribute> code = util::dpc<class Code_attribute>(attributes.at(0));

	return std::move(code->code);
}
