#ifndef TYPE_HPP
#define TYPE_HPP

#include <string>

class type {
public:
	virtual ~type() {}
};
class type_int : public type {};
class type_bool : public type {};
class type_void : public type {};
class type_class : public type
{
public:
	type_class(std::string name) : name(name) {}

	std::string const name;
};

class type_array : public type
{
public:
	type_array(class type *contained) : contained(contained) {}

	class type *contained;
};

#endif // TYPE_HPP
