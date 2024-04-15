
#ifndef AUTO_MAP_HPP
#define AUTO_MAP_HPP

#include <map>
#include <queue>

template<typename TValue>
class AutoMap
{
private:

	std::map<size_t, TValue> _map;
	size_t _maxKey = 0;
	std::queue<size_t> _freeKeys;

public:

	size_t add(TValue value);
	bool remove(size_t key);
	TValue get(size_t key);
	std::map<size_t, TValue>& getMap();
};

template<typename TValue>
inline size_t AutoMap<TValue>::add(TValue value)
{
	if (_freeKeys.empty()) {
		_map[_maxKey] = value;
		_maxKey++;
		return _maxKey - 1;
	}

	size_t usedKey = _freeKeys.front();
	_map[usedKey] = value;
	_freeKeys.pop();
	return usedKey;
}

template<typename TValue>
inline bool AutoMap<TValue>::remove(size_t key)
{
	auto iterator = _map.find(key);
	if (iterator == _map.end())
		return false;

	_map.erase(iterator);
	if (key == (_maxKey - 1))
		_maxKey--;
	else
		_freeKeys.push(key);

	return true;
}

template<typename TValue>
inline TValue AutoMap<TValue>::get(size_t key)
{
	return _map[key];
}

template<typename TValue>
inline std::map<size_t, TValue>& AutoMap<TValue>::getMap()
{
	return _map;
}

#endif