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
		std::cout << str << std::endl;
	#endif
}

void initShaders();
void initDataBuffers();

void addPreUpdate(std::function<void()> updt);
void addUpdate(std::function<void()> updt);

GLFWwindow* getWindow();

//calling glUseProgramStages for each stage.
void useShaders(GLuint vertex, GLuint fragment);

//TODO: mat
void setViewMatrix(Matrix view);
Matrix getViewMatrix();

void setResolution(unsigned width, unsigned height);
unsigned getResWidth();
unsigned getResHeight();

void setFbo(GLuint fbo);
GLuint getFbo();

void bindSprites();

//draw sprites for each texture
void drawSprites();

}

}

#endif
