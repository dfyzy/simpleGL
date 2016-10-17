#ifndef SIMPLE_TEXTURE_H
#define SIMPLE_TEXTURE_H
//Maybe rename?
#include <list>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "simpleTextureI.hpp"

const int SHADER_ATTRIB_COUNT = 4;

unsigned windowWidth, windowHeight;

class SimpleTexture : public SimpleTextureI {
private:
	GLuint texture;
	std::list<unsigned> sprites;

public:
	SimpleTexture(unsigned w, unsigned h, GLuint id) : SimpleTextureI(w, h), texture(id) {}

	void createSprite(float x, float y, float z, float w, float h, Color c, float texX, float texY, float texW, float texH);

	void draw();

};

#endif
