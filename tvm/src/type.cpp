#include "type.hpp"

std::map<enum type::elem, class type_elem> type::elems;
std::map<std::string, class type_class> type::classes;
std::map<class type*, class type_array> type::arrays;

template<typename first, typename second>
static second *get_singleton(std::map<first, second> &map, first selector)
{
	auto i = map.find(selector);
	if (i != map.end())
		return &i->second;

	second ret(selector);

	map.insert(std::make_pair(selector, std::move(ret)));

	return &map.find(selector)->second;
}

class type_elem *type::get(enum elem elm)
{
	return get_singleton(elems, elm);
};

class type_class *type::get(std::string class_name)
{
	return get_singleton(classes, class_name);
};

class type_array *type::get(class type *tpe)
{
	return get_singleton(arrays, tpe);
};
