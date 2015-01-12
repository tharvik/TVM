#ifndef UTIL_HPP
#define UTIL_HPP

#include <typeinfo>
#include <memory>

namespace util
{
	template<typename to, typename from = void*>
	std::shared_ptr<to> dpc(std::shared_ptr<from> obj);
};

template<typename to, typename from>
std::shared_ptr<to> util::dpc(std::shared_ptr<from> obj)
{
	auto ptr = std::dynamic_pointer_cast<to>(obj);

	if (ptr == nullptr)
		throw std::bad_cast();

	return ptr;
}

#endif // UTIL_HPP
