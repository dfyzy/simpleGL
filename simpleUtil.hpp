#ifndef SIMPLE_UTIL_H
#define SIMPLE_UTIL_H

#ifdef DEBUG
	#include <iostream>
#endif

#include "simpleTexture.hpp"

namespace simpleUtil {

	inline void print(const char* str) {
		#ifdef DEBUG
			std::cout << str << std::endl;
		#endif
	}

	void initBuffers();
	SimpleTexture* getEmptyTexture();

	void checkTextures();
	void drawTextures();

	void checkSprites();

}

#endif
