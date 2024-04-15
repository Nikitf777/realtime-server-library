
#ifndef STREAM_LIST_HPP
#define STREAM_LIST_HPP

#include <list>
#include "ByteStream.hpp"


template<typename T>
class StreamList : public std::list<T>
{
public:
	void writeAllToStream(ByteStream& stream);
};

template<typename T>
inline void StreamList<T>::writeAllToStream(ByteStream& stream)
{
	stream << (byte)this->size();
	//for (auto i = this->begin(); i != this->end(); ++i) {
	//	stream << this->operator[](i);
	//}
	//}
	for (auto el : *this)
		stream << el;

	this->clear();
}

#endif