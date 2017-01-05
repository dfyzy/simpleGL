#include <libpng/png.h>
#include <zlib.h>

#include <list>
#include <memory>

#include "simpleGL.hpp"
#include "simpleUtil.hpp"

namespace simpleUtil {
	GLenum textureFiltering = GL_NEAREST;

	inline void setFiltering(GLuint texture, GLenum filtering) {
		glBindTexture(GL_TEXTURE_RECTANGLE, texture);

		glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, filtering);
		glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, filtering);
	}

	void genTexture(GLuint* texture, GLenum filtering) {
		glGenTextures(1, texture);

		setFiltering(*texture, filtering);

		glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

}

using namespace simpleUtil;

void simpleGL::setTextureFiltering(GLenum tf) {
	if ((tf != GL_LINEAR) && (tf != GL_NEAREST)) {
		print("Wrong filtering type");
		return;
	}

	textureFiltering = tf;
}


SimpleTexture::SimpleTexture(const char* path) {
	print("Loading texture");

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

	png_get_IHDR(png_ptr, info_ptr, &pixelWidth, &pixelHeight, nullptr, nullptr, nullptr, nullptr, nullptr);

	genTexture(&texture, textureFiltering);

	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA, pixelWidth, pixelHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	std::unique_ptr<png_byte[]> row(new png_byte[4*pixelWidth]);

	for (png_uint_32 i = pixelHeight; i > 0; i--) {
		png_read_row(png_ptr, row.get(), nullptr);
		glTexSubImage2D(GL_TEXTURE_RECTANGLE, 0, 0, i - 1, pixelWidth, 1, GL_RGBA, GL_UNSIGNED_BYTE, row.get());
	}

	png_read_end(png_ptr, nullptr);
	png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
	fclose(file);
}

void SimpleTexture::setFiltering(GLenum tf) const {
	if ((tf != GL_LINEAR) && (tf != GL_NEAREST)) {
		print("Wrong filtering type");
		return;
	}

	simpleUtil::setFiltering(texture, tf);
}

void SimpleTexture::unload() const {
	if (texture == 0)	return;
	print("Unloading texture");

	glDeleteTextures(1, &texture);
}
