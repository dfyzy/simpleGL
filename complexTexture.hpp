#ifndef COMPLEX_TEXTURE_H
#define COMPLEX_TEXTURE_H

#include <list>
#include <memory>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "simpleTexture.hpp"

class ComplexTexture : public SimpleTexture {
private:
	GLuint texture;
	std::list<SimpleSprite> sprites;

public:
	ComplexTexture(unsigned w, unsigned h, GLuint id) : SimpleTexture(w, h), texture(id) {}

	SimpleSprite* createSprite(float x, float y, float z, float w, float h, Color c,
											float texX, float texY, float texW, float texH);

	void draw();

};

#endif
