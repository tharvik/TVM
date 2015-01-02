#ifndef POOL_HPP
#define POOL_HPP

#include <map>

template<typename first, typename second>
class pool
{
	public:
		second *get(first selector) const;

	private:
		std::map<first, second*> map;
};

template<typename first, typename second>
second *pool<first, second>::get(first selector) const
{
	auto i = map.find(selector);
	if (i != map.end())
		return i->second;

	second *ret = new second(selector);

	map.insert(std::make_pair(selector, ret));
	return ret;
}

#endif // POOL_HPP
