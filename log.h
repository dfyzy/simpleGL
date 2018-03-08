#ifndef SIMPLE_LOG_H
#define SIMPLE_LOG_H

#include <string>

namespace simpleGL {

void print(const std::string& str);

inline void println(const std::string& str) {
	print(str + "\n");
}

}

#endif
