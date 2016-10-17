#include <libpng/png.h>
#include <zlib.h>

#include "simpleGL.hpp"
#include "simpleTexture.hpp"
#include "threadUtil.hpp"
#include "textureUtil.hpp"

std::list<std::unique_ptr<SimpleTexture>> textures;

std::string texturePath;
SimpleTextureI* returnValue = nullptr;

void loadTexture() {
	std::cout << "Loading texture: " << texturePath << std::endl;

	FILE *file = fopen(texturePath.c_str(), "rb");
	if (!file) {
		std::cout << "Error opening texture" << std::endl;
		return;
	}

	texturePath.clear();

	png_byte header[8];
	fread(header, 1, 8, file);
	if (png_sig_cmp(header, 0, 8)) {
		std::cout << "Not a png" << std::endl;
		fclose(file);
		return;
	}

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	if (!png_ptr) {
		std::cout << "Failed to create read struct" << std::endl;
		fclose(file);
		return;
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		std::cout << "Failed to create info struct" << std::endl;
		png_destroy_read_struct(&png_ptr, nullptr, nullptr);
		fclose(file);
		return;
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		std::cout << "Libpng error" << std::endl;
		png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
		fclose(file);
		return;
	}

	png_init_io(png_ptr, file);

	png_set_sig_bytes(png_ptr, 8);

	png_read_info(png_ptr, info_ptr);

	png_uint_32 width, height;
	png_get_IHDR(png_ptr, info_ptr, &width, &height, nullptr, nullptr, nullptr, nullptr, nullptr);

	GLuint texture;
	glGenTextures(1, &texture);

	SimpleTexture* st = new SimpleTexture(width, height, texture);
	textures.push_back(std::unique_ptr<SimpleTexture>(st));

	returnValue = st;
	returnReady.notify_one();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	std::unique_ptr<png_byte> row(new png_byte[4*width]);

	for (png_uint_32 i = height; i > 0; i--) {
		png_read_row(png_ptr, row.get(), nullptr);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, i - 1, width, 1, GL_RGBA, GL_UNSIGNED_BYTE, row.get());
	}

	png_read_end(png_ptr, nullptr);
	png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
	fclose(file);
}

void checkTextures() {
	mutex.lock();
	bool empty = texturePath.empty();
	mutex.unlock();

	if (!empty)	loadTexture();
}

SimpleTextureI* simpleGL::addTexture(std::string path) {
	boost::unique_lock<boost::mutex> lock(mutex);
	returnValue = nullptr;

	texturePath = path;

	do {
		returnReady.wait(lock);
	} while (!returnValue);

	return returnValue;
}
