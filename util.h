#ifndef SIMPLE_UTIL_H
#define SIMPLE_UTIL_H

#ifdef DEBUG
	#include <iostream>
#endif

#include <functional>

#include "sprite.h"

namespace simpleGL {

struct vboType { enum E { VERTEX, TEXTURE, COLOR, COUNT }; };

namespace util {

inline void print(std::string str) {
	#ifdef DEBUG
		std::cout << str << std::endl;
	#endif
}

void initShaders();
void initSprites();

void addPreUpdate(std::function<void()> updt);
void addUpdate(std::function<void()> updt);

GLFWwindow* getWindow();

void bindQuadData(unsigned id, vboType::E type, Matrix model);

//calling glUseProgramStages for each stage.
void useShaders(GLuint vertex, GLuint fragment);

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
