#ifndef SIMPLE_UTIL_H
#define SIMPLE_UTIL_H

#ifdef DEBUG
	#include <iostream>
#endif

#include "simpleSprite.hpp"

namespace simpleUtil {

	inline void print(const char* str) {
		#ifdef DEBUG
			std::cout << str << std::endl;
		#endif
	}

	//creating program pipeline and default shader programs.
	void initShaders();
	//creating buffers for each sprite attribute. binding them to layout positions in shaders.
	void initBuffers();

	void initFbos();

	void initFonts();
	void closeFonts();

	void genTexture(GLuint* texture, GLenum filtering);

	//setting default shaders for given sprite.
	void setDefaultShaders(SimpleSprite* sprite, bool empty);

	void setTextShader(SimpleSprite* sprite);

	//calling glUseProgramStages for each stage.
	void useShaders(GLuint vertex, GLuint geometry, GLuint fragment);

	void useOverlayShaders();

	//draw sprites for each texture
	void drawSprites();

}

#endif
