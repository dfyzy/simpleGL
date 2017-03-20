#ifndef SIMPLE_UTIL_H
#define SIMPLE_UTIL_H

#ifdef DEBUG
	#include <iostream>
#endif

#include <functional>

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

void addUpdate(std::function<void()> updt);

GLFWwindow* getWindow();

void bindData(unsigned id, vboType::E type, Matrix model);

//calling glUseProgramStages for each stage.
void useShaders(GLuint vertex, GLuint fragment, Color color);

//TODO: mat
void setCameraData(Matrix view);

void setResolution(unsigned width, unsigned height);
void setDefaultResolution();

void bindSprites();

//draw sprites for each texture
void drawSprites();

}

}

#endif
