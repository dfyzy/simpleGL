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

	//check if current thread has opengl context.
	bool isCurrentThread();

	//creating program pipeline and default shader programs.
	void initShaders(float aspect);
	//creating buffers for each sprite attribute. binding them to layout positions in shaders.
	void initBuffers();

	//setting default shaders for given sprite.
	void setDefaultShaders(SimpleSprite* sprite, bool empty);
	//calling glUseProgramStages for each stage.
	void useShaders(GLuint vertex, GLuint fragment);

	//checks for updates
	void checkTextures();
	void checkSprites();
	void checkShaders();

	//draw sprites for each texture
	void drawTextures();

}

#endif
