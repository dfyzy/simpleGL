#include <libpng16/png.h>

#include <memory>

#include "image.h"
#include "util.h"

namespace {

GLuint currentImage;


void bindImage(GLuint image) {
	if (image != currentImage) {

		glBindTexture(GL_TEXTURE_RECTANGLE, image);
		currentImage = image;
	}
}

}

namespace simpleGL {

void Image::unbind() {
	bindImage(0);
}

Image::Image(GLenum filtering) {
	util::println("Image:load");

	glGenTextures(1, &id);

	setFiltering(filtering);

	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

Image* Image::loadData(unsigned pwidth, unsigned pheight, GLenum pformat, GLint pinternal, GLenum ptype, const void* data) {
	util::println("Image:loadData");

	width = pwidth;
	height = pheight;
	internal = pinternal;
	format = pformat;
	type = ptype;

	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, internal, width, height, 0, format, type, data);

	GLenum error;
	while((error = glGetError()) != GL_NO_ERROR) {
		std::string errorString;
		if (error == GL_INVALID_ENUM)					errorString = "invalid enum";
		else if (error == GL_INVALID_VALUE)			errorString = "invalid value";
		else if (error == GL_INVALID_OPERATION)	errorString = "invalid operation";

		util::println("error:OpenGL:" + errorString);
	}

	return this;
}

Image* Image::loadData(std::string path) {
	util::println("Image:load file:" + path);

	FILE *file = fopen(path.c_str(), "rb");
	if (!file) {
		util::println("error:Image:failed to open file");
		return this;
	}

	png_byte header[8];
	fread(header, 1, 8, file);
	if (png_sig_cmp(header, 0, 8)) {
		util::println("error:Image:file is not a png");
		fclose(file);
		return this;
	}

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	if (!png_ptr) {
		util::println("error:Image:failed to create read struct");
		fclose(file);
		return this;
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		util::println("error:Image:failed to create info struct");
		png_destroy_read_struct(&png_ptr, nullptr, nullptr);
		fclose(file);
		return this;
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		util::println("error:Image:libpng error");
		png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
		fclose(file);
		return this;
	}

	png_init_io(png_ptr, file);

	png_set_sig_bytes(png_ptr, 8);

	png_read_info(png_ptr, info_ptr);

	png_get_IHDR(png_ptr, info_ptr, &width, &height, nullptr, nullptr, nullptr, nullptr, nullptr);

	loadData(width, height, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	std::unique_ptr<png_byte[]> row(new png_byte[4*width]);

	for (png_uint_32 i = height; i > 0; i--) {
		png_read_row(png_ptr, row.get(), nullptr);
		glTexSubImage2D(GL_TEXTURE_RECTANGLE, 0, 0, i - 1, width, 1, GL_RGBA, GL_UNSIGNED_BYTE, row.get());
	}

	png_read_end(png_ptr, nullptr);
	png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
	fclose(file);

	return this;
}

void Image::setFiltering(GLenum gle) {
	if ((gle != GL_LINEAR) && (gle != GL_NEAREST)) {
		util::println("error:OpenGL:wrong filtering type");
		return;
	}

	filtering = gle;

	glBindTexture(GL_TEXTURE_RECTANGLE, id);

	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, filtering);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, filtering);
}

void Image::bind() const {
	bindImage(id);
}

Image::~Image() {
	util::println("Image:unload");

	glDeleteTextures(1, &id);
}

}
