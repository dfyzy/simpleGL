/* Some util
*/

#ifndef SIMPLE_UTIL_H
#define SIMPLE_UTIL_H

#include <list>

namespace simpleGL {

namespace util {

//for containers of objects that modify this container on destruction
template<template<typename...> typename Container, typename Type, typename ... Args>
void unloadContainer(Container<Type*, Args...>& container) {
	Container<Type*, Args...> containerCopy = container;
	container.clear();

	for (Type* t : containerCopy) {
		t->unload();
	}
}

}

}

#endif
