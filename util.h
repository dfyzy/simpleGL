/* Some util
*/

#ifndef SIMPLE_UTIL_H
#define SIMPLE_UTIL_H

#ifdef DEBUG
	#include <iostream>
#endif

#include <functional>

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

//Util update functions are ran before and after the user update function
void addPreUpdate(std::function<void()> updt);
void addPostUpdate(std::function<void()> updt);

}

}

#endif
