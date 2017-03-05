#ifndef SIMPLE_UTIL_H
#define SIMPLE_UTIL_H

#ifdef DEBUG
	#include <iostream>
#endif

#include "sprite.hpp"

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

GLuint getMsaaFbo();

//setting default shaders for given sprite.
void setDefaultShaders(simpleGL::UnsortedSprite* sprite, bool empty);

void setOverlayShaders(simpleGL::UnsortedSprite* sprite);
void setLightingShaders(simpleGL::UnsortedSprite* sprite);
void setTextShader(simpleGL::Sprite* sprite);

//calling glUseProgramStages for each stage.
void useShaders(GLuint vertex, GLuint fragment, simpleGL::Color color);

void setResolution(unsigned width, unsigned height);
void setDefaultResolution();

//draw sprites for each texture
void drawSprites();

}

#endif
