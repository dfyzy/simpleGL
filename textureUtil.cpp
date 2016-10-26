#include <libpng/png.h>
#include <zlib.h>
#include <boost/thread.hpp>

#include "simpleGL.hpp"
#include "complexTexture.hpp"
#include "simpleUtil.hpp"

std::list<ComplexTexture> textures;

boost::mutex textureMutex;
boost::condition_variable condVariable;
bool returnReady;

std::string texturePath;
SimpleTexture* returnValue = nullptr;

inline void notify() {
	returnReady = true;
	condVariable.notify_one();
}

void loadTexture() {
	simpleUtil::print("Loading texture");

	FILE *file = fopen(texturePath.c_str(), "rb");
	if (!file) {
		simpleUtil::print("Error opening texture");
		texturePath.clear();
		notify();
		return;
	}

	texturePath.clear();

	png_byte header[8];
	fread(header, 1, 8, file);
	if (png_sig_cmp(header, 0, 8)) {
		simpleUtil::print("Not a png");
		fclose(file);
		notify();
		return;
	}

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	if (!png_ptr) {
		simpleUtil::print("Failed to create read struct");
		fclose(file);
		notify();
		return;
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		simpleUtil::print("Failed to create info struct");
		png_destroy_read_struct(&png_ptr, nullptr, nullptr);
		fclose(file);
		notify();
		return;
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		simpleUtil::print("Libpng error");
		png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
		fclose(file);
		notify();
		return;
	}

	png_init_io(png_ptr, file);

	png_set_sig_bytes(png_ptr, 8);

	png_read_info(png_ptr, info_ptr);

	png_uint_32 width, height;
	png_get_IHDR(png_ptr, info_ptr, &width, &height, nullptr, nullptr, nullptr, nullptr, nullptr);

	GLuint texture;
	glGenTextures(1, &texture);

	textures.push_back(ComplexTexture(width, height, texture));

	returnValue = &(*--textures.end());
	notify();

	glBindTexture(GL_TEXTURE_RECTANGLE, texture);

	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	std::unique_ptr<png_byte> row(new png_byte[4*width]);

	for (png_uint_32 i = height; i > 0; i--) {
		png_read_row(png_ptr, row.get(), nullptr);
		glTexSubImage2D(GL_TEXTURE_RECTANGLE, 0, 0, i - 1, width, 1, GL_RGBA, GL_UNSIGNED_BYTE, row.get());
	}

	png_read_end(png_ptr, nullptr);
	png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
	fclose(file);
}

void simpleUtil::checkTextures() {
	textureMutex.lock();
	bool empty = texturePath.empty();
	textureMutex.unlock();

	//this happens only when main thread is waiting for notify so we don't need lock here
	if (!empty)	loadTexture();
}

SimpleTexture* simpleGL::loadTexture(std::string path) {
	boost::unique_lock<boost::mutex> lock(textureMutex);
	returnReady = false;

	texturePath = path;

	do 	condVariable.wait(lock);
	while	(!returnReady);

	return returnValue;
}

void ComplexTexture::unload() {
	boost::lock_guard<boost::mutex> lock(textureMutex);

	for (auto it = sprites.begin(); it != sprites.end(); it++)
		it->deleteData();

	glDeleteTextures(1, &texture);//not sure if this can change value of texture. no reason why it should.

	textures.remove(*this);
}

void simpleUtil::drawTextures() {
	for (auto it = textures.begin(); it != textures.end(); it++)
		it->draw();
}
