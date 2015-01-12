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

	class cp cp(cp::parse(file));
	class self self(self::parse(file));
	class interface interface(interface::parse(file));
	class field field(field::parse(file, cp));
	class methods methods(methods::parse(file, cp));

	auto inst = std::shared_ptr<class bc>(new bc(magic, minor_version, major_version, std::move(cp),
		      std::move(self), std::move(interface), std::move(field), std::move(methods)));

	bcs.insert(std::make_pair(path, inst));

	return bcs.find(path)->second;
}
