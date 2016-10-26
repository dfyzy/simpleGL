#ifndef SIMPLE_UTIL_H
#define SIMPLE_UTIL_H

#ifdef DEBUG
	#include <iostream>
#endif

namespace simpleUtil {

	void print(const char* str) {
		#ifdef DEBUG
			std::cout << str << std::endl;
		#endif
	}

	void initBuffers();
	void setResolution(unsigned w, unsigned h);

	void checkTextures();
	void drawTextures();

	void checkSprites();

}

#endif
