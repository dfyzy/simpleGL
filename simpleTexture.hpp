#ifndef SIMPLE_TEXTURE_H
#define SIMPLE_TEXTURE_H

#include <list>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "simpleTextureI.hpp"

class SimpleTexture : public SimpleTextureI {
private:
	GLuint texture;
	std::list<unsigned> sprites;

public:
	SimpleTexture(unsigned w, unsigned h, GLuint id) : SimpleTextureI(w, h), texture(id) {}

	void draw() {}

};

#endif
