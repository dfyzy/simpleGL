#ifndef SIMPLE_UTIL_H
#define SIMPLE_UTIL_H

#ifdef DEBUG
	#include <iostream>
#endif

#include <functional>

#include "sprite.h"

namespace simpleGL {

namespace util {

inline void print(std::string str) {
	#ifdef DEBUG
		std::cout << str << std::flush;
	#endif
}

inline void println(std::string str) {
	print(str + "\n");
}

void addPreUpdate(std::function<void()> updt);
void addUpdate(std::function<void()> updt);

//draw sprites for each texture
void drawSprites();

}

}

#endif
