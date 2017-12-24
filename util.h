/* Some util
*/

#ifndef SIMPLE_UTIL_H
#define SIMPLE_UTIL_H

#ifdef DEBUG
	#include <iostream>
#endif

#include <string>
#include <list>

namespace simpleGL {

namespace util {

inline void print(const std::string& str) {
	#ifdef DEBUG
		std::cout << str << std::flush;
	#endif
}

inline void println(const std::string& str) {
	print(str + "\n");
}

//when object's list is mutated in the list elements' destructors.
template<typename T>
void unloadList(std::list<T*>& list) {
	std::list<T*> listCopy = list;

	for (T* t : listCopy) {
		t->unload();
	}
}

}

}

#endif
