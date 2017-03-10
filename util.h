#ifndef SIMPLE_UTIL_H
#define SIMPLE_UTIL_H

#ifdef DEBUG
	#include <iostream>
#endif

#include "sprite.h"

namespace simpleGL {

struct vboType { enum E { VERTEX, TEXTURE, COUNT }; };

namespace util {

inline void print(const char* str) {
	#ifdef DEBUG
		std::cout << str << std::endl;
	#endif
}

void initShaders();
void initSprites();
void initFbos();
void initLights();
void initFonts();
void initTexts();
void closeFonts();

GLuint getMsaaFbo();

void bindData(unsigned id, vboType::E type, Vector centre, Vector bounds, double sinRot, double cosRot);

//calling glUseProgramStages for each stage.
void useShaders(GLuint vertex, GLuint fragment, simpleGL::Color color);

void setCameraData(simpleGL::Vector position, simpleGL::Vector scale, double rotation);

void setResolution(unsigned width, unsigned height);
void setDefaultResolution();

void pollButtons(GLFWwindow* window);

//draw sprites for each texture
void drawSprites();

}

}

#endif
