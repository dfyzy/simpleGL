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

	bool isCurrentThread();

	void initShaders(float aspect);
	void initBuffers();

	SimpleTexture* getEmptyTexture();

	void setDefaultShaders(SimpleSprite* sprite, bool empty);
	void useShaders(GLuint vertex, GLuint fragment);


	void checkTextures();
	void checkSprites();
	void checkShaders();

	void drawTextures();
	
}

#endif
