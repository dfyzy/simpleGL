#include <libpng16/png.h>

#include <memory>

#include "image.h"
#include "log.h"

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
	println("Image:load");

	glGenTextures(1, &id);

	setFiltering(filtering);

	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

Image::~Image() {
	println("Image:unload");

	glDeleteTextures(1, &id);
}

void Image::loadData(unsigned pwidth, unsigned pheight, GLenum pformat, GLint pinternal, GLenum ptype, const void* data) {
	println("Image:loadData");

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

		println("error:OpenGL:" + errorString);
	}
}

void Image::loadData(const std::string& path) {
	println("Image:load file:" + path);

	FILE *file = fopen(path.c_str(), "rb");
	if (!file) {
		println("error:Image:failed to open file");
		return;
	}

	png_byte header[8];
	fread(header, 1, 8, file);
	if (png_sig_cmp(header, 0, 8)) {
		println("error:Image:file is not a png");
		fclose(file);
		return;
	}

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	if (!png_ptr) {
		println("error:Image:failed to create read struct");
		fclose(file);
		return;
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		println("error:Image:failed to create info struct");
		png_destroy_read_struct(&png_ptr, nullptr, nullptr);
		fclose(file);
		return;
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		println("error:Image:libpng error");
		png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
		fclose(file);
		return;
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

	return;
}

void Image::setFiltering(GLenum gle) {
	if ((gle != GL_LINEAR) && (gle != GL_NEAREST)) {
		println("error:OpenGL:wrong filtering type");
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

}
