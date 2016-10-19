#ifndef SIMPLE_TEXTURE_H
#define SIMPLE_TEXTURE_H
//Maybe rename?
#include <list>
#include <memory>

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

	unsigned* createSprite(float x, float y, float z, float w, float h, Color c, float texX, float texY, float texW, float texH);

	void draw();

};

std::list<std::unique_ptr<SimpleTexture>> textures;

#endif
