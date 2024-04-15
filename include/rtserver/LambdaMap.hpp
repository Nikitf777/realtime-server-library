
#ifndef LAMBDAMAP_HPP
#define LAMBDAMAP_HPP

#include <map>
#include <functional>

template<typename TKey, typename TValue>
class LambdaMap : public std::map<TKey, TValue>
{
public:

	void runFunction(std::function<void(LambdaMap<TKey, TValue>* thisMap)> function);
};

template<typename TKey, typename TValue>
inline void LambdaMap<TKey, TValue>::runFunction(std::function<void(LambdaMap<TKey, TValue>* thisMap)> function)
{
	function(this);
}

#endif // LAMBDAMAP_HPP