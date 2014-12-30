#ifndef TYPE_HPP
#define TYPE_HPP

#include <string>

class type {};
class type_int : public type {};
class type_void : public type {};
class type_class : public type
{
public:
	type_class(std::string name) : name(name) {}

	std::string const name;
};

#endif // TYPE_HPP
