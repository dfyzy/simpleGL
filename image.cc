#include <libpng16/png.h>
#include <zlib.h>

#include <list>
#include <memory>

#include "image.h"
#include "util.h"

namespace simpleGL {

Image::Image(GLenum filtering) {
	glGenTextures(1, &id);

	setFiltering(filtering);

	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

Image::Image(unsigned width, unsigned height, GLenum format, GLenum filtering) : Image(filtering) {
	resize(width, height, format);
}

Image::Image(const char* path, GLenum filtering) : Image(filtering) {
	util::print("Loading image");

	FILE *file = fopen(path, "rb");
	if (!file) {
		util::print("Error opening texture");
		return;
	}

	png_byte header[8];
	fread(header, 1, 8, file);
	if (png_sig_cmp(header, 0, 8)) {
		util::print("Not a png");
		fclose(file);
		return;
	}

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	if (!png_ptr) {
		util::print("Failed to create read struct");
		fclose(file);
		return;
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		util::print("Failed to create info struct");
		png_destroy_read_struct(&png_ptr, nullptr, nullptr);
		fclose(file);
		return;
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		util::print("Libpng error");
		png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
		fclose(file);
		return;
	}

	png_init_io(png_ptr, file);

	png_set_sig_bytes(png_ptr, 8);

	png_read_info(png_ptr, info_ptr);

	png_get_IHDR(png_ptr, info_ptr, &width, &height, nullptr, nullptr, nullptr, nullptr, nullptr);

	resize(width, height, GL_RGBA);

	std::unique_ptr<png_byte[]> row(new png_byte[4*width]);

	for (png_uint_32 i = height; i > 0; i--) {
		png_read_row(png_ptr, row.get(), nullptr);
		glTexSubImage2D(GL_TEXTURE_RECTANGLE, 0, 0, i - 1, width, 1, GL_RGBA, GL_UNSIGNED_BYTE, row.get());
	}

	png_read_end(png_ptr, nullptr);
	png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
	fclose(file);
}

void Image::setFiltering(GLenum newFiltering) {
	if ((newFiltering != GL_LINEAR) && (newFiltering != GL_NEAREST)) {
		util::print("Wrong filtering type");
		return;
	}

	filtering = newFiltering;

	glBindTexture(GL_TEXTURE_RECTANGLE, id);

	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, filtering);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, filtering);
}

void Image::resize(unsigned newWidth, unsigned newHeight, GLenum newFormat) {
	width = newWidth;
	height = newHeight;
	format = newFormat;

	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
}

Image::~Image() {
	util::print("Unloading texture");

	glDeleteTextures(1, &id);
}

}
