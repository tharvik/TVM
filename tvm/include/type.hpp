#ifndef TYPE_HPP
#define TYPE_HPP

#include <string>
#include <map>

class type {
public:
	enum elem {
		INT,
		BOOL,
		VOID
	};

	static class type_elem *get(enum elem elm);
	static class type_class *get(std::string class_name);
	static class type_array *get(class type *tpe);

	virtual bool operator <(class type const &other) const;

protected:
	type() {}

private:
	static std::map<enum elem, class type_elem> elems;
	static std::map<std::string, class type_class> classes;
	static std::map<class type *, class type_array> arrays;

public:
	virtual ~type() {}
};

class type_elem : public type {
public:
	enum elem elm;

	type_elem(enum elem elm) : elm(elm) {}

	bool operator <(class type_elem const &other) const;
};

class type_class : public type
{
public:
	std::string const name;

	type_class(std::string name) : name(name) {}

	bool operator <(class type_class const &other) const;
};

class type_array : public type
{
public:
	class type const *contained;

	type_array(class type *contained) : contained(contained) {}

	bool operator <(class type_array const &other) const;
};

#endif // TYPE_HPP
