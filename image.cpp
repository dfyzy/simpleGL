#include <libpng/png.h>
#include <zlib.h>

#include <list>
#include <memory>

#include "simpleGL.hpp"
#include "simpleUtil.hpp"

namespace simpleUtil {
	GLenum defaultFiltering = GL_NEAREST;

}

using namespace simpleUtil;
using namespace simpleGL;

Image::Image(unsigned width, unsigned height, GLenum format) : width(width), height(height) {
	genImage(defaultFiltering, format);
}

void simpleGL::setDefaultFiltering(GLenum tf) {
	if ((tf != GL_LINEAR) && (tf != GL_NEAREST)) {
		print("Wrong filtering type");
		return;
	}

	defaultFiltering = tf;
}


Image::Image(const char* path) {
	print("Loading image");

	FILE *file = fopen(path, "rb");
	if (!file) {
		print("Error opening texture");
		return;
	}

	png_byte header[8];
	fread(header, 1, 8, file);
	if (png_sig_cmp(header, 0, 8)) {
		print("Not a png");
		fclose(file);
		return;
	}

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	if (!png_ptr) {
		print("Failed to create read struct");
		fclose(file);
		return;
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		print("Failed to create info struct");
		png_destroy_read_struct(&png_ptr, nullptr, nullptr);
		fclose(file);
		return;
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		print("Libpng error");
		png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
		fclose(file);
		return;
	}

	png_init_io(png_ptr, file);

	png_set_sig_bytes(png_ptr, 8);

	png_read_info(png_ptr, info_ptr);

	png_get_IHDR(png_ptr, info_ptr, &width, &height, nullptr, nullptr, nullptr, nullptr, nullptr);

	genImage(defaultFiltering, GL_RGBA);

	std::unique_ptr<png_byte[]> row(new png_byte[4*width]);

	for (png_uint_32 i = height; i > 0; i--) {
		png_read_row(png_ptr, row.get(), nullptr);
		glTexSubImage2D(GL_TEXTURE_RECTANGLE, 0, 0, i - 1, width, 1, GL_RGBA, GL_UNSIGNED_BYTE, row.get());
	}

	png_read_end(png_ptr, nullptr);
	png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
	fclose(file);
}

void Image::setFiltering(GLenum filtering) const {
	if ((filtering != GL_LINEAR) && (filtering != GL_NEAREST)) {
		print("Wrong filtering type");
		return;
	}

	glBindTexture(GL_TEXTURE_RECTANGLE, id);

	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, filtering);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, filtering);
}

void Image::genImage(GLenum filtering, GLenum format) {
	glGenTextures(1, &id);

	setFiltering(filtering);

	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
}

void Image::unload() {
	if (id != 0) {
		print("Unloading texture");

		glDeleteTextures(1, &id);
	}
}
