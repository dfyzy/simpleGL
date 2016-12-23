#include <libpng/png.h>
#include <zlib.h>

#include <list>
#include <memory>

#include "simpleGL.hpp"
#include "simpleUtil.hpp"

namespace simpleUtil {
	SimpleTexture* emptyTexture = new SimpleTexture(100, 100, 0);
	std::list<SimpleTexture*> textures;

	GLenum textureFiltering {GL_NEAREST};

	inline void setFiltering(GLuint texture, GLenum filtering) {
		glBindTexture(GL_TEXTURE_RECTANGLE, texture);

		glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, filtering);
		glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, filtering);
	}
}

using namespace simpleUtil;

namespace simpleGL {

	SimpleTexture* getEmptyTexture() {
		return emptyTexture;
	}

	void setTextureFiltering(GLenum tf) {
		if ((tf != GL_LINEAR) && (tf != GL_NEAREST)) {
			print("Wrong filtering type");
			return;
		}

		for (SimpleTexture* st : textures)
			setFiltering(st->getTexture(), tf);

		textureFiltering = tf;
	}

	SimpleTexture* loadTexture(std::string path) {
		print("Loading texture");

		FILE *file = fopen(path.c_str(), "rb");
		if (!file) {
			print("Error opening texture");
			return nullptr;
		}

		png_byte header[8];
		fread(header, 1, 8, file);
		if (png_sig_cmp(header, 0, 8)) {
			print("Not a png");
			fclose(file);
			return nullptr;
		}

		png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
		if (!png_ptr) {
			print("Failed to create read struct");
			fclose(file);
			return nullptr;
		}

		png_infop info_ptr = png_create_info_struct(png_ptr);
		if (!info_ptr) {
			print("Failed to create info struct");
			png_destroy_read_struct(&png_ptr, nullptr, nullptr);
			fclose(file);
			return nullptr;
		}

		if (setjmp(png_jmpbuf(png_ptr))) {
			print("Libpng error");
			png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
			fclose(file);
			return nullptr;
		}

		png_init_io(png_ptr, file);

		png_set_sig_bytes(png_ptr, 8);

		png_read_info(png_ptr, info_ptr);

		png_uint_32 width, height;
		png_get_IHDR(png_ptr, info_ptr, &width, &height, nullptr, nullptr, nullptr, nullptr, nullptr);

		GLuint texture;
		glGenTextures(1, &texture);

		SimpleTexture* simpleTex = new SimpleTexture(width, height, texture);
		textures.push_back(simpleTex);

		setFiltering(texture, textureFiltering);

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

		return simpleTex;
	}
}

SimpleTexture::~SimpleTexture() {
	print("Unloading texture");

	textures.remove(this);

	glDeleteTextures(1, &texture);
}

void SimpleTexture::setFiltering(GLenum tf) const {
	if ((tf != GL_LINEAR) && (tf != GL_NEAREST)) {
		print("Wrong filtering type");
		return;
	}

	simpleUtil::setFiltering(texture, tf);
}
